#include "types.h"
int txtlb_get_element();
void show_progress_frame();
void hide_progress_frame();
char *exec_query_get_single_value(char *data, char *query, char *database);

pw_widget frame_mccwrk_aaloc_report,lbl_mcc_wrk_alloc_report[6],txt_entry_mccwrk_id,Butt_mccwrkid_search,combo_mcc_wkcode,lbox_mccwrk_details,combo_mcc_grp,Butt_mcc_wrks_back;

void show_frame_mis_report();
void hide_frame_mccwrk_aaloc_report();
int exec_query_load_into_lbox_double(pw_widget lbox, char *query, char *database);
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
int exec_query_load_into_lbox_double(pw_widget lbox_muster_att_grpdet , char *query, char *database);

char query[1024],sel_mccwrkid[20];
int lbox_relese(pw_widget wdgt, int idx1)
{
	char *taskname1=NULL,*taskname2=NULL,sel_fromdate[20],sel_todate[20];
	int idx = pw_tlistbox_get_selection(lbox_mccwrk_details);
	pw_tlistbox_get_element(lbox_mccwrk_details,&taskname1,idx, 0);
	txtlb_get_element(lbox_mccwrk_details, &taskname1, idx,0 );
	pw_tlistbox_get_element(lbox_mccwrk_details,&taskname2,idx, 1);
	txtlb_get_element(lbox_mccwrk_details, &taskname2, idx,1);
	if(idx==-1)
	{
		return -1;
	}
	strcpy(sel_fromdate,taskname1);
	strcpy(sel_todate,taskname2);
	free(taskname1);
	free(taskname2);
	printf("selected sel_taskname is %s\n", sel_fromdate);
	printf("selected sel_taskname is %s\n", sel_todate);
	pw_combobox_clear(combo_mcc_grp);
	sprintf(query,"select distinct a.group_name from mcc_work_allocation_details b,household_details a where substr(b.alloc_from_date,1,10)='%s' and substr(b.alloc_to_date,1,10)='%s' and a.group_id=b.group_id",sel_fromdate,sel_todate);
	exec_query_load_into_combo(combo_mcc_grp, query, DB_PACK);
	return 0;
}
void combobox_mccwrkid(pw_widget widget, int index)
{
	unsigned idx;
	char *workcode=NULL;
	show_progress_frame("PROCESSING PLEASE WAIT");
	pw_combobox_get_selection(combo_mcc_wkcode, &workcode, &idx);
	strcpy(sel_mccwrkid, workcode);
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	pw_tlistbox_clear(lbox_mccwrk_details);
	sprintf(query,"select distinct substr(alloc_from_date,1,10),substr(alloc_to_date,1,10) from mcc_work_allocation_details where substr(alloc_to_date,1,10)>=current_date");  
	exec_query_load_into_lbox_double(lbox_mccwrk_details, query, DB_PACK);
	sqlite3_close(dbhandle);
	free(workcode);
	hide_progress_frame();
	return;
}
int get_mccsrch_wrkid()
{
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");

	char *srch_chk_qty=NULL,*search_cnt=NULL;
	srch_chk_qty=strpbrk(pw_get_text(txt_entry_mccwrk_id),"-*+.");
	printf("srch_chk_qty is %s\n",srch_chk_qty);
	while(srch_chk_qty != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(txt_entry_mccwrk_id);
		return 0;
	}

	free(srch_chk_qty);
	strcpy(sel_mccwrkid,pw_get_text(txt_entry_mccwrk_id));
	sprintf(query,"select  count(*) from mcc_work_allocation_details where work_code like '%%%s%%' and substr(alloc_to_date,1,10) >=current_date",sel_mccwrkid);

	search_cnt = exec_query_get_single_value(search_cnt, query, DB_PACK);
	if(atoi(search_cnt)==0)
	{
		show_message_frame("NO DATA");
		pw_entry_clear(txt_entry_mccwrk_id);
		return 0;
	}

	sprintf(query,"select distinct substr(work_code, 13, 6) from mcc_work_allocation_details where  work_code like '%%%s%%' and substr(alloc_to_date,1,10) >=current_date",sel_mccwrkid);
	pw_combobox_clear(combo_mcc_wkcode);
	pw_set_text(combo_mcc_wkcode,"--Select--");
	pw_entry_clear(txt_entry_mccwrk_id);
	exec_query_load_into_combo(combo_mcc_wkcode, query, DB_PACK);
	free(search_cnt);
	return 0;
}
int get_mcc_wrks()
{
	pw_combobox_clear(combo_mcc_wkcode);
	pw_set_text(combo_mcc_wkcode,"--Select--");
	sprintf(query,"select distinct substr(work_code, 13, 6) from mcc_work_allocation_details where substr(alloc_to_date,1,10)>=current_date");
	exec_query_load_into_combo(combo_mcc_wkcode, query, DB_PACK);
	return 0;
}
void get_mccwrks_back()
{
	hide_frame_mccwrk_aaloc_report();
	show_frame_mis_report();
}
void create_frame_mccwrk_aaloc_report()
{
	frame_mccwrk_aaloc_report =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_mccwrk_aaloc_report);

	lbl_mcc_wrk_alloc_report[0] =  pw_create_label(frame_mccwrk_aaloc_report, 0, 10, 65, 20,
			PW_TEXT, "WORK ID:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	txt_entry_mccwrk_id =  pw_create_entry(frame_mccwrk_aaloc_report, 65, 10, 65, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	pw_focus(txt_entry_mccwrk_id);

	Butt_mccwrkid_search = pw_create_button(frame_mccwrk_aaloc_report, 155, 10,76, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SEARCH,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_mccsrch_wrkid,
			PW_CONFIG_END);
	lbl_mcc_wrk_alloc_report[1] =  pw_create_label(frame_mccwrk_aaloc_report, 0, 44, 65, 20,
			PW_TEXT, "WORK ID:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	combo_mcc_wkcode = pw_create_combobox(frame_mccwrk_aaloc_report, 65, 44, 90, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_mcc_wkcode ,combobox_mccwrkid);

	lbl_mcc_wrk_alloc_report[2] = pw_create_label(frame_mccwrk_aaloc_report ,01, 74, 110, 15,
			PW_TEXT, "From Date",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_mcc_wrk_alloc_report[3] = pw_create_label(frame_mccwrk_aaloc_report ,100, 74, 130, 15,
			PW_TEXT, "To Date",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbox_mccwrk_details = pw_create_tlistbox(frame_mccwrk_aaloc_report, 0,89, 240, 86,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,lbox_relese,
			PW_CONFIG_END);

	pw_tlistbox_addcolumn(lbox_mccwrk_details, 120);
	pw_update(lbox_mccwrk_details,1);

	lbl_mcc_wrk_alloc_report[4] =  pw_create_label(frame_mccwrk_aaloc_report, 0,180 , 70, 25,
			PW_TEXT, "Group Names",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	combo_mcc_grp = pw_create_combobox(frame_mccwrk_aaloc_report, 70,180, 160, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	Butt_mcc_wrks_back = pw_create_button(frame_mccwrk_aaloc_report, 10, 235, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_mccwrks_back,
			PW_CONFIG_END);

}
void show_mccwrk_aaloc_report()
{
	if(!frame_mccwrk_aaloc_report){
		create_frame_mccwrk_aaloc_report();
		pw_show(frame_mccwrk_aaloc_report);
	}
	else
		pw_show(frame_mccwrk_aaloc_report);
	get_mcc_wrks();
}
void hide_frame_mccwrk_aaloc_report()
{
	pw_hide(frame_mccwrk_aaloc_report);
}
