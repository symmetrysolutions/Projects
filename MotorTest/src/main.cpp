#include <Arduino.h>
#include "motorControl.h"
#include "sensorUtils.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "esp32_s3_pins.h"

// My Car Service For BlueTooth Control
#define SERVICE_UUID "12345678-1234-1234-1234-1234567890AB"
#define TX_UUID      "12345678-1234-1234-1234-1234567890AC"
#define RX_UUID      "12345678-1234-1234-1234-1234567890AD"


#define MIN_SPEED 25
#define MAX_SPEED 255
#define FORWARD_PIN 35
#define BACKWARD_PIN 36
#define STOP_PIN 37

motor motorA1;
motor motorA2;

motor motorB1;
motor motorB2;
void motorTest();
void increaseMotorsSpeed();   
void decreaseMotorsSpeed();
void frequencyTest();
void checkObstacleTest();
void setRightMotorsSpeed(int speed);
void setLeftMotorsSpeed(int speed);
void setForwardMotion();
void setBackwardMotion();
void setRotateRight();
void setRotateLeft();
void stopMotors();
boolean isObstacleDetected(float threshold);

int throttle = 0;
int steering = 0;
unsigned long lastCmd = 0;

BLECharacteristic *tx;

class RXCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *c) {
        String cmd = String(c->getValue().c_str());
        cmd.trim();
        
        lastCmd = millis();
        
        if (cmd.startsWith("T:")) {
            throttle = cmd.substring(2).toInt();
        }
        else if (cmd.startsWith("S:")) {
            steering = cmd.substring(2).toInt();
        }
        else if (cmd == "STOP") {
            throttle = 0;
            steering = 0;
        }
    }
};

void setupBlueTooth() {
	BLEDevice::init("ESP32 Car");
	BLEServer *server = BLEDevice::createServer();
	BLEService *service = server->createService(SERVICE_UUID);
	
	tx = service->createCharacteristic(TX_UUID, BLECharacteristic::PROPERTY_NOTIFY);
	tx->addDescriptor(new BLE2902());
	
	service->createCharacteristic(RX_UUID, BLECharacteristic::PROPERTY_WRITE)->setCallbacks(new RXCallbacks());
	
	service->start();
	server->getAdvertising()->start();
}

void setup()   
{   
	 Serial.begin(115200);   
	 
	pinMode(FORWARD_PIN, OUTPUT);
  	pinMode(BACKWARD_PIN, OUTPUT);
  	pinMode(STOP_PIN, OUTPUT);
	digitalWrite(FORWARD_PIN, LOW);
  	digitalWrite(BACKWARD_PIN, LOW);
  	digitalWrite(STOP_PIN, LOW);

	 // Initialize Motor Structure with Pins   
	setupMotor(motorA1, 0, ENA1_PIN, INA1_PIN, INA2_PIN);
	setupMotor(motorA2, 1, ENA2_PIN, INA3_PIN, INA4_PIN);
	setupMotor(motorB1, 2, ENB1_PIN, INB1_PIN, INB2_PIN);
	setupMotor(motorB2, 3, ENB2_PIN, INB3_PIN, INB4_PIN);

	 // Initialize Distance Sensor
	 distanceSensorSetup(TRIG_PIN, ECHO_PIN);
	 setupBlueTooth();
}   

void loop()   
{   
	// motorTest();
	// frequencyTest();
	checkObstacleTest();
	// delay(500); // Wait before next reading

	
	if (millis() - lastCmd > 2000) {
		throttle = 0;
		steering = 0;
	}
} 

void setForwardMotion() {
	Serial.println("Moving Forward");
	digitalWrite(FORWARD_PIN, HIGH);
	digitalWrite(BACKWARD_PIN, LOW);
	digitalWrite(STOP_PIN, LOW);
	setMotorDirectionForward(motorA1);
	setMotorDirectionForward(motorB1);
	setMotorDirectionForward(motorA2);
	setMotorDirectionForward(motorB2);
}

void setBackwardMotion() {
	Serial.println("Moving Backward");
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN, HIGH);
	digitalWrite(STOP_PIN, LOW);
	setMotorDirectionBackward(motorA1);
	setMotorDirectionBackward(motorB1);
	setMotorDirectionBackward(motorA2);
	setMotorDirectionBackward(motorB2);
}

void setRotateRight() {
	Serial.println("Rotating Right");
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN, HIGH);
	digitalWrite(STOP_PIN, LOW);
	setMotorDirectionForward(motorA1);
	setMotorDirectionForward(motorB1);
	setLeftMotorsSpeed(230);
	setRightMotorsSpeed(0);
	// setMotorDirectionBackward(motorA2);
	// setMotorDirectionBackward(motorB2);
}

void setRotateLeft() {
	Serial.println("Rotating Left");
	// setMotorDirectionBackward(motorA1);
	// setMotorDirectionBackward(motorB1);
	setMotorDirectionForward(motorA2);
	setMotorDirectionForward(motorB2);
	setRightMotorsSpeed(230);
	setLeftMotorsSpeed(0);
}

void stopMotors() {
	Serial.println("Stopping Motors");
	digitalWrite(FORWARD_PIN, LOW);
	digitalWrite(BACKWARD_PIN, LOW);
	digitalWrite(STOP_PIN, HIGH);
	setMotorSpeed(motorA1, 0);
	setMotorSpeed(motorB1, 0);
	setMotorSpeed(motorA2, 0);
	setMotorSpeed(motorB2, 0);
}

void setRightMotorsSpeed(int speed) {
	Serial.print("Setting Right Motors Speed to: ");
	Serial.println(speed);
	setMotorSpeed(motorA2, speed);
	setMotorSpeed(motorB2, speed);
}

void setLeftMotorsSpeed(int speed) {
	Serial.print("Setting Left Motors Speed to: ");
	Serial.println(speed);
	setMotorSpeed(motorA1, speed);
	setMotorSpeed(motorB1, speed);
}

void setRightTurn() {
	setRightMotorsSpeed(150);
	setLeftMotorsSpeed(230);
}

void setLeftTurn() {
	setRightMotorsSpeed(230);
	setLeftMotorsSpeed(150);
}

void setMotorSpeed(int speed) {
	Serial.println("Setting Forward Speed");
	setRightMotorsSpeed(speed);
	setLeftMotorsSpeed(speed);
}

boolean isObstacleDetected(float threshold = 30.0) {
	float distance = distanceSensorReadCM(TRIG_PIN, ECHO_PIN); // Read distance in cm

	// digitalWrite(STOP_PIN, HIGH);

	while(distance < 0.10) {
		// Wait for a valid reading
		delay(400);
		distance = distanceSensorReadCM(TRIG_PIN, ECHO_PIN); // Read distance in cm
	}
	// digitalWrite(STOP_PIN, LOW);

	Serial.print("Distance to Object: ");
	Serial.print(distance);
	Serial.println(" cm");

	return distance < threshold; // Adjust threshold as needed
}

void checkObstacleTest() {

	 if(isObstacleDetected(30.0)) {
	   Serial.println("Object detected within 30 cm! Stopping and reversing...");
	   stopMotors();
	   delay(1000);
	   setBackwardMotion();
	   setMotorSpeed(200);
	   delay(2000);
	   setRotateRight();
	   delay(4000);
	 }
	 setForwardMotion();
	 setMotorSpeed(250);
	 delay(500); // Wait before next reading
	}

void frequencyTest() {
	setForwardMotion();
	delay(10000);
	 Serial.println("Frequency 100");
	 ledcSetup(motorA1.channel, 100, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 500");
	 ledcSetup(motorA1.channel, 500, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 1000");
	 ledcSetup(motorA1.channel, 1000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 2000");
	 ledcSetup(motorA1.channel, 2000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 3000");
	 ledcSetup(motorA1.channel, 3000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 4000");
	 ledcSetup(motorA1.channel, 4000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
	 Serial.println("Frequency 5000");
	 ledcSetup(motorA1.channel, 5000, 8); // Channel 0, 10 kHz frequency, 8-bit resolution
	 setMotorSpeed(motorA1, 0);
	 increaseMotorsSpeed();
	 delay(5000);
}

void motorTest()   
{   
	 Serial.println("Motors are stopped now");
	 stopMotors();
	 Serial.println("Set direction FORWARD");   
	 setForwardMotion();
	 Serial.println("Increasing Speed");   
	 increaseMotorsSpeed();
	 Serial.println("Decreasing Speed");   
	 decreaseMotorsSpeed();
	 Serial.println("Stopping Motors");
	 stopMotors();
	 delay(5000);
	 Serial.println("Turn Right");   
	 setRightTurn();   
	 Serial.println("Looping Right");   
	 delay(5000);   
	 Serial.println("Stopping Motors");
	 stopMotors();
	 delay(2000);
	 Serial.println("Turn Left");
	 setLeftTurn();
     delay(5000);
	 Serial.println("Stopping Motors");
	 stopMotors();
	 delay(2000);
	 Serial.println("Straight Path");
	 setForwardMotion();
	 setMotorSpeed(250);
	 delay(5000);
}

void increaseMotorsSpeed()   
{   
	 for (int speed = MIN_SPEED; speed <= MAX_SPEED; speed+=5) {   
	   setMotorSpeed(motorA1, speed);
	   Serial.print("Testing Frequency: ");
	   Serial.print(ledcReadFreq(motorA1.channel));
	   Serial.print(", Speed: ");
	   Serial.println(speed);
	   setMotorSpeed(motorB1, speed);
	   setMotorSpeed(motorA2, speed);
	   setMotorSpeed(motorB2, speed);
	   delay(1000); // Add small delay between changes   
	 }   
}

void decreaseMotorsSpeed()   
{   
	 for (int speed = MAX_SPEED; speed >= MIN_SPEED; speed--) {   
	   setMotorSpeed(motorA1, speed); 
	   setMotorSpeed(motorB1, speed);
	   setMotorSpeed(motorA2, speed); 
	   setMotorSpeed(motorB2, speed);
	   delay(40); // Add small delay between changes   
	 }   
}  
