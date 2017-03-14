#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#include <math.h>
#include <signal.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <asm-generic/termbits.h>
#include <sys/ioctl.h>
#include <sys/time.h>
//#define SHOWDATA
#include "vcom_test.h"

/*
 * The Average algorithm : 
 * avg(n) = avg(n-1)+(NowValue-avg(n-1))/n 
 */
float __avg_count(float now_itv, int round)
{
	static float avg_itv = 0.0;	
	float dif;

	dif = (now_itv-avg_itv) / (float)round;
	avg_itv += dif;

	return avg_itv;
}
	
int _pkdata_itv_chk(struct timeval *wtime, struct timeval *rtime, struct vcom_test *vcom)
{
	struct timeval res;
	float tmp;
	float tmp_itv;
	float set_itv;
	float now_itv;
		
	set_itv =  vcom->itv/1000.0;
	timersub(rtime, wtime, &res);
	tmp = (res.tv_sec*1000000 + res.tv_usec) / 1000000.0;
	
	if(tmp < 0.001){
        return -1;
    }
	tmp_itv = fabs(set_itv - tmp);

//	printf("\nSet %.2f, tmp %.2f, tmp_itv %.6f\n", set_itv, tmp, tmp_itv);
	if(tmp_itv > 3.0){
		printf("\n***** Critical time interval(wrtime-rtime) %.3f sec ****\n", tmp);	
		return -1;
	}
	
	now_itv = tmp_itv;
	
	if(now_itv > vcom->max_itv){
		vcom->max_itv = now_itv;
	}
	
	vcom->avg_itv = __avg_count(now_itv, vcom->round);

	return 0;
}

int _recvdata(int fd, unsigned char *rxbuf, int recvlen)
{
	int rlen;
	int tmp;
	int ret;
	fd_set rfds;
	struct timeval tv;

	rlen = 0;
	alarm(RECVDATATIMEOUT);
	signal(SIGALRM, alarm_handle);

	while(rlen != recvlen){
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		tv.tv_sec = 0;
		tv.tv_usec = 200*1000;
		ret = select(fd+1, &rfds, 0, 0, &tv);
		if(ret <= 0){
			continue;
		}
		
		if(FD_ISSET(fd, &rfds)){
			tmp = read(fd, rxbuf+rlen, recvlen-rlen);
			rlen += tmp;
		}

		if(timeout){
			timeout = 0;
			return -1;
		}
	}
	
	return rlen;
}

int _set_termios(struct vcom_test *vcom, struct termios2 *newtio)
{
	int ret;
	
	vcom->fd = open(vcom->path, O_RDWR);
	if(vcom->fd == -1){
		printf("Failed to open com port\n");
		exit(0);
	}
	printf("Open ComPort, fd %d\n", vcom->fd);
	
	if((ret = ioctl(vcom->fd, TCGETS2, newtio)) < 0){
		printf("Failed to get termios setting !\n");
		return -1;
	}
	printf("BEFORE setting : ospeed %d ispeed %d\n", newtio->c_ospeed, newtio->c_ispeed);
	
	newtio->c_iflag &= ~(ISTRIP|IUCLC|IGNCR|ICRNL|INLCR|ICANON|IXON|PARMRK);
	newtio->c_iflag |= (IGNBRK|IGNPAR);
	newtio->c_lflag &= ~(ECHO|ICANON|ISIG);
	newtio->c_cflag &= ~CBAUD;
	newtio->c_cflag |= BOTHER;
	/* For 0a/0d issue */
	newtio->c_oflag &= ~(OPOST);
	newtio->c_ospeed = vcom->baud;
	newtio->c_ispeed = vcom->baud;

	if((ret = ioctl(vcom->fd, TCSETS2, newtio)) < 0){
		printf("Failed to get termios setting !\n");
		return -1;
	}
	printf("AFTER setting : ospeed %d ispeed %d\n", newtio->c_ospeed, newtio->c_ispeed);
	
	return 0;
}

int pack_size_test(struct vcom_test *vcom)
{
	int ret;
	int rlen;
	int wlen;
	unsigned char txbuf[1];
	unsigned char rxbuf[DATALEN*5] = {0};
	fd_set wfds;
	fd_set rfds;
	struct timeval tv;
	
	txbuf[0] = 1;
	wlen = 0;
	rlen = 0;

	do{
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_SET(vcom->fd, &rfds);
		FD_SET(vcom->fd, &wfds);
		tv.tv_sec = 3;
		tv.tv_usec = 0;
		
		ret = select(vcom->fd+1, &rfds, &wfds, 0, &tv);	
		if(ret < 0){
			break;
		}
		
		if(!ret){
			continue;
		}
	
		if(FD_ISSET(vcom->fd, &wfds) && !rlen){
			wlen += write(vcom->fd, txbuf, 1);
			if(wlen <= 0){
				printf("Round %d, write FAILED !!\n", vcom->round);
				return -1;
			}
			usleep(50*1000);
			show_result(wlen, rlen, vcom);
		}
	
		if(FD_ISSET(vcom->fd, &rfds) && !rlen){
			rlen = read(vcom->fd, rxbuf, DATALEN);
			if(rlen <= 0){
				printf("Round %d, read length %d, FAILED !!\n", vcom->round, rlen);
				break;
			}
			print_data(rxbuf, rlen, "Read");
		}
	
		if(rlen > 0){
			if(wlen != vcom->pack_size && vcom->round != 1){
				printf("Wlen (%d) != SetPackLen (%d)\n", wlen , vcom->pack_size);
				vcom->pack_size_err++;
			}
			show_result(wlen, rlen, vcom);
			wlen = 0;
			rlen = 0;
			vcom->round++;
		}
	}while(1);

	return 0;
}

int pack_test(struct vcom_test *vcom)
{
	int i;
	int ret;
	int wlen;
	int rlen;
	unsigned char txbuf[DATALEN];
	unsigned char rxbuf[DATALEN];
	struct timeval tv;
	struct timeval wtime;
	struct timeval rtime;
	fd_set rfds;
	fd_set wfds;
	
	for(i = 0; i < DATALEN; i++){
		txbuf[i] = i;
	}
	printf("Init write data done !\n");
	
	wlen = 0;
	rlen = 0;
	timerclear(&wtime);
	timerclear(&rtime);

	do{
		FD_ZERO(&rfds);
		FD_ZERO(&wfds);
		FD_SET(vcom->fd, &rfds);
		FD_SET(vcom->fd, &wfds);
		tv.tv_sec = 3;
		tv.tv_usec = 0;
	
		ret = select(vcom->fd+1, &rfds, &wfds, 0, &tv);
		if(ret < 0){
			printf("Select failed !\n");
			break;
		}
	
		if(!ret){
			// That's mean select nothing 
			continue;
		}
	
		if(FD_ISSET(vcom->fd, &wfds) && !wlen && !rlen){
			wlen = write(vcom->fd, txbuf, DATALEN);
			if(wlen != DATALEN){
				printf("Round %d, write FAILED !!\n", vcom->round);
				break;
			}
			if(vcom->itv != 0){
				gettimeofday(&wtime, NULL);
			}
			print_data(txbuf, wlen, "Write");
		}
	
		if(FD_ISSET(vcom->fd, &rfds) && wlen == DATALEN && !rlen){	
			if(!vcom->ec){	// with normal type
				rlen = _recvdata(vcom->fd, rxbuf, DATALEN);
			}else{
				rlen = read(vcom->fd, rxbuf, DATALEN);
			}
			if(vcom->itv != 0){
				gettimeofday(&rtime, NULL);
			}	
			print_data(rxbuf, rlen, "Read");	
		}
		
		if(wlen == DATALEN && rlen > 0){			
			compare_data(rxbuf, rlen, vcom);
			if(vcom->itv != 0){
				_pkdata_itv_chk(&wtime, &rtime, vcom);
			}
			show_result(wlen, rlen, vcom);
			vcom->round++;
			wlen = rlen = 0;
			if(!vcom->itv){
				sleep(1);
			}
			sleep(0);
		}
	}while(1);
	
	return 0;
}
							
int main(int argc, char **argv)
{
	struct termios2 newtio;
	struct vcom_test vcom;
	
	if(argc < 3){
		printf("./packtest [com port path] [baud rate]\n");
		exit(0);
	}
	printf("Enter end-character in Hex (0 for disable) : ");
	scanf("%lx", &vcom.ec);
	printf("Enter interval time (0 for disable) : ");
	scanf("%d", &vcom.itv);
	printf("Enter pack-size (0 for disable) : ");
	scanf("%d", &vcom.pack_size);
	
	
	vcom.path = argv[1];
	vcom.baud = atoi(argv[2]);
	
	printf("Setting : \n");
	printf("Com port path : %s\n", vcom.path);
	printf("baud rate : %d\n", vcom.baud);
	if(vcom.ec != 0){
		printf("end-character : %lx\n", vcom.ec);
	}
	if(vcom.itv != 0){
		printf("iterval : %d ms\n", vcom.itv);
	}

	if(_set_termios(&vcom, &newtio) == -1){ exit(0); }
	
	vcom.err = 0;
	vcom.ecerr = 0;
	vcom.round = 1;
	vcom.max_itv = 0;
	vcom.avg_itv = 0;
	vcom.pack_size_err = 0;
	
	if(vcom.pack_size != 0){
		pack_size_test(&vcom);
	}else{
		pack_test(&vcom);
	}
	printf("Ready to close...\n");
	close(vcom.fd);
	
	return 0;
}






