#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include "types.h"


int gp_test();

void show_progress_frame();
void hide_progress_frame();
void show_frame_gprs_page();
char *exec_query_get_single_value(char *data, char *query, char *database);


int get_page_mvo(char *Url)
{

	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	curl=curl_easy_init();
	if(curl)
	{
		fp = fopen("/root/MVO_status.txt", "w");

		//		printf("\n coming to get_page_up \n");
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
		// Exception is implemented as per suggestions

		else
		{
			show_message_frame(errmsg);
			return -1;
		}	
	}

	return(0);
}
char jobcard_mv[22],groupname_mv[50],groupid_mv[15],workcode_mv[20],gpslon_mv[10],gpslat_mv[10],crowbar_mv[2],crdate_mv[15];
int get_mvo_call(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(jobcard_mv,argv[0]);
	strcpy(groupname_mv,argv[1]);
	strcpy(groupid_mv,argv[2]);
	strcpy(workcode_mv,argv[3]);
	strcpy(gpslon_mv,argv[4]);
	strcpy(gpslat_mv,argv[5]);
	strcpy(crowbar_mv,argv[6]);
	strcpy(crdate_mv,argv[7]);
	return(0);

}
char tabbuff[1024],Url[1024],query[1024],status_yes[10],status_no[10];
int get_count_mvo_callbk(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(status_yes,argv[0]);
	strcpy(status_no,argv[1]);
	return(0);	
}

int Mvo_upload()
{

	char *status_count=NULL,*fa_remarks=NULL;
	char upload_buff[100],buffer[100];
	char bufftext[100];
	FILE *fp_mvo=NULL;
	int i,j,ret1,ret2,rc,count=0;
	int ret,count_up;
	sqlite3 *dbhandle=NULL;
	char *error;
	tabbuff[0]='\0';
	Url[0]='\0';


	i = sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select count(*) from MVO_Details where status='N'");

	status_count = exec_query_get_single_value(status_count, query, DB_PACK);

	count_up = atoi(status_count);

	if(atoi(status_count)==0)
	{

		show_message_frame("NO RECORDS TO UPLOAD");
		return -1;

	}

CODE:
	for(j=0; j<atoi(status_count); j++)
	{

		sprintf(query,"select replace(remarks,' ','_') from MVO_remarks where status='N'");
		fa_remarks = exec_query_get_single_value(fa_remarks, query, DB_PACK);

		sprintf(query,"select jobcardno,replace(group_name,' ','_'),group_id,work_code,Gpslongitude,Gpslatitude,crowbar,substr(created_date,1,10) from MVO_Details where status='N' limit 1");

		sqlite3_exec(dbhandle, query, get_mvo_call, NULL, NULL);

		sprintf(upload_buff,"Uploading Please Wait \n Remaining Record Count:%d",count_up);

		show_progress_frame(upload_buff);

		sprintf(tabbuff,"%s,%s,%s,%s,%s,%s,%s,%s,%s",jobcard_mv,groupname_mv,groupid_mv,workcode_mv,gpslon_mv,gpslat_mv,crowbar_mv,crdate_mv,fa_remarks);

		//		sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/MVOMustersUpload.aspx?Data=");

//		sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS/MVOMustersUpload.aspx?Data=");
		sprintf(Url,"%s",MVO_MUSTER_UP);
		strcat(Url,tabbuff);

		printf("\n URL is %s\n",Url);

		ret=get_page_mvo(Url);

		if(ret==0)
		{

			fp_mvo=fopen("/root/MVO_status.txt","r");

			bufftext[0]='\0';

			while(fgets(bufftext,100,fp_mvo))
			{	
				printf("response from the server is %s \n",bufftext);	

				ret1=strcmp(bufftext,"Success");

				ret2=strcmp(bufftext,"Exists");

				if(ret1==0||ret2==0)
				{

					sprintf(query,"update MVO_Details set status='Y' where status='N' and work_code='%s' and group_id='%s' and  substr(created_date,1,10)='%s' and jobcardno='%s'",workcode_mv,groupid_mv,crdate_mv,jobcard_mv);

					printf("query2 is %s\n",query);
					rc=sqlite3_exec(dbhandle, query, NULL, NULL,&error);

					if(rc==0)
					{
						count_up=count_up-1;    
					}
					else
					{
						break;

					}
				}
				else
				{
					printf("\n Error response from the server\n");										   																 if(count<=3)
					{
						count=count+1;
						goto CODE;


					}
					hide_progress_frame();
					pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_WARNING, "ALERT", "TRY AGAIN");

					return -1;
				}
			}

			fclose(fp_mvo);
		}

	}

	hide_progress_frame();

	sprintf(query,"select count(*),(select count(*) from MVO_Details where status='N') from MVO_Details where status='Y' and substr(created_date,1,10)=current_date");

	sqlite3_exec(dbhandle, query, get_count_mvo_callbk, NULL, NULL);

	sprintf(query,"update MVO_remarks set status='Y' where status='N'");

	sqlite3_exec(dbhandle, query, NULL, NULL,&error);

	sprintf(buffer,"Uploaded Count:%s \n  Remaing count:%s",status_yes,status_no);
	pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
			PW_MSGBOX_ICON_INFO, "INFO", buffer);

	sqlite3_close(dbhandle);
	return(1);
}
int Mvo_gprs_conn()
{
	int res;
	ret= gp_test();
	if(ret<0)
	{
		show_message_frame("NOT CONNECTED TRY AGAIN");
	}
	else
	{

		ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
				PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
		if(ret==PW_MSGBOX_RETURN_OK)
		{
			res=Mvo_upload();
			return 0;
		}
		else
			return 0;
	}
	return 0;
}
