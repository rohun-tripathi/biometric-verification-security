#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "types.h"
#include <autils.h>
char *exec_query_get_single_value(char *data, char *query, char *database);
void show_progress_frame();

void hide_progress_frame();
char *get_Gp_code();
int down_file_server_batch_info(char *Url_day)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	fp = fopen("/root/batch_info.txt", "w");

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
char hour[2],min[2],sec[2],day[2],month[2],year[4];
int get_server_time(void *parg, int argc, char **argv, char **colnames)
{
	hour[0]='\0';
	min[0]='\0';
	sec[0]='\0';
	day[0]='\0';
	month[0]='\0';
	year[0]='\0';
	strcpy(hour, argv[0]);
	strcpy(min, argv[1]);
	strcpy(sec, argv[2]);
	strcpy(day, argv[3]);
	strcpy(month, argv[4]);
	strcpy(year, argv[5]);
	return 0;
}
int batch_info_dwn()
{
	char Url_batch[100]="";
	char fname1[256],batch_buff[100],query_delete[512],query_select[512];
	int imgFD1=0, fsz1=0;
	FILE *ffp_dwn_batch=NULL;
	struct stat fst1;	
	sprintf(Url_batch,"%s",BATCHINFO_DOWN);
	//	sprintf(Url_batch,"%s","http://nrega.ap.gov.in:9090/eMMS_test/BatchCategoryDetails.aspx?Code");
	//	sprintf(Url_batch,"%s","http://nrega.ap.gov.in:9090/eMMS/BatchCategoryDetails.aspx?Code");
	show_progress_frame(" Downloading Please Wait ");

	printf("\n get_Gp_code is %s\n",get_Gp_code());
	strcat(Url_batch,get_Gp_code());
	printf("\n Url_batch is %s\n",Url_batch);

	ret=down_file_server_batch_info(Url_batch);
	printf("\n ret value is %d\n",ret);

	if(ret==0)
	{
		ffp_dwn_batch=fopen("/root/batch_info.txt","r");
		batch_buff[0]='\0';
		while(fgets(batch_buff,100,ffp_dwn_batch))
		{
			if((strcmp(batch_buff,"NoData")==0))
			{
				hide_progress_frame();

				remove("/root/batch_info.txt");
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","NO DATA FOUND FOR BATCH INFO ");
				return -1;
			}
		}
		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/batch_info.txt Batch_info\" 1 >/tmp/out 2> /root/batch_error.txt");
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}
		//sprintf(query_delete,"delete from Batch_info where version_no<(select max(version_no) from Batch_info)");
		sprintf(query_delete,"delete from Batch_info where created_date!=(select max(created_date) from Batch_info)");
		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		
		sprintf(query_select,"select substr(created_date,12,2),substr(created_date,15,2),substr(created_date,18,2),substr(created_date,9,2),substr(created_date,6,2),substr(created_date,1,4) from  Batch_info where created_date=(select max(created_date) from Batch_info)");		
		sqlite3_exec(dbhandle, query_select, get_server_time, NULL, NULL);
		au_set_time(atoi(hour),atoi(min),atoi(sec),atoi(day),atoi(month),atoi(year));
		hide_progress_frame();
		sprintf(fname1,"/root/batch_error.txt");
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
		hide_progress_frame();
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS","Batch Info Downloaded Successfully");
		//response_wage(comp_count1);
		sqlite3_close(dbhandle);
		return 0;
	}
	return (0);
}
