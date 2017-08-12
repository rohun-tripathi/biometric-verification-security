#include "types.h"

int gp_test();
int Mvo_upload();
int  Gps_Test(double *lat, double *lon);
int Mvo_gprs_conn();
char *exec_query_get_single_value(char *data, char *query, char *database);
int txtlb_get_element();
int exec_query_load_into_lbox_double();
int exec_query_load_into_combo();
void hide_frame_muster_verify();
void show_progress_frame();
void hide_progress_frame();
void show_frame_optionspage();
pw_widget frame_muster_verify,label_muster_verify[5],combo_verify_wkcode,Butt_muster_verify[8],label_get_verify[2],Butt_verify,frame_muster_verify_details,lbox_musterverify_grpdet,combo_verify_grp,combo_verify_hab,lbl_muster_chk[12],frame_gp_report;

char wg_cnt[10],crbar_cnt[10];
double lat, lon;

char query[1024], dt_ass_man_pan[13],village_id[6],habit_id[6],sel_remark[50],sel_workid[10],sel_verifygrp[50],group_id[15];

void cb_verify_details_back()
{
	pw_set_text(combo_verify_grp,"Select");
	crbar_cnt[0]='\0';
	wg_cnt[0]='\0';
	pw_set_text(label_get_verify[1],"");
	pw_set_text(label_get_verify[2],"");
	pw_hide(frame_muster_verify_details);
	pw_show(frame_muster_verify);
}
int get_finalmuster_verify_details()
{
	if(strcmp(sel_remark,"")==0)
	{
		show_message_frame("SELECT REMARKS");
		return 0;		
	}
	char * mvo_count=NULL;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	sprintf(query,"select ifnull(count(*),'0') from MVO_remarks where substr(created_date,1,10)=current_date");
	mvo_count = exec_query_get_single_value(mvo_count, query, DB_PACK);	
	if(atoi(mvo_count)>0)
	{
		ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
				PW_MSGBOX_ICON_QUESTION, "Alert", "REMARKS TAKEN DO U WANT TO UPADTE");

		if(ret==PW_MSGBOX_RETURN_OK)
		{
			sprintf(query,"update MVO_remarks set remarks='%s',status='N' where substr(created_date,1,10)=current_date",sel_remark);

			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);	
			show_message_frame("UPDATED");

			return 0;
		}
		else
		{
			return -1;
		}
	}

	sprintf(query,"insert into MVO_remarks(remarks,status) values('%s','%s')",sel_remark,"N");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);
	show_message_frame("SUBMITTED");
	free(mvo_count);
	return 0;
}

Bool get_update_details(pw_widget wdgt, int idx)
{
	int *arr;
	int sel_count, i;
	char *p_id=NULL;
	char *job_no=NULL,*name=NULL;

	sel_count = pw_tlistbox_get_multiple_selection(lbox_musterverify_grpdet, &arr);
	if(sel_count==0)
	{
		show_message_frame("SELECT ATTENDEE");
		return 0;
	}
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	for (i=0; i < sel_count; i++)
	{
			show_progress_frame("Processing Please Wait");
		txtlb_get_element(lbox_musterverify_grpdet, &job_no, arr[i], 0);
		txtlb_get_element(lbox_musterverify_grpdet, &name, arr[i], 1);

		sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
		p_id = exec_query_get_single_value(p_id, query, DB_PACK);
		Gps_Test(&lat, &lon);
		sprintf(query,"insert into MVO_Details(pid,jobcardno,name,group_name,group_id,work_code,status,Gpslongitude,Gpslatitude,crowbar,mvo_status) values('%s','%s%s%s','%s','%s','%s','%s%s%s','%s','%lf','%lf','%s','%s')",p_id,dt_ass_man_pan,habit_id,job_no,name,sel_verifygrp,group_id,dt_ass_man_pan,village_id,sel_workid,"N",lon,lat,"N","A");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"delete from MVO_Details where rowid not in(select max(rowid) from MVO_Details group by pid,jobcardno,work_code,substr(created_date,1,10))");

		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	}
	pw_tlistbox_clear(lbox_musterverify_grpdet);

	sprintf(query,"select JobCard_id as jobcard_id,member_name from (select substr(jobcardno,15,6) as Jobcard_id,member_name from muster_attendance where group_name='%s'and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date) as a where jobcard_id not in (select substr(jobcardno,15,6) from mvo_details where group_name='%s' and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date)",sel_verifygrp,sel_workid,sel_verifygrp,sel_workid);

	exec_query_load_into_lbox_double(lbox_musterverify_grpdet, query, DB_PACK);
	sqlite3_close(dbhandle);
	hide_progress_frame();
	show_message_frame("UPDATED");
	free(job_no);
	free(name);
	free(arr);
	return(False);
}
Bool  get_crbr_update_details(pw_widget wdgt, int idx)
{
	int *arr;
	int sel_count, i;
	char *p_id=NULL;
	char *str_jc=NULL,*str_name=NULL;
	sel_count = pw_tlistbox_get_multiple_selection(lbox_musterverify_grpdet, &arr);

	if(sel_count==0)
	{
		show_message_frame("SELECT ATTENDEE");
		return 0;
	}
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}

	for (i=0; i < sel_count; i++)
	{
		show_progress_frame("Processing Please Wait");

		txtlb_get_element(lbox_musterverify_grpdet, &str_jc, arr[i], 0);

		txtlb_get_element(lbox_musterverify_grpdet, &str_name, arr[i], 1);

		sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");

		p_id = exec_query_get_single_value(p_id, query, DB_PACK);

		Gps_Test(&lat, &lon);

		sprintf(query,"insert into MVO_Details(pid,jobcardno,name,group_name,group_id,work_code,status,Gpslongitude,Gpslatitude,crowbar,mvo_status) values('%s','%s%s%s','%s','%s','%s','%s%s%s','%s','%lf','%lf','%s','%s')",p_id,dt_ass_man_pan,habit_id,str_jc,str_name,sel_verifygrp,group_id,dt_ass_man_pan,village_id,sel_workid,"N",lon,lat,"N","C");

		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"delete from MVO_Details where rowid not in(select max(rowid) from MVO_Details group by pid,jobcardno,work_code,substr(created_date,1,10))");

		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	}

	pw_tlistbox_clear(lbox_musterverify_grpdet);

	sprintf(query,"select JobCard_id as jobcard_id,member_name from (select substr(jobcardno,15,6) as Jobcard_id,member_name from muster_attendance where group_name='%s'and substr(work_code,13,6)='%s' and crowbar='Y' and substr(created_date,1,10)=current_date ) as a where jobcard_id not in (select substr(jobcardno,15,6) from mvo_details where group_name='%s' and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date)",sel_verifygrp,sel_workid,sel_verifygrp,sel_workid);

	exec_query_load_into_lbox_double(lbox_musterverify_grpdet, query, DB_PACK);
	sqlite3_close(dbhandle);

	hide_progress_frame();

	show_message_frame("UPDATED");

	free(str_jc);
	free(str_name);
	free(arr);
	return(False);
}

int get_verify_details()
{
	pw_tlistbox_clear(lbox_musterverify_grpdet);

	sprintf(query,"select JobCard_id as jobcard_id,member_name from (select substr(jobcardno,15,6) as Jobcard_id,member_name from muster_attendance where group_name='%s'and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date) as a where jobcard_id not in (select substr(jobcardno,15,6) from mvo_details where group_name='%s' and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date and mvo_status='A')",sel_verifygrp,sel_workid,sel_verifygrp,sel_workid);

	exec_query_load_into_lbox_double(lbox_musterverify_grpdet, query, DB_PACK);

	return 0;
}
void create_frame_muster_verify_details(void)
{
	frame_muster_verify_details = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_muster_verify_details);
	lbl_muster_chk[10] =  pw_create_label(frame_muster_verify_details, 05, 05, 230, 35,
			PW_TEXT, "",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CONFIG_END);

	lbox_musterverify_grpdet = pw_create_tlistbox(frame_muster_verify_details, 0, 50, 240, 160,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	pw_tlistbox_addcolumn(lbox_musterverify_grpdet, 160);
	pw_tlistbox_set_selection_type(lbox_musterverify_grpdet,
			PW_TLISTBOX_MULTIPLE_SELECTION);

	Butt_verify = pw_create_button(frame_muster_verify_details, 10, 225,34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_verify_details_back ,
			PW_CONFIG_END);

	Butt_muster_verify[7]= pw_create_button(frame_muster_verify_details, 100,225, 120, 35,
			PW_TEXT, "Delete Wage Seeker",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_update_details,
			PW_CONFIG_END);

	Butt_muster_verify[8]= pw_create_button(frame_muster_verify_details, 154,225, 70, 35,
			PW_TEXT, "Delete",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_crbr_update_details,
			PW_CONFIG_END);
	return;
}
int get_crbar_verify_details()
{
	if(atoi(crbar_cnt)==0)
	{
		show_message_frame("NO DATA");
		return -1;
	}
	if (!frame_muster_verify_details)
	{
		create_frame_muster_verify_details();
		pw_show(frame_muster_verify_details);
		pw_hide(Butt_muster_verify[7]);
	}
	else
		pw_show(frame_muster_verify_details);
	pw_hide(Butt_muster_verify[7]);
	pw_show(Butt_muster_verify[8]);

	pw_set_text(lbl_muster_chk[10],sel_verifygrp);

	pw_tlistbox_clear(lbox_musterverify_grpdet);

	sprintf(query,"select JobCard_id as jobcard_id,member_name from (select substr(jobcardno,15,6) as Jobcard_id,member_name from muster_attendance where group_name='%s'and substr(work_code,13,6)='%s' and crowbar='Y' and substr(created_date,1,10)=current_date) as a where jobcard_id not in (select substr(jobcardno,15,6) from mvo_details where group_name='%s' and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date)",sel_verifygrp,sel_workid,sel_verifygrp,sel_workid);

	exec_query_load_into_lbox_double(lbox_musterverify_grpdet, query, DB_PACK);
	return 0;
}
int get_muster_verify_details()
{
	if((strcmp(habit_id,"")==0) || (strcmp(sel_workid,"")==0) || (strcmp(sel_verifygrp,"")==0))
	{
		show_message_frame("SELECT DATA");
		return 0;
	}
	if (!frame_muster_verify_details)
	{
		create_frame_muster_verify_details();
		pw_show(frame_muster_verify_details);
		pw_hide(Butt_muster_verify[8]);
	}
	else
		pw_show(frame_muster_verify_details);
	pw_hide(Butt_muster_verify[8]);
	pw_show(Butt_muster_verify[7]);
	pw_set_text(lbl_muster_chk[10],sel_verifygrp);
	get_verify_details();
	return 0;
}
int get_verify_present()
{
	char *wgseeker_vrfycount=NULL,*crowbar_vrfycount=NULL;

	sprintf(query,"select count(*) from (select substr(jobcardno,15,6) as Jobcard_id,member_name from muster_attendance where group_name='%s'and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date) as a where jobcard_id not in (select substr(jobcardno,15,6) from mvo_details where group_name='%s' and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date and mvo_status='A')",sel_verifygrp,sel_workid,sel_verifygrp,sel_workid);

	wgseeker_vrfycount = exec_query_get_single_value(wgseeker_vrfycount, query, DB_PACK);

	sprintf(wg_cnt,"%s",wgseeker_vrfycount);

	sprintf(query,"select count(*) from (select substr(jobcardno,15,6) as Jobcard_id,member_name from muster_attendance where group_name='%s'and substr(work_code,13,6)='%s' and crowbar='Y' and substr(created_date,1,10)=current_date) as a where jobcard_id not in (select substr(jobcardno,15,6) from mvo_details where group_name='%s' and substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date )",sel_verifygrp,sel_workid,sel_verifygrp,sel_workid);

	crowbar_vrfycount = exec_query_get_single_value(wgseeker_vrfycount, query, DB_PACK);
	sprintf(crbar_cnt,"%s",crowbar_vrfycount);
	free(wgseeker_vrfycount);
	free(crowbar_vrfycount);
	return 0;
}
pw_widget cmb;
void sel_combo_remarks(pw_widget widget, int index)
{
	unsigned idx;
	char *remarks=NULL;
	pw_combobox_get_selection(cmb, &remarks, &idx);
	strcpy(sel_remark,remarks);
	free(remarks);
	return;
}
void combo_grp_verify(pw_widget widget, int index)
{
	unsigned idx;
	char *selgrp=NULL;
	pw_combobox_get_selection(combo_verify_grp, &selgrp, &idx);
	strcpy(sel_verifygrp,selgrp);
	free(selgrp);
	char *grp_id=NULL;
	sprintf(query,"select distinct group_id from muster_attendance where group_name='%s'",sel_verifygrp);
	grp_id=exec_query_get_single_value(grp_id, query, DB_PACK);
	sprintf(group_id,"%s",grp_id);
	get_verify_present();
	pw_set_text(label_get_verify[1],wg_cnt);
	pw_set_text(label_get_verify[2],crbar_cnt);
	free(selgrp);
	free(grp_id);
	return ;
}
void combo_wrkid_verify(pw_widget widget, int index)
{
	unsigned idx;
	char *workcode=NULL;
	pw_combobox_get_selection(combo_verify_wkcode, &workcode, &idx);
	strcpy(sel_workid, workcode);
	free(workcode);
	pw_combobox_clear(combo_verify_grp);
	sprintf(query,"select distinct group_name from muster_attendance where substr(work_code,13,6)='%s' and substr(created_date,1,10)=current_date", sel_workid);
	exec_query_load_into_combo(combo_verify_grp, query, DB_PACK);
	free(workcode);
	return ;
}
void get_muster_verify_back()
{
	sel_workid[0]='\0';
	sel_verifygrp[0]='\0';
	sel_verifygrp[0]='\0';
	hide_frame_muster_verify();
	show_frame_optionspage();
	return;
}
void combo_habitation(pw_widget widget, int index)
{
	unsigned idx;
	char *habit;

	pw_combobox_get_selection(combo_verify_hab, &habit, &idx);

	strncpy(village_id,habit,3);

	strncpy(habit_id,habit,5);

	pw_combobox_clear(combo_verify_wkcode);

	sprintf(query,"select distinct substr(work_code,13,6) from muster_attendance where substr(created_date,1,10)=current_date and substr(work_code,1,12)='%s%s'",dt_ass_man_pan,village_id);
	exec_query_load_into_combo(combo_verify_wkcode, query, DB_PACK);
	free(habit);
	return ;
}
pw_widget Butt[1];
int habitation_details()
{
	char *dist_mand_pan_code=NULL ;
	sprintf(query,"select substr(jc_code,1,9) from jc_info");
	dist_mand_pan_code = exec_query_get_single_value(dist_mand_pan_code, query, DB_PACK);
	sprintf(dt_ass_man_pan,"%s",dist_mand_pan_code);
	pw_combobox_clear(combo_verify_hab);

	sprintf(query,"select Habitation_Code||'-'||habitation_name from tmp_Habitation_Master ");

	exec_query_load_into_combo(combo_verify_hab, query, DB_PACK);
	return 0;
}
void create_frame_muster_verify(void)
{
	frame_muster_verify = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_muster_verify);

	label_muster_verify[0] =  pw_create_label(frame_muster_verify, 0, 02, 80, 20,
			PW_TEXT, "Habitation",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	combo_verify_hab = pw_create_combobox(frame_muster_verify, 80, 02, 159, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_verify_hab,combo_habitation);
	label_muster_verify[1] =  pw_create_label(frame_muster_verify, 0, 32, 80, 20,
			PW_TEXT, "WORK ID",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	combo_verify_wkcode = pw_create_combobox(frame_muster_verify, 80, 32, 90, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_verify_wkcode,combo_wrkid_verify);

	label_muster_verify[2] =  pw_create_label(frame_muster_verify,0, 62, 80, 20,
			PW_TEXT, "Grp Names",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	combo_verify_grp = pw_create_combobox(frame_muster_verify, 80, 62, 159, 20,
			PW_COMBOBOX_TYPE,PW_COMBOBOX_EDITABLE, 
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_verify_grp,combo_grp_verify);

	label_muster_verify[3] =  pw_create_label(frame_muster_verify, 0, 92, 120, 30,
			PW_TEXT, "Total Present",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_get_verify[1] =  pw_create_label(frame_muster_verify, 120, 92, 40, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	Butt_muster_verify[2]= pw_create_button(frame_muster_verify, 160,92, 60, 30,
			PW_TEXT, "View",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_verify_details,
			PW_CONFIG_END);

	label_muster_verify[4] =  pw_create_label(frame_muster_verify, 0, 132, 120, 30,
			PW_TEXT, "Crowbar count",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_get_verify[2] =  pw_create_label(frame_muster_verify, 120, 132, 40, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	Butt_muster_verify[3]= pw_create_button(frame_muster_verify, 160,132, 60, 30,
			PW_TEXT, "View",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_crbar_verify_details,
			PW_CONFIG_END);

	label_muster_verify[5] =  pw_create_label(frame_muster_verify, 0, 182, 90, 20,
			PW_TEXT, "REMARKS",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	cmb=pw_create_combobox(frame_muster_verify, 90, 182, 135,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(cmb,sel_combo_remarks);

	Butt_muster_verify[0] = pw_create_button(frame_muster_verify, 64,230, 72, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SUBMIT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_finalmuster_verify_details,
			PW_CONFIG_END);

	Butt_muster_verify[1]= pw_create_button(frame_muster_verify, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_verify_back,
			PW_CONFIG_END);

	Butt_muster_verify[2]= pw_create_button(frame_muster_verify, 156,230, 60, 27,
			PW_TEXT, "UPLOAD",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, Mvo_gprs_conn,
			PW_CONFIG_END);
	return;
}

int Mvo_fun_up()
{
	int res;

	ret= gp_test();
	if(ret<0)
	{
		printf("\n not connected\n");
		show_message_frame("NOT CONNECTED TRY AGAIN");
	}
	else
	{
		res=Mvo_upload();
	}
	return 0;
}
int mvo_tables_create()
{
	char query1[1024];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"CREATE TABLE MVO_Details(pid varchar(10) not null,jobcardno char(20),name varchar(50),group_name varchar2(300),group_id varchar(15),work_code char(20),status char(2),Gpslongitude double,Gpslatitude double,crowbar char(2),substr(created_date,1,10) default current_date,mvo_status char(2))");

	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE MVO_remarks(remarks varchar(50),substr(created_date,1,10) default current_date,status char(2))");

	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE Master_remarks(fa_remarks varchar(100))");
                
        sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query1,"insert into Master_remarks values('DATA CORRECT')");
	sqlite3_exec(dbhandle, query1, NULL, NULL, NULL);
	sprintf(query1,"insert into Master_remarks values('FA WRONG DATA')");
        sqlite3_exec(dbhandle, query1, NULL, NULL, NULL);
	sprintf(query1,"insert into Master_remarks values('NO FA')");
        sqlite3_exec(dbhandle, query1, NULL, NULL, NULL);
	sprintf(query1,"insert into Master_remarks values('POS PROBLEM')");
        sqlite3_exec(dbhandle, query1, NULL, NULL, NULL);
	sprintf(query1,"insert into Master_remarks values('DATA PROBLEM')");
        sqlite3_exec(dbhandle, query1, NULL, NULL, NULL);

	char *mvo_count=NULL;
	sprintf(query,"select ifnull(count(*),'0') from mvo_details where status='N'");
	mvo_count = exec_query_get_single_value(mvo_count, query, DB_PACK);
	if(atoi(mvo_count)>0)
	{
		Mvo_fun_up();
	}
	
	free(mvo_count);
	sqlite3_close(dbhandle);

	return 0;
}
void show_frame_muster_verify()
{	
	mvo_tables_create();
	char *dt_count=NULL;
	sprintf(query,"select count(*) from muster_attendance where substr(created_date,1,10)=current_date");
	dt_count = exec_query_get_single_value(dt_count, query, DB_PACK);
	if(atoi(dt_count)==0)
	{
		show_frame_optionspage();
		show_message_frame("NO e-MUSTERS");
		return ;
	}
	show_progress_frame("Processing Please wait...");

	if(!frame_muster_verify)
	{
		create_frame_muster_verify();
		pw_show(frame_muster_verify);
	}
	else
		pw_show(frame_muster_verify);
	pw_combobox_clear(cmb);
	
	sprintf(query,"select fa_remarks from Master_remarks");
	exec_query_load_into_combo(cmb, query, DB_PACK);
	habitation_details();
	pw_combobox_clear(combo_verify_wkcode);
	pw_set_text(label_get_verify[1],"");
	pw_set_text(label_get_verify[2],"");
	hide_progress_frame();
	return;
}
void hide_frame_muster_verify(void)
{
	pw_show(frame_muster_verify);
	return;
}
