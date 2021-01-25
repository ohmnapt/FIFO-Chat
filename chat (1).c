#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_1 "./fifo1to2"
#define FIFO_2 "./fifo2to1"

//นายณภัฒร์ สุวรรณศร
// 6113187 EGCO


int FIFO_FD1, FIFO_FD2;

int main(int argc, char *argv[])
{
	int child;
	char buf1[80],buf2[80]; //bufเพื่อเก็บtextของแต่ละUser
	
	if(argc !=2) //check argc
	{
		fprintf(stderr,"Please Use: %s <[1,2]>\n",*argv);
		exit(EXIT_FAILURE);
	}

	if(access(FIFO_1, F_OK) == -1)
	{
		FIFO_FD1 = mkfifo(FIFO_1, 0777); //สร้างFIFO เซตPermission rwx
		if(FIFO_FD1)
		{
			fprintf(stderr, "Could not create fifo %s\n",FIFO_1);
			exit(EXIT_FAILURE);
		}
	}
	if(access(FIFO_2, F_OK) == -1)
    {
        FIFO_FD2 = mkfifo(FIFO_2, 0777); //สร้างFIFO เซตPermission rwx
        if(FIFO_FD2)
        {
            fprintf(stderr, "Could not create fifo %s\n",FIFO_2);
            exit(EXIT_FAILURE);
        }
    }

        //-----------------------------Part ประมวลผลข้อความ------------------------------//

	FIFO_FD1 = open(FIFO_1, O_RDWR);
	FIFO_FD2 = open(FIFO_2, O_RDWR);

	argv++;

	if(strcmp(*argv, "1") == 0) //User ที่1
	{
		child = fork();
		switch (child)
		{
			case -1 : perror("Forking Process Failed"); exit(EXIT_FAILURE);
			case 0 : while(strncmp(buf1,"end chat", 8)) //ให้child อ่านค่าจากท่อ2 และแสดงผลออกterminal
				{
					read(FIFO_FD2, buf1, 80);
					printf("User 2: %s",buf1);
				}
				break;
			default : while(strncmp(buf2,"end chat", 8)) //Parent รับค่าจากkeyboardส่งเข้าท่อที่1
                {
					fgets(buf2, 80, stdin);
                    write(FIFO_FD1, buf2, strlen(buf2)+1);                                      
                }
		}
	}
	
	if(strcmp(*argv,"2") == 0) //User ที่2
    {
        child = fork();
        switch (child)
        {
            case -1 : perror("Forking Process Failed"); exit(EXIT_FAILURE);
            case 0 : while(strncmp(buf1,"end chat", 8)) //ให้child อ่านค่าจากท่อ1 และแสดงผลออกterminal
                {
                    read(FIFO_FD1, buf1, 80);
					printf("User 1: %s",buf1);
                }
				break;
            default : while(strncmp(buf2,"end chat", 8)) //ให้Parentรับค่าจากkeyboard แล้วส่งเข้าท่อที่2
                {
					fgets(buf2, 80, stdin);
                    write(FIFO_FD2, buf2, strlen(buf2)+1);
                }
        }
    }
		
      
	if(FIFO_FD1 != -1) close(FIFO_FD1);
	if(FIFO_FD2 != -1) close(FIFO_FD2);
	
	exit(EXIT_SUCCESS);
                                                                  
}