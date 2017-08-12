#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include "types.h"

int gprs();
void show_progress_frame();
void hide_progress_frame();
void show_frame_gprs_page();

char *get_device_no();
char *exec_query_get_single_value(char *data, char *query, char *database);
char GroupId[15],Householdcode[20],Demanddays[10],fromdate[15],todate[15],appsdate[30],Gpslongitude[10],Gpslatitude[10],recepit_id[9],apps_date[15];
int get_gp_demand_details(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(GroupId,argv[0]);
	//	printf("group id is %s\n",GroupId);
	strcpy(Householdcode,argv[1]);
	strcpy(Demanddays,argv[2]);
	strcpy(fromdate,argv[3]);
	strcpy(todate,argv[4]);
	strcpy(appsdate,argv[5]);
	strcpy(Gpslongitude,argv[6]);
	strcpy(Gpslatitude,argv[7]);
	strcpy(recepit_id,argv[8]);
	strcpy(apps_date,argv[9]);
	return 0;
}
int get_curl_demand(char *Url)
{
	CURL *curl;
	FILE *fp_gp;
	CURLcode res;
	char errmsg[256];

	curl=curl_easy_init();
	if(curl)
	{
		fp_gp = fopen("/root/demand_status.txt", "w");

		printf("\n coming to get_page_up \n");
		curl_easy_setopt(curl,CURLOPT_URL,Url);
		curl_easy_setopt(curl,CURLOPT_FILE, fp_gp);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);
		fclose(fp_gp);
		curl_easy_cleanup(curl);

		if(res==0)
		{
			return 0;
		}
		else
		{
			printf("\n errmessage is %s\n",errmsg);
			show_message_frame(errmsg);
			return -1;
		}
	}
	return(0);
}

char demand_buff[1024],Url_demand[1024],query[1024],cnt_yes1[10],cnt_no1[10];
int get_count_demand(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(cnt_yes1,argv[0]);
	strcpy(cnt_no1,argv[1]);
	return(0);
}

int demand_upload()
{
	char *d_id =NULL;
	char upload_buff[100],buffer1[75];
	int j,ret1,ret2,rc,ret3,ret4,tot_demandupload_cnt=0;

	demand_buff[0]='\0';
	Url_demand[0]='\0';

	sqlite3 *dbhandle=NULL;
	char buff_text[100];
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	char *cnt_demand=NULL;
	int ret,count_up_demand;
	sprintf(query,"select count(*) from Demand_Group_details where status='N'");
	cnt_demand = exec_query_get_single_value(cnt_demand, query, DB_PACK);

	if((atoi(cnt_demand)==0))
	{
		char *status_gpacnt_no=NULL,*status_issue_cnt=NULL;
		sprintf(query,"select ifnull(count(*),'0') from Demand_Group_details  where substr(demanddate,1,10)=current_date and status='Y'");
		status_gpacnt_no =exec_query_get_single_value(status_gpacnt_no,query,DB_PACK);
		printf("\n status_gpacnt_no=%s\n",status_gpacnt_no);

		sprintf(query,"select ifnull(count(*),'0') from Demand_Group_details where status not in('N','Y','R','R1','D')");
		status_issue_cnt =exec_query_get_single_value(status_issue_cnt,query,DB_PACK);
		printf("\n status_issue_cnt=%s\n",status_issue_cnt);
		sprintf(buffer1,"Demand Uploaded Count:%s \n Demand Issue Count %s \n",status_gpacnt_no,status_issue_cnt);

		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
				PW_MSGBOX_ICON_INFO, "INFO", buffer1);
		free(status_gpacnt_no);
		free(status_issue_cnt);

		return 0;
	}
	count_up_demand =atoi(cnt_demand);
	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
	d_id = exec_query_get_single_value(d_id,query,DB_PACK);
	char *row_no=NULL;
	for(j=0; j<atoi(cnt_demand); j++)
	{
		sprintf(query,"select (max(rowid)-%d) from Demand_Group_details where status='N'",j);
		row_no =exec_query_get_single_value(row_no,query,DB_PACK);
		printf("\n row no=%s\n",row_no);
		int up_rowno = atoi(row_no);
		if(up_rowno==0)
		{
			printf("krsna");
			up_rowno=1;
		}
		free(row_no);
		sprintf(query,"select group_id,Householdcode,Demanddays,fromdate,todate,replace(demanddate,' ','_'),Gpslongitude,Gpslatitude,recepit_id,substr(application_date,1,10) from Demand_Group_details where status='N'  and rowid=%d",up_rowno);
		//sprintf(query,"select group_id,Householdcode,Demanddays,fromdate,todate,replace(demanddate,' ','_'),Gpslongitude,Gpslatitude,recepit_id,substr(application_date,1,10) from Demand_Group_details where status='N'");
		printf("query1 is %s\n",query);

		sqlite3_exec(dbhandle, query, get_gp_demand_details, NULL, NULL);
		sprintf(upload_buff,"Uploading Please Wait \n Remaining Record Count:%d",count_up_demand);

		show_progress_frame(upload_buff);
		sprintf(demand_buff,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",d_id,GroupId,Householdcode,Demanddays,fromdate,todate,appsdate,Gpslongitude,Gpslatitude,recepit_id,apps_date);

		printf("complete tab_ buff is %s\n",demand_buff);
		//				sprintf(Url_demand,"%s","http://nrega.ap.gov.in:9090/eMMS/DemandCapture.aspx?Data=");
		sprintf(Url_demand,"%s",DEMANDCAPTUR);
		strcat(Url_demand,demand_buff);

		printf("\n URL is %s\n",Url_demand);

		ret=get_curl_demand(Url_demand);

		printf("\n ret test is %d\n",ret);
		char *error;
		FILE *ffp_demand=NULL;


		if(ret==0)
		{
			ffp_demand=fopen("/root/demand_status.txt","r");
			buff_text[0]='\0';
			while(fgets(buff_text,100,ffp_demand))
			{
				printf("response from the server is %s \n",buff_text);
				ret1=strcmp(buff_text,"Success");
				ret2=strcmp(buff_text,"Exists.");
				ret3=strcmp(buff_text,"ReceiptIdExists.");
				if(ret1==0||ret2==0)
				{
					sprintf(query,"update Demand_Group_details set status='Y' where status='N'  and Householdcode='%s' and substr(application_date,1,10)='%s'",Householdcode,apps_date);                                                                                                     printf("query2 is %s\n",query);

					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);
					if(rc==0)
					{
						count_up_demand=count_up_demand-1;
						tot_demandupload_cnt++;
						printf("\n upload cnt is %d\n",tot_demandupload_cnt);
					}
					else
					{
						printf("\n update problem\n");
						break;

					}
				}
				else if(ret3==0)
				{
					printf("inside ReceiptIdExists.");

					sprintf(query,"update Demand_Group_details set status='R' where status='N'  and Householdcode='%s' and substr(application_date,1,10)='%s'",Householdcode,apps_date);                                                                                                     printf("query2 is %s\n",query);

					rc =sqlite3_exec(dbhandle, query, NULL, NULL,&error);
					if(rc==0)
					{
						count_up_demand=count_up_demand-1;
						tot_demandupload_cnt++;
						printf("\n upload cnt is %d\n",tot_demandupload_cnt);
					}
					else
					{
						printf("\n update problem\n");
						break;

					}

				}
			}
			fclose(ffp_demand);
			remove("/root/demand_status.txt");
		}
	}

	hide_progress_frame();

	ret4=strcmp(buff_text,"<h1>Bad Request</h1>");
	int ret5=strcmp(buff_text,"<h1>html</h1>");


	if(ret4==0)
	{
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
				PW_MSGBOX_ICON_INFO, "INFO","WRONG DATA IN POS");
		return 0;
	}
	if(ret5==0)
	{
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
				PW_MSGBOX_ICON_INFO, "INFO","SERVER PROBLEM1");
		return 0;
	}
	char *status_gpacnt_no=NULL,*status_issue_cnt=NULL;
	sprintf(query,"select ifnull(count(*),'0') from Demand_Group_details where status='N'");
	status_gpacnt_no =exec_query_get_single_value(status_gpacnt_no,query,DB_PACK);
	printf("\n status_gpacnt_no=%s\n",status_gpacnt_no);

	sprintf(query,"select ifnull(count(*),'0') from Demand_Group_details where status not in('N','Y','R','R1','D')");
	status_issue_cnt =exec_query_get_single_value(status_issue_cnt,query,DB_PACK);
	printf("\n status_issue_cnt=%s\n",status_issue_cnt);

	if(atoi(status_issue_cnt)>0)
	{
		memset(buffer1,'\0',sizeof(buffer1));

		sprintf(buffer1,"Demand Uploaded Count:%d \n  Demand NotUploaded count:%s  \n Demand Issue Count %s \n",tot_demandupload_cnt,status_gpacnt_no,status_issue_cnt);

		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
				PW_MSGBOX_ICON_INFO, "INFO", buffer1);
		return -1;
	}
	else
	{
		show_message_frame("NO RECORDS TO UPLOAD");
		return -1;
	}
	free(status_gpacnt_no);
	free(status_issue_cnt);
	sqlite3_close(dbhandle);
	return 0;
}
