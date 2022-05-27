#include "DHT.h"
#define DHTPIN 2     // Digital Pin of Arduino Board
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
 
DHT dht(DHTPIN, DHTTYPE);

#define RainPIN 3 
int nRainVal;
boolean bIsRaining = false;
String strRaining;
 
void setup() {
  Serial.begin(115200);
  Serial.println("Begin:");
 
  dht.begin();
  pinMode(RainPIN,INPUT);

}
 
void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  dhtsens();
  rainsens();

}



void dhtsens(){
// Reading temperature or humidity takes about 250 milliseconds.
  // Sensor readings may also be up to 2 seconds.
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default).
  float t = dht.readTemperature();
 
  // Check if any reads failed and try again.
  while (isnan(h) || isnan(t)) {
    delay(1000);
    Serial.println("Failed to read from DHT sensor!");
    h = dht.readHumidity();
    t = dht.readTemperature();
  }
 
  // Compute heat index in Celsius (isFahreheit = false)
  // float hic = dht.computeHeatIndex(t, h, false);
 
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C\n");
  // Serial.print("Heat index: ");
  // Serial.print(hic);
  // Serial.print(" *C\n");
}




void rainsens(){
  //nRainVal = analogRead(nRainIn);
  bIsRaining = !(digitalRead(RainPIN));
  
  if(bIsRaining){
    strRaining = "YES";
  }
  else{
    strRaining = "NO";
  }
  
  Serial.print("Raining?: ");
  Serial.print(strRaining);  
  Serial.print("\t Moisture Level: ");
  Serial.println(nRainVal);
}
