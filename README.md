# connectedHives
Connected hive scale with our own LoRa Gateway

Hardware list
--------------------

Gateway. The gateway is not LoRaWAN compatible. We will operate on our own LoRa protocol:
* Dragino LoRa Gateway LG01-P  868Mhz Europe version [Spec](https://www.dragino.com/products/lora-lorawan-gateway/item/117-lg01-p.html) [Datasheet](https://www.dragino.com/downloads/downloads/datasheet/EN/Datasheet_LG01.pdf)
LoRa_Shield_Pin_Mapping.png
<img src="https://raw.github.com/luigi1809/connectedHives/master/img/gateway.jpg" width="200">

Hive scale:
* Arduino Uno R3 [Spec](https://store.arduino.cc/arduino-uno-rev3)
 [Doc](https://www.dragino.com/downloads/downloads/UserManual/LG01_LoRa_Gateway_User_Manual.pdf)
 <img src="https://raw.github.com/luigi1809/connectedHives/master/img/arduino.jpg" width="200">
 
* Dragino LoRa Shield for Arduino Uno 868Mhz Europe version [Spec](https://www.dragino.com/products/lora/item/102-lora-shield.html)
[Pin Mapping](https://raw.github.com/luigi1809/connectedHives/master/img/LoRa_Shield_Pin_Mapping.png)

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/shield.png" width="200">

* 200kg electronic platform scale load cell

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/load-weight-sensor.jpg" width="200">

* HX711 hx711-module-load-cell-voltage-amplifier-1625-63-B.

[Pin Mapping](https://raw.github.com/luigi1809/connectedHives/master/img/hx711.jpg)

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/hx711-card.jpg" width="100">

* Adafruit TPL5110 circuit breakout (for power saving)
<img src="https://raw.github.com/luigi1809/connectedHives/master/img/tpl5110.jpg" width="200">

* DHT22 temperature and humidity sensor

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/dht22.jpg" width="200">


* 2 x 120pcs 10cm Male to Male + Male to Female and Female to Female Jumper Wire Dupont Cable for Arduino [Shop](https://www.aliexpress.com/item/32829927834.html?spm=a2g0s.9042311.0.0.27426c37Ii2tKg)

(to be completed)

Dragino install
--------------------
Plug a network cable between Dragino LAN port and your PC.
Set up manual network address on your PC to your ethernet card.
* IP: 10.130.1.2
* Mask: 255.255.255.0
* Gateway: blank
* DNS: blank

Login on your browser: [http://10.130.1.1/](http://10.130.1.1/). Login: root/dragino
![](https://raw.github.com/luigi1809/connectedHives/master/img/dragino-lg01-p-login.png)

Check that firmware version is the latest: IoT-4.4.4
![](https://raw.github.com/luigi1809/connectedHives/master/img/dragino-lg01-p-firware-version.png)


Software
--------------------
Install Arduino IDE [Download](https://www.arduino.cc/en/Main/Software#)

Open Arduino IDE. In menu file --> preference add following URL to "Additional board URL" :
```
http://www.dragino.com/downloads/downloads/YunShield/package_dragino_yun_test_index.json
```

![](https://raw.github.com/luigi1809/connectedHives/master/img/IDE-prefs.png)

And set "sketchbook location" to : ```<git directory>/Arduino/```

Go tools --> Boards --> Boards Manager, find Dragino boards info and install it.

Select port:
![](https://raw.github.com/luigi1809/connectedHives/master/img/Arduino-IDE-select-module.png)

Select board:
![](https://raw.github.com/luigi1809/connectedHives/master/img/Arduino-IDE-select-board.png)

Then open file Arduino/Gateway/Gateway.ino

Select sketch --> upload. Password is "dragino".

Check http://10.130.1.1/cgi-bin/luci//admin/sensor/mcu -> MCU version should be LoraFwd***
![](https://raw.github.com/luigi1809/connectedHives/master/img/mcu-version.png)


