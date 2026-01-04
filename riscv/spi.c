#include "desktop.h"
#include "utils.h"
#include "spi.h"

#define SPI ((volatile unsigned char *)0x80800000)
#define SPI_CTRL ((volatile unsigned char *)0x80400000)

unsigned char* read_flash() {
  volatile unsigned char *spi_ctrl = SPI_CTRL;
  volatile unsigned char *spi = SPI;

  spi_ctrl[0] = 1;   // request read

  unsigned long count = 0;
  while (spi_ctrl[0] == 0) {
    count++;
  }
  unsigned char buffer[11];
  ultoa(count, buffer);
  println(buffer);

  return spi;
}
