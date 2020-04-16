# Export and visualise data with InfluxDB and grafana on Debian 9
As root :
```
apt-get install -y curl apt-transport-https
curl -sL https://repos.influxdata.com/influxdb.key | apt-key add -
echo "deb https://repos.influxdata.com/debian stretch stable" > /etc/apt/sources.list.d/influxdb.list
apt-get update
apt-get install -y influxdb
systemctl enable influxdb
systemctl start influxdb
```

Modify ```/etc/influxdb/influxdb.conf``` http section to contain the following.

```
[http]
 auth-enabled = true
```
```
systemctl start influxdb
```

Type ```influx``` to access shell :  

```
create database "sensor_data"
create user "admin" with password 'PASS??'
grant all privileges to admin
create user "iot" with password 'PASS??'
grant all on "sensor_data" to "iot"
create user "grafana" with password 'PASS??'
grant read on "sensor_data" to "grafana"
```
Try to send data :

```
curl -i -XPOST 'http://localhost:8086/write?db=sensor_data' -u iot:PASS?? --data-binary "test temp=10 $(date '+%s')000000000"
```

Go influx to access shell :  
```
$ influx -username admin -password PASS??
Connected to http://localhost:8086 version 1.7.10
InfluxDB shell version: 1.7.10
> use sensor_data
Using database sensor_data
> SELECT temp FROM test
name: test
time                temp
----                ----
1586953507000000000 10
```

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
INFLUXDB_LOGIN=iot
INFLUXDB_PASS=PASS???
URL_BASE=http://hostname/
INFLUXDB_DB=sensor_data
```

Curl requests are made to send data. Make sure the gateway has internet connectivity to do so.

With a SCP/SFTP client like WinSCP, copy the file ```dragino/influxdb/api``` to the dragino gateway ```/usr/sbin/api```

Make sure ```/usr/sbin/api``` have the executable right. Connect with a SCP/SFTP client like Putty

```chmod u+x /usr/sbin/api```

```
apt-get install -y apt-transport-https software-properties-common wget
wget -q -O - https://packages.grafana.com/gpg.key | apt-key add -
add-apt-repository "deb https://packages.grafana.com/oss/deb stable main"
apt-get update
apt-get install grafana
systemctl daemon-reload
systemctl enable grafana-server
systemctl start grafana-server
```

Access to grafana on http://hostname:3000/ (login : admin/admin)

Configure data source -> InfluxDB

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/grafana_influx_db_config.png" width="200">

Create new dashboard, import file ```dragino/influxdb/dashboard.json```


<img src="https://raw.github.com/luigi1809/connectedHives/master/img/dashboard.png" width="200">

Optionaly, you can use Apache as TLS reverse-proxy :



```
<VirtualHost *:443>
(...)
        ProxyPassMatch ^/influxdb/(.*) http://127.0.0.1:8086/$1

        ProxyPassMatch ^/grafana/(.*) http://127.0.0.1:8099/$1
        RewriteRule ^/grafana$ https://%{SERVER_NAME}/grafana/ [END,NE,R=permanent]
(...)
</VirtualHost>
```

File ```/etc/grafana/grafana.ini```
```
domain = yourhostname

# The full public facing url you use in browser, used for redirects and emails
# If you use reverse proxy and sub path specify full url (with sub path)
;root_url = %(protocol)s://%(domain)s:%(http_port)s/
root_url = https://%(domain)/grafana/

# Serve Grafana from subpath specified in `root_url` setting. By default it is set to `false` for compatibility reasons.
serve_from_sub_path = false
```

