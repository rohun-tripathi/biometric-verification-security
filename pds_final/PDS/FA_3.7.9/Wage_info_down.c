#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "types.h"

char *exec_query_get_single_value(char *data, char *query, char *database);
void show_progress_frame();
int response_wage();
void hide_progress_frame();
char *get_Gp_code();
int down_file_server_wage_info(char *Url_day)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	fp = fopen("/root/wage_info.txt", "w");

	curl=curl_easy_init();
	if(curl)
	{
		printf("\n coming to get_page_up \n");
		curl_easy_setopt(curl,CURLOPT_URL,Url_day);
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
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK, PW_MSGBOX_ICON_ERROR, "ERROR",errmsg);
			return -1;
		}
	}
	return(0);
}
int Wage_info_down()
{
	char Url_days[100]="";
	char *count_wage_prev=NULL,*count_wage_now=NULL;
	char fname1[256],buff_dys[100],query[1024],query_delete[512];
	int imgFD1=0, fsz1=0,comp_count1,res_wage_now,res_wage_prev;
	FILE *ffp_dwn_days=NULL;
	struct stat fst1;	
	sprintf(Url_days,"%s",WAGEINFO_DOWN);
//	sprintf(Url_days,"%s","http://nrega.ap.gov.in:9090/eMMS/DownloadedDays.aspx?Code=");
	//	sprintf(Url_days,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/DownloadedDays.aspx?Code=");
	show_progress_frame(" Downloading Please Wait ");

	printf("\n get_Gp_code is %s\n",get_Gp_code());
	strcat(Url_days,get_Gp_code());
	printf("\n Url_days is %s\n",Url_days);

	ret=down_file_server_wage_info(Url_days);
	printf("\n ret value is %d\n",ret);

	if(ret==0)
	{
		ffp_dwn_days=fopen("/root/wage_info.txt","r");
		buff_dys[0]='\0';
		while(fgets(buff_dys,100,ffp_dwn_days))
		{
			if((strcmp(buff_dys,"NoData")==0))
			{
				hide_progress_frame();

				remove("/root/wage_info.txt");
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","NO DATA FOUND FOR WAGE INFO ");
				return -1;
			}
		}
		sprintf(query,"select count(*) from Wage_Info");
		count_wage_prev = exec_query_get_single_value(count_wage_prev, query, DB_PACK);

		printf("\n count_wage_prev = %s\n",count_wage_prev);
		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/wage_info.txt Wage_Info\" 1 >/tmp/out 2> /root/wage_error.txt");
		sprintf(query,"select count(*) from Wage_Info");

		count_wage_now = exec_query_get_single_value(count_wage_now, query, DB_PACK);
		printf("\n count_wage_now = %s\n",count_wage_now);
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query_delete,"delete from Wage_Info where rowid not in(select max(rowid) from Wage_Info group by jobcard_no)");
		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(fname1,"/root/wage_error.txt");
		imgFD1 = open(fname1, O_RDONLY);
		fstat(imgFD1, &fst1);
		fsz1 = fst1.st_size;
		printf("the size fsz = %d \n ", fsz1);
		if(fsz1 > 0)
		{
			hide_progress_frame();
			printf("\n problem with the text file\n");
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
					PW_MSGBOX_ICON_ERROR, "ALERT","NREGS DATA MISMATCH");
			return -1;
		}
		res_wage_now=atoi(count_wage_now);
		res_wage_prev=atoi(count_wage_prev);
		comp_count1 = res_wage_now - res_wage_prev;
		 hide_progress_frame();

		printf("\n comp_count = %d\n",comp_count1);
		//pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS","Wage Info Downloaded Successfully");
		free(count_wage_prev);
		free(count_wage_now);
		response_wage(comp_count1);
		sqlite3_close(dbhandle);
		return 0;
	}
	return (0);
}
