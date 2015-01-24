#!/usr/bin/env python
# NU Robotics Club
# Author: Tigist Diriba
# Prototype Rover Remote Control

import serial
import tty
import termios, fcntl, sys, os

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


if __name__ == '__main__':
  proto_rover = None
  if len(sys.argv) > 1:
    proto_rover = RoverDriver(sys.argv[1])
  else:
    proto_rover = RoverDriver()
  fd = sys.stdin.fileno()

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
