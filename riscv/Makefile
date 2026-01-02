# --- Toolchain Definitions ---
AS = riscv32-unknown-elf-as -march=rv32i -mabi=ilp32
GCC = riscv32-unknown-elf-gcc -march=rv32i -mabi=ilp32
OBJCOPY = riscv32-unknown-elf-objcopy
HEXDUMP = hexdump

CFLAGS += -DFORCE_DESKTOP -Wno-int-to-pointer-cast -fno-builtin-putchar -Wno-discarded-qualifiers

# --- Source Files ---
C_SRCS = main.c putchar.c getchar.c TinyBasicPlus.c rand.c spi.c utils.c
S_SRCS = start.S

# --- Object Files ---
OBJS = $(C_SRCS:.c=.o) $(S_SRCS:.S=.o)

# All object files to be linked.
LINK_OBJS = $(OBJS)

# --- Targets ---

# Default target
all: tiny_basic.hex program_data.hex

program_data.hex: program_data.bin
	@echo "==> Creating Data HEX file..."
	$(HEXDUMP) -v -e '"%08x\n"' $< > $@
	@echo "==> Build complete: program_data.hex"

program_data.bin: tiny_basic.elf
	@echo "==> Creating data binary..."
	# This combines data sections (preserves same order of loader.ld)
	$(OBJCOPY) -O binary -j .rodata -j .data -j .sdata -j .sbss -j .bss $< $@
	@echo "==> Build complete: program_data.bin"

tiny_basic.hex: tiny_basic.bin
	@echo "==> Creating HEX file..."
	$(HEXDUMP) -v -e '"%08x\n"' $< > $@
	@echo "==> Build complete: tiny_basic.hex"

tiny_basic.bin: tiny_basic.elf
	@echo "==> Creating bin file..."
	# Use objcopy to create the bin file, and *only* include the .text
	# section. This ignores .bss, .data, and all other sections.
	$(OBJCOPY) -O binary --only-section=.text $< $@
	@echo "==> Build complete: tiny_basic.bin"

tiny_basic.elf: $(OBJS)
	@echo "==> Linking ELF file..."
	$(GCC) $(LINK_OBJS) -o $@ -nostdlib -lgcc -T loader.ld

TinyBasicPlus.c: ../TinyBasicPlus/TinyBasicPlus.ino
	@echo Linking .c file to the Arduino .ino source
	@ln -s $< $@

# Pattern rule for compiling .c files to .o files
%.o: %.c
	@echo "==> Compiling C: $<"
	# "-c" is the standard flag to "compile only, do not link".
	$(GCC) -c $(CFLAGS) $< -o $@

# Pattern rule for assembling .S files to .o files
%.o: %.S
	@echo "==> Assembling: $<"
	$(AS) $< -o $@


.PHONY: clean all
clean:
	@echo "==> Cleaning up built files..."
	rm -f $(OBJS) tiny_basic.elf tiny_basic.hex tiny_basic.bin program_data.bin program_data.hex TinyBasicPlus.c
