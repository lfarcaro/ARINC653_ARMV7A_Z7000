// Locking method
// R0 - SPINLOCK_TYPE pointer
	.align 4
	.global SPINLOCK_LOCK
SPINLOCK_LOCK: .func
	LDREX R1, [R0] // Reads and monitors exclusive access
	CMP R1, #0x00000000
	BNE SPINLOCK_LOCK // Spin-locks
	LDR R1, =0x00000001
	STREX R2, R1, [R0] // Writes and monitors exclusive access, result is in R2 (0 OK, 1 error)
	CMP R2, #0
	BNE SPINLOCK_LOCK
	BX LR
	.endfunc

// Unlocking method
// R0 - SPINLOCK_TYPE pointer
	.align 4
.global SPINLOCK_UNLOCK
SPINLOCK_UNLOCK: .func
	LDREX R1, [R0] // Reads and monitors exclusive access
	LDR R1, =0x00000000
	STREX R2, R1, [R0] // Writes and monitors exclusive access, result is in R2 (0 OK, 1 error)
	CMP R2, #0
	BNE SPINLOCK_UNLOCK
	BX LR
	.endfunc
