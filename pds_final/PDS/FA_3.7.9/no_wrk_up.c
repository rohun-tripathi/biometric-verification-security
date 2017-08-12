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
char *exec_query_get_single_value(char *data, char *query, char *database);

int get_curl_resp_nw(char *Url)
{
	CURL *curl;
	FILE *fp_nw;
	CURLcode res;
	char errmsg[256];

	curl=curl_easy_init();
	if(curl)
	{
		fp_nw = fopen("/root/nw_status.txt", "w");
		curl_easy_setopt(curl,CURLOPT_URL,Url);
		curl_easy_setopt(curl,CURLOPT_FILE, fp_nw);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);
		fclose(fp_nw);	
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
int no_work_up(char *dev_no)
{
	printf("\n dev_no is %s\n",dev_no);
	char buff_text_nw[100];
	FILE *ffp_nw=NULL;
	char *get_date=NULL;
	char tab_buff_nw[1024],Url_nw[1024],query[1024];
	int ret;
	char *p_id =NULL;
	int ret1,ret2;

	tab_buff_nw[0]='\0';
	Url_nw[0]='\0';

	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
	p_id = exec_query_get_single_value(p_id,query,DB_PACK);
	sprintf(query,"select date('now')");
	get_date= exec_query_get_single_value(get_date,query,DB_PACK);
	show_progress_frame("Uploading Please Wait");
	sprintf(tab_buff_nw,"%s,%s%s,%s,%s",p_id,"FA",dev_no,"No_Work",get_date);

	printf("complete tab_ buff is %s\n",tab_buff_nw);

	//	sprintf(Url_nw,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/GPAbstractMusters.aspx?Data=");
	//	sprintf(Url_nw,"%s","http://nrega.ap.gov.in:9090/eMMS/GPAbstractMusters.aspx?Data=");
	sprintf(Url_nw,"%s",NO_WORK_UP);
	strcat(Url_nw,tab_buff_nw);
	printf("\n URL is %s\n",Url_nw);
	ret=get_curl_resp_nw(Url_nw);
	printf("\n ret test is %d\n",ret);

	if(ret==0)
	{
		ffp_nw=fopen("/root/nw_status.txt","r");

		buff_text_nw[0]='\0';

		while(fgets(buff_text_nw,100,ffp_nw))
		{	
			printf("response from the server is %s \n",buff_text_nw);	

			ret1=strcmp(buff_text_nw,"Success");

			ret2=strcmp(buff_text_nw,"Exists");

			if(ret1==0)
			{
				show_message_frame("UPLOADED SUCCESSFULLY");
				hide_progress_frame();
				return 0;
			}
			else if(ret2==0)
			{
			        show_message_frame("Already Uploaded");
				hide_progress_frame();
				return 0;
			}

			else
			{
				show_message_frame("UPLOADING PROBLEM");
				hide_progress_frame();
				return 0;
			}
		}
		fclose(ffp_nw);
	}
	free(p_id);
	free(get_date);
	return(1);
}
