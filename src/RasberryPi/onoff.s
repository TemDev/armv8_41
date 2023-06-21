movz w0, #0x3f00, lsl #16
movz w1, #0xb880
movz w2, #0xb8b8
movz w3, #0xb8a0
movz w16, #0xb898
movz w4, #0x100, lsl #16
movz w5, #0x200, lsl #16
movz w6, #32
movz w7, #0
movz w8, #3, lsl #16
movk w8, #0x8041, lsl #0
movz w9, #8
movz w10, #0x0
movz w11, #130
movz w12, #0x1
movz w13, #0x100, lsl #16
add w13, w13, #8
movz w14, #0x200, lsl #16
add w14, w14, #8


start:
    str w6, [w4], #4
    str w7, [w4], #4
    str w8, [w4], #4
    str w9, [w4], #4
    str w10, [w4], #4
    str w11, [w4], #4
    str w12, [w4], #4
    str x10, [w4], #4

    movz w4, #0x100, lsl #16

check_flag2:
    movz w18, #0x8000, lsl #16
    ldr w17, [w0, w2]
    ands w18, w18, w17
    b.ne check_flag2


    str w13, [w0, w3]

    movz w18, #0xffff


delay:
    movz w19, #0xffff

delay_in:
    
   
    subs w19, w19, #1
    b.ne delay_in
    
   
    subs w18, w18, #1
    b.ne delay

check_flag3:
    movz w18, #0x4000, lsl #16
    ldr w17, [w0, w16]
    ands w18, w18, w17
    b.ne check_flag3

    ldr w29, [w0, w1]

    str w6, [w5], #4
    str w7, [w5], #4
    str w8, [w5], #4
    str w9, [w5], #4
    str w10, [w5], #4
    str w11, [w5], #4
    str w10, [w5], #4
    str w10, [w5], #4

    movz w5, #0x200, lsl #16

check_flag4:
    movz x18, #0x8000, lsl #16
    ldr w17, [w0, w2]
    ands w18, w18, w17
    b.ne check_flag4

    str w14, [w0, w3]

    movz w18, #0xff

delay2:
    movz w20, #0xff

delay_x:
    
    subs w20, w20, #1
    b.ne delay_x
    subs w18, w18, #1
    b.ne delay2

    check_flag:
    movz w18, #0x4000, lsl #16
    ldr w17, [w0, w16]
    ands w18, w18, w17
    b.ne check_flag

    ldr w29, [x0, x1]

b start

