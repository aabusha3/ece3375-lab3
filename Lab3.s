.equ switch, 0xff200040
.equ display,0xff200020
.equ gpio, 0xFF200060
.equ adc, 0xFF204000


.global _start
_start:
//declaring one time variables here


_mainloop:
//part 0: configure switch (channel 0 = 0, channel 1 = 1)
ldr r0, =switch	
ldr r1, [r0]

cmp r1, #0
beq channel1
bne channel2

channel1:
ldr r0,=hexcode
ldr r1, [r0]

ldr r0, =display
str r1, [r0]

//part 1: ADC



b out

channel2:
ldr r0,=hexcode
add r0, #4
ldr r1, [r0]


ldr r0, =display
str r1, [r0]

b out

out:

b _mainloop


















.data
hexcode: 
	.word 0x3F,0x06