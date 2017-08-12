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

int down_file_server_mccdemandgrps_info(char *Url_day)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];
	remove("/root/mcc_demand_grps_info.txt");
	fp = fopen("/root/mcc_demand_grps_info.txt", "w");

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
int mcc_demand_details_down()
{
	char Url_mccdemands[1024]="";
	char *count_mccwrks_prev=NULL,*count_wage_now=NULL;
	FILE *ffp_dwn_mcc_demand_grps=NULL;
	char query_delete[512];
	char query[1024];

	char fname1[256],buff_mccdemand_grps[100];
	int imgFD1=0, fsz1=0,mcc_demandgrp_cnt,res_mcc_demandgrp_now,res_wage_prev;
	struct stat fst1;	
	sprintf(Url_mccdemands,"%s",MCC_DEMAND_GROUPS);
	//	sprintf(Url_mccdemands,"%s","http://nrega.ap.gov.in:9090/eMMS/DownloadDemandCaptureDetails.aspx?Code=");
	show_progress_frame("Downloading Demand \nPlz Wait ");
	printf("\n get_Gp_code is %s\n",get_Gp_code());
	strcat(Url_mccdemands,get_Gp_code());
	printf("\n Url_mccdemands is %s\n",Url_mccdemands);
	ret=down_file_server_mccdemandgrps_info(Url_mccdemands);
	printf("\n ret value is %d\n",ret);
	if(ret==0)
	{
		ffp_dwn_mcc_demand_grps=fopen("/root/mcc_demand_grps_info.txt","r");
		buff_mccdemand_grps[0]='\0';
		while(fgets(buff_mccdemand_grps,100,ffp_dwn_mcc_demand_grps))
		{
			if((strcmp(buff_mccdemand_grps,"NoData")==0))
			{
				hide_progress_frame();

				remove("/root/mcc_demand_grps_info.txt");
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","NO DATA FOUND FOR DEMAND Creation INFO ");
				return -1;
			}
		}
		sprintf(query,"select count(*) from mcc_demand_group_details");
		count_mccwrks_prev = exec_query_get_single_value(count_mccwrks_prev, query, DB_PACK);
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query,"delete from mcc_demand_group_details");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		printf("\n count_mccwrks_prev = %s\n",count_mccwrks_prev);
		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/mcc_demand_grps_info.txt mcc_demand_group_details\" 1 >/tmp/out 2> /root/mcc_demandgrps_error.txt");
		sprintf(query,"select count(*) from mcc_demand_group_details");

		count_wage_now = exec_query_get_single_value(count_wage_now, query, DB_PACK);
		printf("\n count_wage_now = %s\n",count_wage_now);
	
		sprintf(query_delete,"delete from mcc_demand_group_details where rowid not in(select max(rowid) from mcc_demand_group_details group by household_code,worker_code)");
		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(fname1,"/root/mcc_demandgrps_error.txt");
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
		res_mcc_demandgrp_now=atoi(count_wage_now);
		res_wage_prev=atoi(count_mccwrks_prev);
		mcc_demandgrp_cnt = res_mcc_demandgrp_now - res_wage_prev;
		printf("\n comp_count = %d\n",mcc_demandgrp_cnt);
		hide_progress_frame();
		sqlite3_close(dbhandle);
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS","Demand Groups Downloaded Successfully");
	}
	return (0);
}
