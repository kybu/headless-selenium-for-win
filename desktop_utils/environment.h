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
# along with 'Headless Selenium for Win'.If not, see <http://www.gnu.org/licenses/>. */

#include "stdafx.h"

class Environment {
public:
  static bool variableExists(const std::string &);
  static std::string variable(const std::string &);
};

// inline

inline
bool Environment::variableExists(const std::string &var) {
  wchar_t buf[104];

  auto ret = GetEnvironmentVariable(
    boost::from_utf8(var).c_str(),
    buf, 104);

  if (!ret && GetLastError()==ERROR_ENVVAR_NOT_FOUND)
    return false;

  return true;
}

inline
std::string Environment::variable(const std::string &var) {
  wchar_t buf[104];

  auto ret = GetEnvironmentVariable(
    boost::from_utf8(var).c_str(),
    buf, 104);

  if (!ret && GetLastError() == ERROR_ENVVAR_NOT_FOUND)
    throw std::runtime_error(
      (boost::format("No such environment variable: %1%") % var).str());

  return std::string(boost::to_utf8(buf));
}