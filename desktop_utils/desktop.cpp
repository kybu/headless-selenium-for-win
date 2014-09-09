#include "stdafx.h"

using namespace std;
namespace bo = boost;

HDESK Desktop::create(const string &name) {
  HDESK handle =
    CreateDesktop(
    bo::from_utf8(name).c_str(), NULL, NULL, DF_ALLOWOTHERACCOUNTHOOK, GENERIC_ALL, NULL);

  return handle;
}

bool Desktop::exists(const string &name) {
  HDESK handle = OpenDesktop(
    bo::from_utf8(name).c_str(),
    0, FALSE, 0);

  bool e = handle != NULL;
  CloseDesktop(handle);

  return e;
}

// appName = dir
// cmdLine = *.cpp
PROCESS_INFORMATION Desktop::createProcess(
  const string &desktopName, const string &appName,
  const string &cmdLine) {

  STARTUPINFO si;
  memset(&si, 0, sizeof(si));
  si.cb = sizeof(si);
  wstring desktopNameW = bo::from_utf8(desktopName);
  si.lpDesktop = const_cast<wchar_t *>(desktopNameW.c_str());

  PROCESS_INFORMATION pi;
  memset(&pi, 0, sizeof(pi));

  bo::shared_ptr<wchar_t []> cmdLineTmp;
  if (!cmdLine.empty()) {
    wstring tmp = bo::from_utf8(appName + " " + cmdLine);

    cmdLineTmp.reset(new wchar_t[tmp.size()+1]);
    memmove(cmdLineTmp.get(), tmp.c_str(), tmp.size()*sizeof(wchar_t));
    cmdLineTmp[tmp.size()] = 0;
  }

  BOOL created = CreateProcess(
    bo::from_utf8(appName).c_str(),
    cmdLineTmp.get(),
    NULL, NULL, TRUE, NORMAL_PRIORITY_CLASS, NULL, NULL,
    &si, &pi);

  if (created == FALSE)
    throw DesktopError("Cannot create a process!");

  return pi;
}

void Desktop::switchToDefault() {
  HDESK h = OpenDesktop(
    _T("Default"), DF_ALLOWOTHERACCOUNTHOOK, TRUE, GENERIC_ALL);

  switchTo(h);
}

void Desktop::switchTo(const string &desktopName) {
    HDESK h = OpenDesktop(
      bo::from_utf8(desktopName).c_str(),
      DF_ALLOWOTHERACCOUNTHOOK, TRUE, GENERIC_ALL);

    switchTo(h);
}

vector<string> Desktop::desktops() {
  vector<string> desktops;

  EnumDesktops(
    GetProcessWindowStation(), enumDesktopProc, (LPARAM)&desktops);

  return desktops;
}

void Desktop::switchTo(HDESK desktopHandle) {
  if (SwitchDesktop(desktopHandle) == 0)
    cerr << "Cannot switch to a desktop!" << endl;
}

BOOL CALLBACK Desktop::enumDesktopProc(
  _In_  LPTSTR desktop,
  _In_  LPARAM param) {

  vector<string> *desktops = (vector<string> *)param;
  desktops->push_back(bo::to_utf8(desktop));

  return TRUE;
}