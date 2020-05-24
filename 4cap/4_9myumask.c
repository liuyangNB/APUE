/**
 *这里的程序是检验屏蔽权限函数umask的作用。
 */

#include "apue.h"
#include <fcntl.h>

#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)

int main(){
	umask(0);//取消屏蔽

	if(creat("4_9temp1",RWRWRW) < 0){
		err_sys("creat error for 4_9temp1");
	}

	umask(022);//屏蔽其他人的写
	if(creat("4_9temp2",RWRWRW) < 0){
		err_sys("creat error for 4_9temp2");
	}

	exit(0);
}
