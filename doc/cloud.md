# Send data to cloud

Data are send to [The Things Network](https://www.thethingsnetwork.org/) and [Weather Underground](https://www.wunderground.com/weather/api)

LoRa module of the gateway receives the data from the LoRa scale and then call linux process ```/usr/sbin/api``` with datas in argument.

Arguments list is :

1. LoRa End Node ID set in ```Arduino/Client-0/Client-0.ino```
```
unsigned int node_id[3] = {0,0,0}; //LoRa End Node ID
```
* Temperature in °C (integer)
* Temperature in °C (decimal) : temperature is integer.decimal
* Percentage of humidity (integer)
* Percentage of humidity (decimal) : humidity percentage is integer.decimal
* Lux in W/m² (integer)
* Lux un W/m² (decimal) : Lux value is integer.decimal
* Weight in Kg (integer)
* Weight in Kg (decimal) : weight value is integer.decimal

Edit the file ```dragino/api``` to put your own API keys.

```
j=0
for i in "$@"
do
    case $j in
    0)
        thingsNetworkAPIKey="KEY?????"
        nodeid=$i
        wundergoundID="ID????"
        wundergoundPassword="pass???"
        # case i match LoRa End Node ID
        case $i in
            0)
                #ruche gauche
                thingsNetworkAPIKey="KEY?????"
                wundergoundID="ID????"
                wundergoundPassword="pass???"
                name="Ruche gauche"
                ;;
            1)
                #ruche droite
                thingsNetworkAPIKey="KEY?????"
                wundergoundID="ID????"
                wundergoundPassword="pass???"
                name="Ruche droite"
                ;;
```

Curl requests are made to send data. Make sure the gateway has internet connectivity to do so.

With a SCP/SFTP client like WinSCP, copy the file ```dragino/thingspeak-wunderground/api``` to the dragino gateway ```/usr/sbin/api```

Make sure ```/usr/sbin/api``` have the executable right. Connect with a SCP/SFTP client like Putty

```chmod u+x /usr/sbin/api```
