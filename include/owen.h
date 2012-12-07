/**
 * \file  owen.h
 * \brief Interface for OWEN protocol
 * \date  2012-11-25
 *
 * \author Leonid Myravjev <asm@asm.pp.ru>
 * \copyright GNU Public License
 *
 * \section LICENSE
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

/**
 * \brief Макс. размер посылки в пакете owen.
 */
#define MAXPHYSIZE ((1+1+1+2+15+2+1+1)*2)

//// UTILS
unsigned int owen_hash(uint8_t Byte, char nbit, unsigned int CRC);


//////////////// PHYSICAL LAYER ///////////////////
/**
 * \brief Устройства.
 */
enum {
  OWEN_DEV_UNKNOWN, /**< Неизвестное устройство. */
  OWEN_DEV_AUTO,    /**< Автоматическое определение устройства. */
  OWEN_DEV_USBAC4,  /**< Устройство USB AC4.     */
};

typedef struct S_owen_device{
  int fd;
  int type;
  char* name;
} OwenDevice;

/** Открыть OWEN устройство */
OwenDevice* owen_device_open(char* device, int type);

/** Послать данные канального уровня
 * \param  device -- устройство
 * \param  buff -- данные
 * \param  result -- массив для результата
 * \param  size -- размер массива для результата
 *    принимает максимальный размер массива, result
 *    возвращает размер ответа
 *
 * \return  всегда 0
 */
int owen_device_phy_transmit(OwenDevice* device, char* buff, int buffsize, char* result, int *resultsize);

/** Закрыть OWEN устройство */
int owen_device_close(OwenDevice* od);

/////////////// DATA-LINK LEYER //////////////////////
/**
 * \brief Перевернуть слово.
 * Используется для перевода hash в правильное состояние.
 * \todo Нужно обозначить пустым маркосом на big endian.
 */
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


/**
 * \brief Создать пакет канальго уровня. */
OwenDatalink* owen_datalink_new();

/** 
 * \brief Отправить пакет на уровень ниже.
 * \param device
 * \param out
 * \param input
 */
int owen_device_datalink_transmit(OwenDevice* device, OwenDatalink* out, OwenDatalink* input);

/** 
 * \brief Получить результирующий пакет для физического уровня из OwenDatalink. 
 * \param device
 * \param buff
 * \param resultsize
 */
int owen_datalink_getpackage(OwenDatalink* device, char* buff, int *resultsize);

/** 
 * \brief Пропарсить пакет физического уровня и записать в OwenDetalink.
 * \param device
 * \param buff
 * \param resultsize
 */
int owen_datalink_setpackage(OwenDatalink* device, char* buff, int resultsize);

/** 
 * \brief Удалить пакет канального уровня.
 * \param od
 */
int owen_datalink_free(OwenDatalink* od);

/**
 * \brief Отладочная функция.
 * \detailed Печатает в stdout информацию по пакету.
 * \param od
 */
int owen_datalink_printpackage(OwenDatalink* od);

/////////////// PRESENTATION LAYER ///////////////////

/** 
 * \brief Посчитать HASH из имени параметра.
 * \param parm
 */
uint16_t owen_hashstr(char* parm);

/**
 * \brief Получить имя параметра из HASH.
 * \param hash
 */
char* owen_strhash(int hash);

/**
 * \brief Получить данные в пакете представленные в Float виде.
 * \param od
 */
float owen_isFloat(OwenDatalink* od);

/** 
 * \brief Получить данные в пакете представленные в Integer виде.
 * \param od
 */
int owen_isInt(OwenDatalink* od);

/////////////// APPLICATION LAYER ////////////////////


