CC      ?= gcc
CFLAGS  ?= -std=c17 -g\
	-D_POSIX_SOURCE -D_DEFAULT_SOURCE\
	-Wall -Werror -pedantic

.SUFFIXES: .c .o

.PHONY: all clean

all: assemble emulate

assemble: assemble.o
emulate: emulate.o
RegisterInstructionProcessing: RegisterInstructionProcessing.o
bitwise: bitwise.o
branchInstructions: branchInstructions.o
immediateInstructionProcessing: immediateInstructionProcessing.o
singleMtransfer: singleMtransfer.o

clean:
	$(RM) *.o assemble emulate RegisterInstructionProcessing bitwise branchInstructions immediateInstructionProcessing singleMtransfer
	
