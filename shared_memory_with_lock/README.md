The shared memory with "shared_mutex_lock" & the "robust" flag
================================================================

The program working
--------------------
**The HOST process**

1. Create shared memory block with `shm_open`.

2. Initial the `pthread_mutex_lock` with shared mem & `robust`.

3. Initial the `pthread_cond` with shared mem.

4. Initial the `pthread_barrier` with shared mem.

5. `Do Host Things`

6. Waiting for all process to close via `pthread_barrier_wait`.

*The Host things*

1. Host will generate a host value.

2. `pthread_cond_wait` wait for guest generate guest value.

3. Take the guest value from shared memory and compared with host value.

4. Decided which is bigger and put the result to shared memory.

5. `pthread_cond_broadcast` to wake all the guest up.

**The GUEST processes**

1. Open the shared memory block with `shm_open`.

2. `Do Guest Things`

3. Waiting for all process to close via `pthread_barrier_wait`.

*The Guest Thing*

1. To generate a guest value.

2. `pthread_cond_signal` to wake the host up.

3. `pthread_cond_wait` to wait for host put the result to shared memory.

4. Take the result from shared memory and display it.
 
Developed note
---------------
* `PTHREAD_PROCESS_SHARED` : the flag will allow control set(mutex, cond, ...)to be	shared between process.

* `PTHREAD_MUTEX_ROBUST` : ROBUST means the mutex will be release by OS while the mutex owner is dead and not release mutex by itself.
The ROBUST will be trigger by the `pthread_mutex_lock` return code `EOWNERDEAD`, and should fix the mutex lock by calling `pthread_mutex_consistent`.

Usage
-----
* Compile : make in gcc

* Excute Host : ./rshm -h

* Excute Guest : ./rshm -g


