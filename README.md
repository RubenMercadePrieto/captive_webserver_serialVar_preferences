# captive_webserver_serialVar_preferences

Motivation: Debug and configure a station remotely 

One of the easiest ways to debug and configure a working ESP32 repotelly is using Wifi Access Point (AP) mode. The idea would be to have a way to place the ESP32 station under maintenance, e.g. using touchscreen, using button, etc, where the station would restart and load the following code designed for maintenance.

1. To easily access the created Wifi AP with a mobile phone or a laptop, we can use a QR code, e.g. https://www.qrcode-tiger.com/?type=wifi. Such code can be printed on the station box.
![Wifi QR code](/qr_wpa.png)
2. Once connected to the ESP32 Wifi AP, normally you would need to type in the browser the microcontroler IP. In order to avoid this and simplify the process, the system will use a Capture Portal, forcing which page to open by default, and importantly, by opening it directly once the wifi connection is stablished - like in hotels, airports, etc.
3. The main index.html file would show all relevant configuration parameters, in this example it is only the hipothetical measurement interval for data acquision, in seconds. This value is saved in the flash memory using the library Preferences. The value is read and displayed when the webpage is served, e.g.:
![VariablesPref](/image1.png)
4. In that webpage there is also a link to access WebSerial, which is a library that allows bidirectial serial communiation through a webserver. Because this library uses the webserver library ESPAsyncWebServer, everything else implemented has to be defined based on that library.
![WebSerial](/image2.png)
8. From WebSerial we can send and recieve commands to the station, like in the traditional USB serial. In this example, it is programed to recognize the variables TimeMeasSecond and TimeMeasMin, followed by a comma and an integer value, e.g. TimeMeasSecond,12. If the information is recieved correctly, the new value is saved using Preferences in the flash memory. Hence, even if the station reboots, such value is not lost. In this example it is shown after restarting, e.g.
![VariableAfterRestart](/image3.png)



