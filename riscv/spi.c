#include "desktop.h"
#include "utils.h"
#include "spi.h"

#define SPI ((volatile unsigned long (*)[8])0x410000)
#define SPI_CTRL ((volatile unsigned long *)0x420000)

unsigned long read_flash() {
  volatile unsigned long *spi_ctrl = SPI_CTRL;
  volatile unsigned long (*spi)[8] = SPI;

  spi_ctrl[0] = 1;   // request read

  unsigned long cycles = 0;
  while (spi_ctrl[0] == 0) {
    cycles++;
  }

  unsigned char buf[11];
  ultoa(cycles, buf);
  println(buf);

  return (*spi)[0];
}


// TODO: fix this, the offset is targeting words, so the address goes from 0x0 to 0x4
unsigned long read_flash_direct(unsigned int offset) {
  volatile unsigned long (*spi)[8] = SPI;

  return (*spi)[offset];
}
