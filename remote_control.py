#!/usr/bin/env python
# NU Robotics Club
# Author: Tigist Diriba
# Prototype Rover Remote Control

import serial
import tty
import termios, fcntl, sys, os
import XboxController

class RoverDriver(object):
  
  def __init__(self,comm_port='/dev/ttyUSB0'):
    self.comm_time = .01
    try:
      self.xbee_serial = serial.Serial(comm_port,9600,timeout=0.25)
      self.comm_port = comm_port
    except:
      print 'Error reading XBee from com port %s' % (comm_port,)
      self.xbee_serial = None
      self.comm_port = None
  
  def send_command(self, command, duration):
    if self.xbee_serial is not None:
      self.xbee_serial.write(str(command))
    else:
      print 'Cannot send command: XBee serial connection has not been initialized'
  
  def drive_forward(self,duration):
    self.send_command('!f\n',duration)
  
  def drive_backward(self,duration):
    self.send_command('!b\n',duration)
  
  def turn_left(self,duration):
    self.send_command('!l\n',duration)
  
  def turn_right(self,duration):
    self.send_command('!r\n',duration)

  def percentToSpeed(p):
    if(p > 0):
      dir = '1'
    else:
      dir = '0'

    temp = int(255*abs(p))
    temp = str(temp)
    while len(temp)<3:
      temp = '0'+temp
    return dir+temp


  def leftThumbX(self, xValue):
    print xValue
    self.send_command('!x'+percentToSpeed(xValue)+'\n',duration)
    if xValue > 0:
      
      print "Right"
    elif xValue < 0:
      
      print "Left"
        
  def leftThumbY(self, yValue):
    self.send_command('!y'+percentToSpeed(yValue)+'\n',duration)
    if yValue < 0:
      
      print "Forward"
    elif yValue >0:
      
      print "Backwards"


if __name__ == '__main__':
  print "hi"  
  xboxCont = XboxController.XboxController(
    controllerCallBack = None,
    joystickNo = 0,
    deadzone = .1,
    scale = 1,
    invertYAxis = False)

  



  proto_rover = None
  if len(sys.argv) > 1:
    proto_rover = RoverDriver(sys.argv[1])
  else:
    proto_rover = RoverDriver()
  fd = sys.stdin.fileno()

  xboxCont.setupControlCallback(xboxCont.XboxControls.LTHUMBX, lambda (x): proto_rover.leftThumbX(x))
  xboxCont.setupControlCallback(xboxCont.XboxControls.LTHUMBY, lambda (x): proto_rover.leftThumbY(x))
  xboxCont.start()

  oldterm = termios.tcgetattr(fd)
  newattr = termios.tcgetattr(fd)
  newattr[3] = newattr[3] & ~termios.ICANON & ~termios.ECHO
  termios.tcsetattr(fd, termios.TCSANOW, newattr)

  oldflags = fcntl.fcntl(fd, fcntl.F_GETFL)
  fcntl.fcntl(fd, fcntl.F_SETFL, oldflags | os.O_NONBLOCK)
  
  try:
    while 1:
      try:
        c = sys.stdin.read(1).lower()
        if c == 'w':
          proto_rover.drive_forward(.1)
        elif c == 'a':
          proto_rover.turn_left(.1)
        elif c == 's':
          proto_rover.drive_backward(.1)
        elif c == 'd':
          proto_rover.turn_right(.1)
        else:
          print 'Unrecognized command character %c' % c
      except IOError: pass
  finally:
      termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)
      fcntl.fcntl(fd, fcntl.F_SETFL, oldflags)
