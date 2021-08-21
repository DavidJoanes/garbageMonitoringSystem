// This sends the status of refuse in the waste bin as an SMS (which will also contain the GPS location of the bin) to a specific phone number, displays the status on Blynk application and it also sends the status to a local web server.

//Libraries to install:
//Tintygps, Blynk, Sim800L

#define BLYNK_PRINT Serial

// Select your modem:
#define TINY_GSM_MODEM_SIM800

#include <TinyGsmClient.h>
#include <BlynkSimpleTinyGSM.h>

#include <Ultrasonic.h>

#include <TinyGPS.h>
#include <SoftwareSerial.h>


//Your app authentication token (can be fetched from your blynk app
char auth[] = "Owz2FHJaeDPrT9ItdUwiz0hetreKt3S4";//Enter blynk auth token

// Your GPRS credentials
// Leave empty, if missing user or pass
char apn[]  = "web.gprs.mtnnigeria.net";
char user[] = "web";
char pass[] = "web";

// or Software Serial on Uno, Nano
#include <SoftwareSerial.h>
SoftwareSerial SerialAT(2, 3); // RX, TX

TinyGsm modem(SerialAT);

//GSM periheral (I selected 'GSM' as the communication mode on the blynk app, and V1, V2, and V3 as the respective virtual terminals
WidgetLED green(V1);
WidgetLED orange(V2);
WidgetLED red(V3);

Ultrasonic ultrasonic(7);
//int distance = 0;
//int thresh [3] = {20,12,4};

// Pin Assignment
#define trigPin 6
#define echoPin 5

//int state = 0;
//const int pin = 9;

TinyGPS gps;  //Creates a new instance of the TinyGPS object
SoftwareSerial SIM800L(7, 8);

float duration, distance;

void setup() {
  // Debug console
  Serial.begin (9600);
  SIM800L.begin(9600);
  delay(1000);

  // Set GSM module baud rate
  SerialAT.begin(9600);
  delay(3000);

  // Restart takes quite some time
  // To skip it, call init() instead of restart()
  Serial.println("Initializing modem...");
  modem.restart();

  // Unlock your SIM card with a PIN (only if your sim card is locked)
  //modem.simUnlock("1234");

  //Start the Blynk server
  Blynk.begin(auth, modem, apn, user, pass);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
   
}

void loop() {
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }
   
  // Write a pulse to the HC-SR04 Trigger Pin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Measure the response from the HC-SR04 Echo Pin
  duration = pulseIn(echoPin, HIGH);
  
  // Determine distance from duration
  // Use 343 metres per second as speed of sound
  distance = ultrasonic.distanceRead();
  Serial.print(distance);
  Serial.println("DISTANCE: ");
  Serial.println(distance);
  
  distance = (duration / 2) * 0.0343;
  Blynk.run();

  // Send results to Serial Monitor and start the alert process (SMS, BLYNK & WEBSERVER)
  Serial.print(" Distance = ");
  if (distance >= 97.536 || distance <= 2) {
//  if(distance<=thresh[0]&&distance>=thresh[1]&&distance>=thresh[2]){
     digitalWrite(11, LOW);
     digitalWrite(12, LOW);
     Serial.print(distance);
     Serial.println(" cm");
     Serial.println("Apparently empty");
     digitalWrite(13, HIGH);
     
     green.on();
     Serial.println(1);
  }
  else if (distance >= 12.192 || distance <= 2) {
//  else if(distance<=thresh[0]&&distance<=thresh[1]&&distance>=thresh[2]){
    digitalWrite(13, LOW);
    Serial.print(distance);
    Serial.println(" cm");
    Serial.println("Almost full..");
    digitalWrite(12, HIGH);
    digitalWrite(11, LOW);
    delay(200);
    
    green.off();
    orange.on();
    Serial.println(2);
    }
  else if (distance < 12.19 || distance <= 2 || newData==true) {
//  else if(distance<=thresh[0]&&distance<=thresh[1]&&distance<=thresh[2]){
    
    digitalWrite(13, LOW);
    digitalWrite(12, LOW);
    digitalWrite(11, HIGH);
    Serial.print(distance);
    Serial.println(" cm");
    Serial.println("BIN IS FULL, TAKE ACTION!");
    
    green.off();
    orange.off();
    red.on();
    Serial.println(3);

    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    SIM800L.print("AT+CMGF=1\r"); 
    delay(1000);
    SIM800L.println("AT + CMGS = \"+2349032688974\"");// recipient's mobile number with country code
    delay(1000);
//    //The text format of the SMS to be sent
//    SIM800L.print("Latitude = ");
//    SIM800L.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
//    SIM800L.print(" Longitude = ");
//    SIM800L.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
//    delay(10000);
//    SIM800L.println((char)26); // End AT command with a ^Z, ASCII code 26
//    delay(10000);
//    SIM800L.println();
    //The text format of the SMS to be sent
    SIM800L.print("WASTE BIN IS FULL! Please empty now.");
    SIM800L.print("https://www.google.com/maps/?q=");
    SIM800L.print(flat, 6);
    SIM800L.print(",");
    SIM800L.print(flon, 6);
    delay(1000);
    SIM800L.write(0x1A);
    delay(1000);
//    state = 1
  }
  else{
    green.off();
    orange.off();
    red.off();
    Serial.println(0);    
    }
  delay(100);
  
 
  Serial.println(failed);
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");

  delay(500);
}
