#!/bin/ash

# if wifi client enabled, restart wifi connection if no ip address
[ "`/sbin/uci get wireless.sta_0.disabled`" == "0" ] \
&& /sbin/ifconfig wlan0-2 | grep '[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}\.[0-9]\{1,3\}' \
|| /etc/init.d/network restart


uptimeMin=`/usr/bin/awk '{print int(($1/60))}' /proc/uptime`
if [ $uptimeMin -ge 5 ]
then
	# check current micro-controler sketch 
	grep "^LoraFwd" /var/avr/fw_version
	if [ $? -ne 0 ]
	then
		/usr/bin/run-avrdude /root/sketch.hex >/tmp/update_mcu.log 2>&1
		sleep 30
    fi
	
	# check bridge process that links micro-controler and dragino unit
	ps w | grep bridge.py | grep -v grep
	if [ $? -ne 0 ]
	then
    		/usr/bin/reset-mcu
	fi
fi