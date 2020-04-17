# LoRa connectivity

If Arduino reaches the LoRa gateway, the led blinks one time. If not, two times.

For better radio range, change settings from ```Arduino/Client-0/Client-0.ino```.
```
rf95.setTxPower(13);
```
to
```
rf95.setModemConfig(RH_RF95::Bw31_25Cr48Sf512);  //set for pre-configured long range
rf95.setTxPower(20);  //set for 100mw
```

Other setModemConfig includes (longer is the range, lower is the battery life)
<table class="fieldtable">
<tr><th colspan="2">Enumerator</th></tr><tr><td class="fieldname"><a id="ab9605810c11c025758ea91b2813666e3a36c80b8c976c6209e9c929669a0ab320"></a>Bw125Cr45Sf128&#160;</td><td class="fielddoc"><p>Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Default medium range. </p>
</td></tr>
<tr><td class="fieldname"><a id="ab9605810c11c025758ea91b2813666e3a1759bc33893056f011b81e62e5c8af5f"></a>Bw500Cr45Sf128&#160;</td><td class="fielddoc"><p>Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. Fast+short range. </p>
</td></tr>
<tr><td class="fieldname"><a id="ab9605810c11c025758ea91b2813666e3a78f89711f429422d920dc22dbc278548"></a>Bw31_25Cr48Sf512&#160;</td><td class="fielddoc"><p>Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. Slow+long range. </p>
</td></tr>
<tr><td class="fieldname"><a id="ab9605810c11c025758ea91b2813666e3a241b28dcbb17f8a86e3f8edd03c51b21"></a>Bw125Cr48Sf4096&#160;</td><td class="fielddoc"><p>Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. Slow+long range. </p>
</td></tr>
</table>

This is useful for debugging purpose but this is battery consuming as the gateway waits for a reply before switching off the module (for selected time set on TPL5110 module). To save battery, set ```DEBUG_LED_ENABLE``` to 0 on ```Arduino/Client-0/Client-0.ino```.

```
# define DEBUG_LED_ENABLE 0
```