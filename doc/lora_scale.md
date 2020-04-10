# LoRa Scale

Code is in ```Arduino/Client-0/Client-0.ino```

Data send by the scale / sensor:
* temperature humidity from DHT22 sensor
* Lux solar radiation from CJMCU-3001/OPT3001 sensor 
* load sensor with HX711 PCB

Pin mapping to Arduino
--------------------

Original Arduino Pin-out :
![](https://raw.github.com/luigi1809/connectedHives/master/img/Arduino-Uno-Pinout-1.png)

With Dragino LoRa shield, some pins are blocked by shield. Available pin are in blue color. Analogic pin can be used for digital use (if 5V is send, it means 1. 0V means 0) :
![](https://raw.github.com/luigi1809/connectedHives/master/img/LoRa_Shield_Pin_Mapping.png)

* lux OPT3001
    * SDA <-> A4
    * SCL <-> A5
    
<img src="https://raw.github.com/luigi1809/connectedHives/master/img/opt3001.jpg" width="200">

* Adafruit TPL5110. Signal to 1 logic level to break out the circuit for the select amout of time
    * Done <-> A0 
    
<img src="https://raw.github.com/luigi1809/connectedHives/master/img/tpl5110.jpg" width="200">

* led. Light on one time if gateway LG01-P gets the signal or two time otherwise
    * Small leg <-> GND
    * Long leg <-> A1

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/led.jpg" width="200">

* DHT22
    * DAT <-> D5
    
<img src="https://raw.github.com/luigi1809/connectedHives/master/img/dht22.jpg" width="200">

* HX711 load-cell voltage amplifier module
    * DOUT <-> D4
    * CLK <-> D3
    
[Pin Mapping](https://raw.github.com/luigi1809/connectedHives/master/img/hx711.jpg)

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/hx711-card.jpg" width="300">