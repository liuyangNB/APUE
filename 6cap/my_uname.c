#include "apue.h"
#include <sys/utsname.h>
#include <unistd.h>
struct utsname myutsname;
char hostname[1024];
int main(){
	uname(&myutsname);
	printf("print sysname: %s\n",myutsname.sysname);
	printf("print nodename: %s\n",myutsname.nodename);
	printf("print release: %s\n",myutsname.release);

	printf("----------------\n");
	if(gethostname(hostname,1024)!=0){
		err_sys("get host name error");
	}
	printf("pint host name:%s\n",hostname);
	return 0;
}
