#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define button1 3
#define button2 4
#define button3 5
#define button4 6
#define button5 7
#define button6 8
#define button7 9
#define button8 10

LiquidCrystal_I2C lcd(0x27, 20, 4);
LiquidCrystal_I2C lcd2(0x23, 20, 4);
LiquidCrystal_I2C lcd3(0x25, 20, 4);
int state = 0;
int department = 0;
bool teacherAvailable = false;
bool isStudent = false;
bool availability = false;
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
  pinMode(button1, INPUT);
  pinMode(button2, INPUT);
  pinMode(button3, INPUT);
  pinMode(button4, INPUT);
  pinMode(button5, INPUT);
  pinMode(button6, INPUT);
  pinMode(button7, INPUT);
  pinMode(button8, INPUT);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd2.init();
  lcd2.backlight();
  lcd2.clear();
  lcd3.init();
  lcd3.backlight();
  lcd3.clear();
  lcd.setCursor(0, 0);
  lcd.print("Are you a:");
  lcd.setCursor(0, 1);
  lcd.print("1.Parent (button 1)");
  lcd.setCursor(0, 2);
  lcd.print("2.Student (button 2)");
  lcd2.setCursor(0, 0);
  lcd2.print("Awaiting Inquiries..");
  lcd3.setCursor(0, 0);
  lcd3.print("Awaiting Inquiries..");    
}

void loop()
{
  Serial.print("Button 1: ");
  Serial.println(digitalRead(button1));
  Serial.print("Button 2: ");
  Serial.println(digitalRead(button2));
  Serial.print("Button 3: ");
  Serial.println(digitalRead(button3));
  Serial.print("Button 4: ");
  Serial.println(digitalRead(button4));
  Serial.print("Button 5: ");
  Serial.println(digitalRead(button5));
  Serial.print("Button 6: ");
  Serial.println(digitalRead(button6));
  Serial.print("Button 7: ");
  Serial.println(digitalRead(button7));
  Serial.print("Button 8: ");
  Serial.println(digitalRead(button8)); 
 
  if (!selectorChosen && (Serial.available() > 0 || digitalRead(button1) == HIGH || digitalRead(button2) == HIGH || digitalRead(button6) == HIGH)) {
    char incomingByte = Serial.read();
    if (incomingByte == '1' || digitalRead(button1) == HIGH) {
      isStudent = false;
      selectorChosen = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Select a department");
      lcd.setCursor(0, 1);
      lcd.print("and a teacher");
      lcd.setCursor(0, 2);
      lcd.print("ENG DEPT (press 2)");
      lcd.setCursor(0, 3);
      lcd.print(englishTeachers[state] + " (press 1)"); 
  } else if (incomingByte == '2' || digitalRead(button2) == HIGH) {
      isStudent = true;
      selectorChosen = true;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Select a department");
      lcd.setCursor(0, 1);
      lcd.print("and a teacher");
      lcd .setCursor(0, 2);
      lcd.print("ICT DEPT (press 2)");
      lcd.setCursor(0, 3);
      lcd.print(ictTeachers[state] + " (press 1)" );
  } else if (incomingByte == '6' || digitalRead(button6) == HIGH) {
        // Reset Arduino
        resetArduino();
      }
  } else if (selectorChosen || Serial.available() > 0) {
    char incomingByte = Serial.read();
    if (!teacherSelected) {
      if (incomingByte == '1' || digitalRead(button1) == HIGH) {
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
      } else if (incomingByte == '2'|| digitalRead(button2) == HIGH || digitalRead(button6) == HIGH) {
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
      } else if (incomingByte == '3' || digitalRead(button3) == HIGH || digitalRead(button6) == HIGH) {
        teacherSelected = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Awaiting Teacher's");
        lcd.setCursor(0, 1);
        lcd.print("response");    
        lcd.setCursor(0, 2);
        lcd.print("Please wait ...");
      if (incomingByte == '6' || digitalRead(button6) == HIGH) {
        resetArduino();
      }
          if (isStudent) {
                      if (department == 0) { 
                    Serial.println("A student is looking for you, are you available or on hold?");
                    lcd3.clear(); 
                    lcd3.setCursor(0, 0);
                    lcd3.print("A student is looking");
                    lcd3.setCursor(0, 1);
                    lcd3.print("for you, are you: ");
                    lcd3.setCursor(0, 2);
                    lcd3.print("Available (button1)");
                    lcd3.setCursor(0, 3);
                    lcd3.print("On hold (button 2)");
                } else { 
                    Serial.println("A student is looking for you, are you available or on hold?");
                    lcd2.clear();
                    lcd2.setCursor(0, 0);
                    lcd2.print("A student is looking");
                    lcd2.setCursor(0, 1);
                    lcd2.print("for you");
                    lcd2.setCursor(0, 2);
                    lcd2.print("Available (button1)");
                    lcd2.setCursor(0, 3);
                    lcd2.print("On hold (button 2)");
                }
} else {
                         if (department == 0) { 
                    Serial.println("A parent is looking for you, are you available or on hold?");
                    lcd3.clear(); 
                    lcd3.setCursor(0, 0);
                    lcd3.print("A parent is looking");
                    lcd3.setCursor(0, 1);
                    lcd3.print("for you");
                    lcd3.setCursor(0, 2);
                    lcd3.print("Available (button1)");
                    lcd3.setCursor(0, 3);
                    lcd3.print("On hold (button 2)");
                } else {
                    Serial.println("A parent is looking for you, are you available or on hold?");
                    lcd2.clear(); 
                    lcd2.setCursor(0, 0);
                    lcd2.print("A parent is looking");
                    lcd2.setCursor(0, 1);
                    lcd2.print("for you");
                    lcd2.setCursor(0, 2);
                    lcd2.print("Available (button1)");
                    lcd2.setCursor(0, 3);
                    lcd2.print("On hold (button 2)");
                }

  }
          startTime = millis(); 
      } 
    } else {
      if (!teacherAvailable) {
        if ((department == 0 && (incomingByte == '4' || digitalRead(button4) == HIGH || incomingByte == '5' || digitalRead(button5) == HIGH)) ||
            (department == 1 && (incomingByte == '7' || digitalRead(button7) == HIGH || incomingByte == '8' || digitalRead(button8) == HIGH || digitalRead(button6) == HIGH))) {
          if (incomingByte == '4' || digitalRead(button4) == HIGH || digitalRead(button6) == HIGH) {
            teacherAvailable = true;
            availability = true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(englishTeachers[state]);            
            lcd.setCursor(0, 1);
            lcd.print("is available.");
            lcd.setCursor(0, 2);
            lcd.print("Please come inside.");            
            Serial.println("Teacher is available.");
            selectorChosen = false; 
            teacherSelected = false; 
          } else if (incomingByte == '5' || digitalRead(button5) == HIGH || digitalRead(button6) == HIGH) {
            teacherAvailable = false;
            availability = true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print(englishTeachers[state]);
            lcd.setCursor(0, 1);
            lcd.print("is on hold."); 
            lcd.setCursor(0, 2);
            lcd.print("Kindly try ");
            lcd.setCursor(0, 3);
            lcd.print("inquiring later"); 
            Serial.println("Teacher is on hold.");
            selectorChosen = false; 
            teacherSelected = false;    
          } else if (incomingByte == '7' || digitalRead(button7) == HIGH || incomingByte == '8' || digitalRead(button8) == HIGH || digitalRead(button6) == HIGH) {
              if (incomingByte == '7' || digitalRead(button7) == HIGH) {
              teacherAvailable = true;
              availability = true;
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(ictTeachers[state]);            
              lcd.setCursor(0, 1);
              lcd.print(" is available.");
              lcd.setCursor(0, 2);
              lcd.print("Please come inside.");            
              Serial.println("Teacher is available.");
              selectorChosen = false; 
              teacherSelected = false; 
            } else if (incomingByte == '8' || digitalRead(button8) == HIGH || digitalRead(button6) == HIGH) {
              teacherAvailable = false;
              availability = true;
              lcd.clear();
              lcd.setCursor(0, 0);
              lcd.print(ictTeachers[state]);
              lcd.setCursor(0, 1);
              lcd.print("is on hold.");
              lcd.setCursor(0, 2);
              lcd.print("Kindly try ");
              lcd.setCursor(0, 3);
              lcd.print("inquiring later."); 
              Serial.println("Teacher is on hold.");
              selectorChosen = false; 
              teacherSelected = false;    
            }
            if (incomingByte == '6' || digitalRead(button6) == HIGH) {
        // Reset Arduino
        resetArduino();
      }
            } 
        }
      }
    }
  }
}

/*void loop() {
    if (!selectorChosen && (Serial.available() > 0 || digitalRead(button1) == HIGH || digitalRead(button2) == HIGH || digitalRead(button6) == HIGH)) {
        char incomingByte = Serial.read();
        if (incomingByte == '1' || digitalRead(button1) == HIGH) {
            isStudent = false;
            selectorChosen = true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Select a department");
            lcd.setCursor(0, 1);
            lcd.print("and a teacher");
            lcd.setCursor(0, 2);
            lcd.print("ENG DEPT (press 2)");
            lcd.setCursor(0, 3);
            lcd.print(englishTeachers[state] + " (press 1)");
        } else if (incomingByte == '2' || digitalRead(button2) == HIGH) {
            isStudent = true;
            selectorChosen = true;
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Select a department");
            lcd.setCursor(0, 1);
            lcd.print("and a teacher");
            lcd.setCursor(0, 0);
            lcd.print("ICT DEPT");
            lcd.setCursor(0, 1);
            lcd.print(ictTeachers[state]);
        } else if (incomingByte == '6' || digitalRead(button6) == HIGH) {
            // Reset Arduino
            resetArduino();
        }
    } else if (selectorChosen || Serial.available() > 0) {
        char incomingByte = Serial.read();
        if (!teacherSelected) {
            if (incomingByte == '1' || digitalRead(button1) == HIGH) {
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
            } else if (incomingByte == '2'|| digitalRead(button2) == HIGH) {
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
            } else if (incomingByte == '3' || digitalRead(button3) == HIGH) {
                teacherSelected = true;
                lcd.clear();
                lcd.setCursor(0, 0);
                lcd.print("Waiting...");
                lcd.setCursor(0, 1);
                lcd.print("               ");
                if (isStudent) {
                    if (department == 0) { // If ENG department selected
                        Serial.println("A student is looking for you, are you available or on hold?");
                        lcd3.clear(); // Clear LCD3
                        lcd3.setCursor(0, 0);
                        lcd3.print("A student is looking");
                        lcd3.setCursor(0, 1);
                        lcd3.print("for you");
                    } else { // If ICT department selected
                        Serial.println("A student is looking for you, are you available or on hold?");
                        lcd2.clear(); // Clear LCD2
                        lcd2.setCursor(0, 0);
                        lcd2.print("A student is looking");
                        lcd2.setCursor(0, 1);
                        lcd2.print("for you");    
                    }
                } else {
                    // Similar logic for parents
                }
                startTime = millis(); 
            }
        }
    }
}
 */
void resetArduino() {
  asm volatile ("  jmp 0");
}
