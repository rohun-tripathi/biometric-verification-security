#include "types.h"
void hide_frame_gprs_page();
int gp_test();
int Wage_info_down();
int gp_work_upload();
int gprs_remaining_days_info();
int gprs();
int gprs_works();
int gprs_hh_info();
char *exec_query_get_single_value(char *data, char *query, char *database);
void show_progress_frame();
void hide_progress_frame();
char *get_Gp_code();
int work_allocation_upload();
int batch_info_dwn();
int cb_no_work();
int gprs_groups();
int no_work_up();
int ftp_up();
int no_demand_up();
void show_apps_dwnld_frame();
void show_frame_gprs_page();
void hide_frame_pos_appsupdates();
int get_splitjc_update();
int get_splitjc_dwnld();
char *exec_query_get_single_value(char *data, char *query, char *database);
void show_birs_dwnld_frame();
void hide_frame_split_jobcard();
int demand_upload();
int mcc_workalloc_details_down();
int mcc_demand_details_down();
int work_alloc_rejection();
int demand_rejection();
pw_widget btn_DemWork_download,frame_download_secondpage,btn_DemWorkRej_update,btn_downloadmainpage_back,btn_down_back;
pw_widget frame_download_mainpage,btn_Hhdownload, btn_wksdownload ,btn_downloadmainpage_back,button[5],btn_grpdownload,btn_wageinfodays,frame_pos_appsupdates,btn_pos_update[4],btn_demandinfo;
pw_widget frame_split_jobcard,btn_split_download,btn_split_update,Butt_option_split_job,btn_split_back;
pw_widget frame_upload_options,btn_upload_options[5],btn_upload_options_back;
void hide_frame_download_secondpage();
void hide_frame_download_mainpage();
void hide_frame_upload_options();
void show_frame_download_mainpage();
void show_frame_birup_login();
void show_frame_pos_appsupdates();
void cb_uploadmainpage_back_page()
{
	hide_frame_upload_options();
	pw_show(frame_gprs_page);

}
void cb_downloadmainpage_back_page()
{
	hide_frame_download_mainpage();
	pw_show(frame_gprs_page);
}
void cb_options_page()
{
	hide_frame_gprs_page();
	pw_show(frame_optionspage);
}
int cb_no_work()
{
	int res;
	ret= gp_test();
	printf("\n ret is %d\n",ret);
	ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
			PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
	char *dev_no_nw=NULL,qry[256];
	sprintf(qry,"select substr(jc_code,10,1) from jc_info");
	dev_no_nw = exec_query_get_single_value(dev_no_nw,qry,DB_PACK);
	if(ret==PW_MSGBOX_RETURN_OK)
		res=no_work_up(dev_no_nw);
	free(dev_no_nw);
	return 0;
}
void cb_birs_dwnld()
{
	hide_frame_pos_appsupdates();
	show_birs_dwnld_frame();
}
void cb_birs_upld1()
{
	hide_frame_pos_appsupdates();
	show_frame_birup_login();
}
int cb_birs_upld()
{
	ret= gp_test();

	int j,i,result,ret_ftp;
	char path[256];
	char *status_count=NULL,*row_no=NULL,query[512],*birname=NULL,url[500],*status_count2=NULL,up_bir[100],*pid=NULL;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	sprintf(query,"select count(*) from enrollment_master where enroll_status='N'");
	status_count=exec_query_get_single_value(status_count,query,DB_PACK);
	if(atoi(status_count)==0)
	{
		show_message_frame("No Birs To Upload");
		return 0;
	}
	else
	{
		sprintf(query,"select ifnull(max(rowid),0) from enrollment_master where enroll_status='N'");
		row_no =exec_query_get_single_value(row_no,query,DB_PACK);
		sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
		pid=exec_query_get_single_value(pid,query,DB_PACK);
		for(j=0; j<atoi(status_count); j++)
		{
			sprintf(query,"select count(*) from enrollment_master where enroll_status='N'");
			status_count2=exec_query_get_single_value(status_count2,query,DB_PACK);
			sprintf(up_bir,"Uploading Please wait \n Remaining person birs :%s",status_count2);
			show_progress_frame(up_bir);
			sqlite3 *dbhandle=NULL;
			sqlite3_open(DB_PACK, &dbhandle);
			sprintf(query,"select jobcard_no from enrollment_master where enroll_status='N' and rowid=%s",row_no);
			birname=exec_query_get_single_value(birname,query,DB_PACK);
			for(i=0; i<=8; i++)
			{
				path[0]='\0';
				sprintf(path,"ls -l /home/Backup/|grep -c %s_%d.bir",birname,i);
				result=system(path);
				if(result==0)
				{
					url[0]='\0';
					sprintf(url,"curl -T /home/Backup/%s_%d.bir -P- -u nregs/wssftp:test@123 ftp://125.22.8.69/Biometric_Data/%s/",birname,i,pid);
					printf("\n photo path is %s \n",url);
					ret_ftp = system(url);
					printf("ret_ftp value is %d\n",ret_ftp);
					if(ret_ftp!=0)
					{
						show_message_frame("SERVER Problem");
						hide_progress_frame();
						return 0;
						break;
					}
				}
			}	

			sprintf(query,"update enrollment_master set enroll_status='Y' where rowid='%s'",row_no);
			sqlite3_exec(dbhandle,query,NULL,NULL,NULL);
			sprintf(query,"select ifnull(max(rowid),0) from enrollment_master where enroll_status='N'");
			row_no =exec_query_get_single_value(row_no,query,DB_PACK);
			if(atoi(row_no)==0)
			{
				break;
			}
		}
	}
	free(row_no);
	free(status_count);
	free(status_count2);
	free(birname);
	free(pid);
	sqlite3_exec(dbhandle,query,NULL,NULL,NULL);
	sqlite3_close(dbhandle);
	hide_progress_frame();
	show_message_frame("Successfully uploaded");

	return 0;
}
pw_widget frame_biruplogin_page,entry_birupid[3],lbl_birup_login[3],btn_birupback,btn_birup_ok;

void entry_birup_back()
{
	pw_hide(frame_biruplogin_page);
	show_frame_pos_appsupdates();
}
int entry_birup_event(pw_widget widget, pw_event event)
{
	int  ret, ret1;
	char username[10], password[10];
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	strcpy(username, pw_get_text(entry_birupid[1]));
	strcpy(password, pw_get_text(entry_birupid[2]));

	printf("username = %s\n", username);
	printf("password = %s\n", password);

	ret=strcmp(username, "4321");
	printf("ret is %d\n", ret);
	ret1=strcmp(password, "4321");
	printf("ret1 is %d\n", ret1);

	if(ret==0 && ret1==0)
	{
		pw_hide(frame_biruplogin_page);
		show_frame_pos_appsupdates();
		cb_birs_upld();
	}
	else
	{
		show_message_frame("WRONG LOGIN TRY AGAIN");
		pw_entry_clear(entry_birupid[2]);
		return -1;
	}

	return 0;
}
void create_frame_biruplogin_page(void)
{
	frame_biruplogin_page = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE,
			PW_CONFIG_END);
	pw_hide(frame_biruplogin_page);
	lbl_birup_login[1] = pw_create_label(frame_biruplogin_page, 5, 20, 70, 30,
			PW_TEXT, "UserId:",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_birupid[1] = pw_create_entry(frame_biruplogin_page, 100, 20, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha
			, PW_NA);
	pw_focus(entry_birupid[1]);

	pw_entry_clear(entry_birupid[1]);
	lbl_birup_login[2] = pw_create_label(frame_biruplogin_page, 5, 70, 70, 30,
			PW_TEXT, "Password:",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_birupid[2] = pw_create_entry(frame_biruplogin_page, 100, 70, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_num
			, PW_NA);

	pw_entry_passwdchar( entry_birupid[2], '*' );
	pw_entry_clear(entry_birupid[2]);

	btn_birupback = pw_create_button(frame_biruplogin_page, 40, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_birup_back,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);
	btn_birup_ok = pw_create_button(frame_biruplogin_page, 140, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_OK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_birup_event,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);
}

void show_frame_birup_login(void)
{
	if(!frame_biruplogin_page)
	{
		create_frame_biruplogin_page();
		pw_show(frame_biruplogin_page);
	}
	else
		pw_show(frame_biruplogin_page);
	pw_entry_clear(entry_birupid[1]);
	pw_entry_clear(entry_birupid[2]);
	pw_focus(entry_birupid[1]);
}


void cb_apps_updates()
{
	hide_frame_pos_appsupdates();
	show_apps_dwnld_frame();
}
void get_pos_update_back()
{
	hide_frame_pos_appsupdates();
	show_frame_gprs_page();	
}
void create_frame_pos_appsupdates()
{
	frame_pos_appsupdates =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_pos_appsupdates);

	btn_pos_update[0] = pw_create_button(frame_pos_appsupdates, 30,25, 190, 40,
			PW_TEXT, "New Version Download",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_apps_updates,
			PW_CONFIG_END);

	btn_pos_update[1] = pw_create_button(frame_pos_appsupdates, 30,85, 190, 40,
			PW_TEXT, "Birs Download",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_birs_dwnld,
			PW_CONFIG_END);
	btn_pos_update[2] = pw_create_button(frame_pos_appsupdates, 30,145, 190, 40,
			PW_TEXT, "Birs Upload",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_birs_upld1,
			PW_CONFIG_END);

	btn_pos_update[3] = pw_create_button(frame_pos_appsupdates, 20,220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_pos_update_back,
			PW_CONFIG_END);

}
void show_frame_pos_appsupdates()
{
	if(!frame_pos_appsupdates){
		create_frame_pos_appsupdates();
		pw_show(frame_pos_appsupdates);
	}
	else
		pw_show(frame_pos_appsupdates);
	return;
}
void hide_frame_pos_appsupdates()
{
	pw_hide(frame_pos_appsupdates);
}
void cb_pos_appsupdates()
{
	hide_frame_gprs_page();
	show_frame_pos_appsupdates();
}

void get_split_back()
{
	hide_frame_split_jobcard();
	show_frame_gprs_page();
	return;
}

void create_frame_split_jobcard(void)
{
	frame_split_jobcard = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_split_jobcard);

	btn_split_download =pw_create_button(frame_split_jobcard, 30,50, 190, 45,
			PW_TEXT, "SPLIT JOBCARDS DOWNLOAD",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_splitjc_dwnld,
			PW_CONFIG_END);
	btn_split_update = pw_create_button(frame_split_jobcard, 30,145, 190, 45,
			PW_TEXT, "SPLITJOBCARDS UPDATE",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_splitjc_update,
			PW_CONFIG_END);

	btn_split_back =pw_create_button(frame_split_jobcard, 30,220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_split_back,
			PW_CONFIG_END);
	return ;
}


void cb_split_jobcard()
{
	if(! frame_split_jobcard)
	{
		create_frame_split_jobcard();
		pw_show(frame_split_jobcard);
	}
	else
		pw_show(frame_split_jobcard);
	return;
}
void hide_frame_split_jobcard(void)
{
	pw_hide(frame_split_jobcard);
	return;
}
int get_demand_dwnld()
{
	int res;
	ret= gp_test();
	res=mcc_demand_details_down();
	return 0;
}
int get_alloc_dwnld()
{
	int res;
	ret= gp_test();
	res=mcc_workalloc_details_down();
	return 0;
}
int get_reject_dwnld()
{
	int res;
	ret= gp_test();
	res=work_alloc_rejection();
	res=demand_rejection();
	return 0;
}
void get_main_back()
{
	hide_frame_download_secondpage();
	pw_show(frame_download_mainpage);
}
void create_frame_download_secondpage(void)
{
	frame_download_secondpage = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_download_secondpage);

	btn_DemWork_download =pw_create_button(frame_download_secondpage, 30,30, 190, 40,
			PW_TEXT, "Demand Capture",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_demand_dwnld,
			PW_CONFIG_END);

	btn_DemWork_download =pw_create_button(frame_download_secondpage, 30,90, 190, 40,
			PW_TEXT, " Work Allocation ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_alloc_dwnld,
			PW_CONFIG_END);

	btn_DemWorkRej_update = pw_create_button(frame_download_secondpage, 30,150, 190, 40,
			PW_TEXT, "Demand & Work Rejection Download",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_reject_dwnld,
			PW_CONFIG_END);


	btn_down_back =pw_create_button(frame_download_secondpage, 30,220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_main_back,
			PW_CONFIG_END);
	return ;
}


void show_frame_download_secondpage()
{
	if(! frame_download_secondpage)
	{
		create_frame_download_secondpage();
		pw_show(frame_download_secondpage);
	}
	else
		pw_show(frame_download_secondpage);
	return;
}
void hide_frame_download_secondpage(void)
{
	pw_hide(frame_download_secondpage);
	return;
}
void cb_work_alloc_details()
{
	hide_frame_download_mainpage();
	show_frame_download_secondpage();
	return;
}

int cb_wks_download_page()
{
	int res;
	ret= gp_test();
	res=gprs_works();
	return 0;
}

int cb_hh_download_page()
{
	int res;
	ret= gp_test();
	res=gprs_hh_info();
//	res=gprs_groups();
	return 0;
}
int cb_remaining_wrkd_page()
{
	gp_test();
	gprs_remaining_days_info();
	Wage_info_down();
	batch_info_dwn();
	return 0;
}
char ch1[3],wday[2];
int check_weekdays()
{
	char ch1[3]="%w",*wkday=NULL,query[100];
	sprintf(query,"select strftime('%s','now')",ch1);
	wkday=exec_query_get_single_value(wkday,query,DB_PACK);
	printf("query is %s",query);
	sprintf(wday,"%s",wkday);
	free(wkday);
	return 0;
}

int res_gp,res;
int ftp_upload()
{
	printf("\n res_gp is %d\n",res_gp);
	ret= gp_test();
	res=ftp_up();
	return 0;
}

int cb_wrkallocupload_page()
{
	int res;
	ret= gp_test();
	ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
			PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
	if(ret==PW_MSGBOX_RETURN_OK)
	{
		res=work_allocation_upload();
		return 0;
	}
	return 0;
}
int cb_demandupload_page()
{
	check_weekdays();
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	int res;
	ret= gp_test();
	ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
			PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
	if(ret==PW_MSGBOX_RETURN_OK)
	{
		res=demand_upload();
		return 0;
	}
	return 0;
}
int cb_nwd_up()
{
/*	check_weekdays();
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	char *cnt_demand=NULL,query[512];
	sprintf(query,"select count(*) from no_work_demand where status='N'");
	cnt_demand = exec_query_get_single_value(cnt_demand, query, DB_PACK);

	printf("day of week is %s\n",wday);
	if((atoi(wday)>1)&&(atoi(wday)<6)&&(atoi(cnt_demand)>0))
	{
		sprintf(query,"update no_work_demand set status='D' where status='N'");                                                                                                     printf("query update in uploadis %s\n",query);

		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
		sprintf(query,"update no_work_demand set status='R1' where status='R'");                                                                                                     printf("query update in uploadis %s\n",query);

		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

		return 0;
	}

	sqlite3_close(dbhandle);
*/
	int res;
	ret= gp_test();
	ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
			PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
	if(ret==PW_MSGBOX_RETURN_OK)
	{
		res = no_demand_up();
		return 0;
	}
	return 0;
}
int cb_labourupload_page()
{
	ret= gp_test();

	ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
			PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
	if(ret==PW_MSGBOX_RETURN_OK)
	{
		printf("\n res_gp is %d\n",res_gp);
		res=gprs();
		return 0;
	}
	return 0;
}
void create_frame_upload_options()
{
	frame_upload_options =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_upload_options);

	btn_upload_options[0] = pw_create_button(frame_upload_options, 30, 10, 180,30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Labour Report Upload",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_labourupload_page,
			PW_CONFIG_END);

	btn_upload_options[1] = pw_create_button(frame_upload_options, 30, 50, 180,30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "NO WORK",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_no_work,
			PW_CONFIG_END);

	btn_upload_options[2] = pw_create_button(frame_upload_options, 30, 90, 180,30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Demand Upload",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demandupload_page,
			PW_CONFIG_END);

	btn_upload_options[3] = pw_create_button(frame_upload_options, 30, 130, 180,30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Work Allocation Upload",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wrkallocupload_page,
			PW_CONFIG_END);


	btn_upload_options[4] = pw_create_button(frame_upload_options, 30, 170, 180,30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "No Work Demand Upload",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_nwd_up,
			PW_CONFIG_END);

	btn_upload_options_back = pw_create_button(frame_upload_options, 25, 230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_uploadmainpage_back_page,
			PW_CONFIG_END);
}
void show_upload_options()
{
	if(!frame_upload_options)
	{
		create_frame_upload_options();
		pw_show(frame_upload_options);
	}
	else
		pw_show(frame_upload_options);
}
void hide_frame_upload_options()
{
	pw_hide(frame_upload_options);
}
void cb_upload_page()
{
	hide_frame_gprs_page();
	show_upload_options();
}
void create_frame_download_mainpage(void)
{
	frame_download_mainpage =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_download_mainpage);
	btn_Hhdownload = pw_create_button(frame_download_mainpage, 30, 30, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_HH,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_hh_download_page,
			PW_CONFIG_END);
	btn_wksdownload = pw_create_button(frame_download_mainpage, 135, 30, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_WRKS,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wks_download_page,
			PW_CONFIG_END);
	btn_demandinfo = pw_create_button(frame_download_mainpage, 30, 145, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Demand/Work Aloc",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_work_alloc_details,
			PW_CONFIG_END);

	btn_wageinfodays = pw_create_button(frame_download_mainpage, 135, 145, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Mandays/Batch Info",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_remaining_wrkd_page,
			PW_CONFIG_END);

	btn_downloadmainpage_back = pw_create_button(frame_download_mainpage, 25, 230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_downloadmainpage_back_page,
			PW_CONFIG_END);
	return;
}
void show_frame_download_mainpage(void)
{
	if(!frame_download_mainpage)
	{
		create_frame_download_mainpage();
		pw_show(frame_download_mainpage);
	}
	else
		pw_show(frame_download_mainpage);
}
void hide_frame_download_mainpage(void)
{
	pw_hide(frame_download_mainpage);
}
void cb_download_mainpage(void)
{
	hide_frame_gprs_page();
	show_frame_download_mainpage();
	return;
}
void create_frame_gprs_page(void)
{
	frame_gprs_page = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_gprs_page);
	button[1] = pw_create_button(frame_gprs_page, 30, 20, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_UPLOAD,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_upload_page,
			PW_CONFIG_END);
	button[2] = pw_create_button(frame_gprs_page, 30, 135, 75, 75,     /*TO upload data and photos using pc internet connection										modified by 508316 */
			PW_TEXT, "  POS  Updates",
			PW_WRAP, PW_CHAR_WRAP,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FGCOLOR, "RED",
			PW_FONT, "Sans Bold 11",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pos_appsupdates,
			PW_CONFIG_END);

	button[3] = pw_create_button(frame_gprs_page, 130, 20, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_DWNLOAD,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_download_mainpage,
			PW_CONFIG_END);

	Butt_option_split_job = pw_create_button(frame_gprs_page,130, 135, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Split Job Cards",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_split_jobcard,
			PW_CONFIG_END);


	button[4] = pw_create_button(frame_gprs_page, 30, 220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_options_page,
			PW_CONFIG_END);
	return ;
}
void  show_frame_gprs_page(void)
{
	if(! frame_gprs_page)
	{
		create_frame_gprs_page();
		pw_show(frame_gprs_page);
	}
	else
		pw_show(frame_gprs_page);
	return;
}

void hide_frame_gprs_page(void)
{
	pw_hide(frame_gprs_page);
}
