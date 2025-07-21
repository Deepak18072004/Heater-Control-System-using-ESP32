#include "DHT.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// GPIO PINS
#define DHTPIN 16
#define DHTTYPE DHT22
#define HEATER_PIN 12
#define STATUS_LED_PIN 14

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Temperature Thresholds
const float TARGET_TEMP = 28.0;
const float HYSTERESIS = 2.0;
const float OVERHEAT_TEMP = 50.0;

enum SystemState {
  IDLE,
  HEATING,
  STABILIZING,
  TARGET_REACHED,
  OVERHEAT
};

SystemState currentState = IDLE;
const char* stateNames[] = {"IDLE", "HEATING", "STABILIZING", "TARGET_REACHED", "OVERHEAT"};

DHT dht(DHTPIN, DHTTYPE);
const int TASK_DELAY_MS = 1000;
void updateStatusLED() {
  if (currentState == OVERHEAT) {
    digitalWrite(STATUS_LED_PIN, LOW);
  } else {
    digitalWrite(STATUS_LED_PIN, digitalRead(HEATER_PIN));
  }
}
// OLED 
void updateOLED(float temp) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(0, 0);
  display.print("State: ");
  display.println(stateNames[currentState]);

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temp);
  display.println(" C");

  display.setCursor(0, 32);
  display.print("Heater: ");
  display.println(digitalRead(HEATER_PIN) ? "ON" : "OFF");

  display.display();
}

void heaterControlTask(void *pvParameters) {
  for (;;) {
    float temp = dht.readTemperature();

    if (isnan(temp)) {
      Serial.println("Error reading DHT sensor!");
    } else {
      if (temp >= OVERHEAT_TEMP && currentState != OVERHEAT) {
        currentState = OVERHEAT;
        digitalWrite(HEATER_PIN, LOW);
        Serial.println("CRITICAL: OVERHEAT DETECTED!");
      }

      switch (currentState) {
        case IDLE:
          digitalWrite(HEATER_PIN, LOW);
          if (temp < TARGET_TEMP - HYSTERESIS)
            currentState = HEATING;
          break;

        case HEATING:
          digitalWrite(HEATER_PIN, HIGH);
          if (temp >= TARGET_TEMP)
            currentState = STABILIZING;
          break;

        case STABILIZING:
          digitalWrite(HEATER_PIN, LOW);
          currentState = TARGET_REACHED;
          break;

        case TARGET_REACHED:
          digitalWrite(HEATER_PIN, LOW);
          if (temp < TARGET_TEMP - HYSTERESIS)
            currentState = HEATING;
          break;

        case OVERHEAT:
          digitalWrite(HEATER_PIN, LOW);
          break;
      }

      Serial.printf("State: %-16s | Temp: %.2f°C | Heater: %s\n",
        stateNames[currentState],
        temp,
        digitalRead(HEATER_PIN) ? "ON" : "OFF");

      updateOLED(temp);
    }

    updateStatusLED();
    vTaskDelay(pdMS_TO_TICKS(TASK_DELAY_MS));
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(HEATER_PIN, OUTPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(HEATER_PIN, LOW);
  digitalWrite(STATUS_LED_PIN, LOW);
  dht.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED init failed");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Heater Control Init...");
  display.display();
  delay(2000);
  display.clearDisplay();

  xTaskCreate(
    heaterControlTask,
    "Heater Control",
    4096,
    NULL,
    1,
    NULL
  );
}

void loop() {

}
