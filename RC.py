#!/usr/bin/python

import sys
op = sys.argv[1].split(',')
bytes = bytearray()
for i in op:
    bytes.append(int(i,16))
filebyte = open("ROM","wb")
filebyte.write(bytes)
filebyte.close()
