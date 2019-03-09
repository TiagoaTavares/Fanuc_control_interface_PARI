/**
 * @file main.c
 * @author Tiago Tavares (tiagoatavares@ua.pt)
 * @brief Main file, wich makes the fork to obtain the 2 needed processes. (SON AND FATHER). 
 * @version 0.1
 * @date 2019-01-27
 * 
 * @copyright Copyright (c) 2019
 * 
 */


#define _MAIN_C_

#include "myf.h"

int main(int argc, char *argv[])
{
    int pid = fork();
    int s_id;
    if (pid == -1)
    {
        printf("Could not fork(). Exiting\n");
        return -1;
    }

    if (pid == 0) /* The child */
    {
        
        MessageFromChild((char *)"I am the child\n");
        ChildMain3(argc,argv);
        //printf("My pid:%d\n", getpid());
        //printf("pid_filho=%d\n",pid);
        char string[15];
        sprintf(string,"kill -1 %d",getpid());
        system(string);
        MessageFromChild((char *) "CHILD ended\n");
    }
    else /* The parent */
    {
        MessageFromParent((char *) "I am the parent\n");
        s_id = ParentMain3(argc,argv); //Get s_id from return value to know the id to destroy the shared mem
        if (s_id > 0)
            shmctl(s_id, IPC_RMID, NULL); //Allow elimination of shared memory
        //if you comment the previous line the shared memory is NOT cleared from system!

        char string[15];
        sprintf(string,"kill -1 %d",getpid());
        system(string);

        MessageFromParent((char *) "PARENT ended\n");
    }

    return 0;
}
