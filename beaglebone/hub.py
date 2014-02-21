#!/usr/bin/python
from nrf24 import NRF24
import time


sensor_pipes   = [[0x3a, 0x3a, 0x3a, 0x3a, 0xd2], [0x3a, 0x3a, 0x3a, 0x3a, 0xc3]]
base_pipes = [[0xf0, 0xf0, 0xf0, 0xf0, 0xd2], [0xf0, 0xf0, 0xf0, 0xf0, 0xc3]]

pipes = [[0xe7, 0xd3, 0xf0, 0x35, 0x78], # Us, base station
	 [0xc2, 0xc2, 0xc2, 0xc2, 0xc2], # Arduino 1
	 [0xc2, 0xc2, 0xc2, 0xc2, 0xc3]] # Arduino 2

radio = NRF24()
radio.begin(1, 0, "P9_15", "P9_16") #Set CE and IRQ pins
radio.setRetries(15,15)
radio.setPayloadSize(8)
radio.stopListening()

#radio.openWritingPipe(pipes[0])
#radio.openReadingPipe(1, pipes[1])
#radio.openReadingPipe(2, pipes[2])

radio.openReadingPipe(1, sensor_pipes[0])
radio.openReadingPipe(2, sensor_pipes[1])

radio.startListening()
radio.printDetails()

while True:
	pipe = [1]
	if radio.available(pipe):
		recv_buffer = []
		radio.read(recv_buffer)
		if pipe[0] == 1:
			print "TEMP", recv_buffer[0]
			radio.stopListening()
			radio.openWritingPipe(base_pipes[0])
			s = [1]
			radio.write(s)
			print "wrote"
			radio.startListening()
		if pipe[0] == 2:
			print "LIGHT", recv_buffer[0]

	time.sleep(0.001)

