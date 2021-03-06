
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/prctl.h> 
#include <sys/wait.h>
#include <syslog.h>

// volatile int status;
void handle_zombies(){
    int status = -1;
    pid_t pid = waitpid(-1, &status, 0);
    printf("Received SIGCHILD with pid %d, status: %d\n", pid, status);
}

int main(int argc, char const *argv[])
{
	prctl(PR_SET_CHILD_SUBREAPER);
    printf("I am reaper, my pid is %d, my dad is %d\n", getpid(), getppid());
    pid_t parent = fork();
    if (parent == 0){
    	pid_t child = fork();
    	if(child > 0){
    		printf("I am parent, my pid is %d\n", getpid());
    		sleep(3);
    		return 20;
    	} 
    		printf("I am the child. my pid is %d\n", getpid());
	    	printf("My dad is %d\n", getppid());
    		sleep(10);
    		// printf("My new dad is %d\n", getppid());
	    	return 13;
    	

    }
    else{
    	int status;
        waitpid(parent, &status, 0);
        printf("Reaped process %d\n", WEXITSTATUS(status));
        // signal(SIGCHLD, handle_zombies);


        // getchar();
        // waitpid(parent, &status, 0);
        getchar();
        wait(&status);

        // waitid(P_PGID, -1, NULL, WNOWAIT)
    	printf("Reaped process %d\n", WEXITSTATUS(status));
		getchar();
    }
	return 0;
}