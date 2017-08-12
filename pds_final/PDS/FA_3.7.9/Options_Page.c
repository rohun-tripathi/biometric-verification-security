#include "types.h"

void hide_frame_optionspage();
void show_frame_beneficary_details();
void show_frame_get_muster();
void show_frame_gprs_page();
void show_frame_mis_report();
void hide_frame_mcc_module();
int cb_update();
int get_reject_dwnld();
void hide_frame_fa_module();
void show_frame_loginpage();
void show_frame_pos_infodetails();
int get_alloc_dwnld();
void show_frame_mcclogin_page();
void show_fa_module();
void show_frame_loginmvo();
void show_progress_frame();
void hide_progress_frame();
void hide_frame_pos_updates();
void show_demand_options();
void show_frame_pos_updates();
int apps_download();
int demand_upload();
int get_demand_dwnld();
int batch_info_dwn();
int gp_test();
int work_allocation_upload();
pw_widget entry_birid[2],enroll_pswd,frame_birlogin_page,frame_enrolllogin_page,lbl_bir_login[2],enroll_userid,lbl_enroll_login[3],btn_bir_ok,btn_birback,btn_bio_login,btn_changepwd,btn_enroll_ok,btn_enroll_back, frame_changepwd_page,lbl_changepwd_login[4],btn_changepwd_ok,btn_changepwd_back,entry_changepwdid[4],Butt_option_update;

extern char get_date[15];
void hide_frame_birlogin_page();
void show_frame_birlogin_page();
void hide_frame_enrolllogin_page();
void show_frame_enrolllogin_page();
void hide_frame_changepwd_page();
void show_frame_changepwd_page();

void show_bir_module();
char *exec_query_get_single_value(char *data, char *query, char *database);
char *get_device_no();

char device_no[2],gp_code[10],username1[10],vers_no[10],sel_todt[11],query[1024];
char *get_device_no()
{
	return device_no;
}
char *get_Gp_code()
{
	return gp_code;
}
char *get_login_id()
{
	return (char *)pw_get_text(enroll_userid);
}

pw_widget Butt_option_muster , Butt_option_sync , Butt_option_pos,Butt_option_MIS,Butt_option_FA,Butt_option_MCC,frame_fa_module,Butt_option_back[2];
pw_widget frame_mcc_module,Butt_option_enroll,Butt_option_info,Butt_option_MVO,Butt_option_posbckp;
pw_widget frame_split_jobcard,btn_split_download,btn_split_update,Butt_option_split_job,btn_split_back;
void hide_frame_split_jobcard();
int gprs_hh_info();
extern char date[15];
void cb_dc()
{
	hide_frame_fa_module();
	char query[1024],*dwn_st_cnt=NULL;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);

	sprintf(query,"select ifnull(count(*),'0') from download_status where download_status='N'" );
	dwn_st_cnt = exec_query_get_single_value(dwn_st_cnt,query,DB_PACK);
	if(atoi(dwn_st_cnt)==1)
	{
		get_demand_dwnld();
		get_alloc_dwnld();
		get_reject_dwnld();
		gprs_hh_info();
			sprintf(query," delete from mcc_demand_group_details where household_code||worker_code||group_id NOT IN(select household_code||worker_code||group_id from household_details)");
        sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"update  download_status set download_status='Y'");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	sprintf(query,"delete from Demand_rejection_details where substr(todate,1,10)<current_date");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	sprintf(query,"delete from allocation_rejection_details where substr(alloc_to_date,1,10)<current_date");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	sprintf(query,"update Demand_Group_details set status='R' where Householdcode IN(select jobcard_no from Demand_rejection_details) and recepit_id IN(select substr(receipt_id,7,8) from Demand_rejection_details) and group_id IN(select group_id from Demand_rejection_details)");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	sprintf(query,"update work_allocation_details set status='R' where work_code||group_id||alloc_from_date||alloc_to_date||demand_dt||demand_frm_dt||demand_to_dt||substr(allocation_date,1,10) IN(select work_code||group_id||substr(alloc_from_date,1,10)||substr(alloc_to_date,1,10)||substr(demand_dt,1,10)||substr(demand_frm_dt,1,10)||substr(demand_to_dt,1,10)||substr(allocation_date,1,10) from allocation_rejection_details)");
         sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	sqlite3_close(dbhandle);
	show_demand_options();
}
void cb_mvo(void)
{
	hide_frame_optionspage();
	show_frame_loginmvo();

}
void cb_pos_info(void)
{
	hide_frame_fa_module();
	show_frame_pos_infodetails();//moves to Pos_infodetails.c file
}
void cb_mis_call(void)
{
	hide_frame_optionspage();
	show_frame_mis_report();
}
void cb_mcc_back(void)
{
	hide_frame_mcc_module();
	pw_show(frame_optionspage);
	return;
}
void cb_beneficary(void)
{
	hide_frame_mcc_module();
	show_frame_enrolllogin_page();
	return;
}
void cb_posmngt_call(void)
{
	hide_frame_mcc_module();
	show_frame_loginpage();
	return;
}
void cb_apps_update()
{
	show_progress_frame("Processing Please Wait");

	system("cp /mnt/disk/FA  /usr/local/apps/nrega_AP/");

	system("cp /mnt/disk1/FA  /usr/local/apps/nrega_AP/");

	system("cp /mnt/disk2/FA  /usr/local/apps/nrega_AP/");

	show_message_frame("APPLICATION UPDATED");
	system("killall -9 FA");

	hide_progress_frame();

	return;
}
void cb_pendrive_backup()
{	

	char photos_dt[25],file_path_pho[100];
	int disk,disk1,disk2,disk3;
	disk=system("test -d /mnt/disk");
	disk1=system("test -d /mnt/disk1");
	disk2=system("test -d /mnt/disk2");
	disk3=system("test -d /mnt/disk3");
	if(disk==0)  
	{
		show_progress_frame("Processing Please Wait");
		sprintf(photos_dt,"photos_backup_%s",date);
		sprintf(file_path_pho,"cp -r /home/Photos  /root/%s",photos_dt);
		printf("\n dbfile path is %s \n",file_path_pho);
		system(file_path_pho);
		system("rm /home/Photos/*");
		system("cp -r /root/* /mnt/disk");

		system("umount /mnt/disk*");
		system("rm -r /mnt/disk*");
		show_message_frame("Backup Taken");
		hide_progress_frame();
	}
	else if(disk1==0)
	{
		show_progress_frame("Processing Please Wait");
		sprintf(photos_dt,"photos_backup_%s",date);
		sprintf(file_path_pho,"cp -r /home/Photos  /root/%s",photos_dt);
		printf("\n dbfile path is %s \n",file_path_pho);
		system(file_path_pho);
		system("rm /home/Photos/*");
		system("cp -r /root/* /mnt/disk1");

		system("umount /mnt/disk*");
		system("rm -r /mnt/disk*");
		show_message_frame("Backup Taken");
		hide_progress_frame();
	}
	else if(disk2==0)
	{
		show_progress_frame("Processing Please Wait");
		sprintf(photos_dt,"photos_backup_%s",date);
		sprintf(file_path_pho,"cp -r /home/Photos  /root/%s",photos_dt);
		printf("\n dbfile path is %s \n",file_path_pho);
		system(file_path_pho);
		system("rm /home/Photos/*");
		system("cp -r /root/* /mnt/disk2");
		system("umount /mnt/disk*");
		system("rm -r /mnt/disk*");
		show_message_frame("Backup Taken");
		hide_progress_frame();
	}
	else if(disk3==0)
	{
		show_progress_frame("Processing Please Wait");
		sprintf(photos_dt,"photos_backup_%s",date);
		sprintf(file_path_pho,"cp -r /home/Photos  /root/%s",photos_dt);
		printf("\n dbfile path is %s \n",file_path_pho);
		system(file_path_pho);
		system("rm /home/Photos/*");
		system("cp -r /root/* /mnt/disk3");

		system("umount /mnt/disk*");
		system("rm -r /mnt/disk*");
		show_message_frame("Backup Taken");
		hide_progress_frame();
	}
	else
		show_message_frame("INSERT PENDRIVE");
	return;
}
void cb_delete_birs()
{
	hide_frame_pos_updates();
	show_frame_birlogin_page();
}

//------------------delete----photos------login------page------------------------------------
void entry_bir_back()
{
	hide_frame_birlogin_page();
	show_frame_pos_updates();
}
int entry_bir_event(pw_widget widget, pw_event event)
{
	char username[10], password[10];
	int  ret, ret1;
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "qwerty");
	strcpy(username, pw_get_text(entry_birid[1]));
	strcpy(password, pw_get_text(entry_birid[2]));

	printf("username = %s\n", username);
	printf("password = %s\n", password);
	ret=strcmp(username, "ph1221");
	printf("ret is %d\n", ret);
	ret1=strcmp(password, "ph1221");
	printf("ret1 is %d\n", ret1);

	if(ret==0 && ret1==0)
	{
		hide_frame_birlogin_page();
		int ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
				PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Delete Photos");
		if(ret==PW_MSGBOX_RETURN_OK)
		{
			show_progress_frame("Processing Please Wait");
			system("rm -r /root/*");
			hide_progress_frame();	 	

			show_frame_pos_updates();
			show_message_frame("photos deleted"); 
		}else
		{

			show_frame_pos_updates();
		}
	}
	else
	{
		show_message_frame("WRONG LOGIN TRY AGAIN");
		pw_entry_clear(entry_birid[2]);
		return -1;
	}

	return 0;
}
void create_frame_birlogin_page(void)
{
	frame_birlogin_page = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE,
			PW_CONFIG_END);
	pw_hide(frame_birlogin_page);
	lbl_bir_login[1] = pw_create_label(frame_birlogin_page, 5, 20, 70, 30,
			PW_TEXT, "UserId:",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_birid[1] = pw_create_entry(frame_birlogin_page, 100, 20, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha
			, PW_NA);
	pw_focus(entry_birid[1]);

	pw_entry_clear(entry_birid[1]);

	lbl_bir_login[2] = pw_create_label(frame_birlogin_page, 5, 70, 70, 30,
			PW_TEXT, "Password:",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_birid[2] = pw_create_entry(frame_birlogin_page, 100, 70, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha
			, PW_NA);

	pw_entry_passwdchar( entry_birid[2], '*' );
	pw_entry_clear(entry_birid[2]);

	btn_birback = pw_create_button(frame_birlogin_page, 40, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_bir_back,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);

	btn_bir_ok = pw_create_button(frame_birlogin_page, 140, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,

			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_OK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_bir_event,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);
}
void show_frame_birlogin_page(void)
{
	if(!frame_birlogin_page)
	{
		create_frame_birlogin_page();
		pw_show(frame_birlogin_page);
	}
	else
		pw_show(frame_birlogin_page);
	pw_entry_clear(entry_birid[1]);
	pw_entry_clear(entry_birid[2]);
	pw_focus(entry_birid[1]);
}

void hide_frame_birlogin_page(void)
{
	pw_hide(frame_birlogin_page);
}
//---------------------------------------------------------------------------------------------------
//----bio enroll login
void change_pwd()
{
	strcpy(username1, pw_get_text(enroll_userid));
	if(strcmp(username1," ")==0)
	{
		show_message_frame("Select a USER");
	}
	else
	{
		hide_frame_enrolllogin_page();
		show_frame_changepwd_page();
	}
}
void entry_enroll_back()
{

	hide_frame_enrolllogin_page();
	show_mcc_module();
}
int entry_enroll_ok()
{
	char password1[10],query[512],*pwd=NULL;
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "qwerty");
	strcpy(username1, pw_get_text(enroll_userid));
	strcpy(password1, pw_get_text(enroll_pswd));

	printf("username = %s\n", username1);
	printf("password = %s\n", password1);

	if(strcmp(username1,"")==0 || strcmp(password1,"")==0)
	{
		show_message_frame("Enter Login Details");
		return 0;
	}

	else
	{
		sprintf(query,"select password from enroll_login where user='%s'",username1);
		pwd=exec_query_get_single_value(pwd,query,DB_PACK);
		if(strcmp(pwd,password1)==0)
		{
			hide_frame_enrolllogin_page();
			show_frame_beneficary_details(TAG_ENROLL, "Next");

		}
		else
		{
			show_message_frame("WRONG LOGIN TRY AGAIN");
			pw_entry_clear(enroll_pswd);
			return -1;
		}
	}
	return 0;
}
void create_frame_enrolllogin_page(void)
{
	frame_enrolllogin_page = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE,
			PW_CONFIG_END);
	pw_hide(frame_enrolllogin_page);

	lbl_enroll_login[0] = pw_create_label(frame_enrolllogin_page, 00, 02, 240, 20,
			PW_TEXT, "ENROLLMENT LOGIN",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "GREEN",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	lbl_enroll_login[1] = pw_create_label(frame_enrolllogin_page, 5, 30, 70, 20,
			PW_TEXT, "User Id:",
			PW_FGCOLOR, "WHITE",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	enroll_userid = pw_create_combobox(frame_enrolllogin_page, 100, 30, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_insert(enroll_userid,"MPDO",'Y');
	pw_combobox_insert(enroll_userid,"APO",'Y');
	pw_combobox_insert(enroll_userid,"EC",'Y');	

	lbl_enroll_login[2] = pw_create_label(frame_enrolllogin_page, 5, 70, 70, 20,
			PW_TEXT, "Password:",
			PW_FGCOLOR, "WHITE",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	enroll_pswd = pw_create_entry(frame_enrolllogin_page, 100, 70, 80, 25,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha
			, PW_NA);

	pw_entry_passwdchar( enroll_pswd, '*' );
	pw_focus(enroll_pswd);	
	pw_entry_clear(enroll_pswd);

	btn_enroll_back = pw_create_button(frame_enrolllogin_page, 40, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_enroll_back,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);

	btn_enroll_ok = pw_create_button(frame_enrolllogin_page, 140, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_OK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_enroll_ok,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);
	btn_changepwd = pw_create_button(frame_enrolllogin_page, 35, 200, 74, 37,
			PW_TEXT,"Change Password",			
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, change_pwd,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);

	btn_bio_login = pw_create_button(frame_enrolllogin_page, 135, 200, 74, 37,
			PW_TEXT,"BIO LOGIN",			
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);


}
void show_frame_enrolllogin_page(void)
{
	if(!frame_enrolllogin_page)
	{
		create_frame_enrolllogin_page();
		pw_show(frame_enrolllogin_page);

	}
	else
		pw_show(frame_enrolllogin_page);

	pw_hide(btn_bio_login);
	pw_entry_clear(enroll_pswd);
	pw_focus(enroll_pswd);
}

void hide_frame_enrolllogin_page(void)
{
	pw_hide(frame_enrolllogin_page);
}

//------------
//-----------change password page
void entry_changepwd_event()
{
	char  query[512],old[10],new1[10],new2[10],*oldpwd=NULL;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	strcpy(old,pw_get_text(entry_changepwdid[1]));
	strcpy(new1,pw_get_text(entry_changepwdid[2]));
	strcpy(new2,pw_get_text(entry_changepwdid[3]));

	if(strcmp(old,"")==0)
	{
		show_message_frame("Enter Old Password");
		pw_entry_clear(entry_changepwdid[2]);
		pw_entry_clear(entry_changepwdid[3]);
		pw_focus(entry_changepwdid[1]);
	} 
	else
	{
		sprintf(query,"select password from enroll_login where user='%s'",username1);
		oldpwd=exec_query_get_single_value(oldpwd,query,DB_PACK);
		printf("query id '%s",query);
		if(strcmp(old,oldpwd)==0)
		{
			if(strlen(new1)<6)
			{
				show_message_frame("Password Mustbe mininum 6 Digit");
				pw_entry_clear(entry_changepwdid[2]);
				pw_entry_clear(entry_changepwdid[3]);
				pw_focus(entry_changepwdid[2]);		
			}
			else
			{
				if(strcmp(new1,new2)==0)
				{
					sprintf(query,"update enroll_login set password='%s' where user='%s'",new2,username1);
					sqlite3_exec(dbhandle,query,NULL,NULL,NULL);
					show_message_frame("Successfully Updated");
					pw_entry_clear(entry_changepwdid[1]);
					pw_entry_clear(entry_changepwdid[2]);
					pw_entry_clear(entry_changepwdid[3]);
					return ;
				}
				else
				{
					show_message_frame("Enter Same Password Again");
					pw_entry_clear(entry_changepwdid[2]);
					pw_entry_clear(entry_changepwdid[3]);
					pw_focus(entry_changepwdid[2]);			
				}	
			}
		}
		else
		{
			show_message_frame("Enter Old Pwd Correctly");
			pw_entry_clear(entry_changepwdid[1]);
			pw_entry_clear(entry_changepwdid[2]);
			pw_entry_clear(entry_changepwdid[3]);
			pw_focus(entry_changepwdid[1]);
		}
	}
}
void entry_changepwd_back()
{
	hide_frame_changepwd_page();
	show_frame_enrolllogin_page();
}
void create_frame_changepwd_page(void)
{
	frame_changepwd_page = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE,
			PW_CONFIG_END);
	pw_hide(frame_changepwd_page);
	lbl_changepwd_login[1] = pw_create_label(frame_changepwd_page, 5, 20, 70, 30,
			PW_TEXT, "Old Password",
			PW_FGCOLOR,"white",
			PW_BGCOLOR, BLUE,
			PW_CONFIG_END);

	entry_changepwdid[1] = pw_create_entry(frame_changepwd_page, 100, 20, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha,
			PW_NA);

	pw_entry_passwdchar( entry_changepwdid[1], '*' );

	lbl_changepwd_login[2] = pw_create_label(frame_changepwd_page, 5, 70, 70, 30,
			PW_TEXT, "New Password ",
			PW_FGCOLOR,"white",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_changepwdid[2] = pw_create_entry(frame_changepwd_page, 100, 70, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha,
			PW_NA);

	pw_entry_passwdchar( entry_changepwdid[2], '*' );
	pw_focus(entry_changepwdid[1]);	

	lbl_changepwd_login[3] = pw_create_label(frame_changepwd_page, 5, 120, 70, 30,
			PW_TEXT, "Confirm Password",
			PW_FGCOLOR,"white",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);

	entry_changepwdid[3] = pw_create_entry(frame_changepwd_page, 100, 120, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha,
			PW_NA);
	pw_entry_passwdchar( entry_changepwdid[3], '*' );

	btn_changepwd_back = pw_create_button(frame_changepwd_page, 40, 190, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_changepwd_back,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);

	btn_changepwd_ok = pw_create_button(frame_changepwd_page, 140, 190, 70, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT,"UPDATE",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_changepwd_event,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);
}
void show_frame_changepwd_page(void)
{
	if(!frame_changepwd_page)
	{
		create_frame_changepwd_page();
		pw_show(frame_changepwd_page);
	}
	else
		pw_show(frame_changepwd_page);

	pw_entry_clear(entry_changepwdid[1]);
	pw_focus(entry_changepwdid[1]);
}

void hide_frame_changepwd_page(void)
{
	pw_hide(frame_changepwd_page);
}
void cb_pos_updates_back()
{
	hide_frame_pos_updates();
	show_mcc_module();
	return;
}
pw_widget frame_pos_updates,btn_pos_updates[6];
void create_frame_pos_updates()
{
	frame_pos_updates =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_pos_updates);

	btn_pos_updates[0] =pw_create_button(frame_pos_updates, 30,40, 190, 35,
			PW_TEXT, "Photos Backup",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pendrive_backup,
			PW_CONFIG_END);
	btn_pos_updates[2] =pw_create_button(frame_pos_updates, 30,110, 190, 35,
			PW_TEXT, "Version Update(PenDrive)",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_apps_update,
			PW_CONFIG_END);

	btn_pos_updates[3] = pw_create_button(frame_pos_updates, 20, 230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pos_updates_back,
			PW_CONFIG_END);

	btn_pos_updates[4] =pw_create_button(frame_pos_updates, 30,180, 190, 35,
			PW_TEXT, "Delete Photos",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_delete_birs,
			PW_CONFIG_END);
	return;

}

void show_frame_pos_updates(void)
{
	if(!frame_pos_updates){
		create_frame_pos_updates();
		pw_show(frame_pos_updates);
	}
	else
		pw_show(frame_pos_updates);

	return;
}
void hide_frame_pos_updates()
{
	pw_hide(frame_pos_updates);
	return;
}
void cb_pos_backup()
{
	hide_frame_mcc_module();
	show_frame_pos_updates();
	return ;
}

void create_frame_mcc_module(void)
{
	frame_mcc_module = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_mcc_module);
	Butt_option_update = pw_create_button(frame_mcc_module,30, 20, 75, 75, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Update", 
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_update, 
			PW_CONFIG_END);

	Butt_option_pos = pw_create_button(frame_mcc_module, 135, 20, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_POS,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_posmngt_call,
			PW_CONFIG_END);
	Butt_option_enroll =pw_create_button(frame_mcc_module,135, 125, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_ENROLL,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_beneficary,
			PW_CONFIG_END);

	Butt_option_posbckp = pw_create_button(frame_mcc_module,30, 125, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "POS Backup",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pos_backup,
			PW_CONFIG_END);

	Butt_option_back[1] = pw_create_button(frame_mcc_module, 30, 230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_mcc_back,
			PW_CONFIG_END);
	return;
}
void show_mcc_module(void)
{
	if(! frame_mcc_module)
	{
		create_frame_mcc_module();
		pw_show(frame_mcc_module);
	}
	else
		pw_show(frame_mcc_module);
}
void hide_frame_mcc_module(void)
{
	pw_hide(frame_mcc_module);
}
void cb_mcc()
{
	hide_frame_optionspage();
	show_frame_mcclogin_page();
	return;
}
void cb_options_back()
{	
	hide_frame_fa_module();
	pw_show(frame_optionspage);
	uninit_fps(fps);
	return;
}
int upload_demand()
{
	char *status_demand_up=NULL,*status_alloc_up=NULL,query[125];
	sprintf(query,"select ifnull(count(*),'0') from Demand_Group_details  where status='N'");
	status_demand_up =exec_query_get_single_value(status_demand_up,query,DB_PACK);

	sprintf(query,"select ifnull(count(*),'0') from work_allocation_details  where status='N'");
	status_alloc_up =exec_query_get_single_value(status_alloc_up,query,DB_PACK);
	if(atoi(status_demand_up)>0)
	{
		demand_upload();
		return 0;
	}
	if(atoi(status_alloc_up)>0)
	{
	work_allocation_upload();
	return 0 ;
	}
	free(status_demand_up);
	free(status_alloc_up);
	return 0;
}
int check_date()
{
	char *max_date=NULL,*sun_date=NULL,query[125];

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	sprintf(query,"select date(current_date,'weekday 0')");
	sun_date= exec_query_get_single_value(sun_date, query, DB_PACK);
	if(strcmp(sun_date,get_date)==0)
	{
		show_progress_frame("SUNDAY MUSTER NOT ALLOWED IN POS ");
		sleep(3);
		hide_progress_frame();
		system("killall -9 FA");
		return 0 ;
	}
	sprintf(query,"select ifnull(max(substr(created_date,1,10)),'0') <=current_date from muster_attendance");
	max_date = exec_query_get_single_value(max_date, query, DB_PACK);
	if(atoi(max_date)==0)
	{
		show_progress_frame(" WRONG DATE IN POS,SET DATE ");
		sleep(4);
		hide_progress_frame();
		system("killall -9 FA");
		return 0 ;
	}
	sqlite3_close(dbhandle);

	free(max_date);
	free(sun_date);
	return 0;
}
int weekday();
void demad_delay()
{
	char *cnt_demand=NULL,query[512];
	sqlite3 *dbhandle=NULL;

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select count(*) from Demand_Group_details where status='N'");
	cnt_demand = exec_query_get_single_value(cnt_demand, query, DB_PACK);
	if(atoi(cnt_demand)>0)
	{
		sprintf(query,"update Demand_Group_details set status='D' where status='N'");                                                                                                     printf("query update in uploadis %s\n",query);

		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

		sprintf(query,"update Demand_Group_details set status='R1' where status='R'");                                                                                                     printf("query update in uploadis %s\n",query);

		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

		sqlite3_close(dbhandle);
		return ;
	}
	char * cnt_alloc=NULL;
	sprintf(query,"select count(*) from work_allocation_details where status='N'");
	cnt_alloc =  exec_query_get_single_value(cnt_alloc, query, DB_PACK);
	if(atoi(cnt_alloc)>0)
	{
		sprintf(query,"update work_allocation_details set status='D' where status='N'");
		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
		sqlite3_close(dbhandle);
		return ;
	}
	free(cnt_alloc);
	char *cnt_demand1=NULL;
	sprintf(query,"select count(*) from no_work_demand where status='N'");
	cnt_demand1 = exec_query_get_single_value(cnt_demand1, query, DB_PACK);

	if(atoi(cnt_demand1)>0)
	{
		sprintf(query,"update no_work_demand set status='D' where status='N'");                                                                                                     printf("query update in uploadis %s\n",query);

		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
		sprintf(query,"update no_work_demand set status='R1' where status='R'");                                                                                                     printf("query update in uploadis %s\n",query);

		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

		sqlite3_close(dbhandle);
		return ;
	}
	free(cnt_demand1);
}
void cb_muster_roll(void)
{
	check_date();
	int wd =weekday();
	char *chk_batch=NULL,query[250];
        sprintf(query,"select ifnull(Batch_type,'0') from Batch_info");
        chk_batch = exec_query_get_single_value(chk_batch,query, DB_PACK);
	if(strcmp(chk_batch,"B1")==0)
	{
	if(wd==4||wd==5||wd==6||wd==0)
		upload_demand();
	else
	demad_delay();
	}
	else
	{
	if(wd==6||wd==0||wd==1||wd==2||wd==3)
                upload_demand();
	else
	demad_delay();
	}
	free(chk_batch);
	hide_frame_fa_module();
	show_frame_get_muster();
	return;
}
void cb_gprs_call(void)
{
	hide_frame_fa_module();
	show_frame_gprs_page();
	return;
}
pw_widget Butt_option_demandcapture;
void create_frame_fa_module(void)
{
	frame_fa_module = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_fa_module);

	Butt_option_muster = pw_create_button(frame_fa_module ,30, 30, 75, 75, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER, 
			PW_FONT, PW_BOLD_FONT, 
			PW_NORMAL_IMAGE,BUTT_MUSTER,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_muster_roll, 
			PW_CONFIG_END);
	Butt_option_demandcapture = pw_create_button(frame_fa_module ,135, 30, 75, 75,
			PW_TEXT, "Demand & Allocation",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_dc,
			PW_CONFIG_END);

	Butt_option_info = pw_create_button(frame_fa_module,  30, 135, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE, BUTT_POSINFO,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pos_info,
			PW_CONFIG_END);

	Butt_option_sync = pw_create_button(frame_fa_module, 135, 135, 75, 75, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER, 
			PW_FONT, PW_BOLD_FONT, 
			PW_NORMAL_IMAGE,BUTT_SYNC,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_gprs_call, 
			PW_CONFIG_END);

	Butt_option_back[0] = pw_create_button(frame_fa_module, 30, 225, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_options_back,
			PW_CONFIG_END);
	return ;
}
void show_fa_module(void)
{
	if(! frame_fa_module)
	{
		create_frame_fa_module();
		pw_show(frame_fa_module);
	}
	else
		pw_show(frame_fa_module);
	return;
}

void hide_frame_fa_module(void)
{
	pw_hide(frame_fa_module);
}
int cb_hh_download_page();
int cb_wks_download_page();
int cb_work_alloc_details();
int cb_remaining_wrkd_page();

//  query is create table weekly_download(next_download_date varchar(15),downloaded_date default current_date);
// to be updated in database (in mainpage)
int weekly_status()
{
	sqlite3 *dbhandle=NULL;

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	char *del_fp_cnt=NULL;
	sprintf(query,"select ifnull(count(*),'0') from weekly_download");
	del_fp_cnt = exec_query_get_single_value(del_fp_cnt,query,DB_PACK);
	if(atoi(del_fp_cnt)==0)
	{
		cb_remaining_wrkd_page();
		get_demand_dwnld();
		get_alloc_dwnld();
		cb_hh_download_page();
		cb_wks_download_page();
		get_reject_dwnld();
		sprintf(query,"insert into weekly_download(next_download_date)values('%s')",sel_todt);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	else
	{
		char *next_dwndt=NULL,*ck_dwndt=NULL;
		sprintf(query,"select ifnull(next_download_date,'0') from weekly_download");
		next_dwndt = exec_query_get_single_value(next_dwndt, query, DB_PACK);

		sprintf(query,"select (case when '%s'>current_date then 'NOK' else 'OK' end)",next_dwndt);
		ck_dwndt = exec_query_get_single_value(ck_dwndt, query, DB_PACK);
		free(next_dwndt);
		if(strcmp(ck_dwndt,"OK")==0)
		{
			cb_remaining_wrkd_page();
			get_demand_dwnld();
			get_alloc_dwnld();
			cb_hh_download_page();
			cb_wks_download_page();
			get_reject_dwnld();
			sprintf(query,"update weekly_download  set next_download_date='%s',downloaded_date=current_date",sel_todt);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
			printf("update query1 = %s \n", query);
		}

		free(ck_dwndt);
	}
	return 0;

}
int daily_download_status()
{
	char *batch_check=NULL,*day_test=NULL,*mon_date=NULL,*frm_date=NULL,*mon_date1=NULL,*frm_date1=NULL,sel_day_test[5];
	day_test =exec_query_get_single_value(day_test,"select strftime('%w','now')",DB_PACK);
	sprintf(sel_day_test, "%s", day_test);
	free(day_test);
	gp_test();
	batch_info_dwn();
	sprintf(query,"select distinct batch_type from batch_info");
	batch_check=exec_query_get_single_value(batch_check,query,DB_PACK);
	if(strcmp(batch_check,"B1")==0)
	{
		if(atoi(sel_day_test)==1)
		{

			sprintf(query,"select date('now','weekday 1','+7 days')");
			mon_date=exec_query_get_single_value(mon_date, query, DB_PACK);
			sprintf(sel_todt, "%s", mon_date);
			free(mon_date);

		}

		else
		{
			sprintf(query, "select date('now','weekday 1')");
			frm_date = exec_query_get_single_value(frm_date, query, DB_PACK);
			sprintf(sel_todt, "%s", frm_date);
			free(frm_date);
		}
	}
	else
	{
		if(atoi(sel_day_test)==4)
		{
			printf("sel_day_test is %s\n",sel_day_test);
			sprintf(query,"select date('now','weekday 4','+7 days')");
			mon_date1=exec_query_get_single_value(mon_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", mon_date1);
			free(mon_date1);
		}
		else
		{
			sprintf(query, "select date('now','weekday 4')");
			frm_date1 = exec_query_get_single_value(frm_date, query, DB_PACK);

			sprintf(sel_todt, "%s", frm_date1);
			free(frm_date1);
		}
	}
	weekly_status();

	return 0;
}
void cb_fa(void)
{
	char *dwn_dt=NULL,*ck_dwndt=NULL,*dwn_st_cnt=NULL;
	char *query_value=NULL,query[256];
	char *dev_no=NULL;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info;");
	query_value = exec_query_get_single_value(query_value, query, DB_PACK);
	sprintf(gp_code,"%s",query_value);

	sprintf(query,"select substr(jc_code,10,1) from jc_info");
	dev_no = exec_query_get_single_value(dev_no,query,DB_PACK);     
	sprintf(device_no,"%s",dev_no);
	sprintf(query,"select ifnull(count(*),'0') from download_status");
	dwn_st_cnt = exec_query_get_single_value(dwn_st_cnt,query,DB_PACK);
	if(atoi(dwn_st_cnt)==0)
	{
		daily_download_status();
		sprintf(query,"insert into download_status(download_status)values('%s')","Y");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		printf("insert of download_status is %s\n",query);
		return ;
	}
	sprintf(query,"select ifnull(download_date,'0') from download_status");
	dwn_dt =exec_query_get_single_value(dwn_dt, query, DB_PACK);
	sprintf(query,"select (case when '%s'!=current_date then 'NOK' else 'OK' end)",dwn_dt);
	ck_dwndt = exec_query_get_single_value(ck_dwndt, query, DB_PACK);

	if(strcmp(ck_dwndt,"NOK")==0)
	{
		sprintf(query,"delete from download_status");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		daily_download_status();
		sprintf(query,"insert into download_status(download_status)values('%s')","N");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	//TODO:Download newversion
	char *ck_verstatus=NULL;
	sprintf(query,"select (case when '%s'<(select max(version_no) from Batch_info) then 'OK' else 'NOK' end)",vers_no);
	ck_verstatus=exec_query_get_single_value(ck_verstatus,query, DB_PACK);
	if(strcmp(ck_verstatus,"OK")==0)
	{
	show_progress_frame("downloading application \n Plz wait");
	apps_download();
	hide_progress_frame();
	}	
	free(ck_verstatus);
	free(dwn_dt);
	free(ck_dwndt);
	free(dwn_st_cnt);
	sqlite3_close(dbhandle);
	hide_frame_optionspage();
	show_fa_module();
}
void create_frame_optionspage(void)
{
	frame_optionspage = pw_create_frame(toplevel, 0, 0, 240, 270, 
			PW_BGCOLOR, VIOLET , 
			PW_CONFIG_END);
	pw_hide(frame_optionspage);

	Butt_option_FA = pw_create_button(frame_optionspage, 30, 30, 75, 75, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER, 
			PW_FONT, PW_BOLD_FONT, 
			PW_NORMAL_IMAGE,BUTT_FA ,          
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_fa, 
			PW_CONFIG_END);
	Butt_option_MVO = pw_create_button(frame_optionspage, 135, 30, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "MVO",
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_MVO,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_mvo,
			PW_CONFIG_END);

	Butt_option_MCC = pw_create_button(frame_optionspage,30, 145, 75, 75, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER, 
			PW_FONT, PW_BOLD_FONT, 
			PW_NORMAL_IMAGE,BUTT_MCC,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_mcc, 
			PW_CONFIG_END);

	Butt_option_MIS = pw_create_button(frame_optionspage, 135, 145, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_MIS,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_mis_call,
			PW_CONFIG_END);
	return;
}

void show_frame_optionspage(void)
{
	if(! frame_optionspage)
	{
		create_frame_optionspage();
		pw_show(frame_optionspage);
	}
	else
		pw_show(frame_optionspage);
}
void hide_frame_optionspage(void)
{
	pw_hide(frame_optionspage);
}
