#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <PubSubClient.h>
#define led 2
#define PIN 4 // Pin connected to Neopixel strip
#define NUMPIXELS 16 // Number of Neopixels in the strip
#define BUTTON 2 // Pin connected to the button
#define DELAY_TIME 100 // Delay time in milliseconds for the animation
#define piezo 19
int won_message_timer = 0;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "connect";
const char* ssid_1 = "Vendi_2.4GH";
const char* ssid_2= "A26_5g";
const char* password = "91489148";
const char* password_1 = "Vendi@123";
const char* password_2 = "9833471993";
const char* topic1 = "sector17/nsg";
const char* topic2 = "sector17/name";

const char* mqtt_server = "91.121.93.94";  // test.mosquitto.org

WiFiClient espClient;

PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;


void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
    digitalWrite(led, LOW);
    delay(300);
    digitalWrite(led, HIGH);
    delay(300);
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') {
    digitalWrite(2, LOW); 
     Serial.print("Led in off"); 
  }
  if ((char)payload[0] == '1') {
    digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
    Serial.println("Led is on");
  }
  if ((char)payload[0] == 's') {
    Serial.println("Got the trigger");
    fortune();
    
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
      for (int i = 0; i < NUMPIXELS; i++)
  {
     pixels.setPixelColor(i, pixels.Color(255,0,0)); // Blue color for the current pixel
     pixels.show();
      delay(50);
  }
  
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("sector17/msg", "MQTT Server is Connected");
      // ... and resubscribe
      client.subscribe("sector17/nsg");
      client.subscribe("sector17/nsg");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
        for (int i = 0; i < NUMPIXELS; i++)
  {
     pixels.setPixelColor(i, pixels.Color(0,0,0)); // Blue color for the current pixel
     pixels.show();
      delay(50);
  }
   
    }
  }
}



void setup() {
  pinMode(2, OUTPUT); 
  Serial.begin(115200);
  wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(piezo, OUTPUT);
  pinMode(led, OUTPUT);
  pixels.begin();
  pixels.setBrightness(250); // Adjust brightness here

}

void loop() {

  if(WiFi.status() != WL_CONNECTED)
  {
    wifi();
    for (int i = 0; i < NUMPIXELS; i++)
  {
     pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Blue color for the current pixel
     pixels.show();
      delay(50);
  }

  }
   if(WiFi.status() == WL_CONNECTED){
     digitalWrite(led, HIGH);
   }

  if (!client.connected()) {
    reconnect();
  }

  client.loop();

//  unsigned long now = millis();
//   if (now - lastMsg > 2000) {
//     lastMsg = now;
//   //  value = analogRead(A0)*0.32;
//      value = millis();
//     snprintf (msg, MSG_BUFFER_SIZE, "Temperature is :%ld", value);
//     Serial.print("Publish message: ");
//     Serial.println(msg);
//     //client.publish("device9148/temp", "ping");
//     client.publish("device9148/temp", msg);
  static uint8_t wheelPos = 0; // Starting position of the wheel
  static uint8_t lastButtonState = HIGH; // Initial button state
  static uint8_t lastWheelPos = 0; // Last wheel position
  static uint8_t spinCounter = 0; // Spin counter
  won_message_timer = 0;
  for (int i = 0; i < NUMPIXELS; i++)
  {
     pixels.setPixelColor(i, pixels.Color(random(0,255), random(0,255), random(0,255))); // Blue color for the current pixel
     pixels.show();
      delay(50);
  }
}




void wifi()
{
  int count = 0;
  // WiFi.begin(ssid, password);
   WiFi.begin("Wokwi-GUEST", "", 6);
  Serial.println("Connecting");

  while(WiFi.status() != WL_CONNECTED) {
  
    digitalWrite(led, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(led, HIGH);
    delay(250);
    count+=1;
    if(count == 15){
  WiFi.begin(ssid_1, password_1);
  Serial.println("Connecting");
  }
    if(count == 30){
  WiFi.begin(ssid_2, password_2);
  Serial.println("Connecting");
  }
    if(count == 45){
    count = 0;
  }
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Connected With: ");
  Serial.println(WiFi.SSID());
  Serial.print("MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("RSSI Value is: ");
  Serial.println(WiFi.RSSI());
}

void buzz()
{
  for(uint8_t nLoop = 0;nLoop < 2;nLoop ++)
         {
  tone(piezo,500,200);
  delay(200);
  tone(piezo,500,200);
  delay(200);
  tone(piezo,500,200);
  delay(200);
  tone(piezo,800,150);
  delay(150);
  tone(piezo,500,500);
  delay(500);
}
}


void fortune()
{
  static uint8_t wheelPos = 0; // Starting position of the wheel
  static uint8_t lastButtonState = HIGH; // Initial button state
  static uint8_t lastWheelPos = 0; // Last wheel position
  static uint8_t spinCounter = 0; // Spin counter

    int spins = random(5, 20); // Random number of spins
    uint8_t winningPos = random(NUMPIXELS); // Random winning position
    uint8_t targetPos = winningPos + spins * NUMPIXELS; // Target position after spinning
    for (int i = 0; i < spins * NUMPIXELS; i++) {
      uint8_t speed = map(i, 0, spins * NUMPIXELS - 1, 50, 10); // Gradually reduce the delay time
      pixels.setPixelColor(wheelPos, pixels.Color(0, 0, 255)); // Blue color for the current pixel
      pixels.show();
      delay(speed);
      pixels.setPixelColor(wheelPos, pixels.Color(0, 0, 0)); // Turn off the current pixel
      wheelPos++;
      if (wheelPos == NUMPIXELS) {
        wheelPos = 0;
      }
    }

    spinCounter = 0; // Reset the spin counter
    while (wheelPos != winningPos) { // Gradually slow down and stop at the winning position
      spinCounter++;
      uint8_t speed = map(spinCounter, 0, 20, 100, 20);
      pixels.setPixelColor(wheelPos, pixels.Color(0, 0, 255)); // Blue color for the current pixel
      
      pixels.show();
      delay(speed);
      pixels.setPixelColor(wheelPos, pixels.Color(0, 0, 0)); // Turn off the current pixel
      wheelPos++;
      if (wheelPos == NUMPIXELS) {
        wheelPos = 0;
      }
    }

    for (int i = 0; i < 2 * NUMPIXELS; i++) { // Blink the winning position for 5 seconds
      pixels.setPixelColor(winningPos, pixels.Color(0, 255, 0)); // Yellow color for the winning pixel
      pixels.show();
      while(won_message_timer == 0){
      won_message_timer++;
      Serial.print("You won : ");
      Serial.println(wheelPos);
       value = wheelPos;
        snprintf (msg, MSG_BUFFER_SIZE, "%ld", value);
        Serial.print("Publish message: ");
        Serial.println(msg);
        client.publish("sector17/msg", msg);
      for (int i = 0; i <= value; i++) 
      {
    digitalWrite(led, LOW);
    delay(400);
    digitalWrite(led, HIGH);
    delay(400);
      }
        buzz();
      }
      delay(DELAY_TIME);
      pixels.setPixelColor(winningPos, pixels.Color(0, 0, 0)); // Turn off the winning pixel
      pixels.show();
      delay(DELAY_TIME);
    }
  }