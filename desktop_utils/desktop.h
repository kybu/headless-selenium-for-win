#pragma once

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

