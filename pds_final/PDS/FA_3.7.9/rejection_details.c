#include "types.h"
void show_group_details();
void hide_frame_group_details();

void show_work_alloc_rejection();
void show_alloc_rejec_details();
void show_demand_mis_reports();
void hide_frame_alloc_rejec_details();
void hide_frame_work_alloc_rejection();
void hide_frame_demand_rejection();
char *exec_query_get_single_value(char *data, char *query, char *database);
int txtlb_get_element();
void show_text_frame(char *msg);
void hide_text_frame();
int exec_query_load_into_lbox_double(pw_widget lbox_muster_att_grpdet , char *query, char *database);
int exec_query_load_into_lbox_single(pw_widget lbox_muster_att_grpdet , char *query, char *database);
pw_widget frame_work_alloc_rejection,frame_alloc_rejec_details,lbl_alloc_rejection[6],rejection_date,lbox_alloc_recj_details,lbl_alloc_reject[10],Butt_alloc_rejec[3];
pw_widget frame_demand_rejection,lbl_demand_rejection[6],rejection_date1,lbox_demand_recj_details,Butt_demand_rejec[3],textentry;
pw_widget lbl_group_details[5],lbox_group_recj_details,Butt_group_details[5],frame_group_details;
char rej_dt[20],sel_grpname[150],sel_receipt_no[20],max_alcatn_rejdt[15];
int get_rejectgroupids()
{
	char query[1024],*maxrej=NULL;
	pw_tlistbox_clear(lbox_alloc_recj_details);
	sprintf(query,"select max(substr(allocation_date,1,10)) from allocation_rejection_details");
	maxrej=exec_query_get_single_value(maxrej,query,DB_PACK);
	strcpy(max_alcatn_rejdt,maxrej);
	free(maxrej);
	
	pw_set_text(rejection_date,max_alcatn_rejdt);
	sprintf(query,"select distinct  substr(a.work_code,13,6),b.group_name from allocation_rejection_details a,household_details b where substr(a.allocation_date,1,10)='%s' and a.group_id=b.group_id",max_alcatn_rejdt);
	exec_query_load_into_lbox_double(lbox_alloc_recj_details, query, DB_PACK);
	return 0;
}

void cb_alloc_rej_sel_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
}

int  cb_alloc_rej_sel_press()
{
	char *str1=NULL,w_c[20],query[512];
	int idx = pw_tlistbox_get_selection(lbox_alloc_recj_details);
	pw_tlistbox_get_element(lbox_alloc_recj_details, &str1,idx, 0);
	txtlb_get_element(lbox_alloc_recj_details, &str1, idx, 0);
	if(idx==-1)
	{
		return -1;
	}
	strcpy(w_c,str1);
	printf("sel workcode name is %s\n",w_c);
	char *error_code=NULL;
	sprintf(query,"select distinct error_code from allocation_rejection_details where substr(work_code,13,6)='%s'",w_c);
	error_code=exec_query_get_single_value(error_code, query, DB_PACK);
	show_text_frame(error_code);
	free(str1);
	return 0;
}
int get_rejection_date(pw_widget widget, pw_event e)
{
	char rej1_dt[12],query[512];
	int day_of_week1,year1=0,month1=0,day1=0;
	rej_dt[0]='\0';
	rej1_dt[0]='\0';
	day_of_week1 = pw_create_calendar(frame_work_alloc_rejection, 37, 77, &day1, &month1, &year1, PW_NA);
	snprintf(rej_dt, 32, "%d-%02d-%02d", year1,month1,day1);
	snprintf(rej1_dt, 32, "%02d-%02d-%d", day1,month1,year1);
	printf("allocation to date of rej is %s\n",rej_dt);
	char *ck_dt=NULL;
	sprintf(query,"select (case when '%s'>(select date('now')) then 'NOK' else 'OK' end)",rej_dt);
	printf("query of case %s\n",query);
	ck_dt = exec_query_get_single_value(ck_dt, query, DB_PACK);

	if(strcmp(ck_dt,"NOK")==0)
	{
		show_message_frame("FUTURE DATE");
		pw_set_text(rejection_date, "");
		return 0;
	}
	free(ck_dt);

	pw_set_text(rejection_date,rej1_dt);
	char *cnt_day=NULL;
	sprintf(query,"select ifnull(count(*),'0') from allocation_rejection_details where substr(allocation_date,1,10)='%s'",rej_dt);
	cnt_day = exec_query_get_single_value(cnt_day, query, DB_PACK);
	if(atoi(cnt_day)==0)
	{
		show_message_frame("NO Rejected-Groups");
		pw_tlistbox_clear(lbox_alloc_recj_details);
		get_rejectgroupids();
		return -1;
	}
	free(cnt_day);
	pw_tlistbox_clear(lbox_alloc_recj_details);
	sprintf(query,"select distinct  substr(a.work_code,13,6),b.group_name from allocation_rejection_details a,household_details b where substr(a.allocation_date,1,10)='%s' and a.group_id=b.group_id",rej_dt);
	exec_query_load_into_lbox_double(lbox_alloc_recj_details, query, DB_PACK);
	return 0;
}
void get_alloc_rej_back1()
{
	hide_frame_alloc_rejec_details();
	show_work_alloc_rejection();
}
void get_alloc_rejec_details()
{
	char *sss=NULL;
	int idx = pw_tlistbox_get_selection(lbox_alloc_recj_details);
	pw_tlistbox_get_element(lbox_alloc_recj_details, &sss,idx, 0);
	txtlb_get_element(lbox_alloc_recj_details, &sss, idx, 0);
	if(idx==-1)
	{
		show_message_frame("SELECT A GROUP");
		return ;
	}
	hide_frame_work_alloc_rejection();
	show_alloc_rejec_details();
}
void create_frame_alloc_rejec_details()
{
	frame_alloc_rejec_details=pw_create_frame(toplevel,0,0,240,270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_alloc_rejec_details);

	lbl_alloc_reject[0] =  pw_create_label(frame_alloc_rejec_details,00, 00, 120,20,
			PW_TEXT, "Demand Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_alloc_reject[1] = pw_create_label(frame_alloc_rejec_details,120, 00, 85,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_alloc_reject[2] =  pw_create_label(frame_alloc_rejec_details,00, 22, 85,15,
			PW_TEXT, "From Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_alloc_reject[3] = pw_create_label(frame_alloc_rejec_details,05, 37, 85,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_alloc_reject[4] =  pw_create_label(frame_alloc_rejec_details,120, 22, 85,15,
			PW_TEXT, "To Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_alloc_reject[5] = pw_create_label(frame_alloc_rejec_details,120, 37, 85,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_alloc_reject[6] =  pw_create_label(frame_alloc_rejec_details,00, 60, 85,18,
			PW_TEXT, "Recepit NO",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_alloc_reject[7] = pw_create_label(frame_alloc_rejec_details,85, 60, 65,18,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	textentry = pw_create_entry(frame_alloc_rejec_details,85, 60, 65,18,
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_hide(textentry);

	Butt_alloc_rejec[0]= pw_create_button(frame_alloc_rejec_details, 175,60, 60, 18,
			PW_TEXT,"EDIT",	
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	Butt_alloc_rejec[2]= pw_create_button(frame_alloc_rejec_details, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_alloc_rej_back1,
			PW_CONFIG_END);


}
void show_alloc_rejec_details()
{
	if(!frame_alloc_rejec_details)
		create_frame_alloc_rejec_details();
	pw_show(frame_alloc_rejec_details);

}
void hide_frame_alloc_rejec_details()
{
	pw_hide(frame_alloc_rejec_details);
}

int CB_DELETE()
{

	char *sss=NULL;
	int idx = pw_tlistbox_get_selection(lbox_alloc_recj_details);
	pw_tlistbox_get_element(lbox_alloc_recj_details, &sss,idx, 0);
	txtlb_get_element(lbox_alloc_recj_details, &sss, idx, 0);
	if(idx==-1)
	{
		show_message_frame("SELECT A GROUP");
		return -1;
	}

	char *str=NULL,*str1=NULL,sel_grpname[150],sel_work_code[20],query[512],*count=NULL;
	idx = pw_tlistbox_get_selection(lbox_alloc_recj_details);
	pw_tlistbox_get_element(lbox_alloc_recj_details, &str,idx, 0);
	txtlb_get_element(lbox_alloc_recj_details, &str, idx, 0);
	pw_tlistbox_get_element(lbox_alloc_recj_details, &str1,idx, 1);
	txtlb_get_element(lbox_alloc_recj_details, &str1, idx, 1);

	if(idx==-1)
	{
		return -1;
	}
	strcpy(sel_grpname,str1);
	strcpy(sel_work_code,str);
	printf("sel_grpname is %s\n",sel_grpname);
	printf("sel_work_code is %s\n",sel_work_code);
	free(str);
	free(str1);
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select count(*) from work_allocation_details where substr(work_code,13,6)='%s' and group_name='%s' and substr(allocation_date,1,10)='%s' and  status='Y'",sel_work_code,sel_grpname,rej_dt);
	count=exec_query_get_single_value(count,query,DB_PACK);

	if(atoi(count)==0)
	{
		show_message_frame("No Rejected Groups");
	}
	else
	{
		sprintf(query,"select distinct group_id,jobcardno,demand_frm_dt where substr(work_code,13,6)='%s' and group_name='%s' and substr(allocation_date,1,10)='%s' and  status='Y'",sel_work_code,sel_grpname,rej_dt);
		
		sprintf(query,"delete from work_allocation_details where substr(work_code,13,6)='%s' and group_name='%s' and substr(allocation_date,1,10)='%s' and  status='Y'",sel_work_code,sel_grpname,rej_dt);
		printf("delete query is '%s'",query);	
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		show_message_frame("Successfully Deleted \n Re-allocate Again");
	}	
	sqlite3_close(dbhandle);
	return 0;
}
void get_alloc_rej_back()
{
	hide_frame_work_alloc_rejection();
	show_demand_mis_reports();
}
void create_frame_work_alloc_rejection()
{
	frame_work_alloc_rejection =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_work_alloc_rejection);

	lbl_alloc_rejection[0] = pw_create_label(frame_work_alloc_rejection,0, 00, 77, 25,
			PW_TEXT, "Allocated Date",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	rejection_date =  pw_create_label(frame_work_alloc_rejection , 100, 00, 80, 25,
			PW_BGCOLOR, "white",
			PW_FGCOLOR, "black",
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_rejection_date,
			PW_CONFIG_END);

	lbl_alloc_rejection[1] =  pw_create_label(frame_work_alloc_rejection,0, 30, 120, 15,
			PW_TEXT, "Work Code",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_alloc_rejection[2] =  pw_create_label(frame_work_alloc_rejection,137, 30, 120, 15,
			PW_TEXT, "Group Name",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);


	lbox_alloc_recj_details = pw_create_tlistbox(frame_work_alloc_rejection, 0, 45, 235, 165,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alloc_rej_sel_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_alloc_rej_sel_press,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_alloc_recj_details,140);
	pw_tlistbox_set_selection_type(frame_work_alloc_rejection,PW_TLISTBOX_SINGLE_SELECTION);
	pw_update(lbox_alloc_recj_details,1);
/*	Butt_alloc_rejec[1] = pw_create_button(frame_work_alloc_rejection, 134,230, 100, 30,
			PW_TEXT, "DELETE",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, CB_DELETE,
			PW_CONFIG_END);*/

	Butt_alloc_rejec[2]= pw_create_button(frame_work_alloc_rejection, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_alloc_rej_back,
			PW_CONFIG_END);

}
void show_work_alloc_rejection()
{
	if(!frame_work_alloc_rejection)
		create_frame_work_alloc_rejection();
	pw_show(frame_work_alloc_rejection);
	get_rejectgroupids();

}
void hide_frame_work_alloc_rejection()
{
	pw_hide(frame_work_alloc_rejection);
}
void cb_demand_rej_sel_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
}
char error_cat[10];
int  cb_demand_rej_sel_press()
{
	char *str1=NULL,w_c[20],query[512];
	int idx = pw_tlistbox_get_selection(lbox_demand_recj_details);
	pw_tlistbox_get_element(lbox_demand_recj_details, &str1,idx, 0);
	txtlb_get_element(lbox_demand_recj_details, &str1, idx, 0);
	if(idx==-1)
	{
		return -1;
	}
	strcpy(w_c,str1);
	printf("sel_recepit_no name is %s\n",w_c);
	char *error_code=NULL;
	sprintf(query,"select distinct error_code from demand_rejection_details where substr(receipt_id,9,6)='%s'",w_c);
	error_code=exec_query_get_single_value(error_code, query, DB_PACK);
	show_text_frame(error_code);
	error_cat[0]='\0';
	strcpy(error_cat,error_code);
	free(str1);
	free(error_code);
	return 0;
}
int CB_VIEW()
{
	char *ss=NULL,query[512];
	int idx = pw_tlistbox_get_selection(lbox_demand_recj_details);
	pw_tlistbox_get_element(lbox_demand_recj_details, &ss,idx, 0);
	txtlb_get_element(lbox_demand_recj_details, &ss, idx, 0);
	if(idx==-1)
	{
		show_message_frame("SELECT A GROUP");
		return -1;
	}
	char *str=NULL,*str1=NULL;
	idx = pw_tlistbox_get_selection(lbox_demand_recj_details);
	pw_tlistbox_get_element(lbox_demand_recj_details, &str,idx, 0);
	txtlb_get_element(lbox_demand_recj_details, &str, idx, 0);
	pw_tlistbox_get_element(lbox_demand_recj_details, &str1,idx, 1);
	txtlb_get_element(lbox_demand_recj_details, &str1, idx, 1);

	if(idx==-1)
	{
		return -1;
	}
	strcpy(sel_grpname,str1);
	strcpy(sel_receipt_no,str);
	printf("sel_grpname is %s\n",sel_grpname);
	printf("sel_receipt_no is %s\n",sel_receipt_no);
	free(str);
	free(str1);
	hide_frame_demand_rejection();
	show_group_details();
	sprintf(query,"select distinct substr(h.household_code,15,4)||h.worker_code||'-'||lower(h.name) from household_details h ,Demand_rejection_details d where h.household_code||h.worker_code=d.jobcard_no and substr(d.receipt_id,9,6)='%s' and d.group_id=(select group_id from household_details where group_name='%s')  ",sel_receipt_no,sel_grpname);
	exec_query_load_into_lbox_single(lbox_group_recj_details,query,DB_PACK);
	pw_set_text(lbl_group_details[0],sel_grpname);
	return 0;
}

int CB_DELETE1()
{	char query[512],*count1=NULL;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select count(*) from Demand_Group_details d,Demand_rejection_details r where d.Householdcode=r.jobcard_no and substr(d.recepit_id,3,6)='%s' and d.group_id=(select distinct group_id from household_details where group_name='%s')",sel_receipt_no,sel_grpname);
	count1=exec_query_get_single_value(count1,query,DB_PACK);

	if(atoi(count1)==0)
	{
		show_message_frame("No Rejected Groups");
	}
	else
	{
		sprintf(query,"delete from Demand_Group_details where Householdcode IN(select distinct jobcard_no from Demand_rejection_details where substr(receipt_id,9,6)='%s' and group_id=(select distinct group_id from household_details where group_name='%s')) and substr(recepit_id,3,6)='%s' and group_id=(select distinct group_id from household_details where group_name='%s')",sel_receipt_no,sel_grpname,sel_receipt_no,sel_grpname);
		printf("delete query is '%s'",query);	
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		show_message_frame("Successfully Deleted \n Re-Demand Again");
	}	
	sqlite3_close(dbhandle);
	return 0;
}

int get_rejectgroupids1()
{
	char query[1024],*max_date=NULL;
	pw_tlistbox_clear(lbox_demand_recj_details);
	sprintf(query,"select max(substr(demanddate,1,10)) from Demand_rejection_details");
	max_date=exec_query_get_single_value(max_date,query,DB_PACK);
	sprintf(query,"select distinct  substr(receipt_id,9,6),group_id from Demand_rejection_details where substr(demanddate,1,10)='%s'",max_date);
	exec_query_load_into_lbox_double(lbox_demand_recj_details, query, DB_PACK);
	pw_set_text(rejection_date1,max_date);
	free(max_date);	
	return 0;
}
int get_rejection_date1(pw_widget widget, pw_event e)
{
	char rej1_dt[12],query[512];
	int day_of_week1,year1=0,month1=0,day1=0;
	rej_dt[0]='\0';
	rej1_dt[0]='\0';
	day_of_week1 = pw_create_calendar(frame_demand_rejection, 37, 77, &day1, &month1, &year1, PW_NA);
	snprintf(rej_dt, 32, "%d-%02d-%02d", year1,month1,day1);
	snprintf(rej1_dt, 32, "%02d-%02d-%d", day1,month1,year1);
	printf("demand to date of rej is %s\n",rej_dt);
	char *ck_dt=NULL;
	sprintf(query,"select (case when '%s'>(select date('now')) then 'NOK' else 'OK' end)",rej_dt);
	printf("query of case %s\n",query);
	ck_dt = exec_query_get_single_value(ck_dt, query, DB_PACK);

	if(strcmp(ck_dt,"NOK")==0)
	{
		show_message_frame("FUTURE DATE");
		pw_set_text(rejection_date1, "");
		return 0;
	}
	free(ck_dt);

	pw_set_text(rejection_date1,rej1_dt);
	char *cnt_day=NULL;
	sprintf(query,"select ifnull(count(*),'0') from Demand_rejection_details where substr(demanddate,1,10)='%s'",rej_dt);
	cnt_day = exec_query_get_single_value(cnt_day, query, DB_PACK);
	if(atoi(cnt_day)==0)
	{
		show_message_frame("NO Rejected-Groups");
		pw_tlistbox_clear(lbox_demand_recj_details);
		get_rejectgroupids1(); 
		pw_set_text(rejection_date1,"");
		return -1;
	}
	free(cnt_day);
	pw_tlistbox_clear(lbox_demand_recj_details);
	sprintf(query,"select distinct  substr(a.receipt_id,9,6),b.group_name from Demand_rejection_details a,household_details b where substr(a.demanddate,1,10)='%s' and a.group_id=b.group_id",rej_dt);
	exec_query_load_into_lbox_double(lbox_demand_recj_details, query, DB_PACK);
	return 0;
}

void get_demand_rej_back()
{
	hide_frame_demand_rejection();
	show_demand_mis_reports();
}
void create_frame_demand_rejection()
{
	frame_demand_rejection =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_demand_rejection);

	lbl_demand_rejection[0] = pw_create_label(frame_demand_rejection,0, 00, 45, 20,
			PW_TEXT, "Date",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	rejection_date1 =  pw_create_label(frame_demand_rejection , 45, 00, 80, 25,
			PW_BGCOLOR, "white",
			PW_FGCOLOR, "black",
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_rejection_date1,
			PW_CONFIG_END);

	lbl_demand_rejection[1] =  pw_create_label(frame_demand_rejection,0, 30, 120, 15,
			PW_TEXT, "Receipt No",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_demand_rejection[2] =  pw_create_label(frame_demand_rejection,137, 30, 120, 15,
			PW_TEXT, "Group Name",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);


	lbox_demand_recj_details = pw_create_tlistbox(frame_demand_rejection, 0, 45, 235, 165,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_demand_rej_sel_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_demand_rej_sel_press,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_demand_recj_details,140);
	pw_tlistbox_set_selection_type(frame_demand_rejection,PW_TLISTBOX_SINGLE_SELECTION);
	pw_update(lbox_demand_recj_details,1);

	Butt_demand_rejec[1] = pw_create_button(frame_demand_rejection, 134,230, 100, 30,
			PW_TEXT, "VIEW",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,CB_VIEW,
			PW_CONFIG_END);

	Butt_demand_rejec[2]= pw_create_button(frame_demand_rejection, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demand_rej_back,
			PW_CONFIG_END);

}
void show_demand_rejection()
{
	if(!frame_demand_rejection)
		create_frame_demand_rejection();
	pw_show(frame_demand_rejection);
	get_rejectgroupids1();

}
void hide_frame_demand_rejection()
{
	pw_hide(frame_demand_rejection);
}
void get_group_details_back()
{	pw_tlistbox_clear(lbox_group_recj_details);
	hide_frame_group_details();
	show_demand_rejection();

}
void create_frame_group_details()
{
	frame_group_details =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_group_details);

	lbl_group_details[0] = pw_create_label(frame_group_details,00, 00, 240, 20,
			PW_TEXT, " ",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_group_details[1]=pw_create_label(frame_group_details,00, 20, 240, 20,
                        PW_TEXT, "Reason:",
                        PW_WRAP, PW_CHAR_WRAP,
                        PW_FONT, PW_BOLD_FONT,
                        PW_BGCOLOR, VIOLET,
                        PW_FGCOLOR, "RED",
                        PW_CONFIG_END);

	lbl_group_details[2]=pw_create_label(frame_group_details,60, 20, 240, 20,
                        PW_TEXT, "",
			PW_JUSTIFY, PW_JUSTIFY_LEFT,
                        PW_WRAP, PW_CHAR_WRAP,
                        PW_FONT, PW_BOLD_FONT,
                        PW_BGCOLOR, VIOLET,
                        PW_FGCOLOR, "white",
                        PW_CONFIG_END);

	lbl_group_details[3] =  pw_create_label(frame_group_details,70, 45, 120, 15,
			PW_TEXT, "Worker Name",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbox_group_recj_details = pw_create_tlistbox(frame_group_details, 0, 61, 235, 155,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	pw_tlistbox_set_selection_type(frame_group_details,PW_TLISTBOX_SINGLE_SELECTION);
	pw_update(lbox_group_recj_details,1);

	Butt_group_details[1] = pw_create_button(frame_group_details, 134,230, 100, 30,
			PW_TEXT, "DELETE",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,CB_DELETE1,
			PW_CONFIG_END);

	Butt_group_details[2]= pw_create_button(frame_group_details, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_group_details_back,
			PW_CONFIG_END);

}
void show_group_details()
{
	if(!frame_group_details)
		create_frame_group_details();
	pw_show(frame_group_details);
	if(atoi(error_cat)==2)
	pw_set_text(lbl_group_details[2],"Inactive household");
	else if(atoi(error_cat)==1401)
	pw_set_text(lbl_group_details[2],"already demanded in mcc");
	else
	pw_set_text(lbl_group_details[2],error_cat);
}
void hide_frame_group_details()
{
	pw_hide(frame_group_details);
}
