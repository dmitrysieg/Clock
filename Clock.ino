// Подключаем библиотеку для работы с дисплеем
#include <QuadDisplay2.h>
// библиотека для работы I²C
#include <Wire.h>
// библиотека для работы с часами реального времени
#include "TroykaRTC.h"

#define PIN_RELAY 4

// TYPES
enum State
{
    OFF,
    ON,
};

// CONSTANTS
uint8_t HOUR_ON = 9;
uint8_t HOUR_OFF = 21;
uint8_t MINUTE_ON = 0;
uint8_t MINUTE_OFF = 0;

// VARIABLES

// создаём объект класса QuadDisplay и передаём номер пина CS
QuadDisplay qd(10);

RTC clock;

uint8_t hour;
uint8_t minute;

uint8_t last_minute;
State state;

void process_clock() {
  clock.read();
  hour = clock.getHour();
  minute = clock.getMinute();
  
  if (hour < HOUR_ON || hour >= HOUR_OFF || (hour == HOUR_ON && minute < MINUTE_ON) || (hour == HOUR_OFF && minute >= MINUTE_OFF) ) {
    digitalWrite(PIN_RELAY, LOW);
    state = OFF;
  } else {
    digitalWrite(PIN_RELAY, HIGH);
      state = ON;
  }
}

void setup()
{
  qd.begin();
  pinMode(PIN_RELAY, OUTPUT);
  
  // инициализация часов
  clock.begin();
  // метод установки времени и даты в модуль вручную
   //clock.set(12,11,00,12,01,2022,THURSDAY);    
  // метод установки времени и даты автоматически при компиляции
  //clock.set(__TIMESTAMP__);
  // что бы время менялось при прошивки или сбросе питания
  // закоментируйте оба метода clock.set();

  process_clock();
}

void loop()
{
  process_clock();

  qd.displayScore(hour, minute, true);
  delay(1000);
}
