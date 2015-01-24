#include <ProtoRover.h>
#define MAX_BUF 100
#define ROVER_SPEED 1.0

enum SERIAL_STATE {
  START_COMMAND,
  WAIT_COMMAND,
  END_COMMAND
};

char stream_buffer[MAX_BUF];
char buf = 0;
int serial_state = START_COMMAND;
int buf_count = 0;
int command_time = 0;
int command_timeout = 2000;
char temp_command = '/0';
char command = '/0';
Rover hal9000;

int counter = 0;

void act();
boolean isCommand(char c);

void setup()
{
  Serial.begin(9600); 
  serial_state = START_COMMAND;
  Serial.println("Starting in start command");
  command_time = millis();
  buf = 0;
  hal9000.reset(
    2,  // left phase
    3,  // left enable
    4,  // right phase
    5   // right enable
  );
}

void loop()
{
  if(serial_state == START_COMMAND) {
    if(waitForData(100)) {
      buf = Serial.read();
      if(buf == '!') {
        serial_state = WAIT_COMMAND;  
      }
      else serial_state = START_COMMAND;
    }
  }
  else if(serial_state == WAIT_COMMAND) {
    if(waitForData(100)) {
      buf = Serial.read();
      if(isCommand(buf)) {
        temp_command = buf;
        serial_state = END_COMMAND;
      }      
      else if(buf != '!') serial_state = START_COMMAND;
    }
  }
  else if(serial_state == END_COMMAND) {
      if(waitForData(100)) {
        buf = Serial.read();
        if(buf == '\n') {
          command = temp_command;
          command_time = millis();
          serial_state = START_COMMAND;
        }
        else if(buf == '!') serial_state == WAIT_COMMAND;
        else if(!isCommand(buf)) serial_state == START_COMMAND;
      }
  }
  else serial_state = START_COMMAND;
  if(millis() - command_time > command_timeout) {
    command = '/0';
  }
  counter++;
  if(counter % 20 == 0) Serial.println(command);
  act();  
}

boolean isCommand(char c)
{
  return c == 'f' || c == 'b' || c == 'l' || c == 'r';
}

void act()
{
  if(command == 'f') {
    hal9000.driveForward();
    hal9000.setSpeed(ROVER_SPEED);
  }
  else if(command == 'b') {
    hal9000.driveBackward();
    hal9000.setSpeed(ROVER_SPEED);
  }
  else if(command == 'l') {
    hal9000.turnLeft();
    hal9000.setSpeed(ROVER_SPEED);
  }
  else if(command == 'r') {
    hal9000.turnRight();
    hal9000.setSpeed(ROVER_SPEED);
  }
  else {
    // Halt
    hal9000.setSpeed(0.);
  }  
}

boolean waitForData(int ms)
{
  int current_time = millis();
  int time_diff = 0;
  while(Serial.available() == 0 && ((time_diff = millis()-current_time) < ms)); 
  if(time_diff > ms) return false;
  return true;
}
