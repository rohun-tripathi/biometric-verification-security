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
void hide_progress_frame();
char *get_device_no();

char *get_Gp_code();

char query[1024];
char query_delete[512];
int down_file_server_mccwrkalloc_info(char *Url_day)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];
				
	remove("/root/mcc_wrks_alloc_info.txt");

	fp = fopen("/root/mcc_wrks_alloc_info.txt", "w");

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
int mcc_workalloc_details_down()
{
	char Url_mccwrks[1024]="";
	char *count_mccwrks_prev=NULL,*count_wage_now=NULL;
	char buff_mccwrk_alloc[100];
	FILE *ffp_dwn_mcc_wrksalloc=NULL;
	char fname1[256];
	int imgFD1=0, fsz1=0,mcc_wrkalloc_cnt,res_mcc_wrkalloc_now,res_wage_prev;
	struct stat fst1;	
	sprintf(Url_mccwrks,"%s",MCC_DEMAND_WID);
//	sprintf(Url_mccwrks,"%s","http://nrega.ap.gov.in:9090/eMMS/DownloadDemandAllocationDetails.aspx?Code=");
	show_progress_frame("Downloading Allocations\n  Plz Wait");
	printf("mcc work allocation details is %s\n",Url_mccwrks);
	printf("\n get_Gp_code is %s\n",get_Gp_code());
	strcat(Url_mccwrks,get_Gp_code());
	printf("\n Url_mccwrks is %s\n",Url_mccwrks);
	ret=down_file_server_mccwrkalloc_info(Url_mccwrks);
	printf("\n ret value is %d\n",ret);
	if(ret==0)
	{
		ffp_dwn_mcc_wrksalloc=fopen("/root/mcc_wrks_alloc_info.txt","r");
		buff_mccwrk_alloc[0]='\0';
		while(fgets(buff_mccwrk_alloc,100,ffp_dwn_mcc_wrksalloc))
		{
			if((strcmp(buff_mccwrk_alloc,"NoData")==0))
			{
				hide_progress_frame();

				remove("/root/mcc_wrks_alloc_info.txt");
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","NO DATA FOUND FOR Work Allocation INFO ");
				return -1;
			}
		}
		sprintf(query,"select count(*) from mcc_work_allocation_details");
		count_mccwrks_prev = exec_query_get_single_value(count_mccwrks_prev, query, DB_PACK);

		printf("\n count_mccwrks_prev = %s\n",count_mccwrks_prev);
		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/mcc_wrks_alloc_info.txt mcc_work_allocation_details\" 1 >/tmp/out 2> /root/mcc_wrkalloc_error.txt");
		sprintf(query,"select count(*) from mcc_work_allocation_details");

		count_wage_now = exec_query_get_single_value(count_wage_now, query, DB_PACK);
		printf("\n count_wage_now = %s\n",count_wage_now);
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query_delete,"delete from mcc_work_allocation_details where rowid not in(select max(rowid) from mcc_work_allocation_details group by demand_id,group_id,household_code,worker_code,substr(alloc_from_date,1,10),substr(alloc_to_date,1,10))");
		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(fname1,"/root/mcc_wrkalloc_error.txt");
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
		res_mcc_wrkalloc_now=atoi(count_wage_now);
		res_wage_prev=atoi(count_mccwrks_prev);
		mcc_wrkalloc_cnt = res_mcc_wrkalloc_now - res_wage_prev;
		printf("\n comp_count = %d\n",mcc_wrkalloc_cnt);
		hide_progress_frame();
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS","Allocation Downloaded Successfully");
		sqlite3_close(dbhandle);
	}
	return (0);
}
