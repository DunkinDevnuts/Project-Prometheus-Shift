#include <U8g2lib.h>
#include "h_shift_pattern.h"

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE); // initialization for the used OLED display

int current_gear = 0; //-1 = R, 0 = N, 1 = 1, 2 = 2, ...  
int switch_mode = 0;//mode 0 = debug screen, mode 1 = full screen

const int sensor_0 = 3;
const int sensor_1 = 4;
const int sensor_2 = 5;
const int sensor_3 = 6;
const int sensor_4 = 7;

void setup() 
{
  Serial.begin(115200);
  u8g2.begin();

  pinMode(sensor_0, INPUT_PULLUP);
  pinMode(sensor_1, INPUT_PULLUP);
  pinMode(sensor_2, INPUT_PULLUP);
  pinMode(sensor_3, INPUT_PULLUP);
  pinMode(sensor_4, INPUT_PULLUP);
}

void loop() 
{
  switch_mode = digitalRead(2); //set screen layout based on digital 2 state
    
  int sensorVal_0 = digitalRead(sensor_0);
  int sensorVal_1 = digitalRead(sensor_1);
  int sensorVal_2 = digitalRead(sensor_2);
  int sensorVal_3 = digitalRead(sensor_3);
  int sensorVal_4 = digitalRead(sensor_4);

  u8g2.clearBuffer();  //clear the internal memory

  u8g2.setFont(u8g2_font_pressstart2p_8u);

  if (switch_mode == LOW) //h shift pattern
  {
    u8g2.clearBuffer();
    u8g2.drawXBMP(0, 0, 128, 64, bitmaps_h_pattern[current_gear+1]);
  }

  else //debug screen
  {
    u8g2.clearBuffer();

    u8g2.setCursor(20, 20);
    u8g2.print(sensorVal_0); //print hall 0 val

    u8g2.setCursor(20, 20 + 32);
    u8g2.print(sensorVal_1); //print hall 1 val

    u8g2.setCursor(20 + 32, 20);
    u8g2.print(sensorVal_2);//print hall 2 val

    u8g2.setCursor(20 + 32, 20 + 32);
    u8g2.print(sensorVal_3);//print hall 3 val

    u8g2.setCursor(20 + 64, 20);
    u8g2.print(sensorVal_4);//print hall 4 val

    u8g2.setCursor(20 + 58, 20 + 24);
    u8g2.print("DEBUG");
    u8g2.setCursor(20 + 54, 20 + 32);
    u8g2.print("SCREEN");
    delay (25);
  }

  //if else statements for gear selection
  if (sensorVal_0 == LOW && sensorVal_2 == LOW)  //third gear
    {
      current_gear = 3;
    }
  else if (sensorVal_2 == LOW && sensorVal_4 == LOW) //fifth gear
    {
     current_gear = 5; 
    }
  else if (sensorVal_1 == LOW && sensorVal_3 == LOW) //fourth gear
    {
     current_gear = 4; 
    }
  else if (sensorVal_0 == LOW) //first gear
    {
     current_gear = 1;
    }
  else if (sensorVal_1 == LOW) //second gear
    {
     current_gear = 2;
    }
  else if (sensorVal_3 == LOW) //sixth gear
    {
     current_gear = 6;
    }
  else if (sensorVal_4 == LOW) //reverse gear
    {
     current_gear = -1;
    }
  else //Neutral
    {
      current_gear = 0;
    }

  u8g2.sendBuffer(); //transfer internal memory to the display
  delay(25);
}
