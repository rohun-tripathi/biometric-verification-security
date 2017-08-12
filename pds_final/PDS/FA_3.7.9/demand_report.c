#include "types.h"
pw_widget frame_demand_report,lbl_demand_report[3],demand_date,lbox_demand_groupnams,Butt_demand_rprt[5];
void show_demand_options();
void hide_frame_demand_report();
char *exec_query_get_single_value(char *data, char *query, char *database);
int exec_query_load_into_lbox_double(pw_widget lbox_muster_att_grpdet , char *query, char *database);
int exec_query_load_into_lbox_single(pw_widget lbox_muster_att_grpdet , char *query, char *database);

pw_widget Butt_edit,frame_demand_grpdetail,lbl_demand_grpdetail[6],textentry,lbl_get_demand_grpdetail[6],lbox_demand_grp_detail;
void hide_frame_demand_grpdetail();
void show_progress_frame();
void hide_progress_frame();
void hide_text_frame();
void show_text_frame();
void show_demand_mis_reports();
int get_groupids();
void txtlb_get_element(pw_widget w, char **s, int idx, int column);
char sel_demand_grpname[20];
char demand_dt[15],demand_frmdt[15],demand_todt[15],sel_recepit_id[9],sel_recepitno[9];
int get_grpdemand_details(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(demand_dt, argv[0]);
	strcpy(demand_frmdt, argv[1]);
	strcpy(demand_todt, argv[2]);
	return 0;
}
int get_demand_capture_details()
{
	char query[512];
	printf("inside details");
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select strftime('%s',application_date),strftime('%s',fromdate),strftime('%s',todate) from Demand_Group_details where group_name='%s' and substr(recepit_id,3,6)='%s'",date_format,date_format,date_format,sel_demand_grpname,sel_recepit_id);
	sqlite3_exec(dbhandle, query, get_grpdemand_details, NULL, NULL);
	printf("query is %s\n",query);
	sqlite3_close(dbhandle);

	pw_tlistbox_clear(lbox_demand_grp_detail);
	sprintf(query,"select  distinct substr(household_code,15,4)||worker_code||'-'|| name from household_details hh,Demand_Group_details dg where hh.is_active='Y' and hh.household_code||hh.worker_code = dg.Householdcode  and substr(recepit_id,3,6)='%s' and hh.group_name=dg.group_name",sel_recepit_id);
	exec_query_load_into_lbox_single(lbox_demand_grp_detail, query, DB_PACK);
	sqlite3_close(dbhandle);

	return 0;
}
void editbutton()
{
	pw_show(textentry);
	pw_set_text(textentry,sel_recepit_id);
	pw_hide(lbl_get_demand_grpdetail[3]);
}

int submit()
{
	sel_recepitno[0]='\0';
	strcpy(sel_recepitno,pw_get_text(textentry));
	printf("sel_recepitno is %s\n",sel_recepitno);
	if(strcmp(sel_recepitno,"")==0)
	{
		show_message_frame("Edit Receipt ID");
		return -1 ;
	}
	else if (strlen(sel_recepitno)!=6)
	{
		show_message_frame("Receipt No. Mustbe 6 Digit");
		pw_entry_clear(textentry);
		return -1 ;
	}
	else if(strcmp(sel_recepitno,"00000")==0)
	{
		show_message_frame("Receipt No. NOT tobe Zero's");
		pw_entry_clear(textentry);
		return -1 ;
	}

	char *chk_qty=NULL; 

	char query[512];
	chk_qty=strpbrk(pw_get_text(textentry),"-*+.abcdefghijklmnopqrstuvwxyz");
	printf("chk_qty is %s\n",chk_qty);
	while(chk_qty != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(textentry);
		return 0;
	}
	free(chk_qty);
	char *recept_cnt=NULL,*recept_cnt1=NULL;
	sprintf(query,"select ifnull(count(*),'0') from Demand_Group_details where substr(recepit_id,3,6)='%s'",sel_recepitno);
	recept_cnt = exec_query_get_single_value(recept_cnt, query, DB_PACK);
	sprintf(query,"select ifnull(count(*),'0') from mcc_demand_group_details where substr(receipt_no,3,6)='%s'",sel_recepitno);
	recept_cnt1 = exec_query_get_single_value(recept_cnt1, query, DB_PACK);

	if(atoi(recept_cnt)>=1 || atoi(recept_cnt1)>=1)
	{
		show_message_frame("Duplicate Receipt No.");
		return 0 ;
	}
	free(recept_cnt);
	free(recept_cnt1);

	char recepit[9];
	recepit[0]='\0';
	strcat(recepit,sel_recepitno);

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("\n error\n");
	}
	sprintf(query,"update Demand_Group_details set recepit_id=replace(recepit_id,'%s','%s'),status='N' where group_name='%s' and substr(recepit_id,3,6)='%s'",sel_recepit_id,recepit,sel_demand_grpname,sel_recepit_id);
	sqlite3_exec(dbhandle,query,NULL,NULL,NULL);
	printf("update query is %s\n",query);
	hide_frame_demand_grpdetail();
	pw_show(frame_demand_report);

	pw_tlistbox_clear(lbox_demand_groupnams);
	sprintf(query,"select distinct  group_name,substr(recepit_id,3,6) from Demand_Group_details");
	exec_query_load_into_lbox_double(lbox_demand_groupnams, query, DB_PACK);

	show_message_frame("SUCCESSFULLY SAVED");
	sqlite3_close(dbhandle);

	return 0;
}
void get_demand_grpsdetail_back()
{
	sel_demand_grpname[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_demand_groupnams);
	pw_entry_clear(textentry);
	pw_hide(textentry);
	pw_show(lbl_get_demand_grpdetail[3]);

	hide_frame_demand_grpdetail();
	pw_show(frame_demand_report);
}
void create_frame_demand_grpdetail()
{
	frame_demand_grpdetail =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	lbl_demand_grpdetail[0] =  pw_create_label(frame_demand_grpdetail,00, 00, 120,20,
			PW_TEXT, "Demand Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_demand_grpdetail[0] = pw_create_label(frame_demand_grpdetail,120, 00, 85,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_demand_grpdetail[1] =  pw_create_label(frame_demand_grpdetail,00, 22, 85,15,
			PW_TEXT, "From Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_demand_grpdetail[1] = pw_create_label(frame_demand_grpdetail,05, 37, 85,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_demand_grpdetail[2] =  pw_create_label(frame_demand_grpdetail,120, 22, 85,15,
			PW_TEXT, "To Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_demand_grpdetail[2] = pw_create_label(frame_demand_grpdetail,120, 37, 85,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_demand_grpdetail[3] =  pw_create_label(frame_demand_grpdetail,00, 60, 85,18,
			PW_TEXT, "Recepit NO",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_demand_grpdetail[3] = pw_create_label(frame_demand_grpdetail,85, 60, 65,18,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	textentry = pw_create_entry(frame_demand_grpdetail,85, 60, 65,18,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	pw_hide(textentry);

	Butt_demand_rprt[5]= pw_create_button(frame_demand_grpdetail, 175,60, 60, 18,
			PW_TEXT,"EDIT",	
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,editbutton,
			PW_CONFIG_END);



	lbl_get_demand_grpdetail[4] = pw_create_label(frame_demand_grpdetail,5, 80, 239,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbox_demand_grp_detail = pw_create_tlistbox(frame_demand_grpdetail, 0, 100, 239, 120,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_set_selection_type(frame_demand_grpdetail,PW_TLISTBOX_SINGLE_SELECTION);

	Butt_demand_rprt[3]= pw_create_button(frame_demand_grpdetail, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demand_grpsdetail_back,
			PW_CONFIG_END);

	Butt_demand_rprt[4]= pw_create_button(frame_demand_grpdetail, 150,230, 70, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SUBMIT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,submit,
			PW_CONFIG_END);

}
void show_frame_demand_grpdetail()
{
	show_progress_frame("PLEASE WAIT....");

	if(!frame_demand_grpdetail){
		create_frame_demand_grpdetail();
		pw_show(frame_demand_grpdetail);
	}
	else
	{
		pw_show(frame_demand_grpdetail);
	}	
	char *rec_sh=NULL,query[512];
	sprintf(query,"select status from Demand_Group_details where substr(recepit_id,3,6)='%s'",sel_recepit_id);
	rec_sh = exec_query_get_single_value(rec_sh, query, DB_PACK);
	if(strcmp(rec_sh,"Y")==0)
	{
		pw_hide(Butt_demand_rprt[4]);
		pw_hide(Butt_demand_rprt[5]);
		pw_show(frame_demand_grpdetail);
	}
	else
	{

		pw_show(Butt_demand_rprt[4]);
		pw_show(Butt_demand_rprt[5]);	
		pw_show(frame_demand_grpdetail);
	}	
	free(rec_sh);
	get_demand_capture_details();
	pw_set_text(lbl_get_demand_grpdetail[0],demand_dt);
	pw_set_text(lbl_get_demand_grpdetail[1],demand_frmdt);
	pw_set_text(lbl_get_demand_grpdetail[2],demand_todt);
	pw_set_text(lbl_get_demand_grpdetail[4],sel_demand_grpname);
	pw_set_text(lbl_get_demand_grpdetail[3],sel_recepit_id);

	hide_progress_frame();
	return ;
}
int get_demand_grp_del()
{
	if(strcmp(sel_demand_grpname, "")==0)
	{
		show_message_frame("SELECT GroupId");
		return -1;
	}
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("\n error\n");
	}
	char *cnt_del=NULL,query[1024];
	sprintf(query,"select count(*)from  Demand_Group_details where group_name='%s' and status='N' and substr(recepit_id,3,6)='%s'",sel_demand_grpname,sel_recepit_id);
	cnt_del= exec_query_get_single_value(cnt_del, query, DB_PACK);

	sprintf(query, "delete from Demand_Group_details where group_name='%s' and status='N'",sel_demand_grpname);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("query is %s\n",query);
	if(atoi(cnt_del)==0)
	{
		show_message_frame("Already UPLOADED");
		return -1;
	}
	else
	{
		show_message_frame("DELETED SUCCESSFULLY");
		get_groupids();
		sel_demand_grpname[0]='\0';
		return -1;
	}
	sqlite3_close(dbhandle);
	free(cnt_del);
	return 0;
}
int get_demand_grp_details()
{
	if(strcmp(sel_demand_grpname, "")==0)
	{
		show_message_frame("SELECT GroupName");
		return -1;
	}
	hide_frame_demand_report();
	show_frame_demand_grpdetail();
	return 0;
}
void hide_frame_demand_grpdetail()
{
	pw_hide(frame_demand_grpdetail);
}
int cb_demand_grp_sel_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
	return 0;
}
int cb_demand_grp_sel_press()
{
	char *str=NULL,*str1=NULL;
	int idx = pw_tlistbox_get_selection(lbox_demand_groupnams);
	pw_tlistbox_get_element(lbox_demand_groupnams, &str,idx, 0);
	txtlb_get_element(lbox_demand_groupnams, &str, idx, 0);
	pw_tlistbox_get_element(lbox_demand_groupnams, &str1,idx, 1);
	txtlb_get_element(lbox_demand_groupnams, &str1, idx, 1);

	if(idx==-1)
	{
		return -1;
	}
	sel_demand_grpname[0]='\0';
	strcpy(sel_demand_grpname,str);
	strcpy(sel_recepit_id,str1);
	printf("sel_demand_grpname is %s\n",sel_demand_grpname);
	printf("sel_recepit_id is %s\n",sel_recepit_id);
	free(str);
	free(str1);
	return 0;
}
int get_demand_date(pw_widget widget, pw_event e)
{
	char buf_dt[12],buf1_dt[12],query[512],*date_select=NULL;
	int day_of_week1,year1=0,month1=0,day1=0;
	buf_dt[0]='\0';
	buf1_dt[0]='\0';
	day_of_week1 = pw_create_calendar(frame_demand_report, 37, 77, &day1, &month1, &year1, PW_NA);
	snprintf(buf_dt, 32, "%d-%02d-%02d", year1,month1,day1);
	snprintf(buf1_dt, 32, "%02d-%02d-%d", day1,month1,year1);
	//only saturday selected----------2013-01-31 changed
	sprintf(query,"select date('%s','weekday 6')",buf_dt);
	date_select = exec_query_get_single_value(date_select,query, DB_PACK);
	printf("demanded to date1 is %s\n",date_select);
	//upto here

	//TODO:add future date condtion,,,,,,,,display default date
	pw_set_text(demand_date,date_select);
	char *cnt_day=NULL;
	sprintf(query,"select ifnull(count(*),'0') from Demand_Group_details where substr(application_date,1,10)='%s'",date_select);
	cnt_day = exec_query_get_single_value(cnt_day, query, DB_PACK);
	if(atoi(cnt_day)==0)
	{
		show_message_frame("NO Demanded-Groups");
		pw_tlistbox_clear(lbox_demand_groupnams);
		get_groupids();
		pw_set_text(demand_date,"");
		return -1;
	}
	free(cnt_day);
	pw_tlistbox_clear(lbox_demand_groupnams);
	sprintf(query,"select distinct  group_name,substr(recepit_id,3,6) from Demand_Group_details where substr(application_date,1,10)='%s'",date_select);
	exec_query_load_into_lbox_double(lbox_demand_groupnams, query, DB_PACK);
	return 0;
}
int get_groupids()
{
	char query[256],*max_date=NULL;
	pw_tlistbox_clear(lbox_demand_groupnams);
	sprintf(query,"select max(substr(application_date,1,10)) from Demand_Group_details");
	max_date=exec_query_get_single_value(max_date,query,DB_PACK);
	sprintf(query,"select distinct  group_name,substr(recepit_id,3,6) from Demand_Group_details where substr(application_date,1,10)='%s'",max_date);
	exec_query_load_into_lbox_double(lbox_demand_groupnams, query, DB_PACK);
	return 0;
}
void get_demand_grps_back()
{
	hide_frame_demand_report();
	show_demand_mis_reports();
}

void create_frame_demand_report(void)
{
	frame_demand_report =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_demand_report);

	lbl_demand_report[0] = pw_create_label(frame_demand_report,0, 00, 45, 20,
			PW_TEXT, "Date",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	demand_date =  pw_create_label(frame_demand_report , 45, 00, 80, 25,
			PW_BGCOLOR, "white",
			PW_FGCOLOR, "black",
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_demand_date,
			PW_CONFIG_END);

	lbl_demand_report[1] =  pw_create_label(frame_demand_report,0, 30, 120, 15,
			PW_TEXT, "  Group Names",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_demand_report[2] =  pw_create_label(frame_demand_report,137, 30, 120, 15,
			PW_TEXT, "Recepit NO",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);


	lbox_demand_groupnams = pw_create_tlistbox(frame_demand_report, 0, 45, 235, 165,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_demand_grp_sel_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_demand_grp_sel_press,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_demand_groupnams,70);
	pw_tlistbox_set_selection_type(frame_demand_report,PW_TLISTBOX_SINGLE_SELECTION);

	Butt_demand_rprt[0] = pw_create_button(frame_demand_report, 60,230, 60, 30,
			PW_TEXT, "Delete",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demand_grp_del,
			PW_CONFIG_END);

	Butt_demand_rprt[1] = pw_create_button(frame_demand_report, 134,230, 100, 30,
			PW_TEXT, "View Details",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demand_grp_details,
			PW_CONFIG_END);

	Butt_demand_rprt[2]= pw_create_button(frame_demand_report, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demand_grps_back,
			PW_CONFIG_END);
}
void show_demand_reports()
{
	if(!frame_demand_report){
		create_frame_demand_report();
		pw_show(frame_demand_report);
	}
	else
		pw_show(frame_demand_report);
	get_groupids();
}
void hide_frame_demand_report()
{
	pw_hide(frame_demand_report);
}
