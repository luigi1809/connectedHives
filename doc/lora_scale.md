# LoRa Scale electronics

Data send by the scale / sensor:
* temperature humidity from DHT22 sensor
* Lux solar radiation from CJMCU-3001/OPT3001 sensor 
* load sensor with HX711 PCB

Pin mapping to Arduino
--------------------

Original Arduino Pin-out :

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/Arduino-Uno-Pinout-1.png" width="600">


With Dragino LoRa shield, some pins are blocked by shield. Available pin are in blue color. Analogic pin can be used for digital use (if 5V is send, it means 1. 0V means 0) :

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/LoRa_Shield_Pin_Mapping.png" width="600">

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
    * DOUT or DT <-> D4
    * CLK or SCK <-> D3

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/hx711-card.jpg" width="300">

Pin mapping to load-cell
--------------------

Diagram is indicative. Refer to the load-cell datasheet. Wire colors defer from constructors to another.
<img src="https://raw.github.com/luigi1809/connectedHives/master/img/hx711.jpg">


Power supply
--------------------

4 x AA Alkaline battery Box (4 x 1.5V = 6V) or 5 x AA ni-mh/li-ion rechargeable battery Box (5 x 1.2V = 6V)

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/4xAA_box.jpg" width="300"><img src="https://raw.github.com/luigi1809/connectedHives/master/img/5xAA_box.jpg" width="300">

Battery box connection <-> Adafruit TPL5110 [Doc](https://raw.github.com/luigi1809/connectedHives/master/pdf/adafruit-tpl5110-power-timer-breakout.pdf)
* red color wire (+) <-> VDD
* black color wire (-) <-> GND 

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/tpl5110.jpg" width="200">

Adafruit TPL5110 <-> Arduino
* DRV <-> Vin
* GND <-> GND

Optionaly, we can use a 2.5x5.5x9mm DC power male plug jack.
In that case, Adafruit TPL5110 <-> Jack
* DRV <-> middle pin (+)
* GND <-> outside pin (-)

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/jack.jpg" width="200"><img src="https://raw.github.com/luigi1809/connectedHives/master/img/positive_polarity.jpg" width="200">

Then Jack to Arduino

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/jack_arduino.jpg">

Set the breakout timer on TPL5110
--------------------
Go to [TPL5110 Doc](https://raw.github.com/luigi1809/connectedHives/master/pdf/adafruit-tpl5110-power-timer-breakout.pdf) section "Notes on the Delay Pin".