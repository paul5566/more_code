#ifndef VCOM_TEST_H
#define VCOM_TEST_H

#define RECVDATATIMEOUT	15

#define DATALEN 		256
#define CONNECTNUM		15
#define CLTTHREADNUM   	2

#define handle_error_en(en, msg) \
			do{ errno = en; perror(msg); exit(EXIT_FAILURE); }while(0)

#ifdef SHOWDATA
#define print_data(buf, len, io)	do{show_data(buf, len, io); }while(0)
#define show_result(wlen, rlen, vcom)	do{}while(0)
#else
#define print_data(buf, len, io)	do{}while(0)
#define show_result(wlen, rlen, vcom)	do{disp_result(wlen, rlen, vcom); }while(0)
#endif

int timeout = 0;		// the recv data clock alarm

struct vcom_test {
	int baud;
	char *path;
	unsigned long int  ec;
	int itv;
	int pack_size;
	double max_itv;
	double avg_itv;
	int round;
	int fd;
	int err;
	int ecerr;
	int pack_size_err;
};
	
static inline void disp_result(int wlen, int rlen, struct vcom_test *vcom)
{
	printf("Round %d, Wlen %d, Rlen %d", vcom->round, wlen, rlen);                
	if(!vcom->pack_size){
		printf(", Err %d", vcom->err);                    
		if(vcom->ec != 0){ 
			printf(", End-Char err %d", vcom->ecerr); 
		} 
		if(vcom->itv != 0){
			printf(", Average-Itv %f, Max-Itv %f ", vcom->avg_itv, vcom->max_itv);
		} 
	}else{
		printf(", Pack-Size err %d  ", vcom->pack_size_err);
	}
	printf(" .     \r");                                            
	fflush(stdout);
}

static inline void show_data(unsigned char *buf, int len, char *io)
{
	int i;
	
	printf("%s :", io);
	for(i = 0; i < len; i++){
		printf(" %x |", *(buf+i));
	}
	printf(" ** %d byte **\n", len);
	if(!strcmp(io, "Read")){
		printf("\n\n");
	}
}
		
static inline void compare_data(unsigned char *rxbuf, int len, struct vcom_test *vcom)
{
	int i;
	
	for(i = 0; i < len-1; i++){
		if(abs(*(rxbuf+i+1) - *(rxbuf+i)) != 1 && *(rxbuf+i) != DATALEN-1){
			vcom->err++;
	//		printf("| <%x,%x> ", *(rxbuf+i), *(rxbuf+i+1));
		}
	}
	if(vcom->ec != 0){
		if(*(rxbuf+len-1) != vcom->ec){	
			vcom->ecerr++;
		}
	}
}

static inline void alarm_handle(int sig)
{
	timeout = 1;	
}

#endif
