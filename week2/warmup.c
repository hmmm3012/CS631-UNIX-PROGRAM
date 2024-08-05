#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


int openNewFile(char* pathname){
	int fd = open(pathname,O_RDWR);
	if (fd < 0){
		printf("cant open file");
		return -1;
	}
	return fd;
}


int main(int argc, char** argv){
	char bufread [10];
	char buf[]="hello test";
	int fdin = stdin->_fileno;
	int fout = stdout->_fileno;
	int ferr = stderr->_fileno;
	printf("STDIN_FILENO: %d, STDOUT_FILENO: %d, STDERR_FILENO: %d\n",STDIN_FILENO,STDOUT_FILENO,STDERR_FILENO);
	printf("File discriptor stdin: %d, stdout: %d, stderr: %d\n", fdin,fout,ferr); 
	int fd = openNewFile("./newfile");
	printf("FD: %d\n",fd);
	//FILE* fp= fopen("./newfile","w");
	//int ffd = fp->_fileno;
	//printf("FFD: %d\n",ffd);
	//write (fd, buf , 3);
	//write (ffd ,buf,5);
	int readbyte = -1;
	readbyte= read(fd,bufread,2);
	if(readbyte < 0){
		printf("cant read file \n");
	}
	printf("Read bytes: %d\n",readbyte);
	bufread[readbyte] = '\0';
	printf("Read data: %s\n",bufread);
	write (fd, buf, 4);
	perror("hello error");
}
