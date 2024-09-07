#include <LiquidCrystal_I2C.h>  // รวมไลบรารีสำหรับการใช้งานจอ LCD แบบ I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);  // กำหนดที่อยู่ของจอ LCD เป็น 0x27 และขนาดเป็น 16 ตัวอักษร 2 บรรทัด

// กำหนดพินสำหรับเซนเซอร์และ Buzzer
const int sensorPins[] = {3, 4, 5, 6};  // พินของเซนเซอร์เหรียญ 1, 2, 5, 10 บาท
const int BUZZER = 8;                   // พินสำหรับ Active Buzzer
int coinCounts[] = {0, 0, 0, 0};        // ตัวแปรสำหรับเก็บจำนวนเหรียญที่นับได้แต่ละประเภท
int prevStates[] = {HIGH, HIGH, HIGH, HIGH};  // สถานะก่อนหน้าของเซนเซอร์แต่ละตัว

// ฟังก์ชันแสดงข้อความเริ่มต้นบนจอ LCD
void intro() {
  lcd.setCursor(2, 0);
  lcd.print("COIN SORTING");
  delay(2000);
  lcd.clear();
}

// ฟังก์ชันสำหรับเล่นเสียง Buzzer
void beep(int duration) {
  digitalWrite(BUZZER, HIGH);
  delay(duration);
  digitalWrite(BUZZER, LOW);
}

// ฟังก์ชันการตั้งค่า
void setup() {
  lcd.init();
  lcd.backlight();
  intro();
  lcd.setCursor(0, 0);
  lcd.print("1B  2B  5B  10B");

  for (int i = 0; i < 4; i++) {
    pinMode(sensorPins[i], INPUT);  // ตั้งค่าพินเซนเซอร์เป็นอินพุต
  }
  
  pinMode(BUZZER, OUTPUT);  // ตั้งค่าพิน Buzzer เป็นเอาต์พุต
  digitalWrite(BUZZER, LOW);  // ปิดเสียง Buzzer ในตอนเริ่มต้น
}

// ฟังก์ชันการทำงานหลัก
void loop() {
  for (int i = 0; i < 4; i++) {
    int currState = digitalRead(sensorPins[i]);  // อ่านสถานะปัจจุบันของเซนเซอร์
    if (prevStates[i] == LOW && currState == HIGH) {  // ตรวจสอบการเปลี่ยนแปลงสถานะ
      coinCounts[i]++;  // เพิ่มจำนวนเหรียญที่นับได้
      beep(200);  // เล่นเสียง Buzzer
    }
    prevStates[i] = currState;  // อัปเดตสถานะก่อนหน้า
    lcd.setCursor(i * 4, 1);  // ปรับตำแหน่งแสดงผลบนจอ LCD
    lcd.print(coinCounts[i]);  // แสดงจำนวนเหรียญที่นับได้
  }
  
  delay(100);  // หน่วงเวลาสั้นๆ เพื่อป้องกันการกดปุ่มซ้ำ (debounce)
}
