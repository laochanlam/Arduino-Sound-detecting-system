
#include <EtherCard.h>

#define STATIC 0  // set to 1 to disable DHCP (adjust myip/gwip values below)

#if STATIC
// ethernet interface ip address
static byte myip[] = { 192,168,0,10 };
// gateway ip address
static byte gwip[] = { 192,168,0,1 };
#endif

// ethernet mac address 
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[500]; // tcp/ip send and receive buffer

const char page[] PROGMEM =

"<html>"
  "<head><title>"
    "Home"
    "</title></head>"
  "<body>"
   "<font size=8>SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!</font>"
  " <font size=8>SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!</font>"
   "<font size=8>SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!SHUT UP!!!!</font>"
  "</body>"
"</html>"
;


const char orgpage[] PROGMEM =

"<html>"
  "<head><title>"
    "Home"
    "</title></head>"
  "<body>"
   "<font size=8>Please Be quiet</font>"
  
  "</body>"
"</html>"
;

int sensorPin = A5; // select the input pin for the potentiometer

int sensorValue = 0;
int counter = 0;
int ledPin = 7; 

void setup(){
  Serial.begin(9600);
  Serial.println("\nGroup 8");
  pinMode(ledPin, OUTPUT);
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
#if STATIC
  ether.staticSetup(myip, gwip);
#else
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
#endif
  Serial.println("");
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GateWay:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  
  Serial.println("");
  delay(2000);
}

void loop(){
  // wait for an incoming TCP packet, but ignore its contents
  if (ether.packetLoop(ether.packetReceive())) 
    if (counter >= 10)
    {
    memcpy_P(ether.tcpOffset(), page, sizeof page);
    ether.httpServerReply(sizeof page - 1);
    }

   if (counter < 10)
    {
    memcpy_P(ether.tcpOffset(), orgpage, sizeof orgpage);
    ether.httpServerReply(sizeof orgpage - 1);
    }

    
   sensorValue = analogRead(sensorPin);
   while (sensorValue > 40)
   {
   counter = counter +1;
   digitalWrite(ledPin, HIGH);
   Serial.println("");
   Serial.print("Level of Sound = ");
   Serial.println(sensorValue, DEC);
   Serial.println("Over the Maximum Value !!!!");
   Serial.print("count = ");
   Serial.println(counter, DEC);
   Serial.println("");
   delay(2000);
   if (counter == 10)
    {
    Serial.println("");
    Serial.print("The Website Have been Changed !!!!!!");
    Serial.println("");
    delay(5000);
    }
    if (counter < 10)
    digitalWrite(ledPin, LOW);
   sensorValue = analogRead(sensorPin);
   }


  Serial.print("Level of Sound = ");
  Serial.println(sensorValue, DEC);

}
