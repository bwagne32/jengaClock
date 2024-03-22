#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h> // for WiFi shield or ESP32
#include "NTP.h"

#include "numbers.h"
#include "login.h"

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwmBoard1 = Adafruit_PWMServoDriver(0x40); // Digits 1 and 2 (hours)
Adafruit_PWMServoDriver pwmBoard2 = Adafruit_PWMServoDriver(0x41); // Digits 3 and 4 (minutes)
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);
// you can also call it with a different address and I2C interface
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates


WiFiUDP wifiUdp;
NTP ntp(wifiUdp);


void setup(){
  Serial.begin(115200);

  // Wifi & time setup
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting ...");
    delay(500);
  }
  ntp.ruleDST("EST", Last, Sun, Mar, 2, -240); // last sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
  ntp.ruleSTD("EST", Last, Sun, Oct, 3, -240); // last sunday in october 3:00, timezone +60min (+1 GMT)
  ntp.begin();

  // PWM setup
  pwmBoard1.begin();
  pwmBoard2.begin();
  

  pwmBoard1.setOscillatorFrequency(27000000);
  pwmBoard2.setOscillatorFrequency(27000000);

  pwmBoard1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  pwmBoard2.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  Wire.setClock(200000);

}

/* Example
    pwm.setPWM(0, 4096, 0); // Pin 0 fully on
    pwm.serPWM(0, 0, 4096); // Pin 0 fully off
*/


void loop(){
  ntp.update();
  
}