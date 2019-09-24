/*
X = 5
Y = 10
X = X * 10 + Y
Z = Y - X
*/
	.syntax unified
	.cpu cortex-m4
	.thumb

.data
	X: .word 5 // X=5, 4 bytes
	Y: .word 10 // Y=10
	Z: .word 0 // Z=0
.text
	.global main

main:
	ldr r1, =X // r1 = X
	ldr r0, [r1] // r0 = r1(X)
	ldr r3, =Y // r3 = Y
	ldr r2, [r3] // r2 = r3(Y)
	ldr r5, =Z // r5 = Z
	ldr r4, [r5] // r4 = r5(Z)
	movs r6, #10
	muls r0, r0, r6 // r0 *= 10
	adds r0, r0, r2 // r0 += Y
	str r0, [r1]
	subs r4, r2, r0 // Z = Y - X
	str r4, [r5] // store r4 back to Z
	str r2, [r3]
L: B L
