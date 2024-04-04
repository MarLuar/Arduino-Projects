#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
int state = 0;
int department = 0;
bool teacherAvailable = false;
bool isStudent = false;
bool selectorChosen = false;
bool teacherSelected = false;
unsigned long startTime = 0;
const unsigned long loopDuration = 5000; 

String englishTeachers[] = {"Miss Caspe", "Miss Kath", "Miss Inoc"};
String ictTeachers[] = {"Sir Rod", "Sir Tata", "Miss Goldee"};
int numEnglishTeachers = 3;
int numICTTeachers = 3;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("1. Parent   ");
  lcd.setCursor(0, 1);
  lcd.print("2. Student");
}

void loop()
{
  if (!selectorChosen && Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (incomingByte == '1') {
      isStudent = false;
      selectorChosen = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("English DEPT");
      lcd.setCursor(0, 1);
      lcd.print(englishTeachers[state]);
    } else if (incomingByte == '2') {
      isStudent = true;
      selectorChosen = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ICT DEPT");
      lcd.setCursor(0, 1);
      lcd.print(ictTeachers[state]);
    } else if (incomingByte == '6') {
      // Reset Arduino
      resetArduino();
    }
  } else if (selectorChosen && Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (!teacherSelected) {
      if (incomingByte == '1') {
        state++;
        if (department == 0) {
          state %= numEnglishTeachers;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("English DEPT");
          lcd.setCursor(0, 1);
          lcd.print(englishTeachers[state]);
        } else if (department == 1) {
          state %= numICTTeachers;
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("ICT DEPT");
          lcd.setCursor(0, 1);
          lcd.print(ictTeachers[state]);
        }
        teacherAvailable = false; 
      } else if (incomingByte == '2') {
        department = (department == 0) ? 1 : 0;
        state = 0;
        lcd.clear();
        if (department == 0) {
          lcd.setCursor(0, 0);
          lcd.print("English DEPT");
          lcd.setCursor(0, 1);
          lcd.print(englishTeachers[state]);
        } else {
          lcd.setCursor(0, 0);
          lcd.print("ICT DEPT");
          lcd.setCursor(0, 1);
          lcd.print(ictTeachers[state]);
        }
        teacherAvailable = false;
      } else if (incomingByte == '3') {
        teacherSelected = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Waiting...");
        lcd.setCursor(0, 1);
        lcd.print("               ");
        Serial.println("A parent/student is looking for you, are you available or on hold?");
        startTime = millis(); 
      }
    } else {
      if (!teacherAvailable) {
        if (incomingByte == '4') {
          teacherAvailable = true;
          lcd.clear();
          lcd.setCursor(1, 0);
          if (department == 0) {
            lcd.print(englishTeachers[state]);
          } else {
            lcd.print(ictTeachers[state]);
          }
          lcd.setCursor(1, 1);
          lcd.print(" is available");
          Serial.println("Teacher is available.");
          selectorChosen = false; 
          teacherSelected = false; 
        } else if (incomingByte == '5') {
          teacherAvailable = false;
          lcd.clear();
          lcd.setCursor(1, 0);
          if (department == 0) {
            lcd.print(englishTeachers[state]);
          } else {
            lcd.print(ictTeachers[state]);
          }
          lcd.setCursor(1, 1);
          lcd.print(" is on hold");
          Serial.println("Teacher is on hold.");
          selectorChosen = false; 
          teacherSelected = false; 
        }
        
        // Check if the waiting time has elapsed
        if (millis() - startTime >= loopDuration) {
          selectorChosen = false; 
          teacherSelected = false; 
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Are you?");
          lcd.setCursor(0, 1);
          lcd.print("1. Parent    2. Student");
        }
      }
    }
  }
}

void resetArduino() {
  asm volatile ("  jmp 0");
}
