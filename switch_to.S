.text
.globl switchTo
switchTo:

	movq %rsi, %rax				# rax is caller saved register.

	popq 56(%rax)					# the trick of the `next rip` which is pushed by cpu.

	movq %rbp, (%rax)			# because this is assembly code, so no `push %rbp; movq %rsp, %rbp` here! And no `pop %rbp; ret` at last!
	movq %rbx, 8(%rax)
	movq %r12, 16(%rax)
	movq %r13, 24(%rax)
	movq %r14, 32(%rax)
	movq %r15, 40(%rax)
	movq %rsp, 48(%rax)


	movq %rdi, %rax

	movq (%rax), %rbp
	movq 8(%rax), %rbx
	movq 16(%rax), %r12
	movq 24(%rax), %r13
	movq 32(%rax), %r14
	movq 40(%rax), %r15
	movq 48(%rax), %rsp

	pushq 56(%rax)					# push the next coroutine's eip, and when `ret` executes, then boom!

	ret