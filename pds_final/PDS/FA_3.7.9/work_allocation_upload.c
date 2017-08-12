#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include "types.h"

FILE *ffp_alloc=NULL;
char Url[1024];

void show_progress_frame();
void hide_progress_frame();
void show_frame_gprs_page();

char *get_device_no();

char *exec_query_get_single_value(char *data, char *query, char *database);

char work_code_alloc[20],group_id_alloc[15],hh_code_alloc[20],wrker_code_alloc[18],demand_dt_alloc[15],alloc_dt[15],d_frm_dt[15],d_to_dt[15],alloc_from_date[15],alloc_to_date[15],alloc_days[10],gpslong_alloc[15],gpslat_alloc[15],alloc_trans[25]; 

int get_allocation_details(void *parg, int argc, char **argv, char **colnames)
{
	printf("inside get values");

	strcpy(work_code_alloc,argv[0]);
	strcpy(group_id_alloc,argv[1]);
	//	strcpy(hh_code_alloc,argv[2]);
	//	strcpy(wrker_code_alloc,argv[3]);
	//	strcpy(demand_dt_alloc,argv[4]);
	strcpy(alloc_dt,argv[2]);
	strcpy(d_frm_dt,argv[3]);
	strcpy(d_to_dt,argv[4]);
	strcpy(alloc_from_date,argv[5]);
	strcpy(alloc_to_date,argv[6]);
	strcpy(alloc_days,argv[7]);
	strcpy(gpslong_alloc,argv[8]);
	strcpy(gpslat_alloc,argv[9]);
	strcpy(alloc_trans,argv[10]);
	return(0);

}
int get_page_alloc(char *Url)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	curl=curl_easy_init();
	if(curl)
	{
		fp = fopen("/root/wrk_alloc_status.txt", "w");

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
			show_message_frame(errmsg);
			return -1;
		}	
	}
	return(0);
}

int work_allocation_upload()
{
	char *wrk_alloc_count=NULL,*pid=NULL,*rowid_no=NULL;
	char upload_buff[100],buffer[200],bufftext[100];
	char tabbuff[1024],query[1024];
	int j,ret1,ret2,ret3,rc,upload_cnt_alloc=0;
	char *error;

	tabbuff[0]='\0';
	Url[0]='\0';
	memset(buffer,'\0',sizeof(buffer));
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	//sprintf(query,"select ifnull(count(*),0) from work_allocation_details where status='N'");
	sprintf(query,"select count(*) from (select distinct work_code,group_id,alloc_from_date,alloc_to_date,demand_frm_dt,demand_to_dt from work_allocation_details where status='N')");
	wrk_alloc_count = exec_query_get_single_value(wrk_alloc_count, query, DB_PACK);

	int count_up_alloc = atoi(wrk_alloc_count);

	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
	pid = exec_query_get_single_value(pid,query,DB_PACK);

	for(j=0; j<atoi(wrk_alloc_count); j++)
	{
		/*sprintf(query,"select ifnull((max(rowid)-%d),'0') from work_allocation_details where status='N'",j);
		rowid_no =exec_query_get_single_value(rowid_no,query,DB_PACK);
		printf("\n row id=%s\n",rowid_no);
		int up_rowid= atoi(rowid_no);
		if(up_rowid==0)
		{
			up_rowid=1;
		}
		printf("\n row id after loop=%d\n",up_rowid);
*/
		//sprintf(query,"select distinct work_code,group_id,substr(allocation_date,1,10),demand_frm_dt,demand_to_dt,alloc_from_date,alloc_to_date,alloc_days,gps_long,gps_lat,replace(allocation_date,' ','_') from work_allocation_details where status='N' and rowid='%d'",up_rowid);
		sprintf(query,"select distinct work_code,group_id,substr(allocation_date,1,10),demand_frm_dt,demand_to_dt,alloc_from_date,alloc_to_date,alloc_days,gps_long,gps_lat,replace(allocation_date,' ','_') from work_allocation_details where status='N'");
	//	sprintf(query,"select distinct work_code,group_id,substr(allocation_date,1,10),demand_frm_dt,demand_to_dt,alloc_from_date,alloc_to_date,alloc_days,gps_long,gps_lat,replace(allocation_date,' ','_') from work_allocation_details where status='N' and rowid='%d'",up_rowid);

		printf("query1 is %s\n",query);

		sqlite3_exec(dbhandle, query,get_allocation_details, NULL, NULL);
	printf("after");

		sprintf(upload_buff,"Uploading Please Wait \n Remaining Record Count:%d",count_up_alloc);
		show_progress_frame(upload_buff);
		printf("\n work_code_alloc is %s\n %s\n",work_code_alloc,hh_code_alloc);

		//sprintf(tabbuff,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",pid,work_code_alloc,group_id_alloc,hh_code_alloc,wrker_code_alloc,demand_dt_alloc,alloc_dt,d_frm_dt,d_to_dt,alloc_from_date,alloc_to_date,alloc_days,gpslong_alloc,gpslat_alloc,alloc_trans);
		sprintf(tabbuff,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s",pid,work_code_alloc,group_id_alloc,alloc_dt,d_frm_dt,d_to_dt,alloc_from_date,alloc_to_date,alloc_days,gpslong_alloc,gpslat_alloc,alloc_trans);

		printf("\n tab buff is %s\n",tabbuff);
		sprintf(Url,"%s",DEMNDWORKALLOCN_UP);
		//		sprintf(Url,"%s","http://nrega.ap.gov.in:9090/emms/DemandAllocation.aspx?Data=");

		strcat(Url,tabbuff);

		printf("\n URL is %s\n",Url);

		int ret=get_page_alloc(Url);

		if(ret==0)
		{

			ffp_alloc=fopen("/root/wrk_alloc_status.txt","r");

			bufftext[0]='\0';

			while(fgets(bufftext,100,ffp_alloc))
			{	
				printf("response from the server is %s \n",bufftext);	

				ret1=strcmp(bufftext,"Success");

				ret2=strcmp(bufftext,"Exists.");//Exists
				ret3=strcmp(bufftext,"Failed");//Exists

				printf("\n ret1 and ret2 is %d %d\n",ret1,ret2);

				if(ret1==0||ret2==0)
				{
					sprintf(query,"update work_allocation_details set status='Y' where status='N' and work_code='%s' and group_id='%s'and substr(demand_frm_dt,1,10)='%s' and substr(demand_to_dt,1,10)='%s' and alloc_from_date='%s' and alloc_to_date='%s' and substr(allocation_date,1,10)='%s'",work_code_alloc,group_id_alloc,d_frm_dt,d_to_dt,alloc_from_date,alloc_to_date,alloc_dt);													printf("query2 is %s\n",query);

					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);

					if(rc==0)
					{
						count_up_alloc=count_up_alloc-1;    
						upload_cnt_alloc++;
						printf("\n upload cnt is %d\n",upload_cnt_alloc);

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
			fclose(ffp_alloc);
			remove("/root/wrk_alloc_status.txt");
		}

	}
	if(strcmp(bufftext,"<h1>Bad Request</h1>")==0)
	{
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
				PW_MSGBOX_ICON_INFO, "INFO","WRONG DATA IN POS");
		return 0;					

	}
	if(strcmp(bufftext,"<h1>html</h1>")==0)
	{	
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
				PW_MSGBOX_ICON_INFO, "INFO","SERVER PROBLEM");
		return 0;
	}

	hide_progress_frame();

	char *status_cnt_no_alloc=NULL,*tot_upload_cnt_alloc=NULL;

	//sprintf(query,"select ifnull(count(*),'0') from work_allocation_details where status='N'");
	// sprintf(query,"select count(distinct alloc_from_date) from work_allocation_details where status='N'");
	sprintf(query,"select count(*) from (select distinct work_code,group_id,alloc_from_date,alloc_to_date,demand_frm_dt,demand_to_dt from work_allocation_details where status='N')");

	status_cnt_no_alloc =exec_query_get_single_value(status_cnt_no_alloc,query,DB_PACK);

	//sprintf(query,"select ifnull(count(*),'0') from work_allocation_details where substr(allocation_date,1,10)=current_date and status='Y'");
	sprintf(query,"select ifnull(count(distinct alloc_from_date),'0') from work_allocation_details where substr(allocation_date,1,10)=current_date and status='Y'");
	tot_upload_cnt_alloc =exec_query_get_single_value(tot_upload_cnt_alloc,query,DB_PACK);

	memset(buffer,'\0',sizeof(buffer));

	sprintf(buffer,"No of Grps NotUploaded count:%s\n No of Grps Upload count :%d\n No of Grps Todays Upload cnt:%s\n",status_cnt_no_alloc,upload_cnt_alloc,tot_upload_cnt_alloc);

	pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_INFO, "INFO", buffer);
	return -1;
	free(wrk_alloc_count);
	free(pid);
	free(rowid_no);
	free(status_cnt_no_alloc);
	free(tot_upload_cnt_alloc);
	sqlite3_close(dbhandle);
	return(1);
}
