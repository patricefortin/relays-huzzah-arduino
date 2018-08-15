# relays-huzzah-arduino
120V Relays controlled by a HUZZAH ESP8266 Breakout, with a webserver

Meant to be controlled from other tools with /api endpoint, but provides a html page on root with buttons for basic standalone use.

Material:
- HUZZAH ESP8266 Breakout (https://www.adafruit.com/product/2471)
- Relay module (https://abra-electronics.com/electromechanical/relays/relay-modules-shields/rm-2-two-channel-relay-module.html)
- 2 Standard 120V dual sockets
- 2 NPN bipolar transistor
- 5V voltage regulator
- 0.33µF and 0.1µF capacitors

With this documentation: https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout

WiFi/Webserver code based on: https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
