#Dragino LG01-P Gateway install

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


