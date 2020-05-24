/*
 *this file is used to creat a hole file;and to see what happend
 * */
#include "apue.h"
#include <fcntl.h>

char buf1[]="qwertyuiop";
char buf2[]="QWERTYUIOP";

int main(){
	int fd;
	if((fd = creat("3_2myfile.txt",FILE_MODE))<0)
		err_sys("creat error");
	if((write(fd,buf1,10))!=10)
		err_sys("write error");
	if((lseek(fd,1648,SEEK_SET)) == -1)
		err_sys("lseek error");
	if((write(fd,buf1,10))!=10)
                err_sys("write error");

	return 0;
}
