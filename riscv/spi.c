#include "desktop.h"
#include "utils.h"
#include "spi.h"

#define SPI ((volatile unsigned long *)0x410000)
#define SPI_CTRL ((volatile unsigned long *)0x420000)

unsigned long read_flash() {
  volatile unsigned long *spi_ctrl = SPI_CTRL;
  volatile unsigned long *spi = SPI;

  spi_ctrl[0] = 1;   // request read

  unsigned long cycles = 0;
  while (spi_ctrl[0] == 0) {
    cycles++;
  }

  unsigned char buf[11];
  ultoa(cycles, buf);
  println(buf);

  return *spi;
}
