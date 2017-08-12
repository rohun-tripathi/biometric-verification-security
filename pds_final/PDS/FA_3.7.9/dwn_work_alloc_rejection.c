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

int wrkalloc_rejc_server_response(char *Url)
{
	CURL *curl;
	FILE *fp1_serv;
	CURLcode res;
	char errmsg[256];

	fp1_serv = fopen("/root/serv_resp_mccwrkalloc.txt", "w");

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

int down_file_server_mccwrkalloc_recinfo(char *Url_day)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	fp = fopen("/root/mcc_wrks_alloc_rejcinfo.txt", "w");

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
int work_alloc_rejection()
{
	char Url_wrks_rej[1024]="";
	char *cnt_wrksrej_prev=NULL,*cnt_wrkrec_now=NULL;
	char buff_mccwrks_rec[100];
	FILE *ffp_dwn_rejceted_wrksalloc=NULL;
	char fname1[256];
	int imgFD1=0, fsz1=0,mcc_wrkalloc_rejcnt,res_mcc_wrkalloc_rejnow,res_mcc_wrkalloc_rejprev;
	struct stat fst1;	
	sprintf(Url_wrks_rej,"%s",MCC_WRKALLOC_REJC);
	//	sprintf(Url_wrks_rej,"%s","http://nrega.ap.gov.in:9090/eMMS/RejectedDemandAllocationDetails.aspx?Code=");
	show_progress_frame("Downloading Allocation \n Rejection details");
	printf("mcc work allocation details is %s\n",Url_wrks_rej);
	printf("\n get_Gp_code is %s\n",get_Gp_code());
	strcat(Url_wrks_rej,get_Gp_code());
	printf("\n Url_wrks_rej is %s\n",Url_wrks_rej);
	ret=down_file_server_mccwrkalloc_recinfo(Url_wrks_rej);
	printf("\n ret value is %d\n",ret);
	if(ret==0)
	{
		ffp_dwn_rejceted_wrksalloc=fopen("/root/mcc_wrks_alloc_rejcinfo.txt","r");
		buff_mccwrks_rec[0]='\0';
		while(fgets(buff_mccwrks_rec,100,ffp_dwn_rejceted_wrksalloc))
		{
			if((strcmp(buff_mccwrks_rec,"NoData")==0))
			{
				hide_progress_frame();

				remove("/root/mcc_wrks_alloc_rejcinfo.txt");
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","NO DATA Found for Work Alloc Rejection ");
				return -1;
			}
		}
		sprintf(query,"select count(*) from allocation_rejection_details");
		cnt_wrksrej_prev = exec_query_get_single_value(cnt_wrksrej_prev, query, DB_PACK);

		printf("\n cnt_wrksrej_prev = %s\n",cnt_wrksrej_prev);
		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/mcc_wrks_alloc_rejcinfo.txt allocation_rejection_details\" 1 >/tmp/out 2> /root/mcc_wrkalloc_rej_error.txt");
		sprintf(query,"select count(*) from allocation_rejection_details");

		cnt_wrkrec_now = exec_query_get_single_value(cnt_wrkrec_now, query, DB_PACK);
		printf("\n cnt_wrkrec_now = %s\n",cnt_wrkrec_now);
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query_delete,"delete from allocation_rejection_details where rowid not in(select max(rowid) from allocation_rejection_details group by group_id,alloc_from_date,alloc_to_date)");
		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(fname1,"/root/mcc_wrkalloc_rej_error.txt");
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
		res_mcc_wrkalloc_rejnow=atoi(cnt_wrkrec_now);
		res_mcc_wrkalloc_rejprev=atoi(cnt_wrksrej_prev);
		mcc_wrkalloc_rejcnt = res_mcc_wrkalloc_rejnow - res_mcc_wrkalloc_rejprev;
		printf("\n comp_count = %d\n",mcc_wrkalloc_rejcnt);
		hide_progress_frame();
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS","Allocation Rejected \n  Downloaded Successfully");
		sqlite3_close(dbhandle);
		return 0;
	}
	return (0);
}
