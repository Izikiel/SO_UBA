#! /usr/bin/env python
 
import socket
import sys
import time
from paises import *

HOST = 'localhost'
PORT = 5555
#CLIENTES = 3
CLIENTES = 50
#CLIENTES = 243#no hay mas paises y tira error de indice python

class TCPFramer:
	def __init__(self, socket):
		self.sock = socket
		self.sock.settimeout(None)
		self.buf = ""
		
	def send(self, message):
		assert(message != "")
		self.sock.sendall(message + '|\n')
	
	def receive(self):
		index = self.buf.find('|')
		if index == -1:    # not a complete message, look for more
			self.buf = self.buf + self.sock.recv(1024)
		
		(res, remaining) = (self.buf.split('|', 1) + [ "" ])[0:2]
		self.buf = remaining
		
		return res.strip()

class Cliente:
	def __init__(self, nombre, posicion):
		self.nombre   = nombre
		self.posicion = posicion
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		sock.connect((HOST, PORT))
		
		self.framer = TCPFramer(sock)

		string = "NOMBRE: %s FILA: %d COLUMNA: %d" % (self.nombre, self.posicion[0], self.posicion[1])
		print(string)

		self.framer.send(string)
		#time.sleep(0.5)

	def esperarMascara(self):
		response = self.framer.receive()
		print("Respuesta: <"+ response+ ">")	

		
	def avanzarUnPaso(self):
		
		if self.posicion[0] == 0:
			direc = "IZQUIERDA"
			next = (0, -1)	
		else:
			direc = "ARRIBA"
			next = (-1, 0)
			
		self.framer.send(direc)
		response = self.framer.receive()
		time.sleep(0.5)
		print("Respuesta: <"+ response+ ">")
		if response == "OK":
			self.posicion = (self.posicion[0] + next[0], self.posicion[1] + next[1])
			if self.posicion == (0,-1):
				return 1#salio y esta todo OK
			else:
				return 0#sigue girando por la matriz en el server pero esta todo OK
		if response == "ERROR":
			return -2
		elif response == "CASILLA_LLENA_O_FUERADERANGO":
			return -3
		elif response == "LIBRE":
			return 1
		else:
			return 0#no es error
				


#	def salir(self):
#		sali = self.posicion == (0,-1)
#		while not sali:
#			sali = self.avanzarUnPaso()
#	
#		print(self.name, "salio")
#		self.sock.close()
 
clientes = []
for i in range(CLIENTES):
	 c = Cliente(paises[i], (1,1))
	 clientes.append(c)
	 
#for cliente in clientes:
#	cliente.salir()

i = 0
while len(clientes) > 0:
	res = clientes[i].avanzarUnPaso()
	if (res == 1):#salio
		clientes[i].esperarMascara()
		clientes.pop(i)
	elif(res == 0):#sigue girando por la matriz
		#nada
		pass
	elif(res < 0):#hubo error o rebote de entrar en la posicion esa
		clientes.pop(i)

	i += 1
		
	if i >= len(clientes):
		i = 0
		
sys.exit(0)