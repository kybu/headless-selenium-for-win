/* # Copyright 2014-2017 Peter Vrabel(kybu@kybu.org)
#
# This file is part of 'Headless Selenium for Win'.
#
# 'Headless Selenium for Win' is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# 'Headless Selenium for Win' is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with 'Headless Selenium for Win'.If not, see <http://www.gnu.org/licenses/>. */


#include "stdafx.h"

using namespace std;
namespace bo = boost;

string desktopName;

template <class SepT, class IterT>
SepT join(IterT begin, const IterT end, const SepT &sep)
{
  typedef typename SepT::value_type     char_type;
  typedef typename SepT::traits_type    traits_type;
  typedef typename SepT::allocator_type allocator_type;
  typedef basic_ostringstream<char_type, traits_type, allocator_type> ostringstream_type;
  ostringstream_type result;

  if (begin != end)
    result << L"\"" << *begin++ << L"\"";
  while (begin != end) {
    result << sep;
    result << L"\"" << *begin++ << L"\"";
  }
  return result.str();
}

string getAppCmdArgs() {
  int argsCount = 0;
  auto cmdOrig = GetCommandLine();
  LOGD << "Command line: " << cmdOrig;

  bo::shared_ptr<wchar_t *> args(
    CommandLineToArgvW(GetCommandLine(), &argsCount),
    LocalFree);

  string cmdLine;
  if (argsCount > 2)
    cmdLine = bo::to_utf8(
      join(args.get() + 1, args.get() + argsCount, wstring(L" ")));

  else if (argsCount == 2)
    cmdLine = bo::to_utf8(*(args.get() + 1));

  return cmdLine;
}

void prepareDesktopName() {
  if (Environment::variableExists("HEADLESS_UNIQUE")) {
    bo::random_device random;

    string name;
    do {
      name = (bo::format("HeadlessDesktop_%1%") % random()).str();
    } while (Desktop::exists(name));

    desktopName = name;
  }
  else
    desktopName = "HeadlessDesktop";
}

// TODO: Implement safer SearchPath.
string findSeleniumDriver(const string &driver) {
  bo::movelib::unique_ptr<wchar_t[]> buffer(new wchar_t[32*1024]);
  wchar_t *bufferEnd = 0;

  DWORD length = SearchPath(
    NULL, bo::from_utf8(driver).c_str(), NULL,
    32 * 1024, buffer.get(), &bufferEnd);

  if (!length)
    throw runtime_error(
      (bo::format("%1% cannot be found!") % driver).str());

  buffer[length] = 0;

  return bo::to_utf8(buffer.get());
}

Desktop::ProcessInTheJob driverInfo;
HANDLE jobHandle = 0;
HDESK desktopHandle = 0;

// Can be run only once because ctrHandle() is executed on a different thread.
// Could be implemented less dodgy in the future.
void cleanUp() {
  using namespace boost;
  static mutex m;
  m.lock();

  auto topWindows = Desktop::allTopLevelWindows(desktopName);
  LOGD << "Top level windows in the headless desktop: " << topWindows.size();
    
  for (HWND w : topWindows) {
    LOGT << (bo::format("Quitting the '%1%' top level window.") % w).str();

    PostMessage(w, WM_ENDSESSION, NULL, ENDSESSION_CLOSEAPP);
    PostMessage(w, WM_QUIT, 0, 0);
  }

  Sleep(2000 - 618);
  LOGD << "Top level windows after sending the quit msg: "
       << Desktop::allTopLevelWindows(desktopName).size();

  if (jobHandle) {
    LOGD << "Going to close the job object.";
    CloseHandle(jobHandle);
  }
}

// Executes on a different thread.
BOOL ctrlHandler(DWORD ctrlType) {
  LOGI << "Exiting ...";

  cleanUp();

  return FALSE;
}

void setupLogger() {
  using namespace boost::log;

  add_console_log(
    cout,
    keywords::format = "%Message%");

#ifdef _DEBUG
  core::get()->set_filter(
    trivial::severity >= trivial::trace);
#else
  core::get()->set_filter(
    trivial::severity >= trivial::info);
#endif
}

int _tmain(int argc, _TCHAR* argv[])
{ 
  try {
    setupLogger();

    prepareDesktopName();

    string cmdLine = getAppCmdArgs();

    string selDriver = "IEDriverServer.exe";
    if (Environment::variableExists("HEADLESS_DRIVER")) {
      selDriver = Environment::variable("HEADLESS_DRIVER");
      LOGD << "HEADLESS_DRIVER env variable: " << selDriver;
    }

    string selDriverPath = findSeleniumDriver(selDriver);

    LOGI << "Selenium driver found at: " << selDriverPath;

    if (Desktop::exists(desktopName))
      LOGW << "WARN: Headless desktop '" << desktopName
           << "' already exists! Selenium driver might be flaky.";

    LOGI; // empty line

    SetConsoleCtrlHandler((PHANDLER_ROUTINE)ctrlHandler, TRUE);

    desktopHandle = Desktop::create(desktopName);
    
    driverInfo = Desktop::createProcessInTheJob(
      desktopName,
      selDriverPath,
      cmdLine);

    if (driverInfo.status == driverInfo.COULD_NOT_ASSIGN_JOB) {
      LOGW << "WARN: Could not use Windows job objects! "
              "That means the cleaning-up procedure might not be reliable.";
      ResumeThread(driverInfo.processInfo.hThread);
    }
    jobHandle = driverInfo.jobHandle;

    CloseHandle(driverInfo.processInfo.hThread);
    Process::wait(driverInfo.processInfo.hProcess);
    CloseHandle(driverInfo.processInfo.hProcess);

    cleanUp();
  }
  catch (runtime_error &e) {
    LOGF << e.what();
    return 1;
  }
  catch (...) {
    LOGF << "A bummer, unknown exception caught!";

    return 1;
  }

  return 0;
}

