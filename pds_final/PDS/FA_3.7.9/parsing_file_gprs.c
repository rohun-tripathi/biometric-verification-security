#include<stdio.h>
#include "types.h"

struct sample
{
	char group_id[20];
	char work_code[20];
	char mandays_allocated[20];
};

int getread_groups(int *length,char *);
int parse_groups(char *data);

FILE *fp_pp=NULL;


int groups_textsql(void)
{
	int length=0,res=0;
        char data[1024];
	
	
	fp_pp=fopen("/root/groups_allocation.txt","w");
                                                                                                                             
        if(fp_pp==NULL)
        {
                printf("file not found\n");
                return -1;
        }
                                                                                                                             
        printf("Parsing started.....\n");
        while(!feof(fp_pp))
        {
                memset(data,0,sizeof(data));
                getread_groups(&length,data);
                if(length<0)
                        break;
                else
                      	res=parse_groups(data);
        }
        fclose(fp_pp);
	
}
                                                                                                                             
int getread_groups(int *length,char *data)
{
                                                                                                                             
    char *line=NULL;
    size_t len=0;
    int read;
                                                                                                                             
    read=getline(&line,&len,fp_pp);
    *length=read;
   strcpy(data,line);
    printf("line in getread :  %s\n",data);
    return 0;
}
                                                                                                                             
int parse_groups(char *data)
{
        char *p;
        struct sample s;
        int length=0;

    	char query[1024];
         p=(char *)strchr(data,'+');
        if(p==NULL)
        {
                printf("error in gropu id");
                return -1;
        }
                                                                                                                             
        length=p-data;
        strncpy(s.group_id,data,length);
        s.group_id[length]='\0';
        data=data+length+1;
	printf("\n group id is: %s \n", s.group_id);

        p=(char *)strchr(data,'+');
        if(p==NULL)
        {
                printf("error in work_code");
                return -1;
        } 
	length=p-data;
        strncpy(s.work_code,data,length);
        s.work_code[length]='\0';
        data=data+length+1;   
	printf("\n work_code: %s\n", s.work_code);
     
	p=(char *)strchr(data,'+');
        if(p==NULL && strlen(data)==0)
        {
                printf("error in mandays_allocated");
                return -1;
        }
        strcpy(s.mandays_allocated,data);
        s.mandays_allocated[strlen(data)] = '\0';

	printf("\n mandays_allocated is: %s\n", s.mandays_allocated);         

	 printf("\n %s %s %s \n",s.group_id,s.work_code,s.mandays_allocated);

	sqlite3 *dbhandle=NULL; 

	 i = sqlite3_open(DB_PACK, &dbhandle);
	 if (dbhandle == NULL)
	 {
		 printf("Unable to open db\n");
	 }

                                                                                                                            
       sprintf(query,"INSERT INTO groups_allocation (group_id,work_code,Mandays_allocated) VALUES ('%s','%s','%s');",s.group_id,s.work_code,s.mandays_allocated);
		
	 printf("query is %s\n",query);

        sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);
	return 0;

}
