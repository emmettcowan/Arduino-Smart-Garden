#include <ArduinoHttpClient.h>
#include "WiFiEsp.h"
#include "Pins.h"
#include "LM35.h"
#include "DHT11.h"
#include "Soilmoist.h"
#include "LDR.h"
#include "pump.h"

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int menu= 0;
double airtemp =0;
double hum =0;
double soilMoist=0;
int buttonpin=10;
int buttonState;          
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 120;
uint32_t Timer=0;
uint32_t waterTimer=0;
long GoogleDoc = 300000; //5 mins    
long watertimeset = 10000;   //this sets how offten it posts to the google doc


/************WIFI****************************************************/
char ssid[] = "iPhone";            
char pass[] = "emmett99";        
int status = WL_IDLE_STATUS;
#ifndef HAVE_HWSERIAL1
#include "SoftwareSerial.h"
SoftwareSerial ESP_Serial(6, 7); // RX, TX
#endif
WiFiEspServer server(80);
RingBuffer buf(8);
void printWifiStatus();
void GoogleDocPost();
/*******************************************************************/


void setup()
{
  PinsInit();
  lcd.begin(16,2);
  /**********************WIFI inislize ****************************/
  Serial.begin(115200);  
  Serial.println("GET /pushingbox?devid=v3E7CF164E3EEB39&temp=" + (String) airtemp + "&hum=" + (String)soilMoist );
  ESP_Serial.begin(9600); 
  WiFi.init(&ESP_Serial);  
  Serial.print("starting");
  lcd.clear();
  lcd.setCursor(0,1);   
  lcd.print("starting WiFi");
  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print(F("Attempting to connect to WPA SSID: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }
  lcd.clear();
  lcd.print("WiFi connected");
  Serial.println(F("You're connected to the network"));
  printWifiStatus();
  // start the web server on port 80

  server.begin();
  /*********************************************************************/

  mainmenu();
  
  Timer=millis();

  pinMode(buttonpin,INPUT);
  
}

void loop()
{
  /********************WIFI MAIN CODE***********************************************************/
  WiFiEspClient client = server.available();  // listen for incoming clients
  if (client) {                                 // if you get a client,
    lcd.clear();
    lcd.print("New client");
    Serial.println(F("New client"));             // print a message out the serial port
    buf.init();                               // initialize the circular buffer
    while (client.connected()) {              // loop while the client's connected
      if (client.available()) {               // if there's bytes to read from the client,
        char c = client.read();               // read a byte, then
        buf.push(c);                          // push it to the ring buffer
        if (buf.endsWith("\r\n\r\n")) {
          sendHttpResponse(client);
          break;
        }
        if (buf.endsWith("/H")) {
          Serial.println("Turn Pump ON");
          waterplant();
        }
      }
    }
    // close the connection
    client.stop();
    lcd.clear();
    lcd.print("client disconted");
    Serial.println(F("Client disconnected"));
    mainmenu();
  }
  /********************************************************************************************/

  /****************************post to google docs*********************************************/
    
    if((millis() - Timer ) > GoogleDoc){
       airtemp = returnDHT11Temp();
       soilMoist = readsoil();

       if(client.connect("api.pushingbox.com", 80))
       { 
          client.print("GET /pushingbox?devid=v3E7CF164E3EEB39&temp=" + (String) airtemp + "&hum=" + (String)soilMoist );
          client.println(" HTTP/1.1"); 
          client.print("Host: ");
          client.println("api.pushingbox.com");
          client.println("User-Agent: ESP8266/1.0");
          client.println("Connection: close");
          client.println();
          Timer=millis();
        }
    }
    
  /********************************************************************************************/
  
  /****************************BUTTON Controlled Menu******************************************/

  int reading = digitalRead(buttonpin);

  if (reading != lastButtonState)  {
    lastDebounceTime = millis();
  }

  if ((millis()-lastDebounceTime) > debounceDelay) {
    if(reading == 0){
       mainmenu();
       reading = !reading;
       menu++;
    } 
  }
  digitalWrite(13,0);
  lastButtonState = reading;

  /*********************************************************************************************/

  /******************************************WATER PLANT****************************************/

  if((millis() - waterTimer ) > watertimeset){
       waterplant();
       waterTimer=millis();
    }


  /*********************************************************************************************/
}

void mainmenu()
{
   switch(menu){
     case 0:
       lcd_Print_Dht();
       break;
     case 1:
       lcd_print_moist();
       break;
     case 2:
      lcd_soiltemp();
      break;
     case 3:
      LDRLcd();
      break;
    }
    if(menu >= 3){
      menu = -1;
    }
  
}

/***********************WiFi funcitons*********************************************************************************************/
void sendHttpResponse(WiFiEspClient client)
{
 // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
    client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nConnection: close\r\n\r\n"));
  client.println(F(R"=====( 
         <!DOCTYPE html>
    <html>
    <head>
    <style>
    body {
      background-color: lightgreen;
    }
    
    h1 {
      color: white;
      text-align: center;
      border: 
    }
    .sideplant{
      position: relative;
      left: 72.5%;
      top:-200px;
      max-width: 25%;
      height: auto;
    }
    h2 {
     background-color:#4CAF50;
      color : white;
      text-align: center;
    }
    .gmitimg{
      position: absolute;
      top: 10px;
      right: 10;
      width: 200px;
      height: 60px;
      border: 2px solid #4d4d4d;
    }
    
    .button {
      width: 70%;
      position: absolute;
      top: 380px;
      left: 10;
      background-color: #4CAF50;
      border: none;
      color: white;
      padding: 15px 32px;
      text-align: center;
      text-decoration: none;
      display: inline-block;
      font-size: 16px;
      margin: 4px 2px;
      cursor: pointer;
      
      font-size: 24px;
    }
    p {
      font-family: verdana;
      font-size: 20px;
    }
    table{
      border-collapse: collapse;
        width: 70%;
    }
    tr{
      background-color:MediumSpringGreen  ;
    }
    table, td, th {
      border-top: 2px solid #4d4d4d;
      border-bottom: 2px solid #4d4d4d;
      padding: 15px;
    }
    tr:hover {background-color: #f5f5f5;}
    button:hover {background-color:mediumseaGreen;}
    </style>
    </head>
    <body>
    
    <h1>Arduino Smart Garden</h1>
    <h2>by Emmett Cowan </h2>
    <img class="gmitimg"src="http://www.denisemcdonagh.com/wp-content/uploads/2018/02/GMIT-LOGO.jpg">
    <table>
      <tr>
        <td>Temprature</td>
     )====="));
  client.println("<td>");
  client.println(returnDHT11Temp());
  client.println("</td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>Humidity (in %)</td>");
  client.println("<td>");
  client.println(returnDHT11Humidity());
  client.println("</td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>soil moisture ( 0-10) </td>");
  client.println("<td>");
  client.println(readsoil());
  client.println("</td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>Soil Temp</td>");
  client.println("<td>");
  client.println(gettempC(LM35_Pin));
  client.println("</td>");
  client.println("</tr>");
  client.println("<tr>");
  client.println("<td>Light Level ( 0-100) </td>");
  client.println("<td>");
  client.println(readLight());
  client.println("</td>");
  client.println("</tr>");   
  client.println(F(R"=====( 
      </tr>
    </table> 
    <a href=\"/H\""><button class="button"> water plant </button></a>
    <img class="sideplant"src="http://2.bp.blogspot.com/-yEK4-90HcX0/UsREQo_aieI/AAAAAAAAAFE/5h4akN3CDIY/s1600/artshare_ru-plants-2.png">
    </body>
    </html>
     )====="));
     

}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();

  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
/************************************************************************************************************************************************/

void GoogleDocPost(){
   
}
