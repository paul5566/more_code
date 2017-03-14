#include <unistd.h>
#include <fcntl.h>
#include <bsd/stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include "shared.h"

struct shared_test *st;

void clean_all(void)
{
	struct shared_member *sm = st->sm;

	for(int i = 0; i < SIZE; ++i){
		LOCK(sm[i]);
		sm[i].val = 0;
		UNLOCK(sm[i]);
	}
}

void show_all()
{
	st->ctrl = SHOW;
}

void change_val(int order)
{
	int rand = arc4random()&64;
	struct shared_member *sm = st->sm;

	LOCK(sm[order]);
	sm[order].val = rand;
	UNLOCK(sm[order]);
	printf("<Guest> change shared(%d) val to %d\n",
				order+1, rand);
}

void option_handle(int opt)
{
	switch(opt){
		case CLEAR:
			clean_all();
			break;
		case SHOW:
			show_all();
			break;
		default:
			change_val(opt-1);
			break;
	}
}

int listen_handle(void)
{
	int ret;
	char rx[8] = {0};
	fd_set rfds;
	struct timeval tv;
	int fd = fileno(stdin);
	int opt;
	size_t rlen;

	do{
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		tv.tv_sec = 1;
		tv.tv_usec = 0;
		
		ret = select(fd+1, &rfds, NULL, NULL, &tv);
		if(ret < 0)
			errReturn("Select failed(%s)\n", 
								strerror(errno));

		if(ret == 0) continue;
		
		if(FD_ISSET(fd, &rfds)){
			fgets(rx, sizeof(rx), stdin);
			rlen = strlen(rx);
			rx[rlen-1] = '\0';

			if(strcmp(rx, "CLEAR") == 0){
				opt = CLEAR;
			}else if(strcmp(rx, "SHOW") == 0){
				opt = SHOW;
			}else{
				opt = atoi(rx);
				if(opt > SIZE || opt < 0)
					errContinue("Input error!\n");
			}
			break;
		}
	}while(1);
	
	return opt;
}

int main(void)
{
	if(shared_test_alloc(GUEST, &st) == -1) 
		return -1;

	do{
		option_handle(listen_handle());	
	}while(1);

	return 0;
}
