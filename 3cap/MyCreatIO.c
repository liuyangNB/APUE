#include "apue.h"
#include <fcntl.h>

int main(){
	FILE* fp = "./Mycreat.txt";
	creat(fp,"w+");

	return 0;
}
