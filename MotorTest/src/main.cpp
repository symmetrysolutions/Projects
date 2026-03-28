#include <Arduino.h>
#include "motorControl.h"
#include "sensorUtils.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "maincontrols.h"

int throttle = 0;
int lastThrottle = 0;
int steering = 0;
int	lastSteering = 0;
unsigned long lastCmd = 0;

BLEServer *server;
BLECharacteristic *tx;
boolean deviceConnected = false;
boolean isAdvertising = false;

class BTServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
	  Serial.println("Device Connected");
 	  digitalWrite(BACKWARD_PIN, LOW);
	  digitalWrite(FORWARD_PIN, HIGH);
   };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
	  isAdvertising = false;
	  Serial.println("Device Disconnected");
	  digitalWrite(BACKWARD_PIN, HIGH);
    }
};

class RXCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *c) {
        String cmd = String(c->getValue().c_str());
        cmd.trim();
        
        lastCmd = millis();
        
		if (cmd.startsWith("D:")) {
			float throttlePercent = 0.0;
			int commaIndex = cmd.indexOf(',');
			if (commaIndex == -1) {
				Serial.print("Invalid Combined Command Format: ");
				Serial.println(cmd);
				return;
			}
			throttlePercent = cmd.substring(2, commaIndex).toInt()/100.0;
			throttle = throttlePercent * MAX_SPEED;
            steering = cmd.substring(commaIndex + 1).toInt();

			Serial.print("Received Combined Command: ");
			Serial.print(throttle);
			Serial.print(",");
			Serial.println(steering);
        } else if (cmd.startsWith("T:")) {
            throttle = cmd.substring(2).toInt();
			Serial.print("Received Throttle Command: ");
			Serial.println(throttle);
        }
        else if (cmd.startsWith("S:")) {
            steering = cmd.substring(2).toInt();
			Serial.print("Received Steering Command: ");
			Serial.println(steering);
        }
        else if (cmd == "STOP") {
            throttle = 0;
            steering = 0;
			Serial.println("Received STOP Command");
		}
		else {
			Serial.print("Received Unknown Command: ");
			Serial.println(cmd);
        }
    }
};

void setupBlueTooth() {
	BLEDevice::init("CAR_GAME");
	server = BLEDevice::createServer();
	BLEService *service = server->createService(SERVICE_UUID);
	
	server->setCallbacks(new BTServerCallbacks());

	tx = service->createCharacteristic(TX_UUID, BLECharacteristic::PROPERTY_NOTIFY);
	tx->addDescriptor(new BLE2902());
	
	service->createCharacteristic(RX_UUID, BLECharacteristic::PROPERTY_WRITE)->setCallbacks(new RXCallbacks());
	
	service->start();
	server->getAdvertising()->start();
	Serial.println("Waiting for a client connection to notify...");
	isAdvertising = true;
	digitalWrite(BACKWARD_PIN, HIGH);
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
	setupMotorPINS();

	 // Initialize Distance Sensor
	 distanceSensorSetup(TRIG_PIN, ECHO_PIN);
	 setupBlueTooth();
}   

void sendStatus() {
    
    String msg = "T:" + String(throttle) +
                 ",S:" + String(steering);
    
    tx->setValue(msg.c_str());
    tx->notify();
}

void loop()   
{   
	// motorTest();
	// frequencyTest();
	// checkObstacleTest();
	// delay(500); // Wait before next reading

	if(deviceConnected) {
		if(isObstacleDetected(30.0)) {
			Serial.println("Object detected within 30 cm! Stopping and reversing...");
			throttle = 0;
			steering = 0;
		}

		if (throttle != lastThrottle) {
			Serial.print("Current Throttle: ");
			Serial.println(throttle);
			if (throttle > 0) {
				if(lastThrottle <= 0) {
					setForwardMotion();
				}
			} else if (throttle < 0) {
				if(lastThrottle >= 0) {
					setBackwardMotion();
				}
			}
			if(throttle == 0) {
				stopMotors();
			} else {
				setAllMotorSpeed(abs(throttle));
			}
			lastThrottle = throttle;
		}
		if(steering != lastSteering) {
			float turnIntensity = abs(steering) / 100.0;
			lastSteering = steering;
			Serial.print("Current Steering: ");
			Serial.println(turnIntensity);
			if(throttle != 0) {
				if (steering > 0) {
					setRightTurn(abs(throttle), abs(throttle) - (abs(throttle) * turnIntensity));
				} else if (steering < 0) {
					setLeftTurn(abs(throttle), abs(throttle) - (abs(throttle) * turnIntensity));
				} else	 {
					setRightMotorsSpeed(abs(throttle));
					setLeftMotorsSpeed(abs(throttle));
				}
			}
		}
	} else {
		if(throttle != 0 || steering != 0) {
			Serial.println("No device connected. Stopping motors.");
			throttle = 0;
			steering = 0;
			stopMotors();
		}

		if(!isAdvertising) {
			server->startAdvertising();
			Serial.println("Waiting for a client connection to notify...");
			isAdvertising = true;
		}
	}
	
	static unsigned long lastStatus = 0;
    
    if (millis() - lastStatus > 1000) {
        sendStatus();
        lastStatus = millis();
    }
} 

boolean isObstacleDetected(float threshold = 30.0) {
	float distance = distanceSensorReadCM(TRIG_PIN, ECHO_PIN); // Read distance in cm

	digitalWrite(STOP_PIN, HIGH);

	while(distance < 0.10) {
		// Wait for a valid reading
		delay(400);
		distance = distanceSensorReadCM(TRIG_PIN, ECHO_PIN); // Read distance in cm
	}
	digitalWrite(STOP_PIN, LOW);

	Serial.print("Distance to Object: ");
	Serial.print(distance);
	Serial.println(" cm");

	return distance < threshold; // Adjust threshold as needed
}
