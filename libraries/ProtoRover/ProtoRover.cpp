#include <ProtoRover.h>

Motor::Motor()
{
  phase_pin = -1;
  enable_pin = -1;  
}

void Motor::reset(int ppin, int epin)
{
  setPhasePin(ppin);
  setEnablePin(epin);
  stop();
}

bool Motor::setPhasePin(int pp)
{
  if(pp >=3 && pp <= 11 &&
     pp != 4 && pp != 7 && pp != 8) {
    phase_pin = pp;
    return true;
  }
  phase_pin = -1;
  return false;
}

bool Motor::setEnablePin(int ep)
{
  if(ep >= 0 && ep <= 13) {
    enable_pin = ep;
    return true;
  }
  enable_pin = -1;
  return false;
}

void Motor::setSpeed(double s)
{
  speed = s;
  duty_cycle = constrain(speed*255, 0, 255);
}

void Motor::drive(double s)
{
  setSpeed(s);
}

void Motor::stop()
{
  setSpeed(0.);
}

void Rover::reset()
{
  // Setup all of the motors
  for(int i = 0; i < NUM_MOTORS; i++) motors[i].reset();
}

void Rover::reset(int left_phase, int left_enable, int right_phase, int right_enable)
{
  motors[FRONT_LEFT].reset(left_phase, left_enable);
  motors[MID_LEFT].reset(left_phase, left_enable);
  motors[BACK_LEFT].reset(left_phase, left_enable);
  motors[FRONT_RIGHT].reset(right_phase, right_enable);
  motors[MID_RIGHT].reset(right_phase, right_enable);
  motors[BACK_RIGHT].reset(right_phase, right_enable);
}

void Rover::turnLeft()
{
  motors[FRONT_LEFT].setDirection(FORWARD);
  motors[MID_LEFT].setDirection(FORWARD);
  motors[BACK_LEFT].setDirection(FORWARD);
  motors[FRONT_RIGHT].setDirection(FORWARD);
  motors[MID_RIGHT].setDirection(FORWARD);
  motors[BACK_RIGHT].setDirection(FORWARD);
}

void Rover::turnRight()
{
  motors[FRONT_LEFT].setDirection(BACKWARD);
  motors[MID_LEFT].setDirection(BACKWARD);
  motors[BACK_LEFT].setDirection(BACKWARD);
  motors[FRONT_RIGHT].setDirection(BACKWARD);
  motors[MID_RIGHT].setDirection(BACKWARD);
  motors[BACK_RIGHT].setDirection(BACKWARD);
}

void Rover::driveForward()
{
  motors[FRONT_LEFT].setDirection(BACKWARD);
  motors[MID_LEFT].setDirection(BACKWARD);
  motors[BACK_LEFT].setDirection(BACKWARD);
  motors[FRONT_RIGHT].setDirection(FORWARD);
  motors[MID_RIGHT].setDirection(FORWARD);
  motors[BACK_RIGHT].setDirection(FORWARD);
}

void Rover::driveBackward()
{
  motors[FRONT_LEFT].setDirection(FORWARD);
  motors[MID_LEFT].setDirection(FORWARD);
  motors[BACK_LEFT].setDirection(FORWARD);
  motors[FRONT_RIGHT].setDirection(BACKWARD);
  motors[MID_RIGHT].setDirection(BACKWARD);
  motors[BACK_RIGHT].setDirection(BACKWARD);
}

void Rover::setSpeed(double s)
{
  speed = s;
  for(int i = 0; i < NUM_MOTORS; i++) motors[i].setSpeed(speed);
}


