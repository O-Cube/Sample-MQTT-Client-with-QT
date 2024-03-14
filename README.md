# Sample-MQTT-Client-with-QT
A MQTT client that stores values from subscribed topics into a MySQL database. 
![image](https://github.com/O-Cube/Sample-MQTT-Client-with-QT/assets/65163799/f3145957-1b42-4377-ab5c-3c1c41e0eb7a)

Components:
  - Mosquitto broker running in wsl.
  - Mosquitto client running in wsl.
  - Sample MQTT client with gui interface for configuration and monitor of activities.
  - MySQL database running in windows.
Operation:
  - Sample client connects to broker together with mosquitto client.
  - Subscribes to desired topics.
  - Writes messages from desired topics into MySQL database.

![image](https://github.com/O-Cube/Sample-MQTT-Client-with-QT/assets/65163799/086819a8-3249-4127-8862-17ab8b2125ab)
