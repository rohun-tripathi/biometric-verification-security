#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include "types.h"

void show_progress_frame();
void hide_progress_frame();

char *exec_query_get_single_value(char *data, char *query, char *database);

int get_page_mem_info(char *Url_m)
{

	CURL *curl;
	FILE *fp_m;
	CURLcode res;
	char errmsg[256];

	curl=curl_easy_init();
	if(curl)
	{
		fp_m = fopen("/root/mem_info.txt", "w");

		curl_easy_setopt(curl,CURLOPT_URL,Url_m);
		curl_easy_setopt(curl,CURLOPT_FILE, fp_m);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);
		fclose(fp_m);	
		curl_easy_cleanup(curl);

		if(res==0)
		{
			return 0;
		}
		else
		{
			show_message_frame(errmsg);
			printf("\n err msg is %s\n",errmsg);
			return -1;
		}	
	}
	return(0);
}
int mem_info_upload(char *pos_user,char *free_memory,char *ver_no,char *dev_no)
{
	char *pid =NULL;
	char bufftext[100];
	FILE *ffp_mem=NULL;
	int ret1;

	char mem_info_tab[1024],mem_infoUrl[1024],query[1024];
	mem_infoUrl[0]='\0';
	mem_info_tab[0]='\0';

	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
	pid = exec_query_get_single_value(pid,query,DB_PACK);
	show_progress_frame("Uploading Please Wait");
	sprintf(mem_info_tab,"%s,%s,%s,%s,%s,%s,%s",ver_no,free_memory,dev_no,"Simno",pid,"FA",pos_user);
	printf("complete tab_ buff is %s\n",mem_info_tab);

	//	sprintf(mem_infoUrl,"%s","http://nrega.ap.gov.in:9090/eMMS/POSManagement.aspx?Data=");
	sprintf(mem_infoUrl,"%s",POS_INFO_UP);
	strcat(mem_infoUrl,mem_info_tab);

	printf("\n URL is %s\n",mem_infoUrl);

	ret=get_page_mem_info(mem_infoUrl);

	if(ret==0)
	{
		ffp_mem=fopen("/root/mem_info.txt","r");

		bufftext[0]='\0';

		while(fgets(bufftext,100,ffp_mem))
		{	
			printf("response from the server is %s \n",bufftext);	
			ret1=strcmp(bufftext,"Success");
			if(ret1==0)
			{
				show_progress_frame("Successfully Uploaded ");
				hide_progress_frame();
				return 0;
			}
			if(strcmp(bufftext,"<h1>Bad Request</h1>")==0)
			{
				hide_progress_frame();
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","WRONG DATA IN POS");
				return -1;					

			}
			if(strcmp(bufftext,"<h1>html</h1>")==0)
			{	
				hide_progress_frame();
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","SERVER PROBLEM");
				return -1;
			}

		}
		fclose(ffp_mem);
		remove("/root/mem_info.txt");

	}

	hide_progress_frame();	
}
