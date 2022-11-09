#include <Servo.h>

// Arduino pin assignment
#define PIN_LED 9
#define PIN_M
#define PIN_POTENTIOMETER 0 // Potentiometer at Pin A3
// Add IR Sensor Definition Here !!!
#define PIN_SERVO 10

#define _DUTY_MIN 553  // servo full clock-wise position (0 degree)
#define _DUTY_NEU 1476 // servo neutral position (90 degree)
#define _DUTY_MAX 2399 // servo full counter-clockwise position (180 degree)

#define _DIST_MIN 100.0   // minimum distance to be measured (unit: mm)
#define _DIST_MAX 250.0
#define _EMA_ALPHA 0.5

#define LOOP_INTERVAL 30   // Loop Interval (unit: msec)
float dist_prev=_DIST_MAX;
float dist_ema;
//float duty;

Servo myservo;
unsigned long last_loop_time;   // unit: msec

void setup()
{
  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds(_DUTY_NEU);
  pinMode(9, OUTPUT);
  
  Serial.begin(2000000);
}

void loop()
{

  unsigned long time_curr = millis();
  int a_value, duty;
  // wait until next event time
  if (time_curr < (last_loop_time + LOOP_INTERVAL))
    return;
  last_loop_time += LOOP_INTERVAL;

  // Remove Next line !!!
  a_value = analogRead(PIN_POTENTIOMETER); 
  // Read IR Sensor value !!!
  // Convert IR sensor value into distance !!!
  float dist = (6762.0/(a_value-9)-4.0)*10.0 - 60.0;
  // we need distance range filter here !!!
  if (dist < _DIST_MIN) {  //범위 밖일때
    dist = _DIST_MIN - 10.0;    // Set Lower Value
    dist = dist_prev;
    digitalWrite(PIN_LED, 1);       // LED OFF
    
  } else if (dist > _DIST_MAX) {
    dist = _DIST_MAX + 10.0;    // Set Higher Value
    dist = dist_prev;
    digitalWrite(PIN_LED, 1);   
        // LED OFF
  } else {    // In desired Range //범위 안일때 여기서 최근 값 받음
    dist_prev=dist;
    digitalWrite(PIN_LED, 0);       // LED ON      
  }
  

  // we need EMA filter here !!!
  dist_ema =_EMA_ALPHA*dist+(1-_EMA_ALPHA)*dist_ema;

  
  if (dist_ema <= _DIST_MIN) {
    //digitalWrite(PIN_LED,1);
    myservo.writeMicroseconds(_DUTY_MIN);
       // LED OFF
  } else if (dist_ema >= _DIST_MAX) {
    //digitalWrite(PIN_LED,1);
    myservo.writeMicroseconds(_DUTY_MAX);     // LED OFF
  } else {    // In desired Range
    //digitalWrite(PIN_LED,0);
    duty = 1846/150*(dist-100)+553;
    myservo.writeMicroseconds(duty);

    //myservo.writeMicroseconds(1860*((_DIST_MAX-_DIST_MIN)*2*(dist_ema-180)/180+_DIST_MIN));
    
  }

  // map distance into duty
  //duty = map(a_value, 0, 1023, _DUTY_MIN, _DUTY_MAX);
  //myservo.writeMicroseconds(duty);

  // print IR sensor value, distnace, duty !!!
  Serial.print("MIN: "); Serial.print(_DIST_MIN);
  Serial.print(",IR: "); Serial.print(a_value);
  Serial.print(",dist:"); Serial.print(dist);
  Serial.print(",ema: "); Serial.print(dist_ema);
  Serial.print(",servo:"); Serial.print(duty);
  Serial.println(",MAX:"); Serial.print(_DIST_MAX);
  Serial.println("");
}
