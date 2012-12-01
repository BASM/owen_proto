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
#include <stdint.h>

#define MAXPHYSIZE ((1+1+1+2+15+2+1+1)*2)

//// UTILS
unsigned int owen_hash(uint8_t Byte, char nbit, unsigned int CRC);


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
// Перевернуть слово
//  используется для перевода hash в правильное состояние.
//  FIXME: работает только на little endian
#define MOTOW(a) ({ ((a>>8)|(a<<8))&0xFFFF; })

#pragma pack(1)
typedef struct S_owen_datalink{
  uint8_t addr;
  uint8_t eaddr:4;
  uint8_t remote:1;
  uint8_t size:3;
  uint16_t hash;
  uint8_t data[15];
  uint16_t crc;
} OwenDatalink;
#pragma pack()


/* Создать пакет канальго уровня */
OwenDatalink* owen_datalink_new();

/* Отправить пакет на уровень ниже */
int owen_device_datalink_transmit(OwenDevice* device, OwenDatalink* out, OwenDatalink* input);

/* Получить результирующий пакет для физического уровня из OwenDatalink. */
int owen_datalink_getpackage(OwenDatalink* device, char* buff, int *resultsize);

/* Пропарсить пакет физического уровня и записать в OwenDetalink. */
int owen_datalink_setpackage(OwenDatalink* device, char* buff, int resultsize);

/* Удалить пакет канального уровня */
int owen_datalink_free(OwenDatalink* od);

/* Отладочная функция.
 *  Печатает в stdout информацию по пакету
 */
int owen_datalink_printpackage(OwenDatalink* od);

/////////////// PRESENTATION LAYER ///////////////////

/* Посчитать HASH из имени параметра */
uint16_t owen_hashstr(char* parm);

/* Получить имя параметра из HASH */
char* owen_strhash(int hash);

/* Получить данные в пакете представленные в Float виде*/
float owen_isFloat(OwenDatalink* od);

/* Получить данные в пакете представленные в Integer виде*/
int owen_isInt(OwenDatalink* od);

/////////////// APPLICATION LAYER ////////////////////


