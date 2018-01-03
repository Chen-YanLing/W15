#include <ArduinoJson.h>
#include <LWiFi.h>
#define LEDR 17
#define LEDG 16
#define LEDB 15
const char* ssid     = "apple";           // insert your SSID
const char* pass     = "starapple";   // insert your password

WiFiClient client;

const char* server = "opendata.epa.gov.tw";  
const char* resource = "/ws/Data/ATM00625/?$format=json"; 
const char* server1 = "maker.ifttt.com";  
const char* resource1 = "/trigger/PM25/with/key/xhIqXHJUek2jKP63U9w18"; 
char json[12000]; 
void turnoff(){
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}


void setup(){
  
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  turnoff();
  
  Serial.begin(115200);
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println();
}

void loop(){
 // connect to server  
  while(!client.connect(server, 80)){
   // wait for connecting server...
  }

  //Send request to resource
  client.print("GET ");
 client.print(resource);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server);
  client.println("Connection: close");
  client.println();

  delay(1000);

  //Reading stream
  client.setTimeout(10000);
   
  //Checking for the end of HTTP stream
  while(!client.find("\r\n\r\n")){
    // wait for finishing header stream reading ...
 }Serial.print("do not woek");

  // Reading the response string
  client.readBytes(json, 12000);

  // process JSON from response string
  DynamicJsonBuffer jsonBuffer;
  JsonArray& rootArray = jsonBuffer.parseArray(json);
  JsonObject& guTingSite = rootArray[0];

 const char* site0 = guTingSite["Site"];
 double pm250 = guTingSite["PM25"];
  
  if(pm250>50){turnoff();
  digitalWrite(LEDR, HIGH);
  delay(1000);

  
  while(!client.connect(server1, 80)){
    // wait for connecting server...
  }
  Serial.println("IFTTT has been connected");
  //Send request to resource
//  client.print("GET https://");
//  client.print(server1);
//  client.println(resource1);
  
  client.print("GET ");
  client.print(resource1);
  //client.print("?value1=");
   //client.println("PM");
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(server1);
  client.println("Connection: close");
  client.println();

  delay(1000);}
  else{turnoff();
  digitalWrite(LEDG, HIGH);
  delay(1000);}
  
   Serial.println("IFTTT has sent");
  // Print data to Serial
 Serial.print("***");
  Serial.print(site0);
  Serial.println(" ***");
  Serial.print("PM25: ");
Serial.println(pm250);
  Serial.println("--------------------"); 

  

  client.stop(); // disconnect from server
  

 
  
  delay(2000); // the OWM free plan API does NOT allow more then 60 calls per minute

 
}


