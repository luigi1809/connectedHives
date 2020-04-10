# LoRa Scale

Code is in ```Arduino/Client-0/Client-0.ino```

Data :
* temperature humidity from DHT22 sensor
* Lux solar radiation from CJMCU-3001/OPT3001 sensor 
* load sensor with HX711 PCB

Pin mapping to Arduino
--------------------

Original Arduino Pin-out :
![](https://raw.github.com/luigi1809/connectedHives/master/img/Arduino-Uno-Pinout-1.png)

With Dragino LoRa shield, some pins are blocked by shield. Available pin are in blue color :
![](https://raw.github.com/luigi1809/connectedHives/master/img/LoRa_Shield_Pin_Mapping.png)

* lux OPT3001
    * SDA <-> A4
    * SCL <-> A5
* Adafruit TPL5110
    * A0
    signal to 1 logic level to break out the circuit
* led A1 light on one time if gateway LG01-P gets the signal or two time otherwise
* DHT22
    * D5
* HX711 PCB scale
    * DOUT D4
    * CLK D3