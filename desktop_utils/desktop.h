#pragma once
#include "stdafx.h"

class DesktopError : public std::runtime_error {
  typedef std::runtime_error MyBase;

public:
  DesktopError(const std::string &msg) : MyBase(msg) {}
};

struct Desktop {
  static HDESK create(const std::string &name);
  static bool exists(const std::string &name);

  static PROCESS_INFORMATION createProcess(
    const std::string &desktopName,
    const std::string &appName,
    const std::string &cmdLine = "");

  static std::vector<std::string> Desktop::desktops();

  static void switchToDefault();
  static void switchTo(HDESK desktopHandle);
  static void switchTo(const std::string &desktopName);

private:
  static BOOL CALLBACK enumDesktopProc(
    _In_  LPTSTR lpszDesktop,
    _In_  LPARAM lParam);
};

