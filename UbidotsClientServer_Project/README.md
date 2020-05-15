# Project Description

In this project, I created several Clients & Servers using an Arduino Nano 33 BLE Sense, an ESP32 DevKit, and an MQTT Protocol with Ubidots. I used the Arduino to capture proximity/gesture data and x/y/z coordinate acceleration data. We then created a server in the Arduino IDE to post data to a peripheral via Bluetooth Low Energy (BLE) signals. Next, I used the ESP32's BLE functionality to capture the Arduino Sensor Data by creating a Client that is subscribed to Characteristics of the Arduino. Lastly, I created an MQTT protocol that allowed the ESP32 to transmit data via WiFi to a Ubidots Client webpage. The ultimate goal of this project was to become familiar with BLE and MQTT data transmission, and to capture real-time sensor data from a remote location.

