	.syntax unified
	.cpu cortex-m4
	.thumb
.data
	result: .word 0
	max_size: .word 0
.text
	.global main
	m: .word 0x5E
	n: .word 0x60

// r0: m
// r1: n
// r2: tmp
// r5: return value
// r7: max_size
main:
	ldr r0, =m
	ldr r0, [r0]
	ldr r1, =n
	ldr r1, [r1]
	push {r0}
	push {r1}
	movs r7, #0 // max_size=0
	bl GCD
	ldr r2, =result
	str r5, [r2] // return_value
	ldr r2, =max_size
	str r7, [r2] // // max_size
end:
	B end
GCD:
	//TODO: Implement your GCD function
	cmp r0, #0 // m==0?
	beq m_zero
	cmp r1, #0 // n==0?
	beq n_zero
	ands r2,r0,1 // m&1==0?
	ands r3,r1,1 // n&1==0?
	orrs r4,r2,r3 // !(m&1) && !(n&1)? ->!(m&1 | n&1)
	cmp r4, #0 // !(m&1 | n&1)?
	beq mn_even
	cmp r2, #0 // m&1==0?
	beq m_even
	cmp r3, #0 // n&1==0?
	beq n_even
	b else
m_zero: // return n
	movs r5, r1
	bx lr
n_zero: // return m
	movs r5, r0
	bx lr
mn_even: // return 2*gcd(m>>1, n>>1)
	push {r0}
	push {r1}
	push {lr}
	ADDS r7, #3 // max_size += 3
	lsr r0, 1 // m>>1
	lsr r1, 1 // n>>1
	bl GCD
	movs r6, #2
	muls r5, r5, r6 // return 2*gcd(m>>1, n>>1)
	pop {lr}
	pop {r1}
	pop {r0}
	bx lr
m_even: // return gcd(m>>1, n)
	push {r0}
	push {r1}
	push {lr}
	adds r7, #3 // max_size += 3
	lsr r0, 1 // m>>1
	bl GCD
	pop {lr}
	pop {r1}
	pop {r0}
	bx lr
n_even: // return gcd(m, n>>1)
	push {r0}
	push {r1}
	push {lr}
	adds r7, #3 // max_size += 3
	lsr r1, 1 // n>>1
	bl GCD
	pop {lr}
	pop {r1}
	pop {r0}
	bx lr
else: // return gcd(abs(m-n), min(m,n))
	push {r0}
	push {r1}
	push {lr}
	adds r7, #3
	cmp r1, r0 // n>=m? // return gcd(n-m, m)
	bge swap
	subs r0, r0, r1 // return gcd(m-n, n)
	b else_return
swap: // return gcd(n-m, m)
	subs r6, r1, r0 // tmp =  n-m
	movs r1, r0 // n = m
	movs r0, r6 // m = n-m
else_return:
	bl GCD
	pop {lr}
	pop {r1}
	pop {r0}
	bx lr
