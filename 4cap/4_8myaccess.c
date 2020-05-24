/**
 *这部分的内容是用于验证access函数的作用
 */
#include "apue.h"
#include <fcntl.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		err_quit("你输入的形式不对，应该是： ./a.out <pathname>");
	}
	if(access(argv[1], R_OK) < 0){
		err_ret("%s 访问错误",argv[1]);
	}else{
		printf("成功访问文件 %s",argv[1]);
	}

	if(access(argv[1], W_OK | R_OK) < 0){
		printf("能打开，但所不能读写");
	}else{
		printf("能打开，能读写");
	}


	return 0;
}
