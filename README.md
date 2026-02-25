# Smart Slot Parking System

A sophisticated IoT-based parking management system using Arduino. This project automates entry/exit gates and provides real-time monitoring of 6 individual parking slots with visual feedback via OLED Display.

## 🚀 Key Features
* **Multi-Sensor Monitoring:** Utilizes 8 Ultrasonic sensors (2 for gates, 6 for individual slots).
* **Dual-Gate Automation:** Automated entry and exit barriers using high-precision servo motors.
* **Real-time OLED Dashboard:** Displays available vs occupied slots and system status.
* **Capacity Alert:** Integrated LED indicator for "Parking Full" status.
* **Optimized Triggering:** Single-pin trigger logic for efficient sensor management.

## 🛠️ Hardware Architecture
* **Microcontroller:** Arduino Uno
* **Display:** OLED SSD1306 (128x64) I2C
* **Actuators:** 2x SG90 Servo Motors (Gate Control)
* **Sensors:** 8x HC-SR04 Ultrasonic Sensors
* **Indicator:** Red LED (Capacity Warning)

## 🔌 Pin Mapping
| Component | Pin | Function |
| :--- | :--- | :--- |
| **TRIG_ALL** | 2 | Shared Trigger for all sensors |
| **ECHO_IN** | 3 | Entry Gate Sensor |
| **ECHO_OUT** | 4 | Exit Gate Sensor |
| **ECHO_SLOT 1-6** | 5, 6, 7, 8, 12, 13 | Individual Slot Sensors |
| **SERVO_IN** | 9 | Entry Barrier Motor |
| **SERVO_OUT** | 10 | Exit Barrier Motor |
| **LED_FULL** | 11 | Full Parking Indicator |
| **OLED SDA/SCL** | A4 / A5 | I2C Communication |

## 💻 System Logic
The system constantly polls the distance from all 8 sensors. 
1.  **Entry/Exit:** If a vehicle is detected within < 30cm at the gates, the respective Servo opens.
2.  **Slot Detection:** If distance in a slot is < 15cm, the system marks it as "Occupied".
3.  **Visuals:** The OLED updates every cycle to show the number of available spaces.

---
*Developed with a focus on smart city automation and hardware-software integration.*