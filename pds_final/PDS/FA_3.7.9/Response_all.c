#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include "types.h"

char *get_Gp_code();
char *get_device_no();
void hide_progress_frame();

int server_response(char *Url)
{
	CURL *curl;
	FILE *fp_serv;
	CURLcode res;
	char errmsg[256];
	fp_serv = fopen("/root/server_response.txt", "w");
	curl=curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl,CURLOPT_URL,Url);
		curl_easy_setopt(curl,CURLOPT_FILE, fp_serv);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);
		fclose(fp_serv);     
		curl_easy_cleanup(curl);

		if(res==0)
		{
			return 0;
		}
		// Exception is implemented as per suggestions
		else
		{
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_ERROR, "ALERT",errmsg);
			hide_progress_frame();			
			return -1;
		}
	}
	return(0);
}
int response_wage(int count)
{
	char Url1[120]="",final_value[32]="",dev_value[50]="";
	char buff[100];
	FILE *ffp_wrk_resp=NULL;
	int ret1;
	sprintf(Url1,"%s",UPDATE_Download_DAYSRESPONSE);
	//      sprintf(Url1,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/UpdateDownloadedDays.aspx?Code=");
	strcat(Url1,get_Gp_code());

	sprintf(final_value,"%sRecCount=%d","&",count);

	strcat(Url1,final_value);

	sprintf(dev_value,"%sMachineName=%s%s","&","FA",get_device_no());

	strcat(Url1,dev_value);

	ret1=server_response(Url1);

	if(ret1==0)
	{
		hide_progress_frame();

		ffp_wrk_resp=fopen("/root/server_response.txt","r");

		buff[0]='\0';
		while(fgets(buff,100,ffp_wrk_resp))
		{
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS",buff);
			fclose(ffp_wrk_resp);
			return 0;
		}
	}

	return 0;

}

int response_remaining_days(int count)
{
	char Url1[120]="",final_value[32]="",dev_value[50]="";
	char buff[100];
	FILE *ffp_wrk_resp=NULL;
	int ret1;
	sprintf(Url1,"%s",UPDATE_REMAIN_DAYSRESPONSE);
	//      sprintf(Url1,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/UpdateRemainingDays.aspx?Code=");
	strcat(Url1,get_Gp_code());

	sprintf(final_value,"%sRecCount=%d","&",count);

	strcat(Url1,final_value);

	sprintf(dev_value,"%sMachineName=%s%s","&","FA",get_device_no());

	strcat(Url1,dev_value);

	ret1=server_response(Url1);

	if(ret1==0)
	{
		hide_progress_frame();

		ffp_wrk_resp=fopen("/root/server_response.txt","r");

		buff[0]='\0';

		while(fgets(buff,100,ffp_wrk_resp))
		{
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS",buff);
			fclose(ffp_wrk_resp);
			return 0;
		}
	}

	return 0;
}
int response_wrk(int count)
{

	char Url1[120]="",final_value[32]="",dev_value[50]="";
	char buff[100];
	FILE *ffp_wrk_resp=NULL;
	int ret1;
	sprintf(Url1,"%s",UPDATEWRK_RESPONSE);
	//	sprintf(Url1,"%s","http://nrega.ap.gov.in:9090/eMMS/UpdateWorks.aspx?Code=");
	strcat(Url1,get_Gp_code());

	sprintf(final_value,"%sRecCount=%d","&",count);

	strcat(Url1,final_value);

	sprintf(dev_value,"%sMachineName=%s%s","&","FA",get_device_no());

	strcat(Url1,dev_value);

	ret1=server_response(Url1);

	if(ret1==0)
	{
		hide_progress_frame();

		ffp_wrk_resp=fopen("/root/server_response.txt","r");

		buff[0]='\0';

		while(fgets(buff,100,ffp_wrk_resp))
		{
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS",buff);
			fclose(ffp_wrk_resp);
			return 0;
		}
	}
	return 0;
}

int response_hh(int count)
{
	char Url1[120]="",final_value[32]="",dev2_value[50]="";
	char buff[100];
	FILE *ffp_hh_resp=NULL;
	int ret1;
	sprintf(Url1,"%s",SSSINFO_RESPONSE);
	//	sprintf(Url1,"%s","http://nrega.ap.gov.in:9090/eMMS/UpdateSSSInfo.aspx?Code=");
	strcat(Url1,get_Gp_code());
	sprintf(final_value,"%sRecCount=%d","&",count);
	strcat(Url1,final_value);	
	sprintf(dev2_value,"%sMachineName=%s%s","&","FA",get_device_no());
	strcat(Url1,dev2_value);
	ret1=server_response(Url1);	

	if(ret1==0)
	{	
		hide_progress_frame();

		ffp_hh_resp=fopen("/root/server_response.txt","r");

		buff[0]='\0';

		while(fgets(buff,100,ffp_hh_resp))
		{
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS",buff);
			fclose(ffp_hh_resp);
			return 0;
		}
	}
	return 0;
}

int response_grp(int count)
{
	char Url1[200]="",final_value[32]="";
	char tot[50]="";
	FILE *ffp_grp_resp=NULL;
	char buff[100];
	int ret1;
	sprintf(Url1,"%s",SSSGRUPALLOCN_RESPONSE);
	//	sprintf(Url1,"%s","http://nrega.ap.gov.in:9090/eMMS/UpdateSSSGroupsAllocation.aspx?Code=");

	strcat(Url1,get_Gp_code());

	sprintf(final_value,"%sRecCount=%d","&",count);

	printf("\n final_value is %s\n",final_value);

	strcat(Url1,final_value);

	sprintf(tot,"%s%s%s","&","MachineName=FA",get_device_no());

	strcat(Url1,tot);

	ret1=server_response(Url1);
	if(ret1==0)
	{
		hide_progress_frame();

		ffp_grp_resp=fopen("/root/server_response.txt","r");

		buff[0]='\0';

		while(fgets(buff,100,ffp_grp_resp))
		{
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS",buff); 
			fclose(ffp_grp_resp);
			return 0;
		}
	}
	return 0;
}
