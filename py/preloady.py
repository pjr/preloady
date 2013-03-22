import socket
import struct

def quotechars(chars):
  return ''.join( ['.', c][c.isalnum()] for c in chars )

def hexdump(chars, sep, width) :
  while chars:
    line = chars[:width]
    chars = chars[width:]
    line = line.ljust(width, '\000')
    print "%s%s -- %s" % (sep.join('%02x' % ord(c) for c in line), sep, quotechars(line))

def connect(arg1, arg2):
  print "arg1 = %s" % arg1
  print "arg2 = %s" % arg2

  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.connect(("192.168.1.13", 9999))
  print "SOCKET = [%d]" % s.fileno()

  hexdump(arg2, ' ', 8)
  print ""
  (family, port, ip, trash) = struct.unpack("hHI8s", arg2)
  print "IP = " + socket.inet_ntoa(struct.pack("I", ip))
  port = socket.ntohs(socket.htons(port) + 1) 
  sr = struct.pack("hHI8s", family, port, ip, trash)
  hexdump(sr, ' ', 8)
  print "Inside python connect"
  return (int(s.fileno()), sr)

def write(arg1):
  return "a" + arg1 

def test(arg1, arg2):
  arg1 = arg1 + 10
  return int(arg1), int(arg1)
