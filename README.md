# Hydroponics

This is the code that runs my ESP32 to automate my hydroponics project which is actually my TFG.

It consists of a vertical hydroponic grow automated with an ESP32 through a wide range of sensors such as pH, temperature, humidity, TDS,....
All these parameters are monitored by the ESP32 and sent to my mqtt broker on a rapsberry pi 3 B+. Once these measurements have been sent to the broker, a dashboard on node-red 
shows all of them in a visual and simple way. 

This dashboard can be consulted both via PC and smartphone remotely, without the need to be on the local network.
