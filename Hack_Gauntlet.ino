
#include "AM232X.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HackPublisher.h"

/*
const char *ssid = "ASUS-F8";
const char *password = "K33pi7$@f3%";

HackPublisher publisher("Bearclasp");  // publisher instance for team "hackers"
int temp = 0;  // variable that holds the temperature
*/

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define pd_scl_dht 22
#define pd_sda_dht 23

TwoWire I2CIMU = TwoWire(0);   //  I2C1 bus
TwoWire I2CDHT = TwoWire(1);   //  I2C2 bus


AM232X AM2320(&I2CDHT);

const int trigpin = 12;
const int echopin = 13;
long duration;
int distance;


void setup() {
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

  // Initialize serial communication
  /*
  Serial.begin(115200);
  while (!Serial) continue;

  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize publisher
  publisher.begin();
  */

  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println("SSD1306 allocation failed");
  for(;;); // Don't proceed, loop forever
}
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Bearclasp");
  display.display();
  
  Serial.begin(115200);
  /*
  I2CDHT.begin(pd_sda_dht, pd_scl_dht, 100000ul); 
  if (! AM2320.begin() )
  {
    Serial.println("Sensor not found");
    while (1);
  }
  AM2320.wakeUp();
  delay(2000);

  Serial.println("Type,\tStatus,\tHumidity (%),\tTemperature (C)");
  */
}

void loop() {
  //Website Comms
  /*
  publisher.store("ultrasonic", 12.56); // store value for ultrasonic sensor
  publisher.store("temp", temp);        // store value for temp
  publisher.store("meow", "woof");      // store value for meow

  publisher.send();                     // send stored data to website

  delay(2000);
  temp++;
  */

  //ULTRASONIC SENSOR
  /*
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  distance = duration * 0.034 / 2;
  
  Serial.println(distance);
  delay(500);
  */
  
  /*
  //TEMPERATURE SENSOR
  Serial.print("AM2320, \t");
  int status = AM2320.read();
  switch (status)
  {
    case AM232X_OK:
      Serial.print("OK,\t");
      break;
    default:
      Serial.print(status);
      Serial.print("\t");
      break;
  }
  // DISPLAY DATA, sensor only returns one decimal.
  Serial.print(AM2320.getHumidity(), 1);
  Serial.print(",\t");
  Serial.println(AM2320.getTemperature(), 1);

  delay(2000);

  */
  
  //GAS SENSOR
  /*
  int sensVal = analogRead(A0);
  float voltage = sensVal * (3.3 / 1023.0);
  if (voltage == 0)
  {
    Serial.println("Not applicable");
  }
  else
  {
    if (voltage > 0 && voltage <= 7.00)
    {
      Serial.println("Good air quality");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,10);
      display.println("Good");
      display.display();
    }

    if (voltage > 7.00 && voltage <= 9.00)
    {
      Serial.println("Okay air quality");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,10);
      display.println("Okay");
      display.display();
    }

    if (voltage > 9.00)
    {
      Serial.println("Bad air quality");
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,10);
      display.println("Bad");
      display.display();
    }
  }
  // print out the value you read:
  Serial.println(voltage);
  delay(500); 
  */
}
