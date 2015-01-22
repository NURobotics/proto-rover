#ifndef PROTOROVER_H
#define PROTO_ROVER_H

#include <Arduino.h>

enum WHEEL {
  FRONT_LEFT,
  FRONT_RIGHT,
  MID_LEFT,
  MID_RIGHT,
  BACK_LEFT,
  BACK_RIGHT,
  NUM_MOTORS
};

enum DIRECTION {
  FORWARD,
  BACKWARD
};

class Motor
{
public:
  Motor();
  Motor(int phase_pin, int enable_pin);
  void setDirection(int d) 
  { 
    if((d==FORWARD || d==BACKWARD) && enable_pin != -1) {
      digitalWrite(enable_pin, d); 
      direction = d; 
    };
  }
  void setSpeed(double speed);
  void drive(double s=1.0);
  bool setPhasePin(int pp);
  bool setEnablePin(int ep);
  void stop();
  void reset(int ppin=-1, int epin=-1);
private:
  int phase_pin;
  int enable_pin;  
  double duty_cycle;
  double speed;
  int direction;
};

class Rover
{
public:
  Rover() {};
  void turnLeft();
  void turnRight();
  void driveForward();
  void driveBackward();
  void setSpeed(double speed);
  void reset();
  void reset(int left_phase, int left_enable, int right_phase, int right_enable);
  Motor motors[NUM_MOTORS];
private:
  double speed;
};

#endif // PROTOROVER_H
