#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_LEN_LINE    10
#define LEN_HOSTNAME	30

int main(void)
{
    char command[MAX_LEN_LINE];
    char *args[] = {command, NULL};
    int ret, status;
    pid_t pid, cpid;

    char hostname[LEN_HOSTNAME+1] = {0x00,};
    gethostname(hostname, LEN_HOSTNAME);
    
    while (true) {
        char *s;
        int len;
        
    printf("MyShell $ ");

	/* 3. username@hostname$ 형식의 shell prompt. username과 hostname 얻기*/
	printf("%s@%s $ ",getpwuid(getuid())->pw_name,hostname);
        s = fgets(command, MAX_LEN_LINE, stdin);
        if (s == NULL) {
            fprintf(stderr, "fgets failed\n");
            exit(1);
        }
        
        len = strlen(command);
        printf("%d\n", len);
        if (command[len - 1] == '\n') {
            command[len - 1] = '\0'; 
        }

	/* 1. exit 입력시 shell 종료 */
        if(!strcmp("exit",command)){
		printf("exit!");
		return 0;
	}
        printf("[%s]\n", command);

	/* 2. a;b;c와 같이 입력시 a,b,c세개의 프로그램 차례대로 실행*/
       	char *p = strtok(command, ";");
	while(p != NULL){
		printf("%s \n",p);
		p = strtok(NULL, ";");
	}

        pid = fork();
        if (pid < 0) {
            fprintf(stderr, "fork failed\n");
            exit(1);
        } 
        if (pid != 0) {  /* parent */
            cpid = waitpid(pid, &status, 0);
            if (cpid != pid) {
                fprintf(stderr, "waitpid failed\n");        
            }
            printf("Child process terminated\n");
            if (WIFEXITED(status)) {
                printf("Exit status is %d\n", WEXITSTATUS(status)); 
            }
        }
        else {  /* child */
            ret = execve(args[0], args, NULL);
            if (ret < 0) {
                fprintf(stderr, "execve failed\n");   
                return 1;
            }
        } 
    }
    return 0;
}
