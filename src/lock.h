#ifndef LOCK_H
#define LOCK_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

class CasLock {

    public:
        CasLock() {
            mutex = 0;
        }

        void lock() {
            while (!(__sync_bool_compare_and_swap (&mutex,0, 1) )) {

            }
        }

        void unlock() {
            while (!(__sync_bool_compare_and_swap (&mutex,1, 0) )) {

            }
        }

        friend class LockGuard;

    private:
        int mutex;
};


class LockGuard {
    public:

        LockGuard(CasLock * _lock) {
            lock = _lock;
            lock->lock();
        }

        ~LockGuard() {
            lock->unlock();
        }

    private:
        CasLock *lock;

};

#endif
