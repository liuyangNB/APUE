/*书本的例子，为三个标准流和一个文件流打印有关缓存有关的状态信息。
 *
 *
 * */

#include "apue.h"

void pr_stdio(const char* , FILE*);
int is_unbuffered(FILE* );
int is_linebuffered(FILE*);
int buffer_size(FILE*);

int main(void){
	FILE* fp;

	fputs("Enter any character\n",stdout);//把数据放到stdout流
	if(getchar() == EOF){//stdin流的数据
		err_sys("get char error");
	}
	fputs("one line to standard error\n",stderr);//stderr流的书u
	
	//打印标准流的数据
	pr_stdio("stdin",stdin);
	pr_stdio("stdout",stdout);
	pr_stdio("stderr",stderr);//???
	
	//打开/etc/passwd在个文档，读
	if((fp = fopen("/etc/passwd","r")) == NULL){
		err_sys("fopen error");
	}

	if( getc(fp) == EOF){
		//读不到数据就是“报错” 因为读到文件尾或者出错 返回一样的值；反正就算没读到
		err_sys("get char error");
	}
	pr_stdio("/etc/passed",fp);
	exit(0);
}


/*子函数的作用：根据fp打印其缓存数据*/
void pr_stdio(const char* name, FILE* fp){
	
	printf("strean = %s ",name);
	if(is_unbuffered(fp)){
		printf("unbuffered");
	}else if(is_linebuffered(fp)){
		printf("line buffered");
	}else{
		printf("fully buffered");
	}

	printf(",buffer size = %d\n", buffer_size(fp));
}


/*
 *the following is nonportable
 * */
#if defined (_IO_UNBUFFERED)

int is_unbuffered(FILE* fp){
	//判断fp是不是无缓存
	return (fp->_flags & _IO_UNBUFFERED);
}


int is_linebuffered(FILE* fp){
	return (fp->_flags & _IO_LINE_BUF);
}

int buffer_size(FILE * fp){
	return (fp->_IO_buf_end - fp->_IO_buf_base);
}

#elif defined (__SNBF)

int is_unbuffered(FILE* fp){
	return (fp->flags & __SNBF);
}

int is_linebuffered(FILE* fp){
	return (fp->flags & __SLBF);
}

int buffer_size(FILE*fp){
	return (fp->_bf.size);
}

#elif defined (_IONBF)

#ifdef _LP64
#define _flag __pad[4]
#define _ptr __pad[1]
#define _base __pad[2]
#endif

int is_unbuffered(FILE* fp){
	return (fp->_flag & _IONBF);
}

int is_linebuffered(FILE* fp){
	return (fp->_flag & _IOLBF);
}

int buffer_size(FILE* fp){
#ifdef _LP64
	return (fp->_base - fp->_ptr);
#else
	return (BUFSIZ);
#endif
}

#else

#error unknown stdio implementation

#endif
