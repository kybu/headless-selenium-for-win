headless-selenium-for-win
=========================

It is quite inconvenient that a browser window pops up when running Selenium
tests. It might cause tests to fail because such window needs to hold user
focus. That is the case with IE at least.

In Linux world, this is solved by running browsers in a frame-buffer. Similar
approach can be taken on Windows platform as well.

Contrary to popular believe, Windows is fully multi-user OS system. On top of
that, it supports virtual desktops even though it is not very often compared
to Linux desktop environments.

This application uses virtual desktops to run web browsers in so that they
do not disturb the main user desktop.

# Basic Ruby example

Following example uses www.google.com search to retrieve weather in London.

    require 'selenium-webdriver'
    
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
    
    # The most important part!
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
    
    driver.quit

Output might be something like this:

    Headless IE binary: ../Debug/headless_ie_selenium.exe
    London, UK
    Tuesday 11:00
    Partly Cloudy
    19
    °C | °F
    Precipitation: 3%
    Humidity: 69%
    Wind: 0 mph
    TemperaturePrecipitationWind
    1921211915141311
    12:0015:0018:0021:0000:0003:0006:0009:00
    Tue
    22°12°
    Wed
    21°11°
    Thu
    22°12°
    Fri
    22°13°
    Sat
    23°13°
    Sun
    23°16°
    Mon
    22°14°
    Tue
    22°13°
    

# Technical details

Supported browsers at the moment: IE.

# License

GPLv3, Copyright 2014 Peter Vrabel

