#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>
#include "types.h"

void show_progress_frame();
void hide_progress_frame();
void show_frame_gprs_page();

char *exec_query_get_single_value(char *data, char *query, char *database);
char query[1024],upload_buff[100];

char n_job[20],n_wrk[5],old_jobcard[20],old_worker_code[5];

int find_bir(char *o_jobcard,char *o_wrker_code,char *n_jobcard,char *n_wrker_code)
{
	char file_path[256];
	for(i=1; i<10; i++)
	{
		sprintf(file_path, "mv /home/Backup/%s%s_%d.bir /home/Backup/%s%s_%d.bir ",o_jobcard,o_wrker_code,i,n_jobcard,n_wrker_code,i);
		//		printf("\n file_path is %s\n",file_path);
		system(file_path);
	}
	return 0;
}

int get_job_callback(void *parg, int argc, char **argv, char **colnames)
{
	n_job[0]='\0';
	n_wrk[0]='\0';
	old_jobcard[0]='\0';
	old_worker_code[0]='\0';
	strcpy(n_job,argv[0]);
	//	printf("\n n job is %s\n",n_job);
	strcpy(n_wrk,argv[1]);
	//	printf("\n work is %s\n",n_wrk);
	strcpy(old_jobcard,argv[2]);
	strcpy(old_worker_code,argv[3]);
	return(0);
}

int get_splitjc_update()
{
	char *job_count=NULL,*rowid_split=NULL;

	char *old_job_count=NULL,*new_job_count=NULL,*status_y=NULL;
	int k;
	char query1[1024];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query,"select ifnull(count(*),0) from splitted_jobcards where status='N' and download_date in(select distinct max(download_date) from splitted_jobcards)");

	job_count = exec_query_get_single_value(job_count, query, DB_PACK);

	if(atoi(job_count)==0)
	{
		show_progress_frame("No Records To Update");	
		sleep(2);
		hide_progress_frame();	
		return(0);
	}
	for(k=0; k<atoi(job_count); k++)
	{
		sprintf(query,"select (max(rowid)-%d) from splitted_jobcards where status='N'",k);
		rowid_split =exec_query_get_single_value(rowid_split,query,DB_PACK);
		printf("\n row id=%s\n",rowid_split);
		int up_rowid_split= atoi(rowid_split);
		if(up_rowid_split==0)
		{
			up_rowid_split=1;
		}

		sprintf(query1,"select new_jobcard,new_worker_code,old_jobcard,old_worker_code from splitted_jobcards where status='N' and rowid=%d and download_date in(select distinct max(download_date) from splitted_jobcards) limit 1",up_rowid_split);
		printf("query1 is %s\n",query1);
		sqlite3_exec(dbhandle, query1, get_job_callback, NULL, NULL);
		sprintf(upload_buff,"Updating Please Wait ");
		show_progress_frame(upload_buff);
		sprintf(query,"select ifnull(count(*),0) from household_details where household_code='%s' and worker_code='%s'",old_jobcard,old_worker_code);
		old_job_count = exec_query_get_single_value(old_job_count, query, DB_PACK);
		if(atoi(old_job_count)==0)	
		{
			sprintf(query,"update splitted_jobcards set status='NO' where old_jobcard='%s' and old_worker_code='%s'",old_jobcard,old_worker_code);
			sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
			printf("\n query is %s\n",query);
		}
		else
		{
			sprintf(query,"select ifnull(count(*),0) from household_details where household_code='%s' and worker_code='%s'",n_job,n_wrk);
			new_job_count = exec_query_get_single_value(new_job_count, query, DB_PACK);
			if(atoi(new_job_count)>0)
			{
				sprintf(query,"update household_details set is_active='N' where household_code='%s' and worker_code='%s'",old_jobcard,old_worker_code);
				printf("\n query is %s\n",query);	
				sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
				find_bir(old_jobcard,old_worker_code,n_job,n_wrk);

				sprintf(query,"update splitted_jobcards set status='Y' where old_jobcard='%s' and old_worker_code='%s'",old_jobcard,old_worker_code);
				sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
				printf("\n query is %s\n",query);
			}
		}
	}
	sprintf(query,"select ifnull(count(*),0) from splitted_jobcards where status='Y'");
	status_y = exec_query_get_single_value(status_y, query, DB_PACK);
	if(atoi(status_y)>0)
	{
		show_message_frame("Updated Successfully");	
		hide_progress_frame();
		sqlite3_close(dbhandle);
		return 0;
	}
	else
	{
		show_message_frame("NO Jobcards Matched\n With HH Details");
		hide_progress_frame();
		sqlite3_close(dbhandle);
		return 0;
	}
	return(0);
}
