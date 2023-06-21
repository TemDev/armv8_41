movz x0, #0x3f00, lsl #16
movz w1, #0xb880
movz w2, #0xb8b8
movz w3, #0xb8a0
movz w4, #0x10, lsl #16
movz w5, #0x20, lsl #16
movz w6, #32
movz w7, #0
movz x8, #3, lsl #16
movk x8, #0x8041, lsl #0
movz w9, #8
movz w10, #0x0
movz w11, #130
movz w12, #0x1
movz x13, #0x100, lsl #16
movk x13, #8, lsl #0
movz x14, #0x200, lsl #16
movk x14, #8, lsl #0

check_flag:
    movz w18, #0
    ldr w17, [x0,x2]
    adds w18, w18, w17
    b.lt check_flag

    str w6, [x4]
    str w7, [x4, #4]
    str w8, [x4, #8]
    str w9, [x4, #12]
    str w10, [x4, #16]
    str w11, [x4, #20]
    str w12, [x4, #24]
    str w10, [x4, #28]
    movz w4, #0x10, lsl #16
    movz x20, #ff00, lsl #16
    
    str w20, [x0, x2]

    str w13, [x0, x3]

    movz x18, #0xffff

delay:
    movz x19, #0xffff

delay_in:
    
   
    subs x19, x19, #1
    b.ne delay_in
    
   
    subs x18, x18, #1
    b.ne delay

check_flag2:
    movz w18, #0
    ldr w17, [x0, x2]
    adds w18,  w18, w17
    b.lt check_flag2

    str w6, [x5]
    str w7, [x5, #4]
    str w8, [x5, #8]
    str w9, [x5, #12]
    str w10, [x5, #16]
    str w11, [x5, #20]
    str w10, [x5, #24]
    str w10, [x5, #28]
    movz w5, #0x20, lsl #16

    str w14, [x0, x3]

    movz x18, #0xffff
delay2:
    nop
    nop
    movz x20, #0xffff

delay_x:
    
   
    subs x20, x20, #1
    b.ne delay_x
    subs x18, x18, #1
    b.ne delay2

b check_flag