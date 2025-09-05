#include <WiFi.h>        // For connecting ESP32 to WiFi
#include <ArduinoOTA.h>  // For enabling over-the-air updates
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include  <Adafruit_BMP280.h>
#include <Wire.h>


Adafruit_BMP280 bmp; // I2C Interface




const char* ssid = "HD-iPhone-14-128GB";  // Change to your WiFi Network name
const char* password = "chaverim";  // Change to your password




WiFiClient  client;


int pinState = 1;


unsigned long myChannelNumber = 3062540;
const char * myWriteAPIKey = "D2NXYIDH48Z3RKWD";


const int trigPin = 5;
const int echoPin = 18;


//define sound speed in cm/uS
#define SOUND_SPEED 0.034


long duration;
float distanceInch;




// Initialize our values
int test = 0;
long pressure;
String myStatus = "Working";


void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);  // Connect to WiFi - defaults to WiFi Station mode


  //set the resolution to 12 bits (0-4095)
  analogReadResolution(12);


  bmp.begin();

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);






  // Ensure WiFi is connected
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("Not connected");
  }




  ArduinoOTA.begin();  // Starts OTA
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}




void loop() {
  int analogValue = analogRead(34); //was 15 now 34
  Serial.println(analogValue);


  pressure = bmp.readPressure();
  Serial.println(pressure);






  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
 
  // Calculate the distance
  float usdistance = duration * SOUND_SPEED/2;
  Serial.println(usdistance);






  ArduinoOTA.handle();  // Handles a code update request
  Serial.println(45);
  // All loop you're code goes here.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not working");
  }




  // set the fields with the values
  ThingSpeak.setField(1, test);
  ThingSpeak.setField(2, usdistance);
  ThingSpeak.setField(3, analogValue);
  ThingSpeak.setField(4, pressure);
 
  // set the status
  ThingSpeak.setStatus(myStatus);
 
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
 
  delay(5000); // Wait 20 seconds to update the channel again
}
