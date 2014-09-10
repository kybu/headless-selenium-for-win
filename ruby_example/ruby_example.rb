# Copyright 2014 Peter Vrabel (kybu@kybu.org)
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
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with 'Headless Selenium for Win'.  If not, see <http://www.gnu.org/licenses/>.


require 'selenium-webdriver'

# Unable to find element on closed window (Selenium::WebDriver::Error::NoSuchWindowError)

Dir.chdir(File.dirname $0)

# Find the Headless IE binary.
headlessBinary =
  if File.exists?('../Debug/headless_ie_selenium.exe')
    '../Debug/headless_ie_selenium.exe'
  elsif File.exists?('../Release/headless_ie_selenium.exe')
    '../Release/headless_ie_selenium.exe'
  else
    raise 'Cound not find headless_ie_selenium.exe!'
  end

puts "Headless IE binary found at: #{headlessBinary}"

Selenium::WebDriver::IE.driver_path = headlessBinary

driver = Selenium::WebDriver.for :ie
driver.get 'https://www.google.com'

sleep 2

searchInput = driver.find_element :name => 'q'

searchInput.send_keys "london weather"
searchInput.submit

sleep 2

weather = driver.find_element :id => 'wob_wc'

puts weather.text

driver.close
