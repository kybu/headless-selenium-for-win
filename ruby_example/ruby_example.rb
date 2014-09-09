require 'selenium-webdriver'

# Unable to find element on closed window (Selenium::WebDriver::Error::NoSuchWindowError)

Dir.chdir(File.dirname $0)

# Find the Headless IE binary.
headlessBinary =
  if File.exists?('../Debug/headless_ie_selenium.exe')
    '../Debug/headless_ie_selenium.exe'
  elsif File.exists?('../Release/headless_ie_selenium.exe')
    '../Release/headless_ie_selenium.exe'
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

driver.quit
