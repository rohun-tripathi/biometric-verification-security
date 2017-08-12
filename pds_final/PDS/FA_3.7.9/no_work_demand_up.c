#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include "types.h"

int cb_emuster();

void show_progress_frame();
void hide_progress_frame();
void show_frame_gprs_page();

char *get_device_no();
char *exec_query_get_single_value(char *data, char *query, char *database);

char group_id_nd[15],group_nam_nd[200],reason_nd[200],date_nd[20],trans_nd[20];
int get_no_demand_grps(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(group_id_nd,argv[0]);
	strcpy(group_nam_nd,argv[1]);
	strcpy(reason_nd,argv[2]);
	strcpy(date_nd,argv[3]);
	strcpy(trans_nd,argv[4]);
	return(0);
}
int get_page_nwd(char *Url_nodemand)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	curl=curl_easy_init();
	if(curl)
	{
		fp = fopen("/root/no_demand.txt", "w");

		curl_easy_setopt(curl,CURLOPT_URL,Url_nodemand);
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
			show_message_frame(errmsg);
			return -1;
		}	
	}
	return(0);
}

int no_demand_up()
{
	char *wrk_alloc_count=NULL,*pid=NULL,*rowid_no=NULL;
	char buffnodemand[100];
	FILE *ffp_nodemand=NULL;
	char *error;
	char upload_buff[100],buffer[200];
	int j,ret1,ret2,ret3,rc,upload_cnt_nd=0;
	int ret,count_up_alloc;
	char tabbuff_nodemand[1024],Url_nodemand[1024],query[1024];

	tabbuff_nodemand[0]='\0';
	Url_nodemand[0]='\0';

	memset(buffer,'\0',sizeof(buffer));

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select ifnull(count(*),0) from no_work_demand where status='N'");
	wrk_alloc_count = exec_query_get_single_value(wrk_alloc_count, query, DB_PACK);

	count_up_alloc = atoi(wrk_alloc_count);

	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
	pid = exec_query_get_single_value(pid,query,DB_PACK);

	for(j=0; j<atoi(wrk_alloc_count); j++)
	{
		sprintf(query,"select ifnull((max(rowid)-%d),'0') from no_work_demand where status='N'",j);
		rowid_no =exec_query_get_single_value(rowid_no,query,DB_PACK);
		printf("\n row id=%s\n",rowid_no);
		int up_rowid= atoi(rowid_no);
		if(up_rowid==0)
		{
			up_rowid=1;
		}
		printf("\n row id after loop=%d\n",up_rowid);

		sprintf(query,"select group_id,replace(group_name,' ','_'),reason,replace(transcation_date,' ','_'),transcation_date from no_work_demand where status='N' and rowid='%d'",up_rowid);

		printf("query1 is %s\n",query);

		sqlite3_exec(dbhandle, query,get_no_demand_grps, NULL, NULL);

		sprintf(upload_buff,"Uploading Please Wait \n Remaining Record Count:%d",count_up_alloc);
		show_progress_frame(upload_buff);

		sprintf(tabbuff_nodemand,"%s,%s,%s,%s,%s",pid,group_id_nd,group_nam_nd,reason_nd,date_nd);

		printf("\n tab buff is %s\n",tabbuff_nodemand);

		//	sprintf(Url_nodemand,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/NotDemandedGroups.aspx?Data=");
//		sprintf(Url_nodemand,"%s","http://nrega.ap.gov.in:9090/eMMS/NotDemandedGroups.aspx?Data=");
		sprintf(Url_nodemand,"%s",NO_DEMAND_GROUPS_UP);
		strcat(Url_nodemand,tabbuff_nodemand);

		printf("\n URL is %s\n",Url_nodemand);

		ret=get_page_nwd(Url_nodemand);

		if(ret==0)
		{

			ffp_nodemand=fopen("/root/no_demand.txt","r");

			buffnodemand[0]='\0';

			while(fgets(buffnodemand,100,ffp_nodemand))
			{	
				printf("response from the server is %s \n",buffnodemand);	

				ret1=strcmp(buffnodemand,"Success");

				ret2=strcmp(buffnodemand,"Exists.");//Exists
				ret3=strcmp(buffnodemand,"Failed");//Exists

				printf("\n ret1 and ret2 is %d %d\n",ret1,ret2);

				if(ret1==0||ret2==0)
				{

					sprintf(query,"update no_work_demand set status='Y' where status='N' and  group_id='%s' and transcation_date='%s'",group_id_nd,trans_nd);																printf("query2 is %s\n",query);
					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);

					if(rc==0)
					{
						count_up_alloc=count_up_alloc-1;    
						upload_cnt_nd++;
						printf("\n upload cnt is %d\n",upload_cnt_nd);

					}
					else
					{
						printf("\n update problem\n");
						break;
					}
				}
				if(ret3==0)
				{
					pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
							PW_MSGBOX_ICON_INFO, "INFO","WRONG DATA With Server");
					return 0;

				}
			}
			fclose(ffp_nodemand);
			remove("/root/no_demand.txt");
		}

	}
	if(strcmp(buffnodemand,"<h1>Bad Request</h1>")==0)
	{
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
				PW_MSGBOX_ICON_INFO, "INFO","WRONG DATA IN POS");
		return 0;					
	}
	if(strcmp(buffnodemand,"<h1>html</h1>")==0)
	{	
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
				PW_MSGBOX_ICON_INFO, "INFO","SERVER PROBLEM");
		return 0;
	}

	hide_progress_frame();
	char *status_cnt_no_upload=NULL,*tot_upload_cnt_nd=NULL;

	sprintf(query,"select ifnull(count(*),'0') from no_work_demand where status='N'");
	status_cnt_no_upload =exec_query_get_single_value(status_cnt_no_upload,query,DB_PACK);

	sprintf(query,"select ifnull(count(*),'0') from no_work_demand where substr(transcation_date,1,10)=current_date and status='Y'");
	tot_upload_cnt_nd =exec_query_get_single_value(tot_upload_cnt_nd,query,DB_PACK);

	memset(buffer,'\0',sizeof(buffer));

	sprintf(buffer,"NotUploaded count:%s\n  Total Upload count :%d\n Todays Upload cnt:%s\n",status_cnt_no_upload,upload_cnt_nd,tot_upload_cnt_nd);

	pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_INFO, "INFO", buffer);
	return -1;

	free(wrk_alloc_count);
	free(pid);
	free(rowid_no);
	free(status_cnt_no_upload);
	free(tot_upload_cnt_nd);
	sqlite3_close(dbhandle);
	//show_frame_gprs_page();
	return(1);
}
