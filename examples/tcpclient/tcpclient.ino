#include <SPI.h>
#include <CC3000.h>

/* the current way of connecting is to use smart config.
 * smart config involves downloading an app (iphone: search TI cc3000 in the app store)
 * (android: gotta build your own app from http://processors.wiki.ti.com/index.php/CC3000_First_Time_Configuration)
 * use the following settings on the app:
 *     fill in your network name and password
 *     the device name is CC3000
 *     Arduinos can't run AES encryption, so do not check the encryption key
 *     
 * just uncomment out the WiFi.begin(ssid, pass) if you want to connect a different way
 * 
 * this example auto streams from google
 */

char ssid[] = "Kung";   //  your network SSID (name) 
char pass[] = "0767127141";    // your network password (use for WPA, or use as key for WEP)
// int keyIndex = 0;       // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  // //serial.println("Send anything.");
  // while (!Serial.available()) { continue; }
  
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    //serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);
  } 
  
  pinMode(3, INPUT);
 // use last time's configs to connect if possible
  //WiFi.begin();
  
  // attempt to connect to Wifi network:
  while ( WiFi.status() != WL_CONNECTED) { 
    
    
    // If you uncomment this block, be sure to comment out WiFi.begin() or else the cc3000
    // will attempt to connect more than once
    //serial.print("Attempting to connect to SSID: ");
    //serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:    
    WiFi.begin(ssid, pass);
    
    
    // hang until smart config
    if (digitalRead(3) == HIGH){
      status = WiFi.beginSmartConfig();
    }
  } 

  //serial.println("Connected to wifi");
  printWifiStatus();
  
  //serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    //serial.println("connected to server");
    // Make a HTTP request:
    client.println("GET /search?q=arduino HTTP/1.1");
    client.println("Host: www.google.com");
    client.println("Connection: close");
    client.println();
  }
}

void loop() {
  // if there are incoming bytes available 
  // from the server, read them and print them:
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    //serial.println();
    //serial.println("disconnecting from server.");
    client.stop();

    // do nothing forevermore:
    while(true);
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  //serial.print("SSID: ");
  //serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  //serial.print("IP Address: ");
  //serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  //serial.print("signal strength (RSSI):");
  //serial.print(rssi);
  //serial.println(" dBm");
}