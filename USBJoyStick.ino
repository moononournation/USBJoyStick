#define XPIN A0
#define YPIN A1
#define AVG_READ 100
#define OUT_SERIAL Serial
#define ANALOG_MAX 1023

#include "Joystick.h"

// Create Joystick
Joystick_ Joystick;

long x_centre;
long y_centre;

long avg_reading(uint8_t pin) {
  long t = 0;
  for (int i = 0; i < AVG_READ; i++) {
    delay(10);
    t += analogRead(pin);
  }
  return t / AVG_READ;
}

long map_reading(long v, long v_centre, long out_from, long out_centre, long out_to) {
  if (v == v_centre) {
    return out_centre;
  } else if (v < v_centre) {
    return map(v, 0, v_centre, out_from, out_centre);
  } else { // v > v_centre
    return map(v, v_centre, ANALOG_MAX, out_centre, out_to);
  }
}

void setup() {
  OUT_SERIAL.begin(115200);
  pinMode(XPIN, INPUT);
  pinMode(YPIN, INPUT);
  x_centre = avg_reading(XPIN);
  y_centre = avg_reading(YPIN);

  Joystick.setXAxisRange(0, 1023);
  Joystick.setYAxisRange(0, 1023);
  Joystick.begin(false);
}

void loop() {
  long x = map_reading(analogRead(XPIN), x_centre, 0, 511, 1023);
  long y = map_reading(analogRead(YPIN), y_centre, 1023, 511, 0);
  OUT_SERIAL.print("x:");
  OUT_SERIAL.print(analogRead(XPIN));
  OUT_SERIAL.print(",");
  OUT_SERIAL.print(x);
  OUT_SERIAL.print(" y:");
  OUT_SERIAL.print(analogRead(YPIN));
  OUT_SERIAL.print(",");
  OUT_SERIAL.println(y);
  Joystick.setXAxis(x);
  Joystick.setYAxis(y);
  Joystick.sendState();
  delay(50);
}
