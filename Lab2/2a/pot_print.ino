// Possible ADC pins on the ESP32: 0,2,4,12-15,32-39; 34-39 are recommended for analog input
int potPin = 34;        // GPIO pin used to connect the potentiometer (analog in)
 
int val;    // variable to read the value from the analog pin
 
void setup()
{
  Serial.begin(115200);
}
 
void loop() {
  val = analogRead(potPin);            // read the value of the potentiometer (value between 0 and 1023)

  // Scale val (0–2495) to (0–4095)
  long scaledVal = map(val, 0, 2495, 0, 4095);

  // Enforce hard max of 4095
  if (scaledVal > 4095) {
    scaledVal = 4095;
  }
  Serial.println(scaledVal);
  delay(50);
}
