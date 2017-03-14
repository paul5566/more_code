#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include "shared.h"

struct shared_test *st;
advLinkedList adv_ll;

void *work(void *data)
{
	struct host_param *param = (struct host_param *)data;
	int order = param->order-1;
	struct shared_member *sm = st->sm;

	do{
		LOCK(sm[order]);
		if(sm[order].val != param->val){
			printf("<Thread %d> value change! orgi(%d) new(%d)\n",
								order+1, param->val, sm[order].val);
			param->val = sm[order].val;
		}else{
			printf("<Thread %d> do something(%d)\n", 
								order+1, param->val);
		}
		UNLOCK(sm[order]);
		sleep(3);
	}while(1);
	
	pthread_exit(NULL);
}

void _do_show(void)
{
	char tmp[64];
	struct mbusWalkThroughData walk = {
		.mString = tmp,
		.bufferSize = 64,
		.currentPointer = 0,
	};
	
	AdvLinkedList_ForEach(&adv_ll, LinkedList_Walk, &walk);
	printf("%s\n", walk.mString);
}	

void *listen_thrd(void *data)
{
	do{
		if(st->ctrl == SHOW){
			_do_show();
			st->ctrl = 0;
		}
	}while(1);
	pthread_exit(NULL);
}

int main(void)
{
	int ret;
	pthread_t tid[SIZE];
	pthread_t ltid;
	struct host_param param[SIZE];

	AdvLinkedList_Init(&adv_ll, 
						SIZE, 
						sizeof(struct host_param));
	
	if(shared_test_alloc(HOST, &st) == -1) 
		return -1;

	for(int i = 0; i < SIZE; ++i){
		param[i].order = i+1;
		param[i].val = i+1;
		ret = AdvLinkedList_Add(&adv_ll, &param[i]);
	}
	printf("Add %d bricks into Adv linkedlist\n", ret);

	for(int i = 0; i < SIZE; ++i){
		ret = pthread_create(&tid[i], NULL, &work, &param[i]);
		if(ret != 0)
			errReturn_en(ret, "pthread_create");
	}

	// create listen command thread	
	ret = pthread_create(&ltid, NULL, &listen_thrd, NULL);
	if(ret != 0)
		errReturn_en(ret, "pthread_create");

	for(int i = 0; i < SIZE; ++i){
		ret = pthread_join(tid[i], NULL);
		if(ret != 0)
			errReturn_en(ret, "pthread_join");
	}
	
	pthread_join(ltid, NULL);

	return 0;
}

	
		
