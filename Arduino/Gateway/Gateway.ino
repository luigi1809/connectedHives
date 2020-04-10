/*
  Upload Data to IoT Server ThingSpeak (https://thingspeak.com/):
  Support Devices: LG01 
  
  Example sketch showing how to get data from remote LoRa node, 
  Then send the value to IoT Server

  It is designed to work with the other sketch dht11_client. 

  modified 24 11 2016
  by Edwin Chen <support@dragino.com>
  Dragino Technology Co., Limited
*/

#include <FileIO.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Console.h>
#include <Process.h>
RH_RF95 rf95;

//If you use Dragino IoT Mesh Firmware, uncomment below lines.
//For product: LG01. 
#define BAUDRATE 115200

unsigned long myChannelNumber[15];
char * myWriteAPIKey[15];

uint16_t crcdata = 0;
uint16_t recCRCData = 0;
float frequency = 868.1;
String apiArgs[15];
int nbData;
int id;

const String Sketch_Ver = "LoraFwdLST20180608";

void uploadData(); // Upload Data to ThingSpeak.

void setup()
{
  Bridge.begin(BAUDRATE);
  Console.begin(); 
  // while(!Console);
  if (!rf95.init())
      Console.println("init failed");
  ;
  // Setup ISM frequency
  rf95.setFrequency(frequency);
  // Setup Power,dBm
  rf95.setTxPower(13);
  // Setup File IO
  FileSystem.begin();
  writeVersion();
  Console.println("LoRa Gateway");
  Console.println("    Upload Data to API");
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

uint16_t CRC16(uint8_t *pBuffer, uint32_t length)
{
    uint16_t wCRC16 = 0;
    uint32_t i;
    if (( pBuffer == 0 ) || ( length == 0 ))
    {
        return 0;
    }
    for ( i = 0; i < length; i++)
    {
        wCRC16 = calcByte(wCRC16, pBuffer[i]);
    }
    return wCRC16;
}

uint16_t recdata( unsigned char* recbuf, int Length)
{
    crcdata = CRC16(recbuf, Length - 2); //Get CRC code
    recCRCData = recbuf[Length - 1]; //Calculate CRC Data
    recCRCData = recCRCData << 8; //
    recCRCData |= recbuf[Length - 2];
}
void loop()
{
  if (rf95.waitAvailableTimeout(2000))// Listen Data from LoRa Node
  {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];//receive data buffer
    uint8_t len = sizeof(buf);//data buffer length
    if (rf95.recv(buf, &len))//Check if there is incoming data
    {
      if (len ==0){
        Console.println("A message has been loss!");
      }else{
        recdata( buf, len);
        Console.print("Get LoRa Packet: ");
        for (int i = 0; i < len; i++)
        {
          Console.print(buf[i],HEX);
          Console.print(" ");
        }
        Console.println();
        if(crcdata == recCRCData){ //Check if CRC is correct
          uint8_t data[] = "   Server ACK";//Reply 
          data[0] = buf[0];
          data[1] = buf[1];
          data[2] = buf[2];
          rf95.send(data, sizeof(data));// Send Reply to LoRa Node
          rf95.waitPacketSent();
          id = buf[0]*4 +  buf[1]*2 + buf[2];
          Console.print("Payload length with CRC: ");
          Console.println(len);
          nbData=len-4;
          int newData[nbData]; //Store Sensor Data here
          for (int i = 0; i < nbData; i++){
            newData[i] = buf[i + 3];
          }
          int th = newData[0];
          int tl = newData[1];
          int hh = newData[2];
          int hl = newData[3];
          Console.print("Get Temperature:");
          Console.print(th);
          Console.print(".");
          Console.println(tl);
          Console.print("Get Humidity:");
          Console.print(hh);
          Console.print(".");
          Console.println(hl);
          apiArgs[0] = id;
//          for (int i = 0; i < nbData/2; i++){
//            apiArgs[i+1] = newData[i*2];
//            apiArgs[i+1] +=".";
//            apiArgs[i+1] += newData[i*2+1];
//          }
          for (int i = 0; i < nbData; i++){
            apiArgs[i+1] = newData[i];
          }
          uploadData();
          Console.println("####################################");
          Console.println("");
        }else{
          uint8_t data[] = "   BAD CRC";//Reply 
          data[0] = buf[0];
          data[1] = buf[1];
          data[2] = buf[2];
          Console.println("bad CRC");
          rf95.send(data, sizeof(data));// Send Reply to LoRa Node
          rf95.waitPacketSent();
          Console.println("####################################");
          Console.println("");
        }
      }
    }else{
      Console.println("recv failed");
    }
  }
}

void uploadData() {//Send Data to API process
  Console.println("Call Linux Command to Send Data");
  
  Process p;    // Create a process and call it "p", this process will execute a Linux curl command
  Console.print("api ");
  p.begin("api");
//  for (int i = 0; i < (nbData/2+1); i++){
//    p.addParameter(apiArgs[i]);
//    Console.print(apiArgs[i]);
//    Console.print(" ");
//  }
  for (int i = 0; i < nbData; i++){
    p.addParameter(apiArgs[i]);
    Console.print(apiArgs[i]);
    Console.print(" ");
  }
  Console.println("");
  p.run();    // Run the process and wait for its termination

  Console.print("Feedback from Linux: ");
  // If there's output from Linux,
  // send it out the Console:
  while (p.available()>0) 
  {
    char c = p.read();
    Console.write(c);
  }
  Console.println("");
  Console.println("Call Finished");
}

//Function to write sketch version number into Linux.
void writeVersion()
{
  File fw_version = FileSystem.open("/var/avr/fw_version", FILE_WRITE);
  fw_version.print(Sketch_Ver);
  fw_version.close();
}
