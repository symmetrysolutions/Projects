#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1331.h>
#include <SPI.h>

#define trigPin  2
#define echoPin  15

//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701

#define WSPIN 32 // Water Sensor Pin
#define WSPWR 4 // Water Sensor Pin
#define THRESHOLD   300

// GPIO pin definitions SSD1331 
#define sclk 18
#define mosi 23
#define rst  21
#define dc   16
#define cs   19

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

Adafruit_SSD1331 display = Adafruit_SSD1331(cs, dc, mosi, sclk, rst);  

void sensorSetup();
void distanceSensor();
void waterSensor();

void setup() {
  Serial.begin(9600);
  delay(2000);
  sensorSetup();
}

void loop() {
  // put your main code here, to run repeatedly:
  display.fillScreen(BLACK);
  display.setTextSize(1);
  distanceSensor();
  waterSensor();
  delay(2000);
}

void sensorSetup() {
  Serial.println("SSD1331 Display Setup");
  display.begin();
  Serial.println("SSD1331 Display Setup Complete");

  // Pin Modes for Ultrasonic Sensor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Pin Modes for Water Sensor
  pinMode(WSPWR, OUTPUT);
  // pinMode(WSPIN, INPUT);
  // Power off Water Sensor
  digitalWrite(WSPWR, LOW);
}

void distanceSensor() {
  long duration;
  float distanceCm, distanceInch;

  // Clear the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distanceCm = (duration * SOUND_SPEED) / 2;
  distanceInch = distanceCm * CM_TO_INCH;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distanceCm);
  Serial.print(" cm, ");
  Serial.print(distanceInch);
  Serial.println(" inch");
  /* */
  // Display distance on SSD1331
  display.setTextColor(GREEN);
  if(distanceInch < 5) {
    display.setTextColor(RED);
  }
  display.setCursor(0, 5);
  display.print("Distance:");
  display.setCursor(0, 15);
  display.print(distanceCm);
  display.print(" cm");
  display.setCursor(0, 30);
  display.print(distanceInch);
  display.print(" inch");
}

void waterSensor() {
  uint16_t value = 0;

  digitalWrite(WSPWR, HIGH);  // turn the sensor ON
  delay(10);                      // wait 10 milliseconds
  value = analogRead(WSPIN); // read the analog value from sensor
  digitalWrite(WSPWR, LOW);   // turn the sensor OFF
/**/
  display.setCursor(0, 45);
  display.print("Water: ");
  display.print(value);

  if (value > THRESHOLD) {
    Serial.print("Water is detected: ");
    Serial.println(value);
     // digitalWrite(LED_PIN, HIGH);  // turn LED ON
  } else {
    Serial.print("Water Not detected: ");
    Serial.println(value);
   // digitalWrite(LED_PIN, LOW);   // turn LED OFF
  }
}