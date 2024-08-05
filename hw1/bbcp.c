#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BBCP_EXIT_ERROR 1
#define BBCP_SUCCESS 0
#define BUFFER_SIZE 32768

void copy_to_dest (unsigned int fds, unsigned int fdt){

	ssize_t s;
	char buf[BUFFER_SIZE];
	while ((s = read(fds, buf, BUFFER_SIZE)) > 0) {
		if (write(fdt, buf, s) != s) {
			fprintf(stderr, "Unable to write: %s\n",
                                        strerror(errno));
                        exit(BBCP_EXIT_ERROR);
                }
        } 
	if (s == -1) {
        	fprintf(stderr, "Error reading %s\n",
                                 strerror(errno));
                exit(BBCP_EXIT_ERROR);
        }
}

int main (int argc, char ** args){

	if (argc != 3){
		fprintf(stderr,"require 2 arguments\n");
		exit(BBCP_EXIT_ERROR);
	}
	
	if(strcmp(args[1],args[2]) == 0){			
		fprintf(stderr,"file %s already exists\n",args[1]);
                exit(BBCP_EXIT_ERROR);
	}

	struct stat statfs, statft;
	int fds , fdt ;
	char* src = args[1];
	char* target = args[2];
	char* srcname;
			
	if((srcname = rindex(src,'/')) != NULL){
		srcname += 1;
		printf("FILE NAME: %s\n",srcname);
	}else{
		srcname =  src;
	}

	if (strcmp(target, ".") == 0){
		target = strcat(target,"/");
		target = strcat(target,srcname);
	}

	if (stat(src,&statfs) == -1 ){
		fprintf(stderr, "Error get status of '%s' : %s\n",
				src, strerror(errno));
		exit(BBCP_EXIT_ERROR);
	}

	if (stat(target,&statft) == 0 ){
		if(statfs.st_ino == statft.st_ino){
			fprintf(stderr, "2 files are the sames\n");
			exit(BBCP_EXIT_ERROR);
		}
	}

	if ((fds = open(src,O_RDONLY)) == -1){
		fprintf(stderr, "Unable to open '%s': %s\n",
                                src, strerror(errno));
	 	exit(BBCP_EXIT_ERROR);
	}
	
	if ((statft.st_mode & S_IFMT) == S_IFDIR){
		printf("TARGET is DIR\n");
		target= strcat(target,src);
	}else{
		printf("TARGET is FILE\n");
	}

	if ((fdt = openat(AT_FDCWD,target,O_CREAT|O_WRONLY|O_TRUNC,statfs.st_mode)) == -1){
		fprintf(stderr, "Unable to open '%s': %s\n",
                                target, strerror(errno));
	 	exit(BBCP_EXIT_ERROR);
	}

	copy_to_dest(fds,fdt);
	return BBCP_SUCCESS;
}
