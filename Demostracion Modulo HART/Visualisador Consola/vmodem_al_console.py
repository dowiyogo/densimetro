#!/usr/bin/env python

import serial
import time
import msvcrt
import sys

class PackCommunication:

	def __init__(self, path, baud_rate):
		self.SFLAG = b'0'
		self.EFLAG = b'1'
		self.EESC  = b'2'
		self.byteT = 1.0/baud_rate

		self.s     = serial.Serial()
		self.error = ""
		try:
			self.s.port     = path
			self.s.baudrate = baud_rate
			self.s.timeout  = 0

			self.s.open()
			time.sleep(2.000)			# arduino reset
		except serial.serialutil.SerialException:
			self.error = path + " not found"

	def open(self):
		return self.s.is_open

	def send(self, string):
		self.s.write(self.SFLAG)
		
		for c in string:
			if c.encode() == self.EFLAG or c.encode() == self.SFLAG or c.encode() == self.EESC:
				self.s.write(self.EESC)
			self.s.write(c.encode())
		
		self.s.write(self.EFLAG)
		self.s.flush()

	def send_bytes(self, _bytes):
		self.s.write(self.SFLAG)
		
		for c in _bytes:
			c = bytes([c])
			if c == self.EFLAG or c == self.SFLAG or c == self.EESC:
				self.s.write(self.EESC)
			self.s.write(c)

		self.s.write(self.EFLAG)
		self.s.flush()

	def recv(self):
		buff = bytearray()
		if (self.s.in_waiting < 2):
			return buff
		
		byte = self.s.read()
		if (byte != self.SFLAG):
			return buff

		while True:
			byte = self.s.read()
			if len(byte) == 0:
				continue
			elif byte == self.EFLAG:
				break
			elif byte == self.EESC:
				byte = self.s.read()
				while len(byte) < 1:
					byte = self.s.read()
			
			buff.extend(byte)

		return buff

	def recv_wait(self):
		while (self.s.in_waiting < 2):
			time.sleep(self.byteT)
		return self.recv()

# Frames to send

frame = [bytes(b'\xff\xff\xff\xff\xff\x02\x80\x00\x00\x82'),bytes(b'\xff\xff\xff\xff\xff\x02\x80\x01\x00\x83'),bytes(b'\xFF\xFF\xFF\xFF\xFF\x02\x80\x06\x00\x84')]
index_frame = 0

# Program

if __name__ == "__main__":
	print("Virtual Modem Console")

	paco = PackCommunication("COM4", 9600)
	if not paco.open():
		print("Error : ", paco.error)
		exit(1)
	print("Starting communication")

	buff_modem = ""
	buff_pc    = ""
	while True:
		# Receiving from modem

		buff_modem = paco.recv()
		if len(buff_modem) > 0:
			#print(buff_modem)
			if buff_modem == b'r':
				buff_modem = paco.recv_wait()
				print("<= recv : [", len(buff_modem),"] ", buff_modem)#buff_modem[0] + buff_modem[1]*256)# #buff_modem[0] + buff_modem[1]*256)#

			elif buff_modem == b't':
				buff_modem = paco.recv_wait()
				print("=> send : [", len(buff_modem),"] ", buff_modem)

			elif buff_modem == b'e':
				buff_modem = paco.recv_wait()
				print("<= error : [", len(buff_modem),"] ", buff_modem)

		# Receiving from keyboard
		if msvcrt.kbhit():
			k = msvcrt.getch()

			if index_frame >= len(frame):
				index_frame = 0
			
			if k == b'\r':
				print("")
				print("Senging : [",len(frame[index_frame]),"] ", frame[index_frame])
				paco.send_bytes(frame[index_frame])
				index_frame  += 1