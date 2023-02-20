#ifndef Utils_h
#define Utils_h

#include <Arduino.h>

// Константы для управления светодиодом
const int LED_PIN = 2;
const int LED_ON = LOW;
const int LED_OFF = HIGH;

class Utils {
  public:
    // Инициализация светодиода
    static void initLed() {
      pinMode(LED_PIN, OUTPUT);
      digitalWrite(LED_PIN, LED_OFF);
    }

    // Управление светодиодом
    static void setLed(bool value) {
      digitalWrite(LED_PIN, value ? LED_ON : LED_OFF);
    }
};

#endif
