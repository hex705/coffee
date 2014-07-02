
// appears stable -- need to test messages

#include <Glue.h>
#include <Scissors.h>

//  this is an UNO sensor node with xBee series2 radio
//  read sensors then send message


// clean up lib so that we don;t need to see teh LCD inclusion

// includes
#include <XBee.h>
#include <XBeeXtnd.h>

// for debug
#include <SoftwareSerial.h>

// for storage of nodes -- may have to stay wehre it is 
#include <LinkedList.h>

// Include required libraries
#include "DHT.h"
// DHT11 sensor pins
#define DHTPIN 7 
#define DHTTYPE DHT11

// DHT instance
DHT dht(DHTPIN, DHTTYPE);

// Variables
int lightLevel;
float humidity;
float temperature;

int lightPin = 0;  // analog 
int ledPin = 6;
int ledState = 0;

// xBee stuff 
XBee xbee = XBee();
XBeeXtnd xbxt; 


// constants and variables
uint8_t ssRX = 8;
// Connect Arduino pin 9 to RX of usb-serial device
uint8_t ssTX = 9;
// Remember to connect all devices to a common Ground: XBee, Arduino and USB-Serial device
SoftwareSerial nss(ssRX, ssTX);


// address of base xBee on YUN
XBeeAddress64 baseNode = XBeeAddress64(0x13A200, 0x40311E08); // NI = sensors

Glue elmers;
Scissors cut;


long readingTime;
int  readingTimeOut = 10000;


void setup() {
  pinMode(6,OUTPUT);
  digitalWrite(6,HIGH);
  delay(1000);
  digitalWrite(6,0);
  
    // Initialize DHT sensor
  dht.begin();
  
  
  // on an UNO we can use the serial port.
  Serial.begin(38400);
  xbee.begin(Serial);
  
    // create and extended object with LCD and radio 
  xbxt.xtCreate(xbee);
  
  
  // start a software serial port as well (rendundancy is good) 
  nss.begin(9600);
  
  elmers.create();
  elmers.setDelimiter('/');
  
  cut.begin(0);
  cut.setDelimiter('/');
  
  
  delay(2000);
  nss.println("=== started ( sensor node test ) === ");
  readingTime= millis();

}

void loop() {
  
  // use scissors and glue
  
  // look for messages from xBee radios (local or remote) 
   int LISTEN = xbxt.xtListen();
   
   if (LISTEN == ZB) {
     
       nss.print(" sensor received xb message " );
       String s = xbxt.zbRxString;
       handleMessage(s);
     
   }
   
   // read sensors and send every 5 minutes or on command
   
   if ( millis() - readingTime > readingTimeOut ) {
       readSensors();
       buildMessage();
       sendMessage( baseNode );
      
       readingTime = millis(); 
   }
 
   
}

void readSensors (){
   // Measure the humidity & temperature
   humidity = dht.readHumidity();
   temperature = dht.readTemperature(); 
   
   lightLevel = analogRead(lightPin);
    
}


void buildMessage(){
    elmers.clear();
    elmers.add("message");
    elmers.add("node23");
    elmers.add(float(humidity));
    elmers.add(float(temperature));
    elmers.add(lightLevel);
   
    nss.println(elmers.debug());
}


void sendMessage(XBeeAddress64 dst) {
  
   xbxt.sendZB( dst,  elmers.getPackage() );
   
}


  
  


