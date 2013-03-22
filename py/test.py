#!/usr/bin/env python

import socks
import socket
import urllib2
socks.setdefaultproxy(socks.PROXY_TYPE_SOCKS5,"127.0.0.1", 8088, True)
socket.socket = socks.socksocket

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = "www.google.com"
ip = socket.gethostbyname(host)
s.connect((host , 80))
s.send('GET / HTTP/1.0\n\n')
print s.recv(8092)
s.close
