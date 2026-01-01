#include "desktop.h"
#include "utils.h"
#include "spi.h"

#define SPI ((volatile unsigned char *)0x410000)
#define SPI_CTRL ((volatile unsigned char *)0x420000)

unsigned char* read_flash() {
  volatile unsigned char *spi_ctrl = SPI_CTRL;
  volatile unsigned char *spi = SPI;

  spi_ctrl[0] = 1;   // request read

  while (spi_ctrl[0] == 0) {}

  return spi;
}
