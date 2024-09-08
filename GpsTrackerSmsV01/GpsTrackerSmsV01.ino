#include <SoftwareSerial.h>
#include <TinyGPS++.h>

SoftwareSerial mySerial(2, 3); 
TinyGPSPlus gps;
String inputStc:\Users\nurha\Downloads\GPS Tracker SMS.rarring = "";   
bool stringComplete = false; 
String latitude, longitude;
String linkMap; 

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);  
  delay(1000); 
}

void loop() {
  serialEvent();
  GpsEvent();
  if (stringComplete) {
    Serial.println(inputString);
    inputString.toUpperCase();
    if(inputString.indexOf("LOKASI")!=-1){
      kirimSMS(linkMap);
      mySerial.println("AT+CMGD=1,4\r\n");
    }
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (mySerial.available()) {
    char inChar = (char)mySerial.read();
    if (inChar == '\n') {
      stringComplete = true;
    }else{
      inputString += inChar;
    }
  }
}

void kirimSMS(String pesan){
  char psn[100];
  int jlhTxt = pesan.length();
  Serial.println("SMS SEND : " + pesan);
  pesan.toCharArray(psn,100);
  mySerial.write("AT+CMGS=\"082165136545\"\r\n");
  delay(1000);
  mySerial.write(psn);
  delay(1000);
  mySerial.write((char)26);
  delay(1000);
}

void GpsEvent() {
  while(Serial.available()) {
    char inChar = (char)Serial.read();
    gps.encode(inChar);
  }
  
  if(gps.location.isValid()) {
    latitude = String(gps.location.lat(),6);
    longitude = String(gps.location.lng(),6);
    linkMap = "www.google.com/maps/place/" + latitude + "," + longitude;
  }else{
    linkMap = "Location not found";
  }
}
