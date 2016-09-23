
#include <WiFi101.h>
#include <ArduinoJson.h>

//WiFiSSLClient sslClient;
WiFiClient client;

//static const char ssid[] = "Mydd2016";
//static const char pass[] = "mydd@2016";

static const char ssid[] = "L3wifi";
static const char pass[] = "abcde12345";

int status = WL_IDLE_STATUS;

// lowest and highest sensor readings:
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

char hostname[] = "api-m2x.att.com";
String uri = "/v2/devices/1abb8e9e9e984652d55837847a1532f2/streams/Ethanol/value";


void setup() {
    delay(10000);
    
    initSerial();

    initWifi();
 pinMode(9, OUTPUT);      // set the LED pin mode

    
}

void loop() {

  // read the sensor on analog A0:
  int sensorReading = analogRead(A1);
  // map the sensor range (four options):
  // ex: 'long int map(long int, long int, long int, long int, long int)'
  int range = map(sensorReading, sensorMin, sensorMax, 0, 3);
  
  // range value:
  switch (range) {
 case 0:    // (flood)
    Serial.println("A lot of Alcohol");
    httpGet();
digitalWrite(9, HIGH);   
    break;
 case 1:    // (rain)
    Serial.println("Urine");
    digitalWrite(9, LOW);
    break;
 case 2:    //(not rain)
    Serial.println("Not exceed the level of alcohol yet");
    digitalWrite(9, LOW);
    break;
  }
  delay(1);  // delay between reads
  
sendEvent(sensorReading);

  delay(10000);
}

void initSerial() {
    // Start serial and initialize stdout
    Serial.begin(115200);
    //Serial.setDebugOutput(true);
}

void initWifi() {
    // Attempt to connect to Wifi network:
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("Connected to wifi");
}


void sendEvent(int value) {

  String json = createJSON(value);
  httpPost(json);

  String response = "";
  char c;
  while (client.available()) {
    c = client.read();
    response.concat(c);
  }

  if (!response.equals("")) {
    if (response.startsWith("HTTP/1.1 201")) {
      Serial.println("Data sent to M2X"); 
    } else {
      Serial.println("Error posting " + json);
      Serial.println(response);
    }
  }

}


String createJSON(int value) {

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  root["value"] = String(value);

  char json[200];
  root.printTo(json, sizeof(json));
  return String(json);
}


void httpPost(String content)
{
    // close any connection before send a new request.
    client.stop();
  
    if (client.connect(hostname, 80)) {
      client.print("PUT ");
      client.print(uri);
      client.println(" HTTP/1.1"); 
      client.print("Host: "); 
      client.println(hostname);
      client.print("X-M2X-KEY: ");
      client.println("ab01f1b281a5155c709451155864f1e9"); 
      client.println("Connection: close");

      client.print("Content-Type: ");
      client.println("application/json");
      client.print("Content-Length: ");
      client.println(content.length());
      client.println();
      client.println(content);
      
      delay(200);
      
    } else {
      Serial.println("Connection failed POST");
    }
}
// get uber
void httpGet()
{
    // close any connection before send a new request.
    client.stop();
  
    if (client.connect("droidcode.tripedin.com", 80)) {
      client.print("GET ");
      client.print("/codebot/sendDummyMessage?type=1");
      client.println(" HTTP/1.1"); 
      client.print("Host: "); 
      client.println("droidcode.tripedin.com1");

      client.println("Connection: close");
      client.println();
      
      delay(200);
      
    } else {
      Serial.println("Connection failed GET");
    }
}








