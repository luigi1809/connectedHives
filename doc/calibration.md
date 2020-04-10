# Scale calibration and scale setup

Put a reference load on the scale. You know its weight and you want to calibrate the LoRa to report the same value. It is recommanded to use a heavy load (e.g. 50 Kg)

To do so upload the following sketch ```Arduino/calibration-load-sensor/calibration-load-sensor.ino```
to the scale arduino. Use the console in Tools --> Serial monitor. Your keyboard keys + / - will allow you to find the correct value of calibration_factor to match the weight.


Put the found calibration in ```Arduino/Client-0/Client-0.ino```

```
float calibration_factor = -44790;
```

Then upload Client-0.ino sketch to the Scale arduino.