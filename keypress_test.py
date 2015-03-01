#!/usr/bin/env python

import termios, fcntl, sys, os
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
        print 'w'
      elif c == 'a':
        print 'a'
      elif c == 's':
        print 's'
      elif c == 'd':
        print 'd'
      else:
        print 'Unrecognized character %c' % c
    except IOError: pass
finally:
    termios.tcsetattr(fd, termios.TCSAFLUSH, oldterm)
    fcntl.fcntl(fd, fcntl.F_SETFL, oldflags)
