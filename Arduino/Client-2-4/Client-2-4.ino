/*
  Upload Data to IoT Server ThingSpeak (https://thingspeak.com/):
  Support Devices: LoRa Shield + Arduino 
  
  Example sketch showing how to read Temperature and Humidity from DHT11 sensor,  
  Then send the value to LoRa Gateway, the LoRa Gateway will send the value to the 
  IoT server

  It is designed to work with the other sketch dht11_server. 

  modified 24 11 2016
  by Edwin Chen <support@dragino.com>
  Dragino Technology Co., Limited
*/

#include <SPI.h>
#include <RH_RF95.h>

# define DEBUG_LED_ENABLE 0

RH_RF95 rf95;
unsigned int node_id[3] = {0,0,4}; //LoRa End Node ID
float frequency = 868.1;
unsigned int count = 1;

/** Broche "DATA" du capteur */
const byte BROCHE_CAPTEUR = 5;

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
    rf95.setTxPower(15);
    
    Serial.println("LoRa End Node"); 
    Serial.println("    DHT22 Temperature and Humidity Sensor\n");
    Serial.print("LoRa End Node ID: ");

    for(int i = 0;i < 3; i++)
    {
        Serial.print(node_id[i],HEX);
    }
    Serial.println();
    pinMode(3, OUTPUT);

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
    state=1; 
    float temperature, humidity;
    dataLength = 7;
    // Use data[0], data[1],data[2] as Node ID
    data[0] = node_id[0] ;
    data[1] = node_id[1] ;
    data[2] = node_id[2] ;
    for (int i=3; i<dataLength; i++){
      data[i] = 255 ;
    }
    /* Lecture de la température et de l'humidité, avec gestion des erreurs */
    // N.B. Remplacer readDHT11 par readDHT22 en fonction du capteur utilisé !
    switch (readDHT22(BROCHE_CAPTEUR, &temperature, &humidity)) {
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
        state=0;
        break;
     
      case DHT_CHECKSUM_ERROR: 
        Serial.println(F("Checksum error sonde de temperature !"));
        state=0;
        break;
    }
    if (state){
        loraSend(data,dataLength);
    }
    digitalWrite(3, HIGH);
    delay(100);
    digitalWrite(3, LOW);
    delay(900000); // Send sensor data every 15 minutes
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
              pinMode(4, OUTPUT);
              digitalWrite(4, HIGH);
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
               digitalWrite(4, LOW); 
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
            pinMode(4, OUTPUT);
            for (int j=0; j<3;j++){
                digitalWrite(4, HIGH);
                delay(500);
                digitalWrite(4, LOW);
                delay(500);
            }
            delay(10000);
            rf95.send(sendBuf, strlen((char*)sendBuf));//resend if no reply
        }
    }
    else
    {
        Serial.println("No reply, is LoRa gateway running?");//No signal reply
        pinMode(4, OUTPUT);
        for (int j=0; j<2;j++){
          digitalWrite(4, HIGH);
          delay(500);
          digitalWrite(4, LOW);
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
