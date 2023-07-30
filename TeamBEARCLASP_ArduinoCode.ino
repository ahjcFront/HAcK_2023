/*Instructions: 
 * For the ring LED, three pixels: ultrasonic, four pixels: temp/humid, five pixels: Gas
 * Tags are placed for ease of finding specific sensor's code
 * Enjoy!!!
 -------------------------------------------POWER BY BEARCLASP--------------------------------------------- 
*/

//This block sets up the NEOPIXEl ring LED
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#define PIN       14
#define NUMPIXELS 12
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//This block includes the necessary libraries for this code to work
#include "Adafruit_AM2320.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HackPublisher.h"

//This blocks sets up the OLED and the AM2320 class
#define OLED_RESET 23
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_AM2320 am2320 = Adafruit_AM2320();

//This blocks connects to the wifi and create a publisher to communicate with the website
const char *ssid = "ASUS-F8";
const char *password = "K33pi7$@f3%";
HackPublisher publisher("bearclasp");  // publisher instance for team "hackers"


//Some necessary global variables for ultrasonic sensor
const int trigpin = 12;
const int echopin = 13;
long duration;
int distance;


void setup() {
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif

  //Starts the ring LED, set pin modes
  pixels.begin();
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);

  // Initialize serial communication
  Serial.begin(115200);
  while (!Serial) continue;

  Wire.begin();
  // initialize OLED with I2C addr 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  // Initialize Temp & Humid Sensor
  am2320.begin();
  
  // Connect to wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Initialize publisher
  publisher.begin();
  
  //In case the oled fails
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
  Serial.println("SSD1306 allocation failed");
  for(;;); // Don't proceed, loop forever
  }

  //Display the glorious Bearclasp tag in the beginning into the oled screen.
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.println("Powered by Bearclasp");
  display.display();
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  //delay(1000);
  
}

void loop() {
  //ULTRASONIC SENSOR
  //This block retrieves the distance from an object using the sensor
  //print the distance to the serial monitor
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  //This blocks changes the pixel 0 - 2 depending on how far an object is
  for(int i=0; i<3; i++) {
    int ultra_color = distance * 2.5;
    pixels.setPixelColor(i, pixels.Color(255, 0, ultra_color));
    pixels.show();
  }
  
  
  //TEMPERATURE SENSOR
  // Read Humidity
  float h = am2320.readHumidity();
  delay(200);
  // Read temperature as Celsius
  float t = am2320.readTemperature();
  Serial.println(h);
  Serial.println(t);

  //If the conditions for FIRE is reached, display red ring LED on pixel 3 - 6
  if (t > 32 && h < 45)
  {
  for(int i=3; i<7; i++) {

    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();
    }
  }

  else // if not fire, display green ring LED
  {
    for(int i=3; i<7; i++) {

    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
    pixels.show();
  }
  }

  //display the distance, temperature and humidity onto the oled screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,10);
  display.print("Distance: ");
  display.print(distance);
  display.println(" cm");
  display.setCursor(0,20);
  display.print("T: ");
  display.print(t);
  display.print(" C ");
  display.print("H: ");
  display.print(h);
  display.println(" %");
  
  display.display();

  //GAS SENSOR
  int sensVal = analogRead(A3);
  float voltage = sensVal * (5.0 / 1023.0);  //convert to voltage (volt)
  Serial.print(voltage);
  if (voltage == 0)                          //in case voltage is 0
  {
    Serial.println("Not applicable");
  }
  else                                       //display good, okay or bad air depending on voltage
  {
    if (voltage > 0 && voltage <= 7.00)
    {
      Serial.println(" - Good air quality");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print(voltage);
      display.println(" - Good air ;)");
      display.display();

      for(int i=7; i<12; i++) {

        pixels.setPixelColor(i, pixels.Color(0, 255, 0));
        pixels.show();
      }
    }

    else if (voltage > 7.00 && voltage <= 10.00)
    {
      Serial.println(" - Okay air quality");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print(voltage);
      display.println(" - Okay air");
      display.display();

      for(int i=7; i<12; i++) {

        pixels.setPixelColor(i, pixels.Color(255, 255, 0));
        pixels.show();
      }
    }

    else if (voltage > 10.00)
    {
      Serial.println(" - Bad air quality");
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0,0);
      display.print(voltage);
      display.println(" - Bad air :(");
      display.display();

      for(int i=7; i<12; i++) {

        pixels.setPixelColor(i, pixels.Color(255, 0, 0));
        pixels.show();
    }
  }

  //This send the sensors' data in real-time to the website
  publisher.store("ultrasonic", distance); 
  publisher.store("temperature", t);
  publisher.store("humidity", h);
  publisher.store("gas", voltage);       
  publisher.send();                 

}
}
//END OF FILE
