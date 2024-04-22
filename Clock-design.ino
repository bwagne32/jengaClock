#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <WiFi.h> // for WiFi shield or ESP32
#include "NTP.h"
#include <vector>

#include "numbers.h"
#include "login.h"

#define core0 0
#define core1 1

using numbers_h::convertStates;
using numbers_h::output;

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


// Seconds display
const uint8_t led1Pin = 5;
const uint8_t led2Pin = 6;


void setup(){
  Serial.begin(115200);

  // Wifi & time setup
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting ...");
    delay(500);
  }
  ntp.ruleDST("EST", First, Sun, Mar, 2, -240); // First sunday in march 2:00, timetone +120min (+1 GMT + 1h summertime offset)
  ntp.ruleSTD("EST", First, Sun, Oct, 3, -240-60); // First sunday in october 3:00, timezone +60min (+1 GMT)
  ntp.begin();

  // PWM setup
  pwmBoard1.begin();
  pwmBoard2.begin();
  

  pwmBoard1.setOscillatorFrequency(27000000);
  pwmBoard2.setOscillatorFrequency(27000000);

  pwmBoard1.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  pwmBoard2.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates

  Wire.setClock(1000);

  // LEDS
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);

  //xTaskCreatePinnedToCore(displaySeconds, "show seconds", 2048, nullptr, 2, NULL, core0);
}

/* Example
    pwm.setPWM(0, 4096, 0); // Pin 0 fully on
    pwm.serPWM(0, 0, 4096); // Pin 0 fully off
*/

// Seg 1 on  [360,360,330,380,380,380,160]
// Seg 1 off [160,180,140,200,200,200,160] 
std::vector<uint16_t> seg1on = {360,360,330,380,380,380,160};
std::vector<uint16_t> seg1off = {160,180,140,200,200,200,160};

// Seg 2 on  [340,380,360,360,350,380,300]
// Seg 2 off [160,200,200,200,180,180,140]
std::vector<uint16_t> seg2on = {340,380,360,360,350,380,300};
std::vector<uint16_t> seg2off = {160,200,200,200,180,180,140};

std::vector<uint16_t> seg3on = {380,380,370,370,330,380,340};
std::vector<uint16_t> seg3off = {180,180,180,180,140,180,180}; //

std::vector<uint16_t> seg4on = {360,300,360,375,360,320,340}; // 
std::vector<uint16_t> seg4off = {180,140,180,180,180,160,180};



void loop(){
  /*
  while(1){
    for(int i = 0; i < 10; i++){
      output(pwmBoard2, 0, 6, convertStates(i), seg3on, seg3off);
      delay(1000);
      output(pwmBoard2, 8, 14, convertStates(i), seg4on, seg4off);
      delay(1000);
    }

  }*/
  
  ntp.update();

  //output(pwmBoard1, 0, 6, convertStates((11 % 12) < 10 ? -1:1), seg1on, seg1off);
  //output(pwmBoard1, 8, 14, convertStates(int(11 % 12 % 10)), seg2on, seg2off);

  int hour = ntp.hours();
  if(hour > 12){
    hour -= 12;
  }
  else if(hour == 0){
    hour = 12;
  }

  output(pwmBoard1, 0, 6, convertStates(hour / 10), seg1on, seg1off);
  //Serial.println(ntp.minutes() % 10);
  output(pwmBoard1, 8, 14, convertStates(hour % 10), seg2on, seg2off);
  //Serial.println(ntp.minutes() / 10);

  output(pwmBoard2, 0, 6, convertStates(ntp.minutes() / 10), seg3on, seg3off);
  //Serial.println(ntp.minutes() % 10);
  output(pwmBoard2, 8, 14, convertStates(int(ntp.minutes() % 10)), seg4on, seg4off);
  //Serial.println(ntp.minutes() / 10);





  //Serial.println(ntp.formattedTime("%A %T")); // debugging
  //output(pwmBoard2, 6, 12, convertStates(ntp.minutes() % 10)); // segment 4
  //output(pwmBoard2, 0, 11, convertStates(ntp.minutes() / 10)); // segment 3
  //output(pwmBoard1, 6, 12, convertStates(ntp.hours() % 12 % 10)); // segment 2  
  //output(pwmBoard2, 0, 11, convertStates((ntp.hours() < 10) ? -1 : 1)); // segment 1
  //Serial.println((ntp.hours() < 10) ? -1 : 1); // debugging
  delay(500);
}

void displaySeconds(void *pvParameters){
  while(true){
    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, LOW);
    delay(1000);
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, HIGH);
    delay(1000);
  }
}
