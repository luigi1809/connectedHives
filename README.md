# connectedHives

Hardware list
--------------------
* Arduino Uno R3 [Spec](https://store.arduino.cc/arduino-uno-rev3)
* Dragino LoRa Gateway LG01-P  868Mhz Europe version [Spec](https://www.dragino.com/products/lora-lorawan-gateway/item/117-lg01-p.html) [Datasheet](https://www.dragino.com/downloads/downloads/datasheet/EN/Datasheet_LG01.pdf) [Doc](https://www.dragino.com/downloads/downloads/UserManual/LG01_LoRa_Gateway_User_Manual.pdf)
* Dragino LoRa Shield for Arduino Uno 868Mhz Europe version [Spec](https://www.dragino.com/products/lora/item/102-lora-shield.html)

* Adafruit TPL5110 circuit breakout (for power saving)
* DHT22 temperature and humidity sensor
* 2 x 120pcs 10cm Male to Male + Male to Female and Female to Female Jumper Wire Dupont Cable for Arduino [Shop](https://www.aliexpress.com/item/32829927834.html?spm=a2g0s.9042311.0.0.27426c37Ii2tKg)
(to be completed)

Software
--------------------
Install Arduino IDE [Download](https://www.arduino.cc/en/Main/Software#)

Open Arduino IDE. In menu file -> preference add following URL to "Additional board URL" :
```
http://www.dragino.com/downloads/downloads/YunShield/package_dragino_yun_test_index.json
```

![](https://raw.github.com/luigi1809/connectedHives/master/img/IDE-prefs.png)

And set "sketchbook location" to : ```<git directory>/Arduino/```

Go tools --> Boards --> Boards Manager, find Dragino boards info and install it.

Select board arduino
