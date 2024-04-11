#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(6, 7);
SoftwareSerial GSM(3, 2);

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
#define sensorPin A4
#define threshold 50 
#define buzzerPin 8
#define RELAY_PIN 9
#define ACCESS_DELAY 5000
unsigned long startTime = millis();

void setup()
{
  Serial.begin(9600);
  GSM.begin(9600);
  Serial.println("Initializing..."); 
  delay(1000);  
  GSM.println("AT");
  updateSerial();
  GSM.println("AT+CMGF=1"); 
  updateSerial();
  pinMode(buzzerPin, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  finger.begin(57600);
  delay(5);
  if (!finger.verifyPassword()) 
  {
    while (1) { delay(1); }
  }
}

void loop()
{
  int fingerprintID = getFingerPrint();
  
  
  int impactSensorValue = analogRead(sensorPin);
  if (impactSensorValue > threshold)
  {
    
    sendSMS("+639923380007", "INTRUDER ALERT! PLEASE CHECK YOUR CAMERAS! ");
    while (millis() - startTime < 30000) { 
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
  }


  if (fingerprintID != -1)
  {
    digitalWrite(RELAY_PIN, LOW);
    delay(ACCESS_DELAY);
    digitalWrite(RELAY_PIN, HIGH);
  }


  delay(50); 
}

void updateSerial() {
  delay(500);
  while (Serial.available()) {
    GSM.write(Serial.read());
  }
  while(GSM.available()) {
    Serial.write(GSM.read());
  }
}


void sendSMS(const char* phoneNumber, const char* message) {
  GSM.println("AT+CMGS=\"" + String(phoneNumber) + "\"");
  updateSerial();
  GSM.print(message);
  updateSerial();
  GSM.write(26);
  delay(1000); 
}


int getFingerPrint() 
{
  int p = finger.getImage();
   if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK) 
  {
  

    sendSMS("+639923380007", "INTRUDER ALERT! PLEASE CHECK YOUR CAMERAS! ");
    while (millis() - startTime < 30000) { 
    digitalWrite(buzzerPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }

  
    return -1;
  }

  // found a match!
  return finger.fingerID;
}
