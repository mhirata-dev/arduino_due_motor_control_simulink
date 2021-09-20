// mh_motorlib.h
// Copyright (C) by M.Hirata 2021
#include <Arduino.h>
//#include <SPI.h>

#ifndef __MH_MOTORLIB__
#define __MH_MOTORLIB__

#define PWM_FREQ        20000
#define MAX_PWM_DUTY    1050
#define PWM_CLOCK       (PWM_FREQ * MAX_PWM_DUTY * 2ul)

// PWM functions
void setup_pwm();
void pwmOUT(int16_t a, int16_t b, int16_t c);

// ADC functions
void setup_adc();
int16_t getPot1Voltage();
int16_t getPot2Voltage();
int16_t getIU();
int16_t getIV();
int16_t getIW();

// angle sensor (AS5048)
/*
void setupAngleSensor(uint16_t slaveSelectPin);
uint16_t readAngle(uint16_t slaveSelectPin);
*/

#endif
