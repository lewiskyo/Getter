#pragma  once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace getter {

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

	friend class MyLockGuard;

private:
	int mutex;
};


class MyLockGuard {
public:

	MyLockGuard(CasLock * _lock) {
		lock = _lock;
		lock->lock();
	}

	~MyLockGuard() {
		lock->unlock();
	}

private:
	CasLock *lock;

};

}