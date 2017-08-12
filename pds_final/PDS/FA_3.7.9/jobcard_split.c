#include<stdio.h>
#include "types.h"
#include<curl/curl.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>

int gp_test();
int jobcards_parsing();
void hide_progress_frame();
void show_progress_frame();

char pid[20],query[1024],query_delete[1024];
char *exec_query_get_single_value(char *data, char *query, char *database);

int serv_response(char *Url)
{
	CURL *curl;
	FILE *fp_serv;
	CURLcode res;
	char errmsg[256];

	fp_serv = fopen("/root/jobcards_response.txt", "w");

	curl=curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl,CURLOPT_URL,Url);
		curl_easy_setopt(curl,CURLOPT_FILE, fp_serv);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);
		fclose(fp_serv);     
		curl_easy_cleanup(curl);
		if(res==0)
		{
			return 0;
		}
		// Exception is implemented as per suggestions
		else
		{
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_ERROR, "ALERT",errmsg);
			hide_progress_frame();			
			return -1;
		}

	}
	return(0);
}

int count;
int response_gprs(int count)
{

	char Url1[100]="",final_value[32]="";
	char buff[100]="";
	FILE *ffp2=NULL;

	int ret1;
	sprintf(Url1,"%s",SPLITJC_UPLOAD);
	//	sprintf(Url1,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/UpdateSplitJobCardInfo.aspx?Code=");

//	sprintf(Url1,"%s","http://nrega.ap.gov.in:9090/eMMS/UpdateSplitJobCardInfo.aspx?Code=");
	strcat(Url1,pid);

	sprintf(final_value,"%sRecCount=%d","&",count);
	strcat(Url1,final_value);
	printf("\n Final _url is %s\n",Url1);
	ret1=serv_response(Url1);

	if(ret1==0)
	{
		hide_progress_frame();

		ffp2=fopen("/root/jobcards_response.txt","r");

		buff[0]='\0';

		while(fgets(buff,100,ffp2))
		{
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_WARNING, "ALERT",buff);
			fclose(ffp2);
			return 0;
		}
	}

	return 0;
}

int down_file_server_jobcards(char *Url)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	fp = fopen("/root/jobcards.txt", "w");

	curl=curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl,CURLOPT_URL,Url);
		curl_easy_setopt(curl,CURLOPT_FILE, fp);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);
		fclose(fp);     
		curl_easy_cleanup(curl);

		if(res==0)
		{
			return 0;
		}

		else
		{	
			hide_progress_frame();

			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_ERROR, "ERROR",errmsg);
			return -1;
		}
	}
	return(0);
}
int gprs_jobcard_split()
{
	char Url[100];
	Url[0]='\0';
	char *p_id=NULL;

	char *count_job_prev=NULL,*count_job_now=NULL;
	char bufftext[100];
	FILE *ffp_down_job;

	char fname3[256];

	int imgFD3=0, fsz3=0,count_job,res_job_now,res_job_prev;

	struct stat fst3;

	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info;");

	p_id = exec_query_get_single_value(p_id, query, DB_PACK);

	sprintf(pid,"%s",p_id);
	sprintf(Url,"%s",SPLITJC_DOWNLD);
	//	sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/SplitJobCardInfo.aspx?Code=");

//	sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS/SplitJobCardInfo.aspx?Code=");

	show_progress_frame(" Downloading Please Wait ");

	strcat(Url,p_id);
	printf("\n Url is %s\n",Url);

	ret=down_file_server_jobcards(Url);
	if(ret==0)
	{
		ffp_down_job=fopen("/root/jobcards.txt","r");

		bufftext[0]='\0';

		while(fgets(bufftext,100,ffp_down_job))
		{
			if((strcmp(bufftext,"NoData")==0))
			{
				hide_progress_frame();				
				remove("/root/jobcards.txt");

				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_WARNING, "ALERT","NO DATA FOUND FOR JOBCARDS IN THE SERVER");		
				return -1;
			}
			else
			{
				sqlite3 *dbhandle=NULL;
				sqlite3_open(DB_PACK, &dbhandle);
				if (dbhandle == NULL)
				{
					printf("Unable to open db\n");
				}

				sprintf(query,"create table IF NOT EXISTS splitted_jobcards(new_jobcard varchar(20),new_worker_code varchar(2),old_jobcard varchar(20),old_worker_code varchar(20),download_date varchar(15),status char(2))");
				//				printf("\n query create is %s\n",query);

				sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

				sprintf(query,"select count(*) from splitted_jobcards");

				count_job_prev = exec_query_get_single_value(count_job_prev, query, DB_PACK);

				//				printf("\n count_job_prev = %s\n",count_job_prev);

				system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/jobcards.txt splitted_jobcards\" 1 >/tmp/out 2> /root/job_error.txt");

				sprintf(query,"select count(*) from splitted_jobcards");

				count_job_now = exec_query_get_single_value(count_job_now, query, DB_PACK);

				//				printf("\n count_job_now = %s\n",count_job_now);

				sprintf(query_delete,"delete from splitted_jobcards where rowid not in(select max(rowid) from splitted_jobcards group by new_jobcard,new_worker_code,old_jobcard,old_worker_code )");

				sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);

				//				printf("query_delete  is %s\n", query_delete);

				sprintf(fname3, "/root/job_error.txt");
				imgFD3 = open(fname3, O_RDONLY);
				fstat(imgFD3, &fst3);
				fsz3 = fst3.st_size;
				//				printf("the size fsz3 = %d \n ", fsz3);
				if(fsz3 > 0)
				{	
					hide_progress_frame();
					//					printf("\n problem with the text file\n");
					pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
							PW_MSGBOX_ICON_ERROR, "ERROR","NREGS DATA MISMATCH");

					return -1;
				}
				res_job_now=atoi(count_job_now);
				res_job_prev=atoi(count_job_prev);
				count_job = res_job_now - res_job_prev;
				//				printf("\n count_job = %d\n",count_job);
				response_gprs(count_job);
				return 0;
			}
		}
	}
	return (0);
}

int get_splitjc_dwnld()
{
	int res;
	ret= gp_test();
	if(ret<0)
	{
		show_message_frame("NOT CONNECTED TRY AGAIN");
		return -1;
	}
	else
	{
		ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
				PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
		if(ret==PW_MSGBOX_RETURN_OK)
			res=gprs_jobcard_split();
		else
			return 0;
	}
	return 0;
}
