/*
 * owen.h  - Interface for OWEN protocol
 * Creation Date: 2012-11-25
 *
 * Copyright (C) 2012 Leonid Myravjev (asm@asm.pp.ru)
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 */
//////////////// PHYSICAL LAYER ///////////////////
enum{ //OWEN_DEV
  OWEN_DEV_UNKNOWN,
  OWEN_DEV_AUTO,
  OWEN_DEV_USBAC4,
};

typedef struct S_owen_device{
  int fd;
  int type;
  char* name;
} OwenDevice;

/* Открыть OWEN устройство */
OwenDevice* owen_device_open(char* device, int type);

/* Послать данные канального уровня
 * * device -- устройство
 * * buff -- данные
 * * result -- массив для результата
 * * size -- размер массива для результата
 *    принимает максимальный размер массива, result
 *    возвращает размер ответа
 *
 * RETURN: всегда 0
 */
int owen_device_phy_transmit(OwenDevice* device, char* buff, int buffsize, char* result, int *resultsize);

/* Закрыть OWEN устройство */
int owen_device_close(OwenDevice* od);

/////////////// DATA-LINK LEYER //////////////////////

/////////////// PRESENTATION LAYER ///////////////////

/////////////// APPLICATION LAYER ////////////////////
