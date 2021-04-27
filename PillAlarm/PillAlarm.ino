#include <DS3231.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <EEPROM.h>

// $$
// [_]D

DS3231  rtc(SDA, SCL);
Time t;
LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

const byte ROWS = 4;
const byte COLS = 4;
int piezoPin = 28;
bool sound = true;

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte colPins[COLS] = {9, 8, 7, 6};
byte rowPins[ROWS] = {5, 4, 3, 2};

int editalarm = 0;
bool stopped = false;
bool alarm_on = false;
int min1 = 9;
int hou1 = 02;
int min2;
int hou2;
int screen = 0;
int test = 1;
int input[] = {0,0};
int pos = 0;
int ledPins[] = {36, 35, 34, 33, 32, 31, 30};
int AM = 52;
int PM = 53;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup()
{
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(4,0);
  lcd.print("MedDuino");
  lcd.setCursor(3,1);
  lcd.print("Hami. MESA");
  delay(2000);
  lcd.clear();
  rtc.begin();
  //rtc.setTime(17,35,00);
  //rtc.setDate(28,02,2020);
  //rtc.setDOW(5);
}

void loop(){
  t = rtc.getTime();
  char key = keypad.getKey();
  if (key){
    Serial.println(key);
     tone(piezoPin, 523.251, 50);
    if (key == 'A'){
      screen = 1;
    }
    if (key == 'B'){
      screen = 2;
    }
    if (key == 'C'){
      screen = 0;
    }
    lcd.clear();
  }
  if (t.sec == 0 && (t.hour == hou1 && t.min == min1 || t.hour == hou2 && t.min == min2)){
    lcd.clear();
    alarm_on = true;
    lcd.setCursor(1,0);
    lcd.print(rtc.getDOWStr());
    if (t.hour == hou1 && t.min == min1){
      lcd.setCursor(0,1);
      lcd.print("Take row 1");
      digitalWrite(AM, HIGH);
    }
  else if (t.hour == hou2 && t.min == min2){
    lcd.setCursor(0,1);
    lcd.print("Take row 2");
    digitalWrite(PM, HIGH);
    }
    digitalWrite(ledPins[t.dow], HIGH);
    while (alarm_on == true){
      tone(piezoPin, 723.251, 100);
       char key = keypad.getKey();
       if (key == 'D'){
        noTone(piezoPin);
          alarm_on = false;
       }
    }
    noTone(piezoPin);

    digitalWrite(AM, LOW);
    digitalWrite(PM, LOW);
    digitalWrite(ledPins[t.dow], LOW);
  }
  
    
  if (screen == 0){
    lcd.setCursor(1,0);
    lcd.print(rtc.getTimeStr());
    lcd.setCursor(13,0);
    lcd.print(rtc.getDOWStr());
    lcd.setCursor(1,1);
    lcd.print(rtc.getDateStr());
  }

if (screen == 1){
    lcd.setCursor(0,0);
    lcd.print("Press * to set");
    lcd.setCursor(0,1);
    lcd.print("Alarm 1");
    if (key == '*'){
      lcd.clear();
      editalarm = 1;
      input[0] = 0;
      input[1] = 0;
      pos = 0;
    }

    // Yes the code is messy and yes, I will go back and improve it.
    // And yes, I know how functions and classes work
    
      while (editalarm == 1){
        char key = keypad.getKey();
         if (key){
           Serial.println(key);
           tone(piezoPin, 523.251, 50);
         }
        lcd.setCursor(0,0);
        lcd.print("Set hour");
        lcd.setCursor(0,1);
        if (input[0] == 0) {
          lcd.print("0");
        } else {
          lcd.print(input[0]);
        }
        lcd.setCursor(1,1);
        if (input[1] == 0) {
          lcd.print("0");
        } else {
          lcd.print(input[1]);
        }
        if (key == '#'){
          Serial.println(pos);
          if (pos == 0){
            pos = 1;
          }
          else {
            pos = 0;
          }
        }
        else if (key == '*'){
          Serial.println(pos);
          if (pos == 0){
            pos = 1;
          }
          else {
            pos = 0;
          }
        }
        
        if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '*' && key != '#'){
          Serial.println(editalarm);
          if (pos == 0 && (key == '0' || key == '1' || key == '2')){
            input[0] = key-'0';
            Serial.println(input[0]);
            Serial.println(key-'0');
            if (key == '2' && input[1] > 3){
              input[1] = 0;
            }
          }
          if (pos == 1){
            if (input[0] == 2){
              if (key == '0' || key == '1' || key == '2' || key == '3'){
                input[1] = key-'0';
                Serial.println(input[1]);
                Serial.println(key-'0');
              }
            }
            else {
              input[1] = key-'0';
                Serial.println(input[1]);
                Serial.println(key-'0');
            }
          }
        }
        if (key == 'C'){
          editalarm = 2;
          hou1 = (input[0]*10)+input[1];
          Serial.println(hou1);
          Serial.println(editalarm);
        }
      }

      
      input[0] = 0;
      input[1] = 0;
      pos = 0;
      
      while (editalarm == 2){
        char key = keypad.getKey();
         if (key){
           Serial.println(key);
           tone(piezoPin, 523.251, 50);
         }
        lcd.setCursor(0,0);
        lcd.print("Set minute");
        lcd.setCursor(0,1);
        if (input[0] == 0) {
          lcd.print("0");
        } else {
          lcd.print(input[0]);
        }
        lcd.setCursor(1,1);
        if (input[1] == 0) {
          lcd.print("0");
        } else {
          lcd.print(input[1]);
        }
        if (key == '#'){
          Serial.println(pos);
          if (pos == 0){
            pos = 1;
          }
          else {
            pos = 0;
          }
        }
        else if (key == '*'){
          Serial.println(pos);
          if (pos == 0){
            pos = 1;
          }
          else {
            pos = 0;
          }
        }
        
        if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '*' && key != '#'){
          Serial.println("test12");
          if (pos == 0 && (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5')){
            input[0] = key-'0';
            Serial.println(input[0]);
            Serial.println(key-'0');
          }
          if (pos == 1){   
                input[1] = key-'0';
                Serial.println(input[1]);
                Serial.println(key-'0');
            
          }
        }
        if (key == 'C'){
          editalarm = 0;
          min1 = (input[0]*10)+input[1];
          Serial.println(min1);
          Serial.println(editalarm);
          input[0] = 0;
          input[1] = 0;
          pos = 0;
          screen = 0;
          lcd.clear();
        }
    }

//8^)


  }
  
  if (screen == 2){
    lcd.setCursor(0,0);
    lcd.print("Press * to set");
    lcd.setCursor(0,1);
    lcd.print("Alarm 2");
    if (key == '*'){
      lcd.clear();
      editalarm = 1;
      input[0] = 0;
      input[1] = 0;
      pos = 0;
    }

    // Yes the code is messy and yes, I will go back and improve it.
    // And yes, I know how functions and classes work
    
      while (editalarm == 1){
        char key = keypad.getKey();
         if (key){
           Serial.println(key);
           tone(piezoPin, 523.251, 50);
         }
        lcd.setCursor(0,0);
        lcd.print("Set hour");
        lcd.setCursor(0,1);
        if (input[0] == 0) {
          lcd.print("0");
        } else {
          lcd.print(input[0]);
        }
        lcd.setCursor(1,1);
        if (input[1] == 0) {
          lcd.print("0");
        } else {
          lcd.print(input[1]);
        }
        if (key == '#'){
          Serial.println(pos);
          if (pos == 0){
            pos = 1;
          }
          else {
            pos = 0;
          }
        }
        else if (key == '*'){
          Serial.println(pos);
          if (pos == 0){
            pos = 1;
          }
          else {
            pos = 0;
          }
        }
        
        if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '*' && key != '#'){
          Serial.println(editalarm);
          if (pos == 0 && (key == '0' || key == '1' || key == '2')){
            input[0] = key-'0';
            Serial.println(input[0]);
            Serial.println(key-'0');
            if (key == '2' && input[1] > 3){
              input[1] = 0;
            }
          }
          if (pos == 1){
            if (input[0] == 2){
              if (key == '0' || key == '1' || key == '2' || key == '3'){
                input[1] = key-'0';
                Serial.println(input[1]);
                Serial.println(key-'0');
              }
            }
            else {
              input[1] = key-'0';
                Serial.println(input[1]);
                Serial.println(key-'0');
            }
          }
        }
        if (key == 'C'){
          editalarm = 2;
          hou2 = (input[0]*10)+input[1];
          Serial.println(hou2);
          Serial.println(editalarm);
        }
      }

      
      input[0] = 0;
      input[1] = 0;
      pos = 0;
      
      while (editalarm == 2){
        char key = keypad.getKey();
         if (key){
           Serial.println(key);
           tone(piezoPin, 523.251, 50);
         }
        lcd.setCursor(0,0);
        lcd.print("Set minute");
        lcd.setCursor(0,1);
        if (input[0] == 0) {
          lcd.print("0");
        } else {
          lcd.print(input[0]);
        }
        lcd.setCursor(1,1);
        if (input[1] == 0) {
          lcd.print("0");
        } else {
          lcd.print(input[1]);
        }
        if (key == '#'){
          Serial.println(pos);
          if (pos == 0){
            pos = 1;
          }
          else {
            pos = 0;
          }
        }
        else if (key == '*'){
          Serial.println(pos);
          if (pos == 0){
            pos = 1;
          }
          else {
            pos = 0;
          }
        }
        
        if (key && key != 'A' && key != 'B' && key != 'C' && key != 'D' && key != '*' && key != '#'){
          Serial.println("test12");
          if (pos == 0 && (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5')){
            input[0] = key-'0';
            Serial.println(input[0]);
            Serial.println(key-'0');
          }
          if (pos == 1){   
                input[1] = key-'0';
                Serial.println(input[1]);
                Serial.println(key-'0');
            
          }
        }
        if (key == 'C'){
          editalarm = 0;
          min2 = (input[0]*10)+input[1];
          Serial.println(min2);
          Serial.println(editalarm);
          input[0] = 0;
          input[1] = 0;
          pos = 0;
          screen = 0;
          lcd.clear();
        }
    }

//:^(


  }
}
