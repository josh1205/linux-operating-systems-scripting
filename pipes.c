/*
    Joshua Lavieri
    Problem 1
*/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#define READ_END 0
#define WRITE_END 1


int main(int argc, char * argv[]) {

	int fd[2], status, done=0;
	pid_t pid;
	pipe(fd);
	/*create the pipe*/
	if (pipe(fd) == -1) {
		fprintf(stderr, "Pipe failed");
		return 1;
	}
	
	/* Initialize system call */
	pid = fork();
	
	/*Error occured*/
	if (pid < 0 ) {
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	/*Left Child*/
	if (pid == 0) {
		/*Close the unused end of the pipe*/
		close(fd[READ_END]);
		
		/*Get Input*/
		dup2(fd[1], STDOUT_FILENO);
		
		/*Execute left command*/
		execlp("ps", "ps", "-aux", (char *) NULL);
		
		/* close the write end of the pipe*/
		close(fd[WRITE_END]);
		
	}
	
	/* Initialize system call */
	pid = fork();
	
	/*Error occured*/
	if (pid < 0 ) {
		fprintf(stderr, "Fork Failed");
		return 1;
	}
	/*Right Child*/
	if (pid == 0){
		/*close the unused end of the pipe*/
		close(fd[WRITE_END]);
		
		/*Get Input*/
		dup2(fd[0], STDIN_FILENO);
		
		/*Execute right command*/
		execlp("sort", "sort", "-r", "-n", "-k", "5", (char *) NULL);
	}
	/*close the read and write end of the pipe*/
	close(fd[READ_END]);
	close(fd[WRITE_END]);
	
	

}
