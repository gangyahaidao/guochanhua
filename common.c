/*
 * common.c
 *
 *  Created on: May 23, 2018
 *      Author: qingpu
 */


#include "common.h"

ssize_t writen(int fd,const void *buf,size_t len)
{
	size_t total = 0;
	ssize_t n;
	while(total<len)
	{
again:
		if((n = write(fd,(char*)buf+total,len-total))<0)
		{
			if(errno==EINTR)
				goto again;
			return -1;
		}
		total += n;
	}
	return total;
}

char *str_contract(const int count, ...)
{
	int i = 0;
	int total_len = 0;
	va_list vaptr;

	va_start(vaptr, count);
	for(i = 0; i < count; i++)
	{
		total_len += strlen(va_arg(vaptr, char*));
	}
	va_end(vaptr);

	char* result = (char*)malloc(total_len + 1);
	if(!result)
	{
		ERR("--malloc failed in str_contract()");
	}
	memset(result, 0, total_len);
	char* head_ptr = result;

	va_start(vaptr, count);
	for(i = 0; i < count; i++)
	{
		char* temp = va_arg(vaptr, char*);
		while(*temp != '\0')
		{
			*result++ = *temp++;
		}
	}
	va_end(vaptr);

	*result = '\0';

	return head_ptr;
}

void del_semvalue(int sem_id)
{
    // 删除信号量
    if (semctl(sem_id, 0, IPC_RMID, 0) == -1)
    {
        ERR("Failed to delete semaphore\n");
    }
}

int semaphore_p(int sem_id)
{
    // 对信号量做减1操作，即等待P（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1;//P()
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        ERR("semaphore_p failed\n");
    }

    return 1;
}

int semaphore_v(int sem_id)
{
    // 这是一个释放操作，它使信号量变为可用，即发送信号V（sv）
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; // V()
    sem_b.sem_flg = SEM_UNDO;
    if (semop(sem_id, &sem_b, 1) == -1)
    {
        ERR("semaphore_v failed\n");
    }

    return 1;
}

/**
 * 返回时间秒
 * */
long sysUsecTime(void){
    struct timeval tv;
    struct timezone tz;
    //struct tm *t;
    gettimeofday(&tv, &tz);
    //printf("tv_sec:%ld\n",tv.tv_sec);
    return tv.tv_sec;
}

//返回当前的毫秒数
long getCurrentMsecTime(){
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm current_time;
    localtime_r(&tv.tv_sec, &current_time);

    int minute = current_time.tm_min;
    int sec = current_time.tm_sec;
    int msec = (int) (tv.tv_usec / 1000);

    return (minute*60+sec)*1000+msec;
}

/**
 * 格式化时间，返回时间字符串
 */
char* get_gmt_time(char* time_ch_buf) {
    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm current_time;
    localtime_r(&tv.tv_sec, &current_time);

    int year = (1900 + current_time.tm_year);
    int month = (1 + current_time.tm_mon);
    int day = current_time.tm_mday;
    int hour = current_time.tm_hour;
    int minute = current_time.tm_min;
    int sec = current_time.tm_sec;
    //int msec = (int) (tv.tv_usec / 1000);

    snprintf(time_ch_buf, 128, "%d-%02d-%02d@%02d:%02d:%02d", year, month, day, hour, minute, sec);
    return time_ch_buf;
}