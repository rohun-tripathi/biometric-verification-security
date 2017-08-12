#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include "types.h"

int cb_emuster();
char *error;
sqlite3 *dbhandle=NULL;
char jobcardno_u[20], crdate_u[20],grpname_u[150],musterid_u[15],workcode_u[30],worker_type_u[10],group_id_u[20],distance_u[10],crowbar_u[10],mate_u[10],gps_long_u[20],gps_lat_u[20],fromdate_u[20],todate_u[20],skilled_type_u[10],Attendance_type[5],Progress_code[20];

void show_progress_frame();
void hide_progress_frame();
void show_frame_gprs_page();

char *get_device_no();

char *exec_query_get_single_value(char *data, char *query, char *database);

int get_muster_callback(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(jobcardno_u,argv[8]);
	strcpy(crdate_u,argv[3]);
	strcpy(grpname_u,argv[7]);
	strcpy(musterid_u,argv[4]);
	strcpy(workcode_u,argv[0]);
	strcpy(worker_type_u,argv[9]);
	strcpy(skilled_type_u,argv[10]);
	strcpy(group_id_u,argv[6]);
	strcpy(distance_u,argv[5]);
	strcpy(crowbar_u,argv[11]);
	strcpy(mate_u,argv[12]);
	strcpy(gps_long_u,argv[13]);
	strcpy(gps_lat_u,argv[14]);
	strcpy(fromdate_u,argv[1]);
	strcpy(todate_u,argv[2]);
	strcpy(Attendance_type,argv[15]);
	strcpy(Progress_code,argv[16]);
	return(0);

}
char tabbuff[1024],Url[1024],query[1024];
int get_page(char *Url)
{

	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	curl=curl_easy_init();
	if(curl)
	{
		fp = fopen("/root/Muster_status.txt", "w");

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

int  muster_duplicate(char *sel_job,char *sel_wrk,char *sel_grp,char *sel_dt)
{
	char query_insert[1024];

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}

	sprintf(query_insert,"insert into muster_duplication(jobcardno, member_name, group_name, muster_id, status, distance, work_code, crowbar, group_id, worker_type, Mate, Gpslongitude, Gpslatitude, skilledtype, from_date, to_date,Attendance_type,progress_code) select jobcardno, member_name, group_name, muster_id, status, distance, work_code, crowbar, group_id, worker_type, Mate, Gpslongitude, Gpslatitude, skilledtype, from_date, to_date,Attendance_type,progress_code from muster_attendance where jobcardno='%s' and substr(created_date,1,10)=current_date",sel_job);

	printf("query muster table is %s\n", query_insert);

	sqlite3_exec(dbhandle, query_insert, NULL, NULL, NULL);

	sprintf(query,"update muster_attendance set status='Y' where status='N' and work_code='%s' and group_name=replace('%s','_',' ') and  substr(created_date,1,10)='%s' and jobcardno='%s'",sel_wrk,sel_grp,sel_dt,sel_job); 

	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	printf("\n update duplicate record is %s\n",query);

	sqlite3_close(dbhandle);

	return 0;
}
int gprs()
{
	char *status_count=NULL,*pid=NULL,*rowid_no=NULL,*status_W_F=NULL,*status_M,*status_W_F_M=NULL,*tot_upload_cnt=NULL;
	char upload_buff[100],buffer[200];
	int j,ret1,ret2,ret3,rc,upload_cnt=0;
	char bufftext[100];
	FILE *ffp=NULL;

	tabbuff[0]='\0';
	Url[0]='\0';
	memset(buffer,'\0',sizeof(buffer));

	int ret,count_up;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select ifnull(count(*),0) from muster_attendance where status='N'");
	status_count = exec_query_get_single_value(status_count, query, DB_PACK);
	printf("\n status_count is %s\n",status_count);
	count_up = atoi(status_count);
	if(atoi(status_count)==0)
	{
		sprintf(query,"select ifnull(count(*),'0') from muster_attendance where substr(created_date,1,10)=current_date and status='Y'");
		tot_upload_cnt =exec_query_get_single_value(tot_upload_cnt,query,DB_PACK);
		sprintf(query,"select ifnull(count(*),0) from muster_attendance where status='M'");
		status_M = exec_query_get_single_value(status_M, query, DB_PACK);
		printf("\n status_M is %s\n",status_M);
		sprintf(query,"select ifnull(count(*),0) from muster_attendance where status in('F','E')");
		status_W_F = exec_query_get_single_value(status_W_F, query, DB_PACK);
		sprintf(query,"select ifnull(count(*),0) from muster_attendance where status in('F','M','N','E')");
		status_W_F_M = exec_query_get_single_value(status_W_F_M, query, DB_PACK);
		if(atoi(status_W_F_M)>0)
		{
			sprintf(buffer,"NotUploaded count:%s\n Todays musters cnt:%s\n Muster Id(Problem):%s\n Future Date/150 Days Exceeded:%s",status_W_F_M,tot_upload_cnt,status_M,status_W_F);
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_INFO, "INFO", buffer);
			return -1;
		}
	}

	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
	pid = exec_query_get_single_value(pid,query,DB_PACK);

	for(j=0; j<atoi(status_count); j++)
	{
		printf("j val is %d",j);
		/*if(upload_cnt==1)
		  {
		  j=0;
		  }*/
		sprintf(query,"select ifnull((max(rowid)-%d),'0') from muster_attendance where status='N'",j);
		rowid_no =exec_query_get_single_value(rowid_no,query,DB_PACK);
		printf("\n row id=%s\n",rowid_no);
		int up_rowid= atoi(rowid_no);
		if(up_rowid==0)
		{
			up_rowid=1;
		}
		printf("\n row id after loop=%d\n",up_rowid);

		sprintf(query,"select work_code,from_date,to_date,replace(created_date,' ','_'),muster_id,distance,group_id,replace(group_name,' ','_'),jobcardno,worker_type,skilledtype,crowbar,mate,gpslongitude,gpslatitude,Attendance_type,Progress_code from muster_attendance where status='N'  and rowid=%d",up_rowid);
//		sprintf(query,"select work_code,from_date,to_date,substr(created_date,1,10),muster_id,distance,group_id,replace(group_name,' ','_'),jobcardno,worker_type,skilledtype,crowbar,mate,gpslongitude,gpslatitude,Attendance_type,Progress_code from muster_attendance where status='N'  and rowid=%d",up_rowid);

		printf("query1 is %s\n",query);

		sqlite3_exec(dbhandle, query, get_muster_callback, NULL, NULL);

		sprintf(upload_buff,"Uploading Please Wait \n Remaining Record Count:%d",count_up);

		show_progress_frame(upload_buff);
		int m=strlen(musterid_u);
		if(m==6)
		{
	sprintf(tabbuff,"%s,%s,%s,%s,%s%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s%s",workcode_u,fromdate_u,todate_u,crdate_u,"201314",musterid_u,distance_u,group_id_u,grpname_u,jobcardno_u,worker_type_u,skilled_type_u,crowbar_u,mate_u,gps_long_u,gps_lat_u,Progress_code,Attendance_type,pid,"FA",get_device_no());
		}
		else
		{
		sprintf(tabbuff,"%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s,%s%s",workcode_u,fromdate_u,todate_u,crdate_u,musterid_u,distance_u,group_id_u,grpname_u,jobcardno_u,worker_type_u,skilled_type_u,crowbar_u,mate_u,gps_long_u,gps_lat_u,Progress_code,Attendance_type,pid,"FA",get_device_no());
		}
		sprintf(Url,"%s",EMUSTER_ATTD);
//		sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS/eMusters.aspx?Data=");

		strcat(Url,tabbuff);

		printf("\n URL is %s\n",Url);

		ret=get_page(Url);

		if(ret==0)
		{

			ffp=fopen("/root/Muster_status.txt","r");

			bufftext[0]='\0';

			while(fgets(bufftext,100,ffp))
			{	
				printf("response from the server is %s \n",bufftext);	

				ret1=strcmp(bufftext,"Success");

				ret2=strcmp(bufftext,"Exists");

				ret3=strcmp(bufftext,"Duplicate");

				if(ret1==0||ret2==0)
				{

					sprintf(query,"update muster_attendance set status='Y' where status='N' and work_code='%s' and group_name=replace('%s','_',' ') and  replace(created_date,' ','_')='%s' and jobcardno='%s'",workcode_u,grpname_u,crdate_u,jobcardno_u);																					printf("query2 is %s\n",query);

					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);

					if(rc==0)
					{
						count_up=count_up-1;    
						upload_cnt++;
						printf("\n upload cnt is %d\n",upload_cnt);

					}
					else
					{
						printf("\n update problem\n");
						break;
					}
				}
				else if(ret3==0)
				{
					muster_duplicate(jobcardno_u,workcode_u,grpname_u,crdate_u);

				}
				else if(strcmp(bufftext,"Musters can not be upload for Future Dates")==0)
				{
					sprintf(query,"update muster_attendance set status='F' where status='N' and work_code='%s' and group_name=replace('%s','_',' ') and  replace(created_date,' ','_')='%s' and jobcardno='%s'",workcode_u,grpname_u,crdate_u,jobcardno_u);                                                                                                                                                                  printf("query2 is %s\n",query);

					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);	
				}
				else if(strcmp(bufftext,"Invalid work code")==0)
				{
					sprintf(query,"update muster_attendance set status='W' where status='N' and work_code='%s' and group_name=replace('%s','_',' ') and  replace(created_date,' ','_')='%s' and jobcardno='%s'",workcode_u,grpname_u,crdate_u,jobcardno_u);                                                                                                                                                                  printf("query2 is %s\n",query);

					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);	
				}
				else if(strcmp(bufftext,"Invalid Muster id")==0) //Invalid Muster id 
				{
					sprintf(query,"update muster_attendance set status='M' where status='N' and work_code='%s' and group_name=replace('%s','_',' ') and  replace(created_date,' ','_')='%s' and jobcardno='%s'",workcode_u,grpname_u,crdate_u,jobcardno_u);                                                                                                                                                                  printf("query2 is %s\n",query);

					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);

				}
				else if(strcmp(bufftext,"100 days exceeded for Household")==0) //100 Days Exceeded
				{
					sprintf(query,"update muster_attendance set status='E' where status='N' and work_code='%s' and group_name=replace('%s','_',' ') and  replace(created_date,' ','_')='%s' and jobcardno='%s'",workcode_u,grpname_u,crdate_u,jobcardno_u);                                                                                                                                                                  printf("query2 is %s\n",query);

					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);

				}
				else if(strcmp(bufftext,"Musters can not be upload on sunday")==0) //100 Days Exceeded
				{
					sprintf(query,"update muster_attendance set status='S' where status='N' and work_code='%s' and group_name=replace('%s','_',' ') and  replace(created_date,' ','_')='%s' and jobcardno='%s'",workcode_u,grpname_u,crdate_u,jobcardno_u);                                                                                                                                                                  printf("query2 is %s\n",query);

					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);
				}

				else
				{		
					printf("\n other problems\n");

				}

			}

			fclose(ffp);
			remove("/root/Muster_status.txt");
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

	char *status_cnt_no=NULL,*W_F_cnt=NULL;

	sprintf(query,"select ifnull(count(*),'0') from muster_attendance where status='N'");
	status_cnt_no =exec_query_get_single_value(status_cnt_no,query,DB_PACK);

	sprintf(query,"select ifnull(count(*),'0') from muster_attendance where substr(created_date,1,10)=current_date and status='Y'");
	tot_upload_cnt =exec_query_get_single_value(tot_upload_cnt,query,DB_PACK);

	sprintf(query,"select ifnull(count(*),0) from muster_attendance where status='M'");
	status_M = exec_query_get_single_value(status_M, query, DB_PACK);
	printf("\n status_M is %s\n",status_M);
	sprintf(query,"select ifnull(count(*),0) from muster_attendance where status in('F','E')");
	status_W_F = exec_query_get_single_value(status_W_F, query, DB_PACK);
	sprintf(query,"select ifnull(count(*),'0') from muster_attendance where status in ('F','M','E','N')");
	W_F_cnt=exec_query_get_single_value(W_F_cnt,query,DB_PACK);

	if(atoi(W_F_cnt)>0)
	{
		memset(buffer,'\0',sizeof(buffer));

		sprintf(buffer,"NotUploaded count:%s\n Total Upload count :%d\n Todays Musters cnt:%s\n  Muster Id(Problem):%s\n Future Dates/150 Days Exceeds:%s",W_F_cnt,upload_cnt,tot_upload_cnt,status_M,status_W_F);

		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_INFO, "INFO", buffer);
		return -1;
	}
	else
	{
		show_message_frame("NO RECORDS TO UPLOAD");
		return -1;
	}

	free(status_count);
	free(pid);
	free(rowid_no);
	free(status_cnt_no);
	free(tot_upload_cnt);
	sqlite3_close(dbhandle);
	show_frame_gprs_page();
	return(1);
}
