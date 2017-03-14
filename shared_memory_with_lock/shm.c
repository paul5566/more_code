#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include <sys/mman.h>	// Shared memory
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
// PATH : /dev/shm/shared_mem
#define SHAREDMEM_FILE	"/shared_mem"
#define ROUND 40
#define TIMEOUT 15
#define HOST_WIN 1
#define GUEST_WIN 2
#define DEUCE 3

#define errExit(en, msg, var) \
	do{ errno = en; perror(msg); shared_param_exit(var); exit(EXIT_FAILURE); }while(0)

struct shared_var{
	int val;
	int round;
	int res;
	int guest_max;
	int guest_num;
	pthread_mutex_t mutex;
	pthread_mutex_t submtx;
	pthread_cond_t cond;
	pthread_barrier_t barr;
};
/*
 * The mutex & shared memory initial
*/
int shared_mutex_init(struct shared_var *var)
{
	pthread_mutexattr_t mtxattr;

	pthread_mutexattr_init(&mtxattr);	// Mutex attr init
	do{
		if(pthread_mutexattr_setpshared(&mtxattr, PTHREAD_PROCESS_SHARED) != 0){
			printf("Failed to set PTHREAD_PROCESS_SHARED\n");
			break;
		}
	
		if(pthread_mutexattr_setrobust(&mtxattr, PTHREAD_MUTEX_ROBUST) != 0){
			printf("Failed to set PTHREAD_MUTEX_ROBUST\n");
			break;
		}
	
		if(pthread_mutex_init(&(var->mutex), &mtxattr) != 0){
			printf("Failed to init mutex\n");
			break;
		}
		
		if(pthread_mutex_init(&(var->submtx), &mtxattr) != 0){
			printf("Failed to init sub mutex\n");
			break;
		}

		printf("Success to %s\n", __func__);
		
		return 0;
	}while(0);
	pthread_mutexattr_destroy(&mtxattr);
	
	return -1;
}
/*
 * The mutex condition & shared memory condition initial
*/
int shared_cond_init(struct shared_var *var)
{
	pthread_condattr_t condattr;
	
	pthread_condattr_init(&condattr);	// Mutex condition attr init

	do{
		if(pthread_condattr_setpshared(&condattr, PTHREAD_PROCESS_SHARED) != 0){
			printf("Failed to set PTHREAD_PROCESS_SHARED in mutex condition\n");
			break;
		}
		
		if(pthread_cond_init(&(var->cond), &condattr) != 0){
			printf("Failed to init mutex condition\n");
			break;
		}
		printf("Success to %s\n", __func__);
		
		return 0;
	}while(0);
	pthread_condattr_destroy(&condattr);	
	
	return -1;
}
/*
 * The barrier & shared memory condition initail
*/
int shared_barrier_init(struct shared_var *var)
{
	pthread_barrierattr_t barrattr;
	
	pthread_barrierattr_init(&barrattr);
	
	do{
		if(pthread_barrierattr_setpshared(&barrattr, PTHREAD_PROCESS_SHARED) != 0){
			printf("Failed to set PTHREAD_PROCESS_SHARED in barrier condition");
			break;
		}
	
		if(pthread_barrier_init(&var->barr, &barrattr, (unsigned) var->guest_max+1) != 0){
			printf("Failed to initial barrier !\n");
			break;
		}
		printf("Success to %s\n", __func__);
		
		return 0;
	}while(0);
	pthread_barrierattr_destroy(&barrattr);
	
	return -1;
}

int shared_mutex_lock(pthread_mutex_t *mtx)
{
	int mtxR;
	
	do{
		mtxR = pthread_mutex_lock(mtx);
		/* Thats meaning the process died while holding the lock */
		if(mtxR == EOWNERDEAD){
			printf("I'm(%d) died !\n", getpid());
			/* Should cleanup the lock holder, use this function */
			pthread_mutex_consistent(mtx);
			break;
		}else if(!mtxR){
			/* lock success */
			break;
		}
		printf("%d cannot get mutex\n", getpid());
	}while(1);
	
	return 0;
}

void shared_param_exit(struct shared_var *var)
{
	/* destroy always success */
	pthread_mutex_destroy(&var->mutex);
	pthread_mutex_destroy(&var->submtx);
	pthread_cond_destroy(&var->cond);
	shm_unlink(SHAREDMEM_FILE);
	pthread_barrier_destroy(&var->barr);
}

int host_things(struct shared_var *var)
{
	int mutexR;
	int condR;
	int host_val;
	int round = 0;
	struct timespec timeout;

	srand((unsigned)time(NULL));
	do{
		host_val = 0;
		/* Lock first */
		shared_mutex_lock(&var->mutex);
		round = (var->round);	
		/* To wait for guest put a number to shared memory */
		clock_gettime(CLOCK_REALTIME, &timeout);
		timeout.tv_sec += TIMEOUT;
		condR = pthread_cond_timedwait(&var->cond, &var->mutex, &timeout);
		if(condR == ETIMEDOUT){
			printf("Waiting timeout !!\n");
		}else if(condR != 0){
			printf("pthread_cond_timedwait FAILED !\n");
			pthread_mutex_unlock(&(var->mutex));
			return -1;
		}
		/* To compete who win */	
		host_val = rand()%50;
		if(host_val > (var->val)){
			var->res = HOST_WIN;
			printf("<HOST> Round %d, Host win !!(host %d | guest %d)\n", round, host_val, var->val);
		}else if(host_val < (var->val)){
			var->res = GUEST_WIN;
			printf("<HOST> Round %d, Guest win !!(host %d | guest %d)\n", round, host_val, var->val);
		}else{
			var->res = DEUCE;
			printf("<HOST> Round %d, Deuce !!(host %d | guest %d)\n", round, host_val, var->val);
		}
		(var->round)++;	// round+1

		sleep(1);
		/* Send a broadcast signal to wakeup all guest */
		pthread_cond_broadcast(&(var->cond));
		pthread_mutex_unlock(&var->mutex);
	}while(round < ROUND);
	
	return 0;
}

int guest_things(struct shared_var *var)	
{
	int condR;
	int round = 0;
	struct timespec timeout;
	
	srand((unsigned)time(NULL));
	do{
		/* Lock first */
		shared_mutex_lock(&var->mutex);
		/* The sub mutex lock, to control all the guest */
		shared_mutex_lock(&var->submtx);
		if(round == var->round && var->round != 0){
			pthread_mutex_unlock(&(var->submtx));
			pthread_mutex_unlock(&(var->mutex));
			continue;
		}	
		var->guest_num++;
		round = (var->round);
		/* Guest put a number to shared memory */
		(var->val) += rand()%10;
		if((var->val) > 50){
			(var->val) = rand()%5;
		}
		/* If all the guest ready, send a signal to host to compete */
		if(var->guest_max == var->guest_num){
			pthread_cond_signal(&(var->cond));	// always success
			var->guest_num = 0;		// signal send, reset the guest count
		}
		/* Release sub mutex */
		pthread_mutex_unlock(&(var->submtx));
		/* Wait for HOST compete */
		clock_gettime(CLOCK_REALTIME, &timeout);
		timeout.tv_sec += TIMEOUT;
		condR = pthread_cond_timedwait(&var->cond, &var->mutex, &timeout);
		if(condR == ETIMEDOUT){
			printf("Waiting timeout !!\n");
		}else if(condR != 0){
			printf("pthread_cond_timedwait FAILED !\n");
			pthread_mutex_unlock(&(var->mutex));
			return -1;
		}
		/* Pirnt the result */
		switch(var->res){
			case HOST_WIN:
				printf("<GUEST> Round %d, Host win !!\n", round);
				break;
			case GUEST_WIN:
				printf("<GUEST> Round %d, Guest win !!\n", round);
				break;
			case DEUCE:
				printf("<GUEST> Round %d, Deuce !!\n", round);
				break;
			default:
				printf("<GUEST> Fuckin wrong (%d)\n", var->res);
				return -1;
		}

		pthread_mutex_unlock(&(var->mutex));
	}while(round < ROUND);
	
	return 0;
}

void usage()
{
	printf("Usage : ./rshm [-h/-g]\n");
	printf("-h      for host\n");
	printf("-g      for guest\n");
}

int main(int argc, char **argv)	
{
	int shm_fd;
	int host = 0;
	int guest = 0;
	int ret;
	char ch;
	static struct shared_var *var;
	
	if(argc != 2){
		usage();
		return -1;
	}

	while((ch = getopt(argc, argv, "hg")) != -1){
		switch(ch){
			case 'h':
				printf("Excute Host mode !\n");
				host = 1;
				break;
			case 'g':
				printf("Excute Guest mode !\n");
				guest = 1;
				break;
			default :
				usage();
				return -1;
		}
	}

	if(!host && !guest){
		usage();
		return -1;
	}
		
	if(host){
		/* Server process init */
		shm_unlink(SHAREDMEM_FILE);		// init the shared memory
		shm_fd = shm_open(SHAREDMEM_FILE, (O_CREAT | O_EXCL | O_RDWR),
					(S_IREAD | S_IWRITE));
		
		if(shm_fd < 0){ 
			errExit(shm_fd, "<HOST> Shared memory open FAILED", var); 
		}
		printf("<HOST> Create Shared memory SUCCESS ! initial value\n");
	}
	
	if(guest){
		/* Client process init */
		shm_fd = shm_open(SHAREDMEM_FILE, O_RDWR, (S_IREAD | S_IWRITE));
		if(shm_fd < 0){
			errExit(shm_fd, "<GUEST> Shared memory open FAILED", var);
		}
		printf("<GUEST> Create shared memory SUCCESS !\n");
	}
	
	/* Set the shared memory size to be size of structure */
	if((ret = ftruncate(shm_fd, sizeof(struct shared_var))) != 0){	// Perhaps only once is enough
		errExit(ret, "ftruncate : ", var);
	}
	/*
	 * Use mmap to connect the structure with shared memory area,
	 * the mmap return value should assign to shraed memory pointer
	*/
	var = mmap(0, sizeof(struct shared_var), PROT_READ | PROT_WRITE, 
			MAP_SHARED, shm_fd, 0);
	if(var == MAP_FAILED){
		errExit(-1, "mmap : ", var);
	}
	
	if(host){
		/* Only HOST need to init the mutex lock & condition */
		if(shared_mutex_init(var) == -1){ exit(0); }
		if(shared_cond_init(var) == -1){ exit(0); }
		var->val = 0;  
        var->round = 0;     
		var->guest_max = 0;     
		printf("Host initial success, begian to do host thing !!\n");
		if(host_things(var) != 0){	// Do host things
			shared_param_exit(var);
			return -1;
		}
		if(shared_barrier_init(var) == -1){ exit(0); }
	}
	
	if(guest){
		printf("Guest initial success, begian to do guest thing !!\n");
		var->guest_max++;
		printf("Now, You are %d guest\n", var->guest_max);
		if(guest_things(var) != 0){	// Do guest things
			shared_param_exit(var);
			return -1;
		}
	}
	/*
	 * I know, the barrier is useless, I just want to try the
	 * pthread_barrier in shared memory !
	*/
	pthread_barrier_wait(&var->barr);
	if(host){
		close(shm_fd);
		shared_param_exit(var);
	}

	return 0;
}
	
