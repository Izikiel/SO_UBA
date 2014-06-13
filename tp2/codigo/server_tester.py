#! /usr/bin/env python
 
import socket
import sys
import time
from paises import *

HOST = 'localhost'
PORT = 5555
#CLIENTES = 3
CLIENTES = 20
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

	def esperarMascara(self, clienteID):
		print("[Cliente " + str(i) + "] Esperando mascara...")
		response = self.framer.receive()
		print("[Cliente " + str(i) + "] Respuesta: <"+ response+ ">")

		
	def avanzarUnPaso(self, clienteID):
		if self.posicion[0] == 0:
			direc = "IZQUIERDA"
			next = (0, -1)	
		else:
			direc = "ARRIBA"
			next = (-1, 0)
					
		print "[Cliente " + str(i) + "] Enviando: " + direc
		self.framer.send(direc)
		response = self.framer.receive()
		time.sleep(0.5)
		if response == "OK":
			self.posicion = (self.posicion[0] + next[0], self.posicion[1] + next[1])
			if self.posicion == (0,-1):
				print "[Cliente " + str(i) + "] Llego a la salida."
				return 1
			else:
				print "[Cliente " + str(i) + "] Se movio pero no llego a la salida."
				return 0
		elif response == "OCUPADO":
			print "[Cliente " + str(i) + "] No se pudo mover, esta ocupada la posicion a donde se intento moverse."
			return 0
		elif response == "ERROR":
			print "[Cliente " + str(i) + "] Hubo ERROR recibido del server."
			return -2
		elif response == "CASILLA_LLENA_O_FUERADERANGO":
			print "[Cliente " + str(i) + "] Conexion rechazada por el servidor por fuera de rango o casilla llena"
			return -3
				
#Main code:
clientes = []
for i in range(CLIENTES):
	 #el 1 de la tupla indica si el cliente esta activo o ya dejo de comunicarse con el servidor, dado que salio o que no pudo entrar al principio
	 c = (Cliente(paises[i], (1,1)), 1)
	 clientes.append(c)

i = 0
clientesActivos = len(clientes)
while clientesActivos > 0:
	if (clientes[i][1] == 1):		
		res = clientes[i][0].avanzarUnPaso(i)
		if (res == 1):#llego a la salida
			clientes[i][0].esperarMascara(i)
			print "[Ya fue liberado. Quitando de la lista al cliente " + str(i) + "]"
			clientes[i] = (clientes[i][0], 0)#indico que el cliente ya no esta mas activo			
			clientesActivos = clientesActivos - 1#decremento la cantidad de clientes activos
		elif(res == 0):#sigue moviendose por la matriz
			#nada, ignoramos y seguimos ciclando
			pass
		elif(res < 0):#hubo error o rebote de entrar en la posicion esa
			print "[El servidor nos reboto la conexion porque esta llena la capacidad de (x,y) para ingresar. Quitando de la lista al cliente " + str(i) + "]"
			clientes[i] = (clientes[i][0], 0)#indico que el cliente ya no esta mas activo
			clientesActivos = clientesActivos - 1#decremento la cantidad de clientes activos
	else:
		pass

	i += 1
	if i >= len(clientes):
		i = 0
		
sys.exit(0)