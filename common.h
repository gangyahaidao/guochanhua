/*
 * common.h
 *
 *  Created on: 2018年4月24日
 *      Author: sip
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/time.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <stdarg.h>
#include <malloc.h>

#include <pthread.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define ERR(msg) do{		\
		fprintf(stderr,"[%s:%d]%s:%s\n",__FILE__,__LINE__,msg,strerror(errno));	\
		exit(-1);	\
	}while(0)

ssize_t writen(int fd,const void *buf,size_t len);

long sysUsecTime(void);

char *str_contract(const int count, ...);

int semaphore_v(int sem_id);
int semaphore_p(int sem_id);
void del_semvalue(int sem_id);

char* get_gmt_time(char* time_ch_buf);
long getCurrentMsecTime();

#endif /* COMMON_H_ */
