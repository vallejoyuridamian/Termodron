#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

uint8_t dataDHT[5];
#ifdef __AVR
  // Use direct GPIO access on an 8-bit AVR so keep track of the port and bitmask for the digital pin connected to the DHT.  Other platforms will use digitalRead.
  uint8_t _bit, _port;
#endif
uint32_t _lastreadtime, _maxcycles;
bool _lastresult;

int anRainVal;
String strRaining;
float humidity = 0;
float temperature = 0;


//Encabezado de funciones
 float readTemperature(bool force=false);
 float readHumidity(bool force=false);
 boolean read(bool force=false);
 uint32_t expectPulse(bool);
 

//clase para manejo de interrupciones
class InterruptLock {
  public:
   InterruptLock() {
    noInterrupts();
   }
   ~InterruptLock() {
    interrupts();
   }

};

void beginDHT() {
  #ifdef __AVR
    _bit = digitalPinToBitMask(DHTPIN);
    _port = digitalPinToPort(DHTPIN);
  #endif
  _maxcycles = microsecondsToClockCycles(1000);  // 1 millisecond timeout for
  _lastreadtime = -2000;
}


float readTemperature(bool force) {
  float f = NAN;
  if (read(force)) {
    f = dataDHT[2] & 0x7F;
    f *= 256;
    f += dataDHT[3];
    f *= 0.1;
    if (dataDHT[2] & 0x80) {
      f *= -1;
    }
  }
  return f;
}

float readHumidity(bool force) {
  float f = NAN;
  if (read()) {
    f = dataDHT[0];
    f *= 256;
    f += dataDHT[1];
    f *= 0.1;
  }
  return f;
}

boolean read(bool force) {
  // Check if sensor was read less than two seconds ago and return early to use last reading.
  uint32_t currenttime = millis();
  if (!force && ((currenttime - _lastreadtime) < 2000)) {
    return _lastresult; // return last correct measurement
  }
  _lastreadtime = currenttime;

  // Reset 40 bits of received data to zero.
  dataDHT[0] = dataDHT[1] = dataDHT[2] = dataDHT[3] = dataDHT[4] = 0;

  // Go into high impedence state to let pull-up raise data line level and start the reading process.
  digitalWrite(DHTPIN, HIGH);
  delay(250);

  // First set data line low for 20 milliseconds.
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delay(20);

  uint32_t cycles[80];
  {
    // Turn off interrupts temporarily because the next sections are timing critical and we don't want any interruptions.
    InterruptLock lock;

    // End the start signal by setting data line high for 40 microseconds.
    digitalWrite(DHTPIN, HIGH);
    delayMicroseconds(40);

    // Now start reading the data line to get the value from the DHT sensor.
    pinMode(DHTPIN, INPUT_PULLUP);
    delayMicroseconds(10);  // Delay a bit to let sensor pull data line low.

    // First expect a low signal for ~80 microseconds followed by a high signal for ~80 microseconds again.
    if (expectPulse(LOW) == 0) {
      _lastresult = false;
      return _lastresult;
    }
    if (expectPulse(HIGH) == 0) {
      _lastresult = false;
      return _lastresult;
    }

    // Now read the 40 bits sent by the sensor. Each bit is sent as a 50 microsecond low pulse followed by a variable length high pulse.
    // If the high pulse is ~28 microseconds then it's a 0 and if it's ~70 microseconds then it's a 1.
    // We measure the cycle count of the initial 50us low pulse and use that to compare to the cycle count of the high pulse to determine
    // if the bit is a 0 (high state cycle count < low state cycle count), or a 1 (high state cycle count > low state cycle count).
    //  Note that for speed all the pulses are read into a array and then examined in a later step.
    for (int i=0; i<80; i+=2) {
      cycles[i]   = expectPulse(LOW);
      cycles[i+1] = expectPulse(HIGH);
    }
  } 

  // Inspect pulses and determine which ones are 0 (high state cycle count < low state cycle count), or 1 (high state cycle count > low state cycle count).
  for (int i=0; i<40; ++i) {
    uint32_t lowCycles  = cycles[2*i];
    uint32_t highCycles = cycles[2*i+1];
    if ((lowCycles == 0) || (highCycles == 0)) {
      //DEBUG_PRINTLN(F("Timeout waiting for pulse."));
      _lastresult = false;
      return _lastresult;
    }
    dataDHT[i/8] <<= 1;
    // Now compare the low and high cycle times to see if the bit is a 0 or 1.
    if (highCycles > lowCycles) {
      // High cycles are greater than 50us low cycle count, must be a 1.
      dataDHT[i/8] |= 1;
    }
    // Else high cycles are less than (or equal to, a weird case) the 50us low cycle count so this must be a zero.  Nothing needs to be changed in the stored data.
  }

  // Check we read 40 bits and that the checksum matches.
  if (dataDHT[4] == ((dataDHT[0] + dataDHT[1] + dataDHT[2] + dataDHT[3]) & 0xFF)) {
    _lastresult = true;
    return _lastresult;
  }
  else {
    _lastresult = false;
    return _lastresult;
  }
}

// Expect the signal line to be at the specified level for a period of time and return a count of loop cycles spent at that level (this cycle count can be
// used to compare the relative time of two pulses).  If more than a millisecond ellapses without the level changing then the call fails with a 0 response.
uint32_t expectPulse(bool level) {
  uint32_t count = 0;
  // On AVR platforms use direct GPIO port access as it's much faster and better for catching pulses that are 10's of microseconds in length:
  #ifdef __AVR
    uint8_t portState = level ? _bit : 0;
    while ((*portInputRegister(_port) & _bit) == portState) {
      if (count++ >= _maxcycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  // Otherwise fall back to using digitalRead (this seems to be necessary on ESP8266 right now, perhaps bugs in direct port access functions?).
  #else
    while (digitalRead(DHTPIN) == level) {
      if (count++ >= _maxcycles) {
        return 0; // Exceeded timeout, fail.
      }
    }
  #endif

  return count;
}



void weather_read(){
  humidity = readHumidity();
  temperature = readTemperature();

  anRainVal = analogRead(AnRainPIN); 
 
  while (isnan(humidity) || isnan(temperature)) {
    delay(1000);
    #ifdef ConsolaMon
    Serial.println("Failed to read from DHT sensor!");
    #endif
    humidity = readHumidity();
    temperature = readTemperature();
  }
   
  if(!(digitalRead(RainPIN))){
    strRaining = "YES";
  }
  else{
    strRaining = "NO";
  }

  #ifdef ConsolaMon
  Serial.print("Raining?: ");
  Serial.print(strRaining);  
  Serial.print("\t Moisture Level: ");
  Serial.println(anRainVal);
 
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t"); 
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\n");
  #endif
}
