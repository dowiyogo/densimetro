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
			
			buff.extend(byte)

		return buff

	def recv_wait(self):
		while (self.s.in_waiting < 2):
			time.sleep(self.byteT)
		return self.recv()



if __name__ == "__main__":
	print("Test PackCommunication")

	paco = PackCommunication("COM4", 9600)
	if not paco.open():
		print("Error : ", paco.error)
		exit(1)
	print("Starting communication")

	buff_slave = ""
	buff_pc    = ""
	while True:
		buff_slave = paco.recv()
		if len(buff_slave) > 0:
			if bytes({buff_slave[0]}) == b'r':
				buff_slave = paco.recv_wait()
				print("")
				print("<=", buff_slave)
				continue

			#print(buff_slave)
			if buff_slave == b'\x00':
				print("[Ok]    Marco de master aceptado")
			elif buff_slave == b'\x02' or buff_slave == b'\x03':
				print("[Error] Preambulo no recibido")
			elif buff_slave == b'\x04':
				print("[Error] Delimitador no recibido")
			elif buff_slave == b'\x05':
				print("[Error] Direccion polling no recibida")
			elif buff_slave == b'\x06':
				print("[Error] Direccion unique no recibida")
			elif buff_slave == b'\x07':
				print("[Error] Expansion no recibida")
			elif buff_slave == b'\x08':
				print("[Error] Comando no recibido")
			elif buff_slave == b'\x09':
				print("[Error] Tamaño Data no recibido")
			elif buff_slave == b'\x0a':
				print("[Error] Data no recibida")
			elif buff_slave == b'\x0b':
				print("[Error] Byte de chequeo no recibido")
			elif buff_slave == b'\x0e':
				print("[Error] Preambulo valor erroneo")
			elif buff_slave == b'\x0f':
				print("[Error] No se detecto comienzo de mensaje")
			elif buff_slave == b'\x10':
				print("[Error] Modulacion no es FSK")
			elif buff_slave == b'\x11':
				print("[Error] Marco con direccion incorrecta")
			elif buff_slave == b'\x12':
				print("[Error] Marco con expansion no 0")
			elif buff_slave == b'\x13':
				print("[Error] Delimitador no es de algun maestro")
			elif buff_slave == b'\x14':
				print("[Error] Direccion polling erronea")
			elif buff_slave == b'\x15':
				print("[Error] Direccion unique erronea")
			elif buff_slave == b'\x16':
				print("[Error] Mal byte de chequeo")
			elif buff_slave == b'\x17':
				print("[Error] Delimitador indica expansion no 0")
			else:
				print("[Error] Error no definido")


		if msvcrt.kbhit():
			k = msvcrt.getch()

			if k == b'\r':
				paco.send(buff_pc)
				buff_pc = ""
				print("")
				continue
			if k == b'\x1b':
				exit(0)

			if buff_pc == "":
				print("> ", end='')
			buff_pc += k.decode("ascii")
			print(buff_pc[-1], end='')
			sys.stdout.flush()
			
