#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>

const int MAXLINE = 4096;

/* This program handles pipes and redirection */

int main() {
    int n;
    int fd[2];
    int stat_val;
    pid_t pid;
    char line[MAXLINE];
    
    /* Before calling fork we create the pipe */
    if (pipe(fd) < 0)
        perror("pipe error");
    
    int readflags = fcntl(fd[0], F_GETFD, 0);
    int writeflags = fcntl(fd[1], F_GETFD, 0);
       
    readflags |= FD_CLOEXEC;
    writeflags |= FD_CLOEXEC;
    fcntl(fd[0], F_SETFD, readflags);
    fcntl(fd[1], F_SETFD, writeflags);
    
    /* We fork the child */
    if ((pid=fork()) < 0)
        perror("fork error");
    
    if (pid == 0) {
        close(fd[0]); /* Close the read end */
        dup2(fd[1], STDOUT_FILENO);
        execl("/bin/ps","ps","auxw",(char *)0);
        exit(0);
    }
    
    if ((pid=fork()) < 0)
        perror("fork error");
        
    if (pid == 0) {
        close(fd[1]); /* Close the write end */
        if (fd[0] != STDIN_FILENO) {
            if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
                perror("dup2 error to stdin");
            close(fd[0]);
        }
        while((n=read(STDIN_FILENO, line, MAXLINE)) > 0)
            write(STDOUT_FILENO, line, n);
        execl("/bin/wc","wc","-l",(char *)0);
        
        exit(0);
    }
    
    wait(&stat_val);
    
    //execl("/bin/ps","ps","-p",,(char *)0); 
    return 0;
}
