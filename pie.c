/* Joshua Lavieri
   Problem 2
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define READ_END 0
#define WRITE_END 1

int n;
int t;

double computePartialSum(int n, int t, int j) {
	//Variables setup
	int iterations = n / t; //used to measue range of values to sum
	int range = (j+1) * iterations;
	int i = range - iterations + 1;
	double partialSum = 0;
	if (j == 0) partialSum = 3.0;
	
	double k;
	for( i; i <= range; i++) {
		if(i % 2 == 0) k = -1;
		else k = 1;
        
		partialSum = partialSum + k*(4/((2*(double)i)*(2*(double)i+1)*(2*(double)i+2)));
		
	}
	return partialSum;

}


int main (int argc, char * argv[]) {
	//Setup variables
	n = atoi(argv[1]);
	t = atoi(argv[2]);
	int fd_1[n][2];
	int fd_2[n][2];
	double sum = 0;
	
	for (int j = 0; j < t; j++) {
		pipe(fd_1[j]);
		pipe(fd_2[j]);
		pid_t pid = fork();
		if (pid == 0) { //Child
		
			//Reading n, j , t into an array
			int arr[3];
			read(fd_1[j][READ_END], arr, sizeof(arr));
			
			//
			double partialSum = computePartialSum(arr[0], arr[1], arr[2]);
			//Getting partialSum value, storing in array and writing to the second pipe for parent to read
			double partialSumArray[1] = {partialSum};
			write(fd_2[j][WRITE_END], partialSumArray, sizeof(partialSumArray));
			exit(0);
		} else { //Parent
			
			//Writes n, j, t to child
			int arr[3] = {n, t, j};
			write(fd_1[j][WRITE_END], arr, sizeof(arr));
		
		}
		waitpid(pid, NULL, 0);
		
	}
	
	
	for (int j = 0; j < t; j++) { //Parent
		//Reading partial sum and adding it to main sum
		double partialSumArray[1];
		read(fd_2[j][READ_END], partialSumArray, sizeof(partialSumArray));
		sum = sum + partialSumArray[0];
		while(wait(NULL) > 0);
	}
	
	printf("This approximation of pi with N=%d and T=%d processes is %2.8f\n", sum, n, t);
}
