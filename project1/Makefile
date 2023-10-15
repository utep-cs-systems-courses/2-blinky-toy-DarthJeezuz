#makefile configuration
COMMON_OBJECTS =
CPU            = msp430g2553
CFLAGS         = -mmcu=${CPU} -I../h
LDFLAGS        = -L/opt/ti/msp430_gcc/include

#switch the compiler for the internal make rules

CC             = msp430-elf-gcc
AS             = msp430-elf-as


all: siren.elf

#additional rules for files
siren.elf: ${COMMON_OBJECTS} sirenMain.o lib/libTimer.a
	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^

load: siren.elf
	msp430loader.sh siren.elf

clean:
	rm -f *.o *.elf

