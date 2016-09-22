
// lowest and highest sensor readings:
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

void setup() {
  // initialize serial communication @ 9600 baud:
  Serial.begin(9600);  
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
    break;
 case 1:    // (rain)
    Serial.println("Urine");
    break;
 case 2:    //(not rain)
    Serial.println("Not exceed the level of alcohol yet");
    break;
  }
  delay(1);  // delay between reads
}
