SYNOPSIS
========

Read from port

DESCRIPTION
===========

Driver for reading from serial port.
It maybe:
* virtual port (emulator)
* linux port (/dev/ttyS0, /dev/ttyUSB0)
* windows port (COM0, COM1...)
* TCP/IP port (forward port)

Drivers work to:
* RS-232
* RS-485

Drivers support protocols:
* ModBus RTU (RS-485)
* ModBus ASCII (RS-485)
* Owen (RS-485)
