#include "types.h"

pw_widget frame_wrk_alloc_report,lbl_wrk_alloc_report[5],wrk_alloc_date,lbox_wrk_alloc_groupnams,Butt_wrk_alloc_rprt[4];
char *exec_query_get_single_value(char *data, char *query, char *database);
int exec_query_load_into_lbox_trriple(pw_widget lbox_muster_att_grpdet , char *query, char *database);
int exec_query_load_into_lbox_single(pw_widget lbox_muster_att_grpdet , char *query, char *database);
void hide_frame_wrk_alloc_report();
void show_progress_frame();
void hide_progress_frame();
void hide_text_frame();
void show_text_frame();
void txtlb_get_element(pw_widget w, char **s, int idx, int column);
void show_demand_mis_reports();
void show_wrk_alloc_report();
void hide_frame_wrk_alloc_details();
int rec_count;
char sel_wrk_alloc_grpname[150],sel_allocwrk_id[18], buf_dt[12],buf1_dt[12],sel_rno[9];
pw_widget frame_wrk_alloc_details;
pw_widget frame_wrk_alloc_details,lbl_get_wrk_alloc_details[12],lbl_wrk_alloc_details[10];
char sel_alloc_wc[20],sel_alloc_ddt[12],sel_alloc_dfrdt[12],sel_alloc_dtodt[12],sel_alloc_dt[12],sel_alloc_frmdt[15][15],sel_alloc_todt[15][15],sel_alloc_days[10];
int get_alloc_wrk_details(void *parg, int argc, char **argv, char **colnames)
{

	strcpy(sel_alloc_wc,argv[0]);
	printf("sel_alloc_wc1 is %s\n",sel_alloc_wc);
	strcpy(sel_alloc_dfrdt,argv[1]);
	strcpy(sel_alloc_dtodt,argv[2]);
	strcpy(sel_alloc_dt,argv[3]);
	strcpy(sel_alloc_frmdt[rec_count],argv[4]);
	strcpy(sel_alloc_todt[rec_count],argv[5]);
	strcpy(sel_alloc_days,argv[6]);
	printf("sel_alloc_days, is %s\n",sel_alloc_days);
	rec_count++;
	return 0;
}
int get_allocwrk_summary()
{
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	char query[1024];
 
	sprintf(query,"select distinct  work_code,strftime('%s',substr(demand_frm_dt,1,10)),strftime('%s',substr(demand_to_dt,1,10)),strftime('%s',substr(allocation_date,1,10)),strftime('%s',substr(alloc_from_date,1,10)),strftime('%s',substr(alloc_to_date,1,10)),alloc_days from work_allocation_details where group_name='%s' and substr(recepit_no,3,6)='%s'  and substr(work_code,13,6)='%s'",date_format,date_format,date_format,date_format,date_format,sel_wrk_alloc_grpname,sel_rno,sel_allocwrk_id);
 
	printf("query is %s\n",query);
	rec_count=0;
	sqlite3_exec(dbhandle, query, get_alloc_wrk_details, NULL, NULL);
 
	sqlite3_close(dbhandle);
	
	pw_set_text(lbl_get_wrk_alloc_details[1],sel_alloc_wc);


	return 0;
}
int get_alloc_grps()
{
	char query[512],*date_now=NULL;
	pw_tlistbox_clear(lbox_wrk_alloc_groupnams);
	sprintf(query,"select date('now')");
	date_now=exec_query_get_single_value(date_now,query,DB_PACK);
	//sprintf(query,"select distinct  substr(work_code,13,6),substr(recepit_no,3,6),group_name from work_allocation_details where substr(allocation_date,1,10)>='%s'",date_now);
	sprintf(query,"select distinct  substr(work_code,13,6),substr(recepit_no,3,6),group_name from work_allocation_details where substr(allocation_date,1,10)>='%s' group by alloc_to_date;",date_now);
	exec_query_load_into_lbox_trriple(lbox_wrk_alloc_groupnams, query, DB_PACK);
	return 0;

}
int get_wrk_alloc_grp_del()
{
	if(strcmp(sel_wrk_alloc_grpname, "")==0)
	{
		show_message_frame("SELECT GroupName");
		return -1;
	}
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("\n error\n");
	}
	char query[512],*cnt_del=NULL;

	sprintf(query," select ifnull(count(*),'0') from work_allocation_details where group_name='%s' and status='N' and recepit_no='%s'",sel_wrk_alloc_grpname,sel_rno);
	cnt_del= exec_query_get_single_value(cnt_del, query, DB_PACK);
	sprintf(query, "delete from work_allocation_details where group_name='%s' and status='N'",sel_wrk_alloc_grpname);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	if(atoi(cnt_del)==0)
	{
		show_message_frame("Already UPLOADED");
		return -1;
	}
	else
	{
		show_message_frame("DELETED SUCCESSFULLY");
		get_alloc_grps();
		sel_wrk_alloc_grpname[0]='\0';
		return -1;
	}
	sqlite3_close(dbhandle);
	free(cnt_del);
	return 0;

}
void get_wrk_alloc_details_back()
{
	sel_wrk_alloc_grpname[0]='\0';
	hide_frame_wrk_alloc_details();
	show_wrk_alloc_report();
}
void create_frame_wrk_alloc_details()
{
	frame_wrk_alloc_details =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_wrk_alloc_details);

	lbl_get_wrk_alloc_details[0] = pw_create_label(frame_wrk_alloc_details,2, 01, 239,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);
	lbl_wrk_alloc_details[0] = pw_create_label(frame_wrk_alloc_details,00, 25, 75,20,
			PW_TEXT, "WrkCode",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_wrk_alloc_details[1] = pw_create_label(frame_wrk_alloc_details,75, 25, 164,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_wrk_alloc_details[2] = pw_create_label(frame_wrk_alloc_details,00, 50, 110,20,
			PW_TEXT, "Demand frmDt",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_wrk_alloc_details[3] = pw_create_label(frame_wrk_alloc_details,05,75, 90,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_wrk_alloc_details[3] = pw_create_label(frame_wrk_alloc_details,120, 50, 119,20,
			PW_TEXT, "Demand ToDt",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_wrk_alloc_details[4] = pw_create_label(frame_wrk_alloc_details,125, 75, 90,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_wrk_alloc_details[4] = pw_create_label(frame_wrk_alloc_details,00, 95, 120,15,
			PW_TEXT, "Allocated Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_wrk_alloc_details[5] = pw_create_label(frame_wrk_alloc_details,120, 95, 164,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_wrk_alloc_details[5] = pw_create_label(frame_wrk_alloc_details,00, 115, 110,20,
			PW_TEXT, "Alloc frmDt1",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_wrk_alloc_details[6] = pw_create_label(frame_wrk_alloc_details,05, 140, 90,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_wrk_alloc_details[6] = pw_create_label(frame_wrk_alloc_details,120, 115, 110,20,
			PW_TEXT, "Alloc ToDt1",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_get_wrk_alloc_details[7] = pw_create_label(frame_wrk_alloc_details,125, 140, 90,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

/*	lbl_wrk_alloc_details[7] = pw_create_label(frame_wrk_alloc_details,01, 198, 120,20,
			PW_TEXT, "Allocated Days",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_get_wrk_alloc_details[8] = pw_create_label(frame_wrk_alloc_details,120, 198, 90,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);*/

	Butt_wrk_alloc_rprt[3]= pw_create_button(frame_wrk_alloc_details, 20,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_wrk_alloc_details_back,
			PW_CONFIG_END);

	lbl_wrk_alloc_details[8] = pw_create_label(frame_wrk_alloc_details,00, 158, 110,20,
			PW_TEXT, "Alloc frmDt2",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_get_wrk_alloc_details[9] = pw_create_label(frame_wrk_alloc_details,05, 180, 90,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_wrk_alloc_details[9] = pw_create_label(frame_wrk_alloc_details,120, 158, 110,20,
			PW_TEXT, "Alloc ToDt2",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_get_wrk_alloc_details[10] = pw_create_label(frame_wrk_alloc_details,125, 180, 90,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);



}
void show_frame_wrk_alloc_details()
{
	if(!frame_wrk_alloc_details)
	{
		create_frame_wrk_alloc_details();
		pw_show(frame_wrk_alloc_details);
	}
	else
		pw_show(frame_wrk_alloc_details);
	pw_set_text(lbl_get_wrk_alloc_details[0],"");
	pw_set_text(lbl_get_wrk_alloc_details[1],"");
	pw_set_text(lbl_get_wrk_alloc_details[3],"");
	pw_set_text(lbl_get_wrk_alloc_details[4],"");
	pw_set_text(lbl_get_wrk_alloc_details[5],"");
	pw_set_text(lbl_get_wrk_alloc_details[6],"");
	pw_set_text(lbl_get_wrk_alloc_details[7],"");
//	pw_set_text(lbl_get_wrk_alloc_details[8],"");
	pw_set_text(lbl_get_wrk_alloc_details[9],"");
	pw_set_text(lbl_get_wrk_alloc_details[10],"");
	get_allocwrk_summary();

	if(rec_count==2)
	{
	pw_show(lbl_wrk_alloc_details[8]);
	pw_show(lbl_wrk_alloc_details[9]);
	pw_set_text(lbl_get_wrk_alloc_details[0],sel_wrk_alloc_grpname);
	pw_set_text(lbl_get_wrk_alloc_details[3],sel_alloc_dfrdt);
	pw_set_text(lbl_get_wrk_alloc_details[4],sel_alloc_dtodt);
	pw_set_text(lbl_get_wrk_alloc_details[5],sel_alloc_dt);
	pw_set_text(lbl_get_wrk_alloc_details[6],sel_alloc_frmdt[0]);
	pw_set_text(lbl_get_wrk_alloc_details[7],sel_alloc_todt[0]);
//	pw_set_text(lbl_get_wrk_alloc_details[8],"14");
	pw_set_text(lbl_get_wrk_alloc_details[9],sel_alloc_frmdt[1]);
	pw_set_text(lbl_get_wrk_alloc_details[10],sel_alloc_todt[1]);
	}  
	else
	{
	pw_hide(lbl_wrk_alloc_details[8]);
	pw_hide(lbl_wrk_alloc_details[9]);
	pw_set_text(lbl_get_wrk_alloc_details[0],sel_wrk_alloc_grpname);
	pw_set_text(lbl_get_wrk_alloc_details[3],sel_alloc_dfrdt);
	pw_set_text(lbl_get_wrk_alloc_details[4],sel_alloc_dtodt);
	pw_set_text(lbl_get_wrk_alloc_details[5],sel_alloc_dt);
	pw_set_text(lbl_get_wrk_alloc_details[6],sel_alloc_frmdt[0]);
	pw_set_text(lbl_get_wrk_alloc_details[7],sel_alloc_todt[0]);
//	pw_set_text(lbl_get_wrk_alloc_details[8],sel_alloc_days);
	}
}
void hide_frame_wrk_alloc_details()
{
	pw_hide(frame_wrk_alloc_details);
}
void get_wrk_alloc_grp_details()
{
	if(strcmp(sel_wrk_alloc_grpname, "")==0)
	{
		show_message_frame("SELECT GroupName");
		return ;
	}
	hide_frame_wrk_alloc_report();
	show_frame_wrk_alloc_details();
}
int cb_wrk_alloc_grp_sel_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
	return 0;
}

int cb_wrk_alloc_grp_sel_press()
{
	char *str=NULL,*str1=NULL,*str2=NULL;
	int idx = pw_tlistbox_get_selection(lbox_wrk_alloc_groupnams);
	pw_tlistbox_get_element(lbox_wrk_alloc_groupnams, &str,idx,0);
	txtlb_get_element(lbox_wrk_alloc_groupnams, &str, idx, 0);
	pw_tlistbox_get_element(lbox_wrk_alloc_groupnams, &str1,idx, 1);
	txtlb_get_element(lbox_wrk_alloc_groupnams, &str1, idx, 1);
	pw_tlistbox_get_element(lbox_wrk_alloc_groupnams, &str2,idx, 2);
	txtlb_get_element(lbox_wrk_alloc_groupnams, &str2, idx,2);

	if(idx==-1)
	{
		return -1;
	}
	sel_wrk_alloc_grpname[0]='\0';
	sel_rno[0]='\0';
	strcpy(sel_wrk_alloc_grpname,str2);
	show_text_frame(sel_wrk_alloc_grpname);
	strcpy(sel_allocwrk_id,str);
	strcpy(sel_rno,str1);
	printf("sel_wrk_alloc_grpnam is %s\n",sel_wrk_alloc_grpname);
	printf("sel_sel_allocwrk_id is %s\n",sel_allocwrk_id);
	printf("sel_rno is %s\n",sel_rno);
	free(str);
	free(str1);
	free(str2);
	return 0;
}
int get_wrk_alloc_date()
{
	int day_of_week1,year1,month1,day1;
	buf_dt[0]='\0';
	buf1_dt[0]='\0';
	day_of_week1 = pw_create_calendar(frame_wrk_alloc_report, 37, 77, &day1, &month1, &year1, PW_NA);
	snprintf(buf_dt, 32, "%d-%02d-%02d", year1,month1,day1);
	snprintf(buf1_dt, 32, "%02d-%02d-%d", day1,month1,year1);
	pw_set_text(wrk_alloc_date,buf1_dt);
	char *cnt_day=NULL;
	char query[512];
	sprintf(query,"select ifnull(count(*),'0') from work_allocation_details where substr(allocation_date,1,10)>='%s'",buf_dt);
	cnt_day = exec_query_get_single_value(cnt_day, query, DB_PACK);
	if(atoi(cnt_day)==0)
	{
		show_message_frame("NO ALLOCATED-Groups");
		pw_tlistbox_clear(lbox_wrk_alloc_groupnams);
		get_alloc_grps();
		pw_set_text(wrk_alloc_date,"");
		return -1;
	}
	free(cnt_day);
	pw_tlistbox_clear(lbox_wrk_alloc_groupnams);
	sprintf(query,"select distinct  substr(work_code,13,6),substr(recepit_no,3,6),group_name from work_allocation_details where substr(allocation_date,1,10)>='%s'",buf_dt);
	exec_query_load_into_lbox_trriple(lbox_wrk_alloc_groupnams, query, DB_PACK);
	return 0;
}

void get_wrk_alloc_grps_back()
{
	hide_frame_wrk_alloc_report();
	show_demand_mis_reports();
}
void create_frame_wrk_alloc_report()
{
	frame_wrk_alloc_report =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_wrk_alloc_report);
	lbl_wrk_alloc_report[0] = pw_create_label(frame_wrk_alloc_report,0, 00, 45, 20,
			PW_TEXT, "Date",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	wrk_alloc_date =  pw_create_label(frame_wrk_alloc_report , 45, 00, 80, 25,
			PW_BGCOLOR, "white",
			PW_FGCOLOR, "black",
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_wrk_alloc_date,
			PW_CONFIG_END);

	lbl_wrk_alloc_report[1] =  pw_create_label(frame_wrk_alloc_report,0, 30, 70, 15,
			PW_TEXT, "WrkCode",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_wrk_alloc_report[1] =  pw_create_label(frame_wrk_alloc_report,75, 30, 60, 15,
			PW_TEXT, "R.NO",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);


	lbl_wrk_alloc_report[2] =  pw_create_label(frame_wrk_alloc_report,135, 30, 110, 15,
			PW_TEXT, "Group Name",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbox_wrk_alloc_groupnams = pw_create_tlistbox(frame_wrk_alloc_report, 0, 45, 235, 165,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_wrk_alloc_grp_sel_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_wrk_alloc_grp_sel_press,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_wrk_alloc_groupnams,60);
	pw_tlistbox_addcolumn(lbox_wrk_alloc_groupnams,90);
	pw_tlistbox_set_selection_type(frame_wrk_alloc_report,PW_TLISTBOX_SINGLE_SELECTION);

	Butt_wrk_alloc_rprt[0] = pw_create_button(frame_wrk_alloc_report, 60,230, 60, 30,
			PW_TEXT, "Delete",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_wrk_alloc_grp_del,
			PW_CONFIG_END);

	Butt_wrk_alloc_rprt[1] = pw_create_button(frame_wrk_alloc_report, 134,230, 100, 30,
			PW_TEXT, "View Details",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_wrk_alloc_grp_details,
			PW_CONFIG_END);
	Butt_wrk_alloc_rprt[2]= pw_create_button(frame_wrk_alloc_report, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_wrk_alloc_grps_back,
			PW_CONFIG_END);
}
void show_wrk_alloc_report()
{
	if(!frame_wrk_alloc_report){
		create_frame_wrk_alloc_report();
		pw_show(frame_wrk_alloc_report);
	}
	else
		pw_show(frame_wrk_alloc_report);
	get_alloc_grps();
}
void hide_frame_wrk_alloc_report()
{
	pw_hide(frame_wrk_alloc_report);
}
