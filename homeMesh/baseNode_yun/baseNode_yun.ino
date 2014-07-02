
// appears stable -- need to test messages

#include <Glue.h>
#include <Scissors.h>
#include <XBeeXtnd.h>

//for the yun 
// keep baud rate low 9600



#include <Bridge.h>
#include <AltSoftSerial.h>
#include <XBee.h>
//  #include <SoftwareSerial.h>
#include <LinkedList.h>
#include <LiquidCrystal.h> 


#include <YunServer.h>
#include <YunClient.h>

// Listen on default port 5555, the webserver on the Yun
// will forward there all the HTTP requests for us.
YunServer server;

Glue elmers;
Scissors cut;

// Define SoftSerial TX/RX pins
// Connect Arduino pin 8 to TX of usb-serial device
//uint8_t ssRX = 8;
// Connect Arduino pin 9 to RX of usb-serial device
//uint8_t ssTX = 9;
// Remember to connect all devices to a common Ground: XBee, Arduino and USB-Serial device
//SoftwareSerial nss(ssRX, ssTX);
LiquidCrystal  lcd(12,11,6,4,3,2);
XBee xbee = XBee();
XBeeXtnd xbxt; 
AltSoftSerial altSerial;

boolean  LIST_ACQUIRED = false;
boolean  nodeFound     = false; 

long addressPrefix = 0x13A200;

// node to find 
XBeeAddress64 nodeOfInterest = XBeeAddress64(0x13A200, 0x4054F0F7);
XBeeAddress64 dst  = XBeeAddress64(addressPrefix, 0x40311E08);
XBeeAddress64 sensorNode = XBeeAddress64(0x13A200, 0x409878DD);


int oldL;
#define timeInterval 15000
long timeOut = 5000;


void setup() {
  
     // initialize serial communication -- need these to see stuff in arduino
  Bridge.begin();
  Console.begin(); 
  

  
  Console.println("You're connected to the Console!!!!");
  
  // start the LCD
  lcd.begin(16, 2);
  altSerial.begin(9600); // no longer 9600
  //Serial.begin(38400);
  xbee.begin(altSerial);
  
  // create and extended object with LCD and radio 
  xbxt.xtCreate(xbee);
  xbxt.setTimeout(5);

  // start a software serial port as well (rendundancy is good) 
 
  delay(2000);

  
  // start code 
  Console.println("===start ( XbXT test_LCD ) ===");
  xbxt.xtSendAtCommand('N', 'I');
  Console.println("=== sent ===");
  
  // listen for commands from away
  server.listenOnLocalhost();
  server.begin();
  
    
  elmers.create();
  elmers.setDelimiter('/');
  
  cut.begin(0);
  cut.setDelimiter('/');
  
  
}


void loop() {
  
  
  // Get clients coming from server
  YunClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }
    
  // put your main code here, to run repeatedly: 
  // listend for any and all meassages -- this is needed for node  list calls too. 
// xbxt.sendZB( dst, "trying to do a send");
 int LISTEN = xbxt.xtListen();
 
 lcdMessage(xbxt.debugString,1);

   // Console.print( "l :: ");
   // Console.println( L );
   
   if ( LISTEN > 0  ) {

     // when I get a message from another ZB this is the resposne
      if (LISTEN == ZB ) {
         Console.print(" received xb message " );
         String s = xbxt.zbRxString;
         Console.println(s);
        
            // node discover is true you need to figure out how to manage time outs etc. 
      }
      
      // don't know why I have this
      if ( LISTEN == ZBSR ) {
         Console.print("Status response :: " );
         Console.println( xbxt.getStatus() );
      }
      
      // when I send a local AT command this is the response 
      if ( LISTEN == AT ) {
       Console.print( " AT command success :: " );
       Console.println(xbxt.zbRxString);
      }
      
  }  // end sort messages 
  


}


void printNodeList() {
 
  Console.println("Address list");
  for(int i = 0; i < xbxt.nodeList.size(); i++){ 
        XBeeAddress64 node = xbxt.nodeList.get(i);
        Console.print("0x");
        Console.print(node.getMsb(),HEX);
        Console.print("\t0x");
        Console.println(node.getLsb(),HEX);
                
   }
  
} // end print 



void process(YunClient client) {
  // read the command
  String webMessage = "";
  webMessage += "*";
  String temp = client.readString();
  temp.trim();
  webMessage += temp;
  webMessage += "/#";
  
  
  lcdMessage("got web message");
  Console.print("web mess: ");
  Console.println(webMessage);
  
  handleMessage( webMessage );
  
}

