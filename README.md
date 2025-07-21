
# 🔥 Heater Control System – ESP32 Project

This project implements a basic heater control system using an **ESP32**, a **DHT22 temperature sensor**, and an **OLED display**. The system uses a finite state machine to control a simulated heater and broadcasts its current state over **BLE (Bluetooth Low Energy)**.

---

## 📌 Features

- ✅ Real-time temperature monitoring using DHT22
- ✅ Finite State Machine with 5 states:
  - `IDLE`
  - `HEATING`
  - `STABILIZING`
  - `TARGET_REACHED`
  - `OVERHEAT`
- ✅ Heater control via GPIO pin (simulated by LED)
- ✅ OLED display showing:
  - System state
  - Current temperature
  - Heater status
- ✅ BLE advertising of current heater state (visible via BLE scanner apps)
- ✅ Status LED feedback (Green LED ON when heater is active)
- ✅ FreeRTOS task handles periodic sensor reading and state update

---

## 🧰 Hardware Required

| Component       | Quantity | Notes                                |
|----------------|----------|--------------------------------------|
| ESP32 Dev Module | 1        | Microcontroller with BLE and WiFi   |
| DHT22 Sensor     | 1        | Temperature sensor (Single-wire)    |
| SSD1306 OLED     | 1        | I2C 128x64 display                   |
| LED (Heater)     | 1        | Simulates heater ON/OFF             |
| LED (Status)     | 1        | Green LED for heater indicator      |
| Resistors        | 2        | 220Ω for LEDs                       |
| Breadboard + Wires | As needed | For connections                  |

---

## 🖥️ Pin Configuration

| Module     | ESP32 GPIO |
|------------|------------|
| DHT22      | GPIO 16    |
| Heater LED | GPIO 12    |
| Status LED | GPIO 14    |
| OLED SDA   | GPIO 21    |
| OLED SCL   | GPIO 22    |

---

## 📡 BLE Functionality

- BLE broadcasts the **current system state** (e.g., `HeaterState: HEATING`)
- You can view it using:
  - [nRF Connect](https://www.nordicsemi.com/Products/Development-tools/nRF-Connect-for-mobile)
  - LightBlue Explorer
  - BLE Scanner (Android/iOS)

> BLE advertising does not require pairing or connection.

---

## ⚙️ System Behavior

| Condition                     | Action                         |
|------------------------------|---------------------------------|
| Temp < 26°C                  | Start Heating                   |
| Temp ≥ 28°C                  | Stop Heating → Stabilizing      |
| Temp stable after threshold  | Move to Target Reached state    |
| Temp ≥ 50°C                  | Overheat protection triggers    |

---

## 🧪 How to Use

1. Connect all components as per the pin config above.
2. Flash the code using **Arduino IDE** with ESP32 board support.
3. Open the **Serial Monitor** to view logs.
4. Observe:
   - OLED display for status
   - Heater LED (ON when heating)
   - BLE broadcast on your phone (via app)

---

## 🚦 Libraries Used

- `DHT.h` – For DHT22 sensor
- `Adafruit_SSD1306` – For OLED display
- `Adafruit_GFX` – Graphics support for OLED
- `BLEDevice.h` – For BLE advertisement (ESP32 built-in)
- `FreeRTOS` – Task scheduler (ESP32 built-in)

---

## 🔗 Notes

- BLE advertising is not supported on Wokwi; test on **real ESP32 hardware**
- If OLED or BLE is not needed, the code can be simplified
- Ensure the correct **board settings** are selected in Arduino IDE:
  - Board: ESP32 Dev Module
  - Upload Speed: 115200 or 921600
  - Partition Scheme: Default


