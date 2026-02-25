#include <Wire.h>
#include <Servo.h>
#include <U8g2lib.h>

// === OLED 0.96 I2C ===
U8G2_SSD1306_128X64_NONAME_F_HW_I2C oled(U8G2_R0, U8X8_PIN_NONE);

// === Konfigurasi Pin ===
#define TRIG_ALL 2      // 1 pin trigger untuk semua sensor
#define ECHO_IN 3       // sensor gerbang masuk
#define ECHO_OUT 4       // sensor gerbang keluar
int ECHO_SLOT[6] = {5, 6, 7, 8, 12, 13}; // sensor tiap slot

#define SERVO_IN_PIN 9
#define SERVO_OUT_PIN 10
#define LED_FULL 11

// === Objek Servo ===
Servo servoIn;
Servo servoOut;

// === Konstanta Sistem ===
#define TOTAL_SLOTS 6
#define DIST_GATE 30      // jarak deteksi mobil di gerbang (cm)
#define DIST_PASS 60      // jarak mobil sudah lewat
#define DIST_SLOT 15      // jarak parkir terisi
unsigned long gateDelay = 200;
unsigned long lastDisplay = 0;

// === Variabel Status ===
bool gateInOpen = false;
bool gateOutOpen = false;
int slotOccupied = 0;

// === Fungsi Baca Ultrasonik ===
long readDistanceCM(int echoPin) {
pinMode(TRIG_ALL, OUTPUT);
digitalWrite(TRIG_ALL, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_ALL, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_ALL, LOW);
pinMode(echoPin, INPUT);
long duration = pulseIn(echoPin, HIGH, 25000);
long distance = duration * 0.034 / 2;
if (distance == 0) distance = 999;
return distance;
}

// .=== Fungsi Gambar Mobil ===
void drawCar(int x, int y) {
  oled.drawBox(x, y +3,18,7);
  oled.drawCircle(x+4,y+12,2,U8G2_DRAW_ALL);
  oled.drawCircle(x +14,y+12,2,U8G2_DRAW_ALL);
}

// === Fungsi Tampilan OLED ===
void drawDisplay() {
  oled.clearBuffer();
  oled.setFont(u8g2_font_6x10_tr);
  oled.setCursor(0, 10);
  oled.print(F("SMART PARKING SYSTEM"));

  int availableSlots = TOTAL_SLOTS - slotOccupied;
  oled.setCursor(0, 25);
  oled.print("Total Slot :");
  oled.print(TOTAL_SLOTS);

  oled.setCursor(0, 38);
  oled.print("Tersedia : ");
  oled.print(availableSlots);

  // Bar seperti baterai
  oled.drawFrame(0, 48, 128, 10);
  int filledWidth = map(slotOccupied, 0, TOTAL_SLOTS, 0, 126);
  oled.drawBox(1, 49, filledWidth, 8);

  // Status penuh
  if (availableSlots == 0) {
    oled.setCursor(0, 63);
    oled.print(F("STATUS: PENUH 🚫"));
    digitalWrite(LED_FULL, HIGH);
  } else {
    oled.setCursor(0, 63);
    oled.print(F("STATUS: TERSEDIA ✅"));
    digitalWrite(LED_FULL, LOW);
  }

  oled. sendBuffer();
}

// === Setup ===
void setup() {
  pinMode(TRIG_ALL, OUTPUT);
  pinMode(ECHO_IN, INPUT);
  pinMode(ECHO_OUT, INPUT);
  for (int i=0;i<6;i++)pinMode(ECHO_SLOT[i], INPUT);
  pinMode(LED_FULL, OUTPUT);

  servoIn.attach(SERVO_IN_PIN);
  servoOut.attach(SERVO_OUT_PIN);
  servoIn.write(0);
  servoOut.write(0);

  oled.begin();
  oled.setFont(u8g2_font_6x10_tr);
  oled.clearBuffer();
  oled.drawStr(0, 30, "Smart Parking Init ... ");
  oled.sendBuffer();
  delay(1500);
}

// === Loop ===
void loop() {
  // Baca sensor gate
  long distIn = readDistanceCM(ECHO_IN);
  long distOut = readDistanceCM(ECHO_OUT);

  // Hitung slot terisi
  int count = 0;
  for (int i = 0; i < 6; i++) {
    long d = readDistanceCM(ECHO_SLOT[i]);
    if (d < DIST_SLOT) count++;
  }
  slotOccupied = count;

  int availableSlots = TOTAL_SLOTS - slotOccupied;

  // ---. Mobil .Masuk .---
  if (distIn < DIST_GATE && availableSlots > 0 && !gateInOpen) {
  servoIn.write(90);
  gateInOpen = true;
  }
  if (gateInOpen && distIn > DIST_PASS) {
  servoIn.write(0);
  gateInOpen = false;
  }

  // ·---. Mobil Keluar-
  if (distOut < DIST_GATE && !gateOutOpen) {
  servoOut.write(90);
  gateOutOpen = true;
  }
  if (gateOutOpen && distOut > DIST_PASS) {
  servoOut.write(0);
  gateOutOpen = false;
  }

  // ·--- Update OLED tiap 500 ms ---
  if (millis()-lastDisplay> 500){
  drawDisplay();
  lastDisplay =millis();
  }

  delay(100);
}

