#include "apue.h"
#include <dirent.h>
#include <limits.h>

typedef int Myfunc(const char*, const struct stat*, int);

static Myfunc myfunc;
static int myftw(char*, Myfunc*);
static int dopath(Myfunc*);
static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;


int main(int argc, char* argv[]){
	int ret;

	if(argc != 2){
		err_quit("usage: myftw <starting-pathname>");
	}

	ret = myftw(argv[1],myfunc);/*do it all*/
	ntot = nreg + ndir + nblk + nchr + nfifo + nslink + nsock;

	if(ntot == 0){
		ntot = 1; //避免除0发生
	}

	printf("regular file    = %7ld, %5.2f %%\n",nreg, nreg*100.0/ntot);

	printf("directories     = %7ld, %5.2f %%\n",ndir, ndir*100.0/ntot);
	
	printf("block special   = %7ld, %5.2f %%\n",nblk, nblk*100.0/ntot);

	printf("char special    = %7ld, %5.2f %%\n",nchr, nchr*100.0/ntot);

	printf("FIFOS           = %7ld, %5.2f %%\n",nfifo, nfifo*100.0/ntot);

	printf("symbolic links  = %7ld, %5.2f %%\n",nslink, nslink*100.0/ntot);

	printf("sockets         = %7ld, %5.2f %%\n",nsock, nsock*100.0/ntot);

	exit(ret);
}


/*
 *通过结构层次下降，从pathname开始，函数func被每个文件调用
 * */

#define FTW_F	1	//file other than dir
#define FTW_D	2	//dir
#define FTW_DNR	3	//dir can not read
#define FTW_NS	4	//file can not stat

static char* fullpath;
static size_t pathlen;
static int myftw(char* pathname, Myfunc* myfunc){
	fullpath = path_alloc(&pathlen);

	if(pathlen <= strlen(pathname)){
		pathlen = strlen(pathname)*2;
		if((fullpath = realloc(fullpath, pathlen)) == NULL){
			err_sys("realloc failed");
		}
	}

	strcpy(fullpath,pathname);
	return (dopath(myfunc));
}


/*
 *从fullpath开始递归执行
 如果fullpath不是文件夹，我们list它，调用func，并返回；
 如果是文件夹，那就递归调研自己
 * */
static int dopath(Myfunc* func){
	struct stat statbuf;
	struct dirent* dirp;
	DIR* dp;
	int ret,n;

	if(lstat(fullpath,&statbuf) < 0){
		return(func(fullpath,&statbuf, FTW_NS));//stat error
	}

	if(S_ISDIR(statbuf.st_mode) == 0){
		return(func(fullpath,&statbuf,FTW_F));//not a directory
	}

	/*here it should be a dir*/
	if( (ret = func(fullpath,&statbuf,FTW_D)) != 0){
		return ret;
	}
	n = strlen(fullpath);
	if(n + NAME_MAX + 2 > pathlen){
		pathlen *= 2;
		if((fullpath = realloc(fullpath,pathlen)) == NULL){
			err_sys("realloc failed");
		}
	}
	fullpath[n++] = '/';
	fullpath[n] = 0;

	if((dp = opendir(fullpath)) == NULL){
		/*can not read dir*/
		return(func(fullpath,&statbuf,FTW_DNR));
	}
	while((dirp = readdir(dp)) != NULL){
		if(strcmp(dirp->d_name,".") == 0 || strcmp(dirp->d_name,"..") == 0){
			continue;//ignore . and ..
		}
		strcpy(&fullpath[n],dirp->d_name);//append name after "/"
		if((ret = dopath(func)) != 0){/*recursive*/
			break;
		}
	}

	fullpath[n-1] = 0;/*erase everthing from slash onward*/
	if(closedir(dp) < 0){
		err_ret("can not close directory %s",fullpath);
	}
	return ret;
}


/*basic function*/
static int myfunc(const char* pathname, const struct stat* statptr, int type){
	switch(type){
		case FTW_F:
			//如果是文件那就判断类型并统计
			switch(statptr->st_mode & S_IFMT){
				case S_IFREG: nreg++; break;
				case S_IFBLK: nblk++; break;
				case S_IFCHR: nchr++; break;
				case S_IFIFO: nfifo++;break;
				case S_IFLNK: nslink++; break;
				case S_IFSOCK: nsock++; break;
				case S_IFDIR:// directories should have type = FTW_D
				      err_dump("for S_IFDIR for %s", pathname);
			}
			break;
		case FTW_D:
			//如果是dir就统计dir
			ndir++;
			break;
		case FTW_DNR:
			//如果是不能读的文件夹就log错误
			err_ret("can not read directory %s", pathname);
			break;
		case FTW_NS:
			//如果是不能读取权限的就log
			err_ret("sata error for %s", pathname);
			break;
		default:
			//其他情况都打log
			err_dump("unknow type %d for pathname %s", type, pathname);
	}

	return 0;
}






