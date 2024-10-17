/*
 * @LastEditors: qingmeijiupiao
 * @Description: 主程序，用于控制电压、电流测量、显示及按键操作等
 * @Author: qingmeijiupiao
 * @Date: 2024-05-26 20:23:08
 */

#include <Arduino.h>
#include "PINS.h"
#include "powerctrl.hpp"// 电源控制
#include "TemperatureSensor.hpp"// 温度传感器
#include "buzz.hpp"// 蜂鸣器
#include "WIRELESSCTRL.hpp"// 无线控制
#include "BUTTONS.HPP"// 按键
#include "SCREEN.hpp"// 屏幕

// 初始化各种模块
POWERCTRL_t power_output(PowerPin);           // 电源控制
TemperatureSensor_t Temperature_sensor;       // 温度传感器
BUZZ_t buzz(BUZZER_PIN);                      // 蜂鸣器

// 初始化程序
void setup() {
    Serial.begin(115200); // 初始化串口

    // 初始化相关外设
    Temperature_sensor.setup();//温度传感器初始化
    power_output.setup();//电源控制初始化
    power_output.off();//插电默认关闭电源
    buzz.setup();//蜂鸣器初始化
    NVSSTORAGE::NVS_read(); // 读取nvs存储
    WIRELESSCTRL::wireless_ctrl_setup();// 无线控制初始化
    // 创建任务
    xTaskCreate(BUTTON::button_task, "button_task", 512, NULL, 5, NULL); // 按键任务
    xTaskCreate(POWERMETER::updatePower, "updatePower", 2048, NULL, 5, NULL);// 电压电流更新任务
    xTaskCreate(SCREEN::updatescreen, "updatescreen", 8192, NULL, 5, NULL);// 屏幕更新任务
    
}

// Arduino主循环,本项目不使用
void loop() {}
