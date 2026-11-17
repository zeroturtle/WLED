#include "wled.h"
#include "USB.h"
#include "USBHIDKeyboard.h"

USBHIDKeyboard Keyboard;

class E3ZKeyboardUsermod : public Usermod {
  private:
    const int sensorPin = 4; // Пин GPIO 4 для черного провода E3Z-R61
    bool lastState = HIGH;
    unsigned long lastTriggerTime = 0;
    const unsigned long debounceDelay = 250; // Защита от дребезга (мс)

  public:
    void setup() {
      pinMode(sensorPin, INPUT_PULLUP);
      Keyboard.begin();
      USB.begin();
    }

    void loop() {
      bool currentState = digitalRead(sensorPin);

      if (currentState == LOW && lastState == HIGH) {
        if (millis() - lastTriggerTime > debounceDelay) {
          Keyboard.write(KEY_RETURN); // Отправка Enter (VK_RETURN)
          lastTriggerTime = millis();
        }
      }
      lastState = currentState;
    }

    uint16_t getId() {
      return USERMOD_ID_UNSPECIFIED;
    }
};

static E3ZKeyboardUsermod e3z_keyboard;
REGISTER_USERMOD(e3z_keyboard);
