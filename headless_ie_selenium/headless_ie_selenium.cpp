/* # Copyright 2014 Peter Vrabel(kybu@kybu.org)
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
# along with Foobar.If not, see <http://www.gnu.org/licenses/>. */


#include "stdafx.h"

using namespace std;
namespace bo = boost;

string desktopName = "HeadlessDesktop";

template <class SepT, class IterT>
SepT join(IterT begin, const IterT end, const SepT &sep)
{
  typedef typename SepT::value_type     char_type;
  typedef typename SepT::traits_type    traits_type;
  typedef typename SepT::allocator_type allocator_type;
  typedef basic_ostringstream<char_type, traits_type, allocator_type> ostringstream_type;
  ostringstream_type result;

  if (begin != end)
    result << *begin++;
  while (begin != end) {
    result << sep;
    result << *begin++;
  }
  return result.str();
}

string getAppCmdArgs() {
  int argsCount = 0;
  bo::shared_ptr<wchar_t *> args(
    CommandLineToArgvW(GetCommandLine(), &argsCount),
    LocalFree);

  string cmdLine;
  if (argsCount > 2)
    cmdLine = bo::to_utf8(
      join(args.get() + 1, args.get() + argsCount - 1, wstring(L" ")));

  else if (argsCount == 2)
    cmdLine = bo::to_utf8(*(args.get() + 1));

  return cmdLine;
}

// TODO: Implement safer SearchPath.
string findIEDriver() {
  bo::shared_ptr<wchar_t[]> buffer(new wchar_t[32*1024]);
  wchar_t *bufferEnd = 0;

  DWORD length = SearchPath(
    NULL, L"IEDriverServer.exe", NULL,
    32 * 1024, buffer.get(), &bufferEnd);

  buffer[length] = 0;

  return bo::to_utf8(buffer.get());
}

int _tmain(int argc, _TCHAR* argv[])
{ 
  string cmdLine = getAppCmdArgs();
  string ieDriverPath = findIEDriver();

  cout << "IE driver found at: " << ieDriverPath << endl;

  if (Desktop::exists(desktopName))
    cout << "WARN: Headless desktop '" << desktopName
         << "' already exists! IE driver might be flaky." << endl;

  cout << endl;

  HDESK desktopHandle = Desktop::create(desktopName);
  Desktop::createProcess(desktopName, "c:\\Windows\\explorer.exe");

  Sleep(2000);

  PROCESS_INFORMATION pi = Desktop::createProcess(
    desktopName,
    ieDriverPath,
    cmdLine);

  Process::wait(pi.hProcess);

  return 0;
}

