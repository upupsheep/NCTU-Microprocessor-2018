	.syntax unified
	.cpu cortex-m4
	.thumb
.data
	user_stack: .zero 128
	expr_result: .word 0
.text
	.global main
	postfix_expr: .asciz "-100 10 20 + - 10 +"

// r0: postfix_eppr
// r1 -> r0
// r2: present char
// r3: next char, r3=10
// r4: sign
// r5: atoi result
// r6: pop -> r6(first)
// r7: pop -> r7(second)
main:
	LDR R0, =postfix_expr
	movs r1, r0 // r1 = "-100 10 20 + - 10 +"
	ldr sp, =user_stack
	adds sp, sp, 128

strtok:
	movs r4, 1 // sign = 1
	movs r5, #0 // atoi_result = 0
	ldrb r2, [r1] // load byte
	cmp r2, #48 // is num?
	bge atoi_1 // is num: atoi
	cmp r2, #45 // '-'
	beq isMinus
	cmp r2, #43 // '+'
	beq add_op
	cmp r2, #32 // 'space'
	beq space
	cmp r2, #0 // 'end'
	beq program_end

program_end:
	pop {r0}
	ldr R1, =expr_result
	str r0, [r1]
	B program_end
atoi_1:
	subs r2, r2, 48 // ascii to num
	adds r5, r5, r2 // result += num
	adds r1, r1, 1 // update pointer(r1)
	ldrb r2, [r1]
	b atoi
atoi:
	//TODO: implement a ¡§convert string to integer¡¨ function
	// result = 0; result += num; result *= 10;
	cmp r2, #32 // is space?
	beq push_stack
	subs r2, r2, 48 // ascii to num
	adds r5, r5, r2 // result += num
	movs r3, #10
	muls r5, r5, r3 // result *= 10
	adds r1, r1, 1 // update pointer(r1)
	ldrb r2, [r1]
	b atoi
push_stack:
	muls r5, r5, r4 // result *= sign
	push {r5}
	adds r1, r1, 1 // update pointer(r1)
	b strtok
isMinus:
	ldrb r3, [r1,1]
	cmp r3, #32 // is space?
	beq minus_op
	movs r4, -1 // is not space: sign = -1
	adds r1, r1, 1 // r1++
	ldrb r2, [r1] // load byte
	b atoi_1
space: // update pointer(r1)
	adds r1, r1, 1 // r1++
	b strtok
add_op:
	pop {r6}
	pop {r7}
	adds r7, r7, r6 // top2+top1
	push {r7}
	adds r1, 1
	b strtok
minus_op:
	pop {r6}
	pop {r7}
	subs r7, r7, r6 // top2-top1
	push {r7}
	adds r1, 1
	b strtok
