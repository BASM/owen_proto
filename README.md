SYNOPSIS
========

Not official OWEN stack (www.owen.ru).

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
* ModBus RTU (RS-485)   (Don't support yet.)
* ModBus ASCII (RS-485) (Don't support yet.)
* Owen (RS-485)


BUILD FOR LINUX
==========

You need install:
* cmake version 2.8 or greater
* gcc any version
* GNU make

Type:
 cmake .
 make


BUILD FOR WINDOWS (Don't support yet.)
==========

You need install:
* cmake version 2.8 or greater (http://www.cmake.org)
* mingw any version

Go to project diretroy and run:
* cmake .
* mingw32-make
or use GUI for cmake: cmake-gui

BUILD FOR WINDOWS FROM LINUX (Don't support yet.)
==========

Don't support yet.

You need install:
* cmake version 2.8 or greater
* mingw32 (cross compiller for windows)
* GNU make

FIXME

AFTER BUILD
==========

You get:
* libowen.a    -- library of owen protocol
* phy_121      -- test for PHY LAYER
* datalink_118 -- test for DATALINK LAYER
* present_119  -- test for PRESENT LAYER
* user_120     -- test for USER LAYER

For linking use:
* include/owen.h  -- header file
* libowen.a       -- library file

For running test you need have AC-4 adaptor (USB->RS-484) from /dev/ttyUSB0
device, or you need fix test/*/main.c files.

LICENCE
=========

Copyright (C) 2012 Leonid Myravjev (asm@asm.pp.ru)
 
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
Or LICENSE file from this repo.
