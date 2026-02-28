#include <Arduino.h>

#define MQPIN 36    // Analog pin connected to MQ-2 sensor
#define MQDPIN D1   // Digital pin connected to MQ-2 sensor digital output

// put function declarations here:
void readSensors();

void setup() {
  Serial.begin(9600);
  analogSetAttenuation(ADC_11db);
  analogReadResolution(10);
  analogSetWidth(10);
  // pinMode(MQDPIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  readSensors();
}

void readSensors() {
  // Wait a few seconds between measurements.
  int sensorValue = analogRead (MQPIN);
  // int digitalValue = digitalRead(MQDPIN);

  // Reading temperature or humidity takes about 250 milliseconds!
  // float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  // float temperature = dht.readTemperature(true);

  /* Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Celsius (isFahrenheit = false)
  float heatIndex = dht.computeHeatIndex(temperature, humidity, true);

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%  Temperature: ");
  Serial.print(temperature);
  Serial.print("F  Heat index: ");
  Serial.print(heatIndex);
  Serial.println("F");
  */

  Serial.print("MQ-2 Sensor Value (Raw): ");
  Serial.println(sensorValue);
  Serial.print("MQ-2 Sensor Value (Converted): ");
  Serial.println((sensorValue * (5.0 / 1023.0) * 100.0)); // assuming 10-bit ADC
  /*
  Serial.print("MQ-2 Digital Value: ");
  Serial.println(digitalValue);

  display.fillScreen(BLACK);
  display.setCursor(0, 5);
  display.setTextSize(1);
  if(temperature > 80){
    display.setTextColor(RED);  
  } else { 
    if(temperature < 45) {
      display.setTextColor(BLUE);
    } else {
      display.setTextColor(GREEN);
    }
  }
  display.printf("Temp: %.1fF\n", temperature);
  display.printf("HeatIdx: %.1fF\n", heatIndex);
  display.setTextColor(YELLOW);
  display.printf("Humidity: %.1f%%\n", humidity);
  display.printf("Gas: %d", sensorValue);
  */
  delay(5000);
}