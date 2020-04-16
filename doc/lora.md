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

This is useful for debugging purpose but this is battery consuming as the gateway waits for a reply before switching off the module (for selected time set on TPL5110 module). To save battery, comment out or remove the following code from ```Arduino/Client-0/Client-0.ino```.

```
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];//Reply data array
uint8_t len = sizeof(buf);//reply data length

if (rf95.waitAvailableTimeout(3000))// Check If there is reply in 3 seconds.
{
    // Should be a reply message for us now   
    if (rf95.recv(buf, &len))//check if reply message is correct
   {
        if(buf[0] == node_id[0] && buf[1] == node_id[1] && buf[2] == node_id[2] ) // Check if reply message has the our node ID
       {
          pinMode(A1, OUTPUT);
          digitalWrite(A1, HIGH);
          Serial.print("Got Reply from Gateway: ");//print reply
          for (int i=0; i<len; i++){
             Serial.print(buf[i],HEX);
          }
          Serial.print(" - ");
          for (int i=3; i<len; i++){
             Serial.print((char)buf[i]);
          }
          Serial.println("");
          
           delay(400);
           digitalWrite(A1, LOW); 
           //Serial.print("RSSI: ");  // print RSSI
           //Serial.println(rf95.lastRssi(), DEC);        
       }else{
           Serial.println("got a resp with another node id");//
           delay(10000);
           rf95.send(sendBuf, strlen((char*)sendBuf));//resend if no reply
        }
    }
    else
    {
        Serial.println("recv failed");//
        pinMode(A1, OUTPUT);
        for (int j=0; j<3;j++){
            digitalWrite(A1, HIGH);
            delay(500);
            digitalWrite(A1, LOW);
            delay(500);
        }
        delay(10000);
        rf95.send(sendBuf, strlen((char*)sendBuf));//resend if no reply
    }
}
else
{
    Serial.println("No reply, is LoRa gateway running?");//No signal reply
    pinMode(A1, OUTPUT);
    for (int j=0; j<2;j++){
      digitalWrite(A1, HIGH);
      delay(500);
      digitalWrite(A1, LOW);
      delay(500);
    }
    delay(10000);
    rf95.send(sendBuf, strlen((char*)sendBuf));//resend data
}
Serial.println("");
```