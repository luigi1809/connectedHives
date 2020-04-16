# connectedHives
Arduino connected hive sensor with more than one year autonomy. Connected to a private and self-managed LoRa Gateway.

Features - monitor :
* Weight (Kg)
* Luminosity (W/m²)
* In-hive temperature (°C) and humidity (%) 

Data are sent to cloud [The Things Network](https://www.thethingsnetwork.org/) and [Weather Underground](https://www.wunderground.com/weather/api) or your own grafana instance.

<img src="https://raw.github.com/luigi1809/connectedHives/master/img/scale.jpg" width="600">
<img src="https://raw.github.com/luigi1809/connectedHives/master/img/hive.jpg" width="300">

Documentation
--------------------
[Hardware list](https://github.com/luigi1809/connectedHives/blob/master/doc/hardware_list.md)

[Dragino LG01-P Gateway setup](https://github.com/luigi1809/connectedHives/blob/master/doc/gateway.md)

[LoRa Scale - electronic setup](https://github.com/luigi1809/connectedHives/blob/master/doc/lora_scale.md)

[LoRa Scale - scale cell calibration and arduino sketch upload](https://github.com/luigi1809/connectedHives/blob/master/doc/calibration.md)

[Watchdog script on Dragino LG01-P Gateway](https://github.com/luigi1809/connectedHives/blob/master/doc/watchdog.md)

[LoRa radio connectivity](https://github.com/luigi1809/connectedHives/blob/master/doc/lora.md)

Export and visualize data :

* [Send data to cloud WeatherUnderground and Thingspeak](https://github.com/luigi1809/connectedHives/blob/master/doc/cloud.md)
* or [Send data to your own grafana on debian 9](https://github.com/luigi1809/connectedHives/blob/master/doc/cloud.md)