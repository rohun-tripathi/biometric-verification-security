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

int dmnd_rejc_server_response(char *Url)
{
	CURL *curl;
	FILE *fp1_serv;
	CURLcode res;
	char errmsg[256];

	fp1_serv = fopen("/root/serv_resp_mccdmnd.txt", "w");

	curl=curl_easy_init();
	if(curl)
	{
		printf("\n coming to get_page_up \n");
		curl_easy_setopt(curl,CURLOPT_URL,Url);
		curl_easy_setopt(curl,CURLOPT_FILE, fp1_serv);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);		
		fclose(fp1_serv);     
		curl_easy_cleanup(curl);

		if(res==0)
		{
			return 0;
		}

		else
		{
			printf("\n errmessage is %s\n",errmsg);
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_ERROR, "ALERT",errmsg);
			hide_progress_frame();			
			return -1;
		}
	}
	return(0);
}
int down_file_server_mccdmnd_recinfo(char *Url_day)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	fp = fopen("/root/mcc_demand_rejcinfo.txt", "w");

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
int demand_rejection()
{
	char Url_demand_rej[1024]="";
	char *cnt_dmndsrej_prev=NULL,*cnt_dmndrec_now=NULL;
	char buff_mccdmnd_rec[100];
	FILE *ffp_dwn_rejceted_demand=NULL;
	char fname1[256];
	int imgFD1=0, fsz1=0,mcc_dmnd_rejcnt,res_mcc_demand_rejnow,res_mcc_demand_rejprev;
	struct stat fst1;	
	sprintf(Url_demand_rej,"%s",MCC_DEMAND_REJC);
	//	sprintf(Url_demand_rej,"%s","http://nrega.ap.gov.in:9090/eMMS/RejectedDemandCaptureDetails.aspx?Code=");
	show_progress_frame(" Downloading Demand \n Rejection Details ");
	printf("mcc work allocation details is %s\n",Url_demand_rej);
	printf("\n get_Gp_code is %s\n",get_Gp_code());
	strcat(Url_demand_rej,get_Gp_code());
	printf("\n Url_demand_rej is %s\n",Url_demand_rej);
	ret=down_file_server_mccdmnd_recinfo(Url_demand_rej);
	printf("\n ret value is %d\n",ret);
	if(ret==0)
	{
		ffp_dwn_rejceted_demand=fopen("/root/mcc_demand_rejcinfo.txt","r");
		buff_mccdmnd_rec[0]='\0';
		while(fgets(buff_mccdmnd_rec,100,ffp_dwn_rejceted_demand))
		{
			if((strcmp(buff_mccdmnd_rec,"NoData")==0))
			{
				hide_progress_frame();

				remove("/root/mcc_demand_rejcinfo.txt");
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","NO DATA Found for Demand Rejection");
				return -1;
			}
		}
		sprintf(query,"select count(*) from Demand_rejection_details");
		cnt_dmndsrej_prev = exec_query_get_single_value(cnt_dmndsrej_prev, query, DB_PACK);

		printf("\n cnt_dmndsrej_prev = %s\n",cnt_dmndsrej_prev);
		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/mcc_demand_rejcinfo.txt Demand_rejection_details\" 1 >/tmp/out 2> /root/mcc_demand_rej_error.txt");
		sprintf(query,"select count(*) from Demand_rejection_details");

		cnt_dmndrec_now = exec_query_get_single_value(cnt_dmndrec_now, query, DB_PACK);
		printf("\n cnt_dmndrec_now = %s\n",cnt_dmndrec_now);
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query_delete,"delete from Demand_rejection_details where rowid not in(select max(rowid) from Demand_rejection_details group by group_id,alloc_from_date,alloc_to_date)");
		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(fname1,"/root/mcc_demand_rej_error.txt");
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
		res_mcc_demand_rejnow=atoi(cnt_dmndrec_now);
		res_mcc_demand_rejprev=atoi(cnt_dmndsrej_prev);
		mcc_dmnd_rejcnt = res_mcc_demand_rejnow - res_mcc_demand_rejprev;
		printf("\n comp_count = %d\n",mcc_dmnd_rejcnt);
		hide_progress_frame();
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS","Demand Rejected \n  Downloaded Successfully");
		sqlite3_close(dbhandle);
		return 0;
	}
	return (0);
}
