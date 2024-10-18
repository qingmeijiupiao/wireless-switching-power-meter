/*
 * @LastEditors: qingmeijiupiao
 * @Description
 * @Author: qingmeijiupiao
 * @Date: 2024-09-14 21:27:28
 */
#include <Arduino.h>
#include <U8g2lib.h>
#include "PowerCtrl.hpp"

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0,U8X8_PIN_NONE,/*clk*/5,/*data*/4);
                        // 设备名，制造商，电量
const int buttonPIN = 0;    // 定义按键端口
const int LEDPIN = 3;    // 定义LED端口
const int buzzer = 10;    // 定义蜂鸣器端口
const int volt_read_pin=1;    // 定义电池电压采样端口

namespace Battery{
  float battery_voltage = 0;    // 电池电压
  float quantity = 0;    // 电池电量
  const float mini_voltage = 3.3;    // 电池最低电压
  void voltreadtask(void *pvParameters){
    while (true){
      battery_voltage = 2*analogReadMilliVolts(volt_read_pin)/1000.0;
      quantity = (battery_voltage-mini_voltage)/4.2-mini_voltage;
      if(quantity<0){
        quantity = 0;
      }
      if(quantity>1){
        quantity = 1;
      }
      delay(1000);
    }
  }
}
void screentask( void *pvParameters ) {
  u8g2.begin();
  PowerCtrl::ctrl_send_data(true,10);
  while (true){
    
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.setCursor(4,20);
    u8g2.print(PowerCtrl::power_data.voltage);
    u8g2.print("V");
    u8g2.setCursor(4,45);
    u8g2.print(PowerCtrl::power_data.current);
    u8g2.print("A");
    
    u8g2.setCursor(4,60);
    if(digitalRead(buttonPIN)){
      u8g2.print("ON");
    }else{
      u8g2.print("OFF");
    }
    u8g2.sendBuffer(); 
    delay(100);
  }
}
void LEDTask(void* p){
    pinMode(LEDPIN, OUTPUT);
    digitalWrite(LEDPIN, HIGH);
    // ledcAttachPin(LEDPIN, 3);
    // ledcSetup(0, 5000, 8);
    // while(1){
    //   for(int i=0;i<255;i++){
    //     ledcWrite(3,i);
    //     delay(5);
    //   }
    //   for(int i=0;i<255;i++){
    //     ledcWrite(3,255-i);
    //     delay(5);
    //   }
    // }
}

void ctrl(void *p){
  bool state=digitalRead(buttonPIN);
  while(1){
    if(state!=digitalRead(buttonPIN)){
      if(digitalRead(buttonPIN)){
        PowerCtrl::power_on();
      }else{
        PowerCtrl::power_off();
      }
    }
    state=digitalRead(buttonPIN);
    delay(1);
  }
}
void setup() 
{
  
  Serial.begin(115200);
  pinMode(buttonPIN, INPUT_PULLUP);

  NVSSTORAGE::NVS_read();
  PowerCtrl::setup();
  xTaskCreate(screentask, "screentask", 16384, NULL, 4, NULL);
  xTaskCreate(ctrl, "ctrl", 2048, NULL, 1, NULL);
  // //xTaskCreate(LEDTask, "LEDTask", 2048, NULL, 5, NULL);
  //xTaskCreate(Battery::voltreadtask, "voltreadtask", 512, NULL, 1, NULL);

  //PowerCtrl::send_pair_package();
  


}
 
void loop() {
}
