CC	= gcc
CFLAGS	= -Wall
BUILD	= assemble bitwise emulate immediateInstructionProcessing

all:	$(BUILD)

clean:
	/bin/rm -f $(BUILD) *.o core a.out

RegisterInstructionProcessing.o:	RegisterInstructionProcessing.c RegisterInstructionProcessing.h emulate.h
bitwise.o:	bitwise.c bitwise.h test_functions.h
branchInstructions.o:	branchInstructions.c branchInstructions.h
emulate:	emulate.o RegisterInstructionProcessing.o bitwise.o branchInstructions.o immediateInstructionProcessing.o singleMTransfer.o
emulate.o:	emulate.c RegisterInstructionProcessing.h bitwise.h branchInstructions.h emulate.h immediateInstructionProcessing.h singleMTransfer.h
immediateInstructionProcessing.o:	immediateInstructionProcessing.c bitwise.h emulate.h immediateInstructionProcessing.h
singleMTransfer.o:	singleMTransfer.c Definitions.h
test_functions.o:	test_functions.c test_functions.h
	
