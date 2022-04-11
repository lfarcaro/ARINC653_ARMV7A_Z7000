#ifndef SPINLOCK_H_
#define SPINLOCK_H_

// Spin lock type
typedef unsigned int SPINLOCK_TYPE;

// Unlocked value
#define SPINLOCK_UNLOCKED 0x00000000

// Locking method
void SPINLOCK_LOCK(SPINLOCK_TYPE *SPINLOCK);

// Unlocking method
void SPINLOCK_UNLOCK(SPINLOCK_TYPE *SPINLOCK);

#endif
