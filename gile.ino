#include <stdio.h>
#include <Wire.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

#define IR1_PIN_DATA A0
#define IR2_PIN_DATA A1
#define IR3_PIN_DATA A2

#define COIN_NUMBER 10

hd44780_I2Cexp lcd;
unsigned int counter = 0;

const int LCD_COLS = 16;
const int LCD_ROWS = 2;

const int accuracy = 10;

struct coin {
  float diameter;
  unsigned int value;
  unsigned int low_median;
  unsigned int middle_mdedian;
};

//int high_signal_basic;
int middle_signal_basic;
int low_signal_basic;

//int high_signal;
int middle_signal;
int low_signal;

bool check_diff(){
  if (((middle_signal - middle_signal_basic) > 50)
  || ((low_signal - low_signal_basic) > 50)) {
    return true;
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  int lcd_status = lcd.begin(LCD_COLS, LCD_ROWS);
  if(lcd_status) // non zero status means it was unsuccesful
  {
    // hd44780 has a fatalError() routine that blinks an led if possible
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(lcd_status); // does not return
  }
  lcd.lineWrap();
  
  lcd.print("Welcome to coin counter!");

  //high_signal_basic = analogRead(IR1_PIN_DATA);
  middle_signal_basic = analogRead(IR2_PIN_DATA);
  low_signal_basic = analogRead(IR3_PIN_DATA);

  Serial.print("Basic Middle Signal is ");
  Serial.print(middle_signal_basic);
  Serial.print("\n");

  Serial.print("Basic Low Signal is ");
  Serial.print(low_signal_basic);
  Serial.print("\n");

  delay(2000);
  lcd.setCursor(0, 1);
    //lcd.setCursor(0, 1);
  //lcd.print("");
  //lcd.print("Enter diameter of coin: ");
}

// the loop function runs over and over again forever
void loop() {
  //Serial.print("Test\n");
  //high_signal = analogRead(IR1_PIN_DATA);
  middle_signal = analogRead(IR2_PIN_DATA);
  low_signal = analogRead(IR3_PIN_DATA);

  if(check_diff()== true) {
    Serial.print("Middle Signal is ");
    Serial.print(middle_signal);
    Serial.print("\n");

    Serial.print("Low Signal is ");
    Serial.print(low_signal);
    Serial.print("\n");

    middle_signal = analogRead(IR2_PIN_DATA);
    low_signal = analogRead(IR3_PIN_DATA);

    counter++;
    
    while(check_diff()== true){
      middle_signal = analogRead(IR2_PIN_DATA);
      low_signal = analogRead(IR3_PIN_DATA);
    
    };
  }
  char msg[10];
  lcd.clear();
  sprintf(msg, "Value is: %d\n", counter);
  lcd.print(msg);
  
 
}
