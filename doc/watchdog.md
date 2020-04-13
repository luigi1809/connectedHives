# Watchdog script on Dragino LG01-P Gateway

Several issues have been detected that affects the stability of the gateway :
* If the gateway is connected to Internet as wifi client, wifi connection sometimes fails to re-connect if connection is cut
* After reboot, the micro-controler unit (MCU) sometimes losts its sketch
* Process bridge.py that bridges MCU and dragino linux gateway might crash

To solve the issue, a [watchdog](https://github.com/luigi1809/connectedHives/blob/master/dragino/watchdog.sh) script have to be installed.

Copy with SCP/SFTP ```dragino/watchdog.sh``` to dragino gateway ```/usr/sbin/```

Setup the script in crontab.

```
cat>> /etc/crontabs/root <<\EOF
* * * * * /usr/sbin/watchdog.sh
EOF
```

Disable the native watchdog script (broken in last firmware version)

```
sed -E 's|(.* checkdog)|#\1|g' -i /etc/crontabs/root
```

Generate sketch binary (or use already compiled binary in ```Arduino/gateway/sketch.hex``` :

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/sketch_hex.png" width="200">

File ```Gateway.ino.with_bootloader.standard.hex``` will be generated in same folder as ```Gateway.ino```. Rename it to ```sketch.hex``` and copy it with SCP/SFTP to dragino gateway ```/root```