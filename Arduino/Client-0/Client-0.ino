/*
  Upload Data to Lora Gateway Dragino LG01-P
  Support Devices: Dragino LoRa Shield + Arduino Uno R3 + Adafruit TPL5110 power breakout (for power saving)
  
  Data :
        * temperature humidity from DHT22 sensor
        * Lux solar radiation from CJMCU-3001/OPT3001 sensor 
        * load sensor with HX711 PCB
        
  Pin :
        * lux OPT3001 - SDA A4 - SCL A5
        * tpl5110 A0 signal to 1 logic level to break out the circuit
        * led A1 light on one time if gateway LG01-P gets the signal or two time otherwise
        * DHT22 D5
        * HX711 PCB scale DOUT D4 - CLK D3
*/


#include <SPI.h>
#include <RH_RF95.h>
#include <Wire.h>
#include "HX711.h"

#define DOUT 4
#define CLK  3

# define DEBUG_LED_ENABLE 1

HX711 scale(DOUT, CLK);
float calibration_factor = -44790;

RH_RF95 rf95;
unsigned int node_id[3] = {0,0,0}; //LoRa End Node ID
float frequency = 868.1;
unsigned int count = 1;

/** Broche "DATA" du capteur */
const byte BROCHE_CAPTEUR_DHT = 5;

// broche capteur lux SDA A4 - SCL A5
// tpl5110 A0
// led A1

/* Code d'erreur de la fonction readDHT11() et readDHT22() */
const byte DHT_SUCCESS = 0;        // Pas d'erreur
const byte DHT_TIMEOUT_ERROR = 1;  // Temps d'attente dépassé
const byte DHT_CHECKSUM_ERROR = 2; // Données reçues erronées
unsigned int data[16] ;
int dataLength = 7; // Payload Length
bool state;

void setup()
{
    Serial.begin(9600);
    if (!rf95.init())
        Serial.println("init failed");
    // Setup ISM frequency
    rf95.setFrequency(frequency);
    // Setup Power,dBm
    rf95.setTxPower(13);
    
    Serial.println("LoRa End Node"); 
    Serial.println("    DHT22 Temperature, Lux and Humidity Sensor\n");
    Serial.print("LoRa End Node ID: ");

    for(int i = 0;i < 3; i++)
    {
        Serial.print(node_id[i],HEX);
    }
    Serial.println();

    //Lux part
    Wire.begin();                                  // Initialize Arduino in I2C master.
    Wire.beginTransmission(0x44);                  // I2C address of OPT3001 = 0x44
    Wire.write(0x01);                              // Config register address 0x01
    Wire.write(0xCE);
    Wire.write(0x10);                              // Write 0xCE10 to turn on sensor
    Wire.endTransmission();
    Serial.println("Data received \t\t Lux");
    pinMode(A0, OUTPUT);
    pinMode(A3, OUTPUT);
    digitalWrite(A3, HIGH);

    
    //load sensor
    scale.set_scale();
    //scale.tare(); //Reset the scale to 0
    scale.set_offset(151000);
}


uint16_t calcByte(uint16_t crc, uint8_t b)
{
    uint32_t i;
    crc = crc ^ (uint32_t)b << 8;
    
    for ( i = 0; i < 8; i++)
    {
        if ((crc & 0x8000) == 0x8000)
            crc = crc << 1 ^ 0x1021;
        else
            crc = crc << 1;
    }
    return crc & 0xffff;
}

uint16_t CRC16(unsigned int *pBuffer,uint32_t length)
{
    uint16_t wCRC16=0;
    uint32_t i;
    if (( pBuffer==0 )||( length==0 ))
    {
      return 0;
    }
    for ( i = 0; i < length; i++)
    { 
      wCRC16 = calcByte(wCRC16, pBuffer[i]);
    }
    return wCRC16;
}

void loop(){
    //delay(3);// delay to intialize lux sensor
    float temperature, humidity;
    dataLength = 11;
    // Use data[0], data[1],data[2] as Node ID
    data[0] = node_id[0] ;
    data[1] = node_id[1] ;
    data[2] = node_id[2] ;
    for (int i=3; i<dataLength; i++){
      data[i] = 255 ;
    }
    /* Lecture de la température et de l'humidité, avec gestion des erreurs */
    // N.B. Remplacer readDHT11 par readDHT22 en fonction du capteur utilisé !
    switch (readDHT22(BROCHE_CAPTEUR_DHT, &temperature, &humidity)) {
      case DHT_SUCCESS: 
        /* Affichage de la température et du taux d'humidité */
        Serial.print(F("Humidite (%): "));
        Serial.println(humidity, 2);
        Serial.print(F("Temperature (°C): "));
        Serial.println(temperature, 2);
        data[3] = temperature;//Get Temperature Integer Part
        data[4] = abs((temperature-(int)temperature)*100);//Get Temperature Decimal Part
        data[5] = humidity;//Get humidity Integer Part
        data[6] = (humidity-(unsigned int)humidity)*100;//Get humidity Decimal Part
        break;
     
      case DHT_TIMEOUT_ERROR: 
        Serial.println(F("Pas de reponse de la sonde de temperature!"));
        data[3]= 0;
        data[4]= 0;
        data[5]= 0;
        data[6]= 0;
        break;
     
      case DHT_CHECKSUM_ERROR: 
        Serial.println(F("Checksum error sonde de temperature !"));
        break;
    }
    //scale
    scale.set_scale(calibration_factor); //Adjust to this calibration factor
    Serial.print("Reading: ");
    float kg;
    kg=scale.get_units()*2.20462 - 9.2;
    Serial.print(kg, 1);
    Serial.print(" Kg"); //Change this to kg and re-adjust the calibration factor if you follow SI units like a sane person
    data[9] = kg;//Get humidity Integer Part
    data[10] = (kg-(unsigned int)kg)*100;//Get humidity Decimal Part
    
    //Lux
    uint16_t rawLux=getLuxRawData();
    for (int time=0; (rawLux == 0) && (time <10); time++ ){
        rawLux=getLuxRawData();
        delay(1000);
    }
    data[7]=(rawLux & 0xFF00) >>8;
    data[8]=(rawLux & 0x00FF) ;
    Serial.println(F("Sending LoRa !"));
    loraSend(data,dataLength);
    Serial.println(F("Sent LoRa !"));
    Serial.println(F("Switching off!"));
    // signal TPL5110 to switch off arduino
    digitalWrite(A0, HIGH);
    delay(100);
    digitalWrite(A0, LOW);
    delay(900000); // Send sensor data every 15 minutes if module not switched off by TPL5110
}

float getLux() 
{
  float fLux;
  Wire.beginTransmission(0x44);
  Wire.write(0x00);                              // Send result register address
  Wire.endTransmission();
  delay(100);
  
  Wire.requestFrom(0x44, 2);                     // Request 2 bytes data from OPT3001
  uint16_t iData;
  uint8_t  iBuff[2];
  while (Wire.available()) 
  { 
    Wire.readBytes(iBuff, 2);
    iData = (iBuff[0] << 8) | iBuff[1];
    Serial.print(iData,BIN);                     // Print the received data
    Serial.print("\t\t");
    fLux = SensorOpt3001_convert(iData);   // Calculate LUX from sensor data
    Serial.println(fLux);                        // Print it on serial terminal
  }
  return fLux;
}

uint16_t getLuxRawData() 
{
  float fLux;
  Wire.beginTransmission(0x44);
  Wire.write(0x00);                              // Send result register address
  Wire.endTransmission();
  delay(100);
  
  Wire.requestFrom(0x44, 2);                     // Request 2 bytes data from OPT3001
  uint16_t iData;
  uint8_t  iBuff[2];
  while (Wire.available()) 
  { 
    Wire.readBytes(iBuff, 2);
    iData = (iBuff[0] << 8) | iBuff[1];
    Serial.print(iData,BIN);                     // Print the received data
    Serial.print("\t\t");
    fLux = SensorOpt3001_convert(iData);   // Calculate LUX from sensor data
    Serial.println(fLux);                        // Print it on serial terminal
  }
  return iData;
}
float SensorOpt3001_convert(uint16_t iRawData)
{
  uint16_t iExponent, iMantissa;
  iMantissa = iRawData & 0x0FFF;                 // Extract Mantissa
  iExponent = (iRawData & 0xF000) >> 12;         // Extract Exponent
  //Serial.println(iMantissa); 
  //Serial.println(iExponent); 
  return iMantissa * (0.01 * pow(2, iExponent)); // Calculate final LUX
}


int loraSend(unsigned int *data,int dataLength){
    Serial.print("###########    ");
    Serial.print("COUNT=");
    Serial.print(count);
    Serial.println("    ###########");
    count++;

    
    uint16_t crcData = CRC16((unsigned int*)data,dataLength);//get CRC DATA
    //Serial.println(crcData,HEX);
    
    Serial.print("Data to be sent(without CRC): ");
    
    int i;
    for(i = 0;i < dataLength; i++)
    {
        Serial.print(data[i],HEX);
        Serial.print(" ");
    }
    Serial.println();
        
    unsigned char sendBuf[50]={0};

    for(i = 0;i < dataLength;i++)
    {
        sendBuf[i] = data[i] ;
    }
    
    sendBuf[dataLength] = (unsigned char)crcData; // Add CRC to LoRa Data
    sendBuf[dataLength+1] = (unsigned char)(crcData>>8); // Add CRC to LoRa Data

    Serial.print("Data to be sent(with CRC):    ");
    for(i = 0;i < (dataLength +2); i++)
    {
        Serial.print(sendBuf[i],HEX);
        Serial.print(" ");
    }
    Serial.println();

    rf95.send(sendBuf, dataLength+2);//Send LoRa Data
    
    if (DEBUG_LED_ENABLE == 0){
      return 0;
    }
    
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
    return 0;
}

/**
 * Lit la température et le taux d'humidité mesuré par un capteur DHT11.
 *
 * @param pin Broche sur laquelle est câblée le capteur.
 * @param temperature Pointeur vers la variable stockant la température.
 * @param humidity Pointeur vers la variable stockant le taux d'humidité.
 * @return DHT_SUCCESS si aucune erreur, DHT_TIMEOUT_ERROR en cas de timeout, ou DHT_CHECKSUM_ERROR en cas d'erreur de checksum.
 */
byte readDHT11(byte pin, float* temperature, float* humidity) {
  
  /* Lit le capteur */
  byte dht_dat[5];
  byte ret = readDHTxx(pin, dht_dat, 18, 1000);
  
  /* Détecte et retourne les erreurs de communication */
  if (ret != DHT_SUCCESS) 
    return ret;
    
  /* Calcule la vraie valeur de la température et de l'humidité */
  *humidity = dht_dat[0];
  *temperature = dht_dat[2];

  /* Ok */
  return DHT_SUCCESS;
}

/**
 * Lit la température et le taux d'humidité mesuré par un capteur DHT22.
 *
 * @param pin Broche sur laquelle est câblée le capteur.
 * @param temperature Pointeur vers la variable stockant la température.
 * @param humidity Pointeur vers la variable stockant le taux d'humidité.
 * @return DHT_SUCCESS si aucune erreur, DHT_TIMEOUT_ERROR en cas de timeout, ou DHT_CHECKSUM_ERROR en cas d'erreur de checksum.
 */
byte readDHT22(byte pin, float* temperature, float* humidity) {
  
  /* Lit le capteur */
  byte dht_dat[5];
  byte ret = readDHTxx(pin, dht_dat, 1, 1000);
  
  /* Détecte et retourne les erreurs de communication */
  if (ret != DHT_SUCCESS) 
    return ret;
    
  /* Calcul la vraie valeur de la température et de l'humidité */
  float fh = dht_dat[0];
  fh *= 256;
  fh += dht_dat[1];
  fh *= 0.1;
  *humidity = fh;
 
  float ft = dht_dat[2] & 0x7f;
  ft *= 256;
  ft += dht_dat[3];
  ft *= 0.1;
  if (dht_dat[2] & 0x80) {
    ft *= -1;
  }
  *temperature = ft;

  /* Ok */
  return DHT_SUCCESS;
}

/**
 * Fonction bas niveau permettant de lire la température et le taux d'humidité (en valeurs brutes) mesuré par un capteur DHTxx.
 */
byte readDHTxx(byte pin, byte* dht_dat, unsigned long start_time, unsigned long timeout) {
  dht_dat[0] = dht_dat[1] = dht_dat[2] = dht_dat[3] = dht_dat[4] = 0;
  // start_time est en millisecondes
  // timeout est en microsecondes
 
  /* Conversion du numéro de broche Arduino en ports / masque binaire "bas niveau" */
  uint8_t bit = digitalPinToBitMask(pin);
  uint8_t port = digitalPinToPort(pin);
  volatile uint8_t *ddr = portModeRegister(port);   // Registre MODE (INPUT / OUTPUT)
  volatile uint8_t *out = portOutputRegister(port); // Registre OUT (écriture)
  volatile uint8_t *in = portInputRegister(port);   // Registre IN (lecture)
  
  /* Conversion du temps de timeout en nombre de cycles processeur */
  unsigned long max_cycles = microsecondsToClockCycles(timeout);
 
  /* Evite les problèmes de pull-up */
  *out |= bit;  // PULLUP
  *ddr &= ~bit; // INPUT
  delay(100);   // Laisse le temps à la résistance de pullup de mettre la ligne de données à HIGH
 
  /* Réveil du capteur */
  *ddr |= bit;  // OUTPUT
  *out &= ~bit; // LOW
  delay(start_time); // Temps d'attente à LOW causant le réveil du capteur
  // N.B. Il est impossible d'utilise delayMicroseconds() ici car un délai
  // de plus de 16 millisecondes ne donne pas un timing assez précis.
  
  /* Portion de code critique - pas d'interruptions possibles */
  noInterrupts();
  
  /* Passage en écoute */
  *out |= bit;  // PULLUP
  delayMicroseconds(40);
  *ddr &= ~bit; // INPUT
 
  /* Attente de la réponse du capteur */
  timeout = 0;
  while(!(*in & bit)) { /* Attente d'un état LOW */
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
  }
    
  timeout = 0;
  while(*in & bit) { /* Attente d'un état HIGH */
    if (++timeout == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
  }

  /* Lecture des données du capteur (40 bits) */
  for (byte i = 0; i < 40; ++i) {
 
    /* Attente d'un état LOW */
    unsigned long cycles_low = 0;
    while(!(*in & bit)) {
      if (++cycles_low == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
    }

    /* Attente d'un état HIGH */
    unsigned long cycles_high = 0;
    while(*in & bit) {
      if (++cycles_high == max_cycles) {
        interrupts();
        return DHT_TIMEOUT_ERROR;
      }
    }
    
    /* Si le temps haut est supérieur au temps bas c'est un "1", sinon c'est un "0" */
    dht_dat[i / 8] <<= 1;
    if (cycles_high > cycles_low) {
      dht_dat[i / 8] |= 1;
    }
  }
  
  /* Fin de la portion de code critique */
  interrupts();
 
  /*
   * Format des données :
   * [1, 0] = humidité en %
   * [3, 2] = température en degrés Celsius
   * [4] = checksum (humidité + température)
   */
   
  /* Vérifie la checksum */
  byte checksum = (dht_dat[0] + dht_dat[1] + dht_dat[2] + dht_dat[3]) & 0xff;;
  if (dht_dat[4] != checksum){
    //Serial.println("Bad CRC checksum");
    return DHT_CHECKSUM_ERROR; /* Erreur de checksum */
  }
  else{
    return DHT_SUCCESS; /* Pas d'erreur */
  }
}
