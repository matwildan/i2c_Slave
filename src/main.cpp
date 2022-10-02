#include <Wire.h>
#include <Arduino.h>

#define slave3 //define slavenum

// int a=0;
#ifdef slave1
  #define pinBuzzer 13 // for slaveNum 1
#endif

#ifdef slave2
  #define pinLed 9 // for slaveNum 2
#endif

char msg[0];
String from;

void receiveEvent(int howMany);

void setup()
{
  #ifdef slave1
    pinMode(pinBuzzer, OUTPUT);   // for slaveNum = 1
    Wire.begin(1);                // join i2c bus with address #4
  #endif

  #ifdef slave2
    pinMode(pinLed, OUTPUT);
    Wire.begin(2);                // join i2c bus with address #4
  #endif

  #ifdef slave3
    Wire.begin(3);                // join i2c bus with address #4
  #endif
  
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  int i = 0;
  Serial.println("[INFO] Received msg: ");
  while(Wire.available()) // loop through all but the last
  {
    msg[i] = Wire.read();
    i=i+1;
  }
  Serial.println(msg);
  String msgToString = String(msg);

  from = msgToString.substring(0,8);

  String tempString = msgToString.substring(15,20);
  float temp = tempString.toFloat();
  Serial.println(temp);

  String humString = msgToString.substring(26,30);
  float hum = humString.toFloat();
  Serial.println(hum);
  if (temp != 0.00 || hum != 0.00)
  {
    if (temp > 30.00 || hum > 95.00)
    {
      Serial.println("[INFO] Melewati batas threshold");
      #ifdef slave1
        Serial.println("[INFO] Slave 1 alert! Suhu/kelembapan tinggi!");
        digitalWrite(pinBuzzer, HIGH);
      #endif

      #ifdef slave2
        Serial.println("[INFO] Slave 2 alert! Suhu/kelembapan tinggi!");
        digitalWrite(pinLed, HIGH);
      #endif

      #ifdef slave3
        Serial.println("[INFO] Slave 3 alert! Suhu/kelembapan tinggi!");
      #endif
    }
    else if (temp < 24.50 || hum < 45.00)
    {
      Serial.println("[INFO] Melewati batas threshold");
      #ifdef slave1
        Serial.println("[INFO] Slave 1 alert! Suhu/kelembapan rendah!");
        digitalWrite(pinBuzzer, HIGH);
      #endif

      #ifdef slave2
        Serial.println("[INFO] Slave 2 alert! Suhu/kelembapan rendah!");
        digitalWrite(pinLed, HIGH);
      #endif

      #ifdef slave3
        Serial.println("[INFO] Slave 3 alert! Suhu/kelembapan rendah!");
      #endif
    }
    else
    {
      Serial.println("[INFO] Suhu/kelembapan normal");
      #ifdef slave1
        digitalWrite(pinBuzzer, LOW);
      #endif

      #ifdef slave2
        digitalWrite(pinLed, LOW);
      #endif
    }
  }
  else
  { 
    #ifdef slave3
      Serial.println(String("[INFO] ") + from + String(" mati"));
    #endif
  }
  Serial.println();
}
