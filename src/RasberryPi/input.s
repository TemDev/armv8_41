nop
nop
nop
nop
b start
.int 0
.int 0
.int 0
data:
    .int 32
    .int 0
    .int 0x38041
    .int 8
    .int 0
    .int 130
    .int 1
    .int 0

    .int 32
    .int 0
    .int 0x38041
    .int 8
    .int 0
    .int 130
    .int 0
    .int 0
start:
    ldr w3, request_address
    ldr w5, write_reg

write:
    add w4, w3, #8
    str w4, [w5]

    movz w18, #0xff

delay:
    movz w19, #0x8fff

delay_in:
    subs w19, w19, #1
    b.ne delay_in
   
    subs w18, w18, #1
    b.ne delay


check_flagE:
    movz w18, #0x4000, lsl #16
    ldr w17, status_reg
    ands w18, w18, w17
    b.ne check_flagE

    add w0, w0, w16
    ldr w27, read_reg
    

    ldr w3, off_light

off:
    add w4, w3, #8
    str w4, write_reg

    ldr w4, request_address

    movz w6, #32
    movz w7, #0
    movz w8, #3, lsl #16
    movk w8, #0x8041, lsl #0
    movz w9, #8
    movz w10, #0x0
    movz w11, #130
    movz w12, #0x1

    str w6, [w4], #4
    str w7, [w4], #4
    str w8, [w4], #4
    str w9, [w4], #4
    str w10, [w4], #4
    str w11, [w4], #4
    str w12, [w4], #4
    str w10, [w4], #4

    str w6, [w4], #4
    str w7, [w4], #4
    str w8, [w4], #4
    str w9, [w4], #4
    str w10, [w4], #4
    str w11, [w4], #4
    str w10, [w4], #4
    str w10, [w4], #4
movz w18, #0xff

delaynew:
    movz w19, #0x8fff

delay_innew:
   
    subs w19, w19, #1
    b.ne delay_innew
    
   
subs w18, w18, #1
b.ne delaynew


    movz w0, #0x3f00, lsl #16
    movk w0, #0xb898, lsl #16

check_flagE2:
    movz w18, #0x4000, lsl #16
    ldr w17, [w0]
    ands w18, w18, w17
    b.ne check_flagE2

    ldr w27, read_reg

b start
write_reg:
    .int 0x3f00b8a0
read_reg:
    .int 0x3f00b880
status_reg:
    .int 0x3f00b898
request_address:
    .int 0x80020
off_light:
    .int 0x80040