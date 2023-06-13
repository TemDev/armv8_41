cmake_minimum_required(VERSION 3.23)
project(armv8_41 C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(armv8_41
        src/assemble.c
        src/assemble.h
        src/AssemblerDecoder.c
        src/AssemblerDecoder.h
        src/AssemblerInput.c
        src/AssemblerInput.h
        src/bitwise.c
        src/bitwise.h
        src/branchInstructions.c
        src/branchInstructions.h
        src/emulate.c
        src/emulate.h
        src/immediateInstructionProcessing.c
        src/immediateInstructionProcessing.h
        src/RegisterInstructionProcessing.c
        src/RegisterInstructionProcessing.h
        src/singleMTransfer.h src/AssemblerInput.h)
