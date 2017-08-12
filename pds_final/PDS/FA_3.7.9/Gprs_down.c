#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curl/curl.h>
#include<sqlite3.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "types.h"

int response_wrk();

int response_grp();

int response_hh();

int gprs_down();
int response_remaining_days();

char *exec_query_get_single_value(char *data, char *query, char *database);

void show_progress_frame();

void hide_progress_frame();

void show_frame_gprs_page();
char *get_Gp_code();
char *get_device_no();


int down_file_server_remaining_days(char *Url)
{
	CURL *curl;
	FILE *fp_rem = NULL;
	CURLcode res;
	char errmsg[256];

	fp_rem = fopen("/root/remaining_days_info.txt", "w");

	curl=curl_easy_init();

	if(curl)
	{
		curl_easy_setopt(curl,CURLOPT_URL,Url);
		curl_easy_setopt(curl,CURLOPT_FILE, fp_rem);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);
		fclose(fp_rem);
		curl_easy_cleanup(curl);
		if(res==0)
		{
			return 0;
		}
		else
		{		
			hide_progress_frame();
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_ERROR, "ERROR",errmsg);

			return -1;
		}
	}
	return(0);
}

FILE *ffp_down=NULL;
char bufftext[100];
char query[1024],query_delete[1024];
int gprs_remaining_days_info()
{
	char Url[100]="";
	char *count_rem_prev=NULL,*count_rem_now=NULL;
	char fname4[256],device4_value[100]="";
	int imgFD4=0, fsz4=0,count_rem_days,res_rem_now,res_rem_prev;
	struct stat fst4;	

	sprintf(Url,"%s",REMANINGDAYS);
	//sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS_Test/RemainingDays.aspx?Code=");

	show_progress_frame(" Downloading Please Wait ");

	strcat(Url,get_Gp_code());

	sprintf(device4_value,"%sMachineName=%s%s","&","FA",get_device_no());

	strcat(Url,device4_value);

	printf("\n Url is %s\n",Url);

	ret=down_file_server_remaining_days(Url);

	printf("\n ret value is %d\n",ret);

	if(ret==0)
	{
		ffp_down=fopen("/root/remaining_days_info.txt","r");
		bufftext[0]='\0';

		while(fgets(bufftext,100,ffp_down))
		{
			if((strcmp(bufftext,"NoData")==0))
			{
				hide_progress_frame();

				remove("/root/remaining_days_info.txt");

				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","NO DATA FOUND FOR REMAINING MANDAYS ");
				return -1;
			}
		}
		sprintf(query,"select count(*) from remaining_mandays_details");

		count_rem_prev = exec_query_get_single_value(count_rem_prev, query, DB_PACK);


		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/remaining_days_info.txt remaining_mandays_details\" 1 >/tmp/out 2> /root/mandays_error.txt");

		sprintf(query,"select count(*) from remaining_mandays_details");

		count_rem_now = exec_query_get_single_value(count_rem_now, query, DB_PACK);

		printf("\n count_hh_now = %s\n",count_rem_now);

		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query_delete,"delete from remaining_mandays_details where rowid not in(select max(rowid) from remaining_mandays_details group by work_code)");

		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);

		sprintf(fname4,"/root/mandays_error.txt");

		imgFD4 = open(fname4, O_RDONLY);

		fstat(imgFD4, &fst4);

		fsz4 = fst4.st_size;

		if(fsz4 > 0)
		{
			hide_progress_frame();

			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
					PW_MSGBOX_ICON_ERROR, "ALERT","NREGS DATA MISMATCH");
			return -1;
		}
		res_rem_now=atoi(count_rem_now);
		res_rem_prev=atoi(count_rem_prev);
		count_rem_days = res_rem_now - res_rem_prev;
		printf("\n count_rem_days = %d\n",count_rem_days);

		response_remaining_days(count_rem_days);
		sqlite3_close(dbhandle);
	}
	return (0);
}

int down_file_server(char *Url)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	fp = fopen("/root/works_registration.txt", "w");

	curl=curl_easy_init();
	if(curl)
	{
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
			hide_progress_frame();
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK, PW_MSGBOX_ICON_ERROR, "ERROR",errmsg);
			return -1;
		}
	}
	return(0);
}

int gprs_works()
{
	char Url[100];
	Url[0]='\0';
	char *count_wrk_prev=NULL,*count_wrk_now=NULL;

	char fname3[256],device_value[100]="";

	int imgFD3=0, fsz3=0,count_wrk,res_wrk_now,res_wrk_prev;

	struct stat fst3;
	sprintf(Url,"%s",WORKS_DOWN);
	//	sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS/Works.aspx?Code=");

	show_progress_frame(" Downloading Please Wait ");

	strcat(Url,get_Gp_code());

	sprintf(device_value,"%sMachineName=%s%s","&","FA",get_device_no());

	strcat(Url,device_value);

	printf("\n Url is %s\n",Url);

	ret=down_file_server(Url);

	printf("\n ret value is %d\n",ret);

	if(ret==0)
	{

		ffp_down=fopen("/root/works_registration.txt","r");

		bufftext[0]='\0';

		while(fgets(bufftext,100,ffp_down))
		{
			if((strcmp(bufftext,"NoData")==0))
			{
				hide_progress_frame();
				remove("/root/works_registration.txt");
				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_ERROR, "ERROR","NO DATA FOUND FOR WORKS");		
				return -1;
			}
		}

		sprintf(query,"select count(*) from Work_Registration");

		count_wrk_prev = exec_query_get_single_value(count_wrk_prev, query, DB_PACK);

		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/works_registration.txt Work_Registration\" 1 >/tmp/out 2> /root/wrk_error.txt");
		sprintf(query,"select count(*) from Work_Registration");

		count_wrk_now = exec_query_get_single_value(count_wrk_now, query, DB_PACK);

		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query_delete,"delete from Work_Registration where rowid not in(select max(rowid) from Work_Registration group by work_code)");

		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);

		sprintf(query_delete,"delete from Work_Registration where work_code IN(select work_code from Work_Registration where description='Closed')");
		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);

		sprintf(fname3, "/root/wrk_error.txt");
		imgFD3 = open(fname3, O_RDONLY);
		fstat(imgFD3, &fst3);
		fsz3 = fst3.st_size;
		if(fsz3 > 0)
		{	
			hide_progress_frame();
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
					PW_MSGBOX_ICON_ERROR, "ERROR","NREGS DATA MISMATCH");
			return -1;
		}
		res_wrk_now=atoi(count_wrk_now);
		res_wrk_prev=atoi(count_wrk_prev);

		count_wrk = res_wrk_now - res_wrk_prev;

		response_wrk(count_wrk);

		sqlite3_close(dbhandle);
	}

	return (0);		
}
int down_file_server_groups(char *Url)
{
	CURL *curl;
	FILE *fp;
	CURLcode res;
	char errmsg[256];

	fp = fopen("/root/groups_allocation.txt", "w");

	curl=curl_easy_init();
	if(curl)
	{
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
			hide_progress_frame();

			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_ERROR, "ERROR",errmsg);
			return -1;
		}
	}
	return(0);
}

int gprs_groups()
{
	char Url[120];
	Url[0]='\0';

	char *count_grp_prev=NULL,*count_grp_now=NULL;
	char fname2[256],device1_value[100]="";
	int imgFD2=0, fsz2=0,count_grp,res_grp_now,res_grp_prev;
	struct stat fst2;	
	sprintf(Url,"%s",GROUPALLOCAION_DOWN);
	//	sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS/SSSGroupsAllocation.aspx?Code=");

	show_progress_frame(" Downloading Please Wait ");

	strcat(Url,get_Gp_code());

	sprintf(device1_value,"%sMachineName=%s%s","&","FA",get_device_no());

	strcat(Url,device1_value);

	printf("\n Url is %s\n",Url);

	ret=down_file_server_groups(Url);

	printf("\n ret value is %d\n",ret);

	if(ret==0)
	{
		ffp_down=fopen("/root/groups_allocation.txt","r");

		bufftext[0]='\0';

		while(fgets(bufftext,100,ffp_down))
		{
			if((strcmp(bufftext,"NoData")==0))
			{
				hide_progress_frame();				
				remove("/root/groups_allocation.txt");

				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_WARNING, "ALERT","NO DATA FOUND FOR GROUPS IN THE SERVER");		
				return -1;
			}
		}

		sprintf(query,"select count(*) from groups_allocation");

		count_grp_prev = exec_query_get_single_value(count_grp_prev, query, DB_PACK);

		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/groups_allocation.txt groups_allocation\" 1 >/tmp/out 2> /root/grp_error.txt");

		sprintf(query,"select count(*) from groups_allocation ");

		count_grp_now = exec_query_get_single_value(count_grp_now, query, DB_PACK);

		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query_delete,"delete from groups_allocation where rowid not in(select max(rowid) from groups_allocation group by group_id,work_code)");

		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);


		sprintf(fname2, "/root/grp_error.txt");
		imgFD2 = open(fname2, O_RDONLY);
		fstat(imgFD2, &fst2);
		fsz2 = fst2.st_size;
		if(fsz2 > 0)
		{	
			hide_progress_frame();
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
					PW_MSGBOX_ICON_ERROR, "ERROR"," NREGS DATA MISMATCH");

			return -1;
		}

		res_grp_now=atoi(count_grp_now);

		res_grp_prev=atoi(count_grp_prev);

		count_grp = res_grp_now - res_grp_prev;


		response_grp(count_grp);

		sqlite3_close(dbhandle);

	}

	return (0);
}
int down_file_server_hh_info(char *Url)
{
	CURL *curl;
	FILE *fp_hh = NULL;
	CURLcode res;
	char errmsg[256];

	fp_hh = fopen("/root/hh_info.txt", "w");

	curl=curl_easy_init();
	if(curl)
	{
		curl_easy_setopt(curl,CURLOPT_URL,Url);
		curl_easy_setopt(curl,CURLOPT_FILE, fp_hh);
		curl_easy_setopt(curl,CURLOPT_ERRORBUFFER,errmsg);
		res=curl_easy_perform(curl);
		fclose(fp_hh);
		curl_easy_cleanup(curl);
		if(res==0)
		{
			return 0;
		}
		else
		{		
			hide_progress_frame();

			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_ERROR, "ERROR",errmsg);

			return -1;
		}
	}
	return(0);
}
int gprs_hh_info()
{
	char Url[100]="";
	char *count_hh_prev=NULL,*count_hh_now=NULL;
	char fname1[256],device2_value[100]="";
	int imgFD1=0, fsz1=0,comp_count,res_hh_now,res_hh_prev;
	struct stat fst1;	
	sprintf(Url,"%s",HH_INFO);
	//		sprintf(Url,"%s","http://nrega.ap.gov.in:9090/eMMS/SSSInfo.aspx?Code=");

	show_progress_frame(" Downloading Please Wait ");

	strcat(Url,get_Gp_code());

	sprintf(device2_value,"%sMachineName=%s%s","&","FA",get_device_no());

	strcat(Url,device2_value);

	printf("\n Url is %s\n",Url);

	ret=down_file_server_hh_info(Url);

	printf("\n ret value is %d\n",ret);

	if(ret==0)
	{

		ffp_down=fopen("/root/hh_info.txt","r");

		bufftext[0]='\0';

		while(fgets(bufftext,100,ffp_down))
		{
			if((strcmp(bufftext,"NoData")==0))
			{
				hide_progress_frame();

				remove("/root/hh_info.txt");

				pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
						PW_MSGBOX_ICON_INFO, "INFO","NO DATA FOUND FOR HOUSEHOLDS ");
				return -1;
			}
		}
		sprintf(query,"select count(*) from household_details");

		count_hh_prev = exec_query_get_single_value(count_hh_prev, query, DB_PACK);

		system("sqlite3 -separator + /usr/local/apps/nrega_AP/db.dat \".import /root/hh_info.txt household_details\" 1 >/tmp/out 2> /root/hh_error.txt");

		sprintf(query,"select count(*) from household_details ");

		count_hh_now = exec_query_get_single_value(count_hh_now, query, DB_PACK);

		printf("\n count_hh_now = %s\n",count_hh_now);

		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");
		}

		sprintf(query_delete,"delete from household_details where rowid not in(select max(rowid) from household_details group by household_code,worker_code,group_type)");

		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(query_delete,"delete from household_details where household_code||worker_code IN(select household_code||worker_code from household_details where is_active='N' and group_type='GENERAL') and group_type='GENERAL'");
		sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(query_delete,"delete from household_details where household_code||worker_code IN(select household_code||worker_code from household_details where is_active='N' and group_type='DISABLED') and group_type='DISABLED'");
                sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(query_delete,"delete from household_details where household_code||worker_code IN(select household_code||worker_code from household_details where is_active='N' and group_type='SSS-T') and group_type='SSS-T'");
                sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(query_delete,"delete from  household_details  where group_type='SSS-T' and household_code||worker_code IN(select household_code||worker_code from household_details where group_type='GENERAL' and is_active='Y')");
		 sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		sprintf(query_delete,"delete from household_details  where group_type='GENERAL' and household_code||worker_code IN(select household_code||worker_code from household_details where group_type='DISABLED' and is_active='Y')");
		 sqlite3_exec(dbhandle, query_delete, NULL, NULL, NULL);
		        sprintf(query,"delete from household_details  where group_type='SSS-T' and household_code||worker_code IN(select household_code||worker_code from household_details where group_type='DISABLED' and is_active='Y')");
        sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from groups_distinct");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"insert into groups_distinct select distinct group_name,group_id from household_details");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(fname1,"/root/hh_error.txt");

	imgFD1 = open(fname1, O_RDONLY);

		fstat(imgFD1, &fst1);

		fsz1 = fst1.st_size;

		if(fsz1 > 0)
		{
			hide_progress_frame();

			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
					PW_MSGBOX_ICON_ERROR, "ALERT","NREGS DATA MISMATCH");
			return -1;
		}
		res_hh_now=atoi(count_hh_now);
		res_hh_prev=atoi(count_hh_prev);
		comp_count = res_hh_now - res_hh_prev;
		response_hh(comp_count);
		sqlite3_close(dbhandle);
	}
	return (0);
}
