#include "types.h"
void show_fa_module();
void hide_demand_options_frame();
void show_demand_grps_frame();
void show_demand_reports();
int demand_upload();
int gp_test();
int work_allocation_upload();
void show_demand_aloc_frame();
void show_frame_demand_pending();
void hide_frame_demand_pending();
void hide_frame_demand_mis_reports();
void show_wrk_alloc_report();
void show_frame_no_work_demand();
void show_demand_options();
void show_work_alloc_rejection();
void show_demand_rejection();
void hide_frame_demand_pending_details();
void show_demand_mis_reports();
int exec_query_load_into_lbox_double(pw_widget lbox_muster_att_grpdet , char *query, char *database);
int exec_query_load_into_lbox_trriple(pw_widget lbox, char *query, char *database);
int txtlb_get_element();
int exec_query_load_into_lbox_single(pw_widget lbox_muster_att_grpdet , char *query, char *database);
char *exec_query_get_single_value(char *data, char *query, char *database);

pw_widget btn_demand_pending_back,btn_demand_pending_back1, btn_demand_pending,frame_demand_pending,frame_demand_pending_details,frame_demand_mis_reports,btn_demand_mis_reports[5],btn_demand_mis_back,frame_nowork_demand_reports,Butt_no_work_demand_reports[2],lbox_nowork_demand_reports,lbl_no_work_demand_reports,lbl_demand_pending,lbox_demand_pending,lbl_demand_pending1,lbox_demand_pending1,
	  lbl_demand_pending2,lbl_demand_pending3,lbl_demand_pending4;

void get_nowrk_demand_back()
{
	pw_hide(frame_nowork_demand_reports);
	show_demand_options();
}
char maxappdate[15];
int delete_no_work_demand_grp()
{	
	char *taskname=NULL,sel_grpname[50];
	i=pw_tlistbox_get_selection(lbox_nowork_demand_reports);
	if(i==-1)
	{
		show_message_frame("select Group");
		return 0;
	}
	else
	{
		pw_tlistbox_get_element(lbox_nowork_demand_reports, &taskname,i, 0);
		txtlb_get_element(lbox_nowork_demand_reports, &taskname, i,0 );
		strcpy(sel_grpname,taskname);
		free(taskname);
		char query[512],*sta=NULL,status[10];
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
			printf("Unable to open db\n");
		printf("sel_grpname \n \n :%s: \n \n",sel_grpname);
		sprintf(query,"select status from no_work_demand where group_name='%s' and applicationdate='%s' ",sel_grpname,maxappdate);
		sta=exec_query_get_single_value(sta,query,DB_PACK);
		sprintf(status,"%s",sta);
		free(sta);
		if(strcmp(status,"Y")==0)
		{
			show_message_frame("Already uploaded");
			return 0;
		}
		else
		{
			sprintf(query,"delete from no_work_demand where group_name='%s' and status='N' and applicationdate='%s'",sel_grpname,maxappdate);
			printf("query is %s",query);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
			pw_tlistbox_remove(lbox_nowork_demand_reports,i,1);
		}
		sqlite3_close(dbhandle);
	}
	return 0;
}
void create_frame_nowork_demand_reports()
{
	frame_nowork_demand_reports = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	pw_hide(frame_nowork_demand_reports);
	lbl_no_work_demand_reports= pw_create_label(frame_nowork_demand_reports,00, 15, 240,15,
			PW_TEXT, "Group Names",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbox_nowork_demand_reports= pw_create_tlistbox(frame_nowork_demand_reports, 00, 35, 239, 160,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,	
			PW_CONFIG_END);
	Butt_no_work_demand_reports[0]= pw_create_button(frame_nowork_demand_reports, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_nowrk_demand_back,
			PW_CONFIG_END);
	Butt_no_work_demand_reports[1] = pw_create_button(frame_nowork_demand_reports, 90,230, 60, 30,
			PW_TEXT, "Delete",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, delete_no_work_demand_grp,
			PW_CONFIG_END);
}

void show_frame_nowork_demand_reports()
{
	char query[300];
	char *max=NULL;
	if(!frame_nowork_demand_reports)
	{
		create_frame_nowork_demand_reports();
		pw_show(frame_nowork_demand_reports);
	}
	else
		pw_show(frame_nowork_demand_reports);
	pw_tlistbox_clear(lbox_nowork_demand_reports);
	sprintf(query,"select ifnull(MAX(applicationdate),0) from no_work_demand");
	max=exec_query_get_single_value(max,query,DB_PACK);
	sprintf(maxappdate,"%s",max);	
	free(max);
	sprintf(query,"select group_name from no_work_demand where applicationdate='%s'",maxappdate);
	exec_query_load_into_lbox_single(lbox_nowork_demand_reports,query,DB_PACK);
}
char wday[10];

void cb_wrkaloc_rejection()
{
	hide_frame_demand_mis_reports();
	show_work_alloc_rejection();
}
void cb_demand_rejection()
{
	hide_frame_demand_mis_reports();
	show_demand_rejection();
}
void cb_wrk_alloc_report()
{
	hide_frame_demand_mis_reports();
	show_wrk_alloc_report();
}
void cb_demandreport()
{	
	hide_frame_demand_mis_reports();
	show_demand_reports();

}
void cb_demand_mis_back()
{
	hide_frame_demand_mis_reports();
	show_demand_options();

}
void cb_no_wrk_demand_report()
{
	hide_frame_demand_mis_reports();
	show_frame_nowork_demand_reports();
}
void cb_demand_pending()
{
	hide_frame_demand_mis_reports();
	show_frame_demand_pending();
}
void cb_demandpending_back1()
{
	hide_frame_demand_pending_details();
	show_frame_demand_pending();
}
char *grpname1=NULL;
void create_frame_demand_pending_details()
{
	frame_demand_pending_details= pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_demand_pending);

	lbl_demand_pending1= pw_create_label(frame_demand_pending_details,00, 15, 240,15,
			PW_TEXT, "",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_demand_pending2=pw_create_label(frame_demand_pending_details,00, 40, 170,15,
			PW_TEXT, "Demand Period",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_demand_pending3=pw_create_label(frame_demand_pending_details,170, 40, 45,15,
			PW_TEXT, "P.Days",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_demand_pending3=pw_create_label(frame_demand_pending_details,210, 40, 30,15,
			PW_TEXT, "No",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbox_demand_pending1 = pw_create_tlistbox(frame_demand_pending_details, 0, 56, 240, 160,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_demand_pending1,30);
	pw_tlistbox_addcolumn(lbox_demand_pending1,20);

	btn_demand_pending_back1 = pw_create_button(frame_demand_pending_details, 30, 220, 40, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demandpending_back1,
			PW_CONFIG_END);

}
void show_frame_demand_pending_details()
{
	if(!frame_demand_pending_details);
	{
		create_frame_demand_pending_details();
		pw_show(frame_demand_pending_details);
	}
	pw_show(frame_demand_pending_details);
	char query[1024];
	sprintf(query,"select fromdate||'/'||todate,(case when allocated_status=='N' then '14' else '7' end),count(Householdcode) from Demand_Group_details where group_name='%s' and allocated_status IN('P','N')",grpname1);
	exec_query_load_into_lbox_trriple(lbox_demand_pending1, query, DB_PACK);
	pw_update(lbox_demand_pending1, 1);

}
void hide_frame_demand_pending_details()
{
	pw_hide(frame_demand_pending_details);
}
void cb_demandpending()
{
	int idx = pw_tlistbox_get_selection(lbox_demand_pending);
	pw_tlistbox_get_element(lbox_demand_pending, &grpname1,idx, 0);

	txtlb_get_element(lbox_demand_pending, &grpname1, idx, 0);
	if(idx==-1)
	{
		show_message_frame("SELECT A GROUP");
		return ;
	}
	hide_frame_demand_pending();
	show_frame_demand_pending_details();
	pw_set_text(lbl_demand_pending1,grpname1);

}
void cb_demandpending_back()
{
	hide_frame_demand_pending();
	show_demand_mis_reports();
}

void create_frame_demand_pending()
{
	frame_demand_pending= pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_demand_pending);

	lbl_demand_pending= pw_create_label(frame_demand_pending,00, 15, 240,15,
			PW_TEXT, "Pending Group List",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbox_demand_pending = pw_create_tlistbox(frame_demand_pending, 0, 56, 240, 160,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_demand_pending,60);
	btn_demand_pending_back = pw_create_button(frame_demand_pending, 30, 220, 40, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demandpending_back,
			PW_CONFIG_END);
	btn_demand_pending = pw_create_button(frame_demand_pending, 120, 220, 80, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "View Details",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demandpending,
			PW_CONFIG_END);
}
void show_frame_demand_pending()
{
	if(!frame_demand_pending);
	{
		create_frame_demand_pending();
		pw_show(frame_demand_pending);
	}
	pw_show(frame_demand_pending);
	char query[1024];
	sprintf(query,"select distinct group_name,substr(recepit_id,3,6) from Demand_Group_details where allocated_status='N' or allocated_status='P'");
	exec_query_load_into_lbox_double(lbox_demand_pending, query, DB_PACK);
	pw_update(lbox_demand_pending, 1);

}
void hide_frame_demand_pending()
{
	pw_hide(frame_demand_pending);
}
void create_frame_demand_mis_reports()
{
	frame_demand_mis_reports = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_demand_mis_reports);

	btn_demand_mis_reports[0] = pw_create_button(frame_demand_mis_reports, 30, 10, 180, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Demand Creation",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demandreport,
			PW_CONFIG_END);
	btn_demand_mis_reports[1] = pw_create_button(frame_demand_mis_reports, 30, 50, 180, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Work Allocation ",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wrk_alloc_report,
			PW_CONFIG_END);
	btn_demand_mis_reports[2] = pw_create_button(frame_demand_mis_reports, 30, 90, 180, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Zero Demand Groups",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_no_wrk_demand_report,
			PW_CONFIG_END);

	btn_demand_mis_reports[3] = pw_create_button(frame_demand_mis_reports, 30, 130, 180, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Demand Rejection",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demand_rejection,
			PW_CONFIG_END);

	btn_demand_mis_reports[4] = pw_create_button(frame_demand_mis_reports, 30, 170, 180, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Work Allocation Rejection",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wrkaloc_rejection,
			PW_CONFIG_END);
	btn_demand_mis_reports[5] = pw_create_button(frame_demand_mis_reports, 30, 210, 180, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Demand Pending",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demand_pending,
			PW_CONFIG_END);


	btn_demand_mis_back = pw_create_button(frame_demand_mis_reports, 10, 240, 34, 25,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demand_mis_back,
			PW_CONFIG_END);

}
void show_demand_mis_reports()
{
	if(!frame_demand_mis_reports);
	{
		create_frame_demand_mis_reports();
		pw_show(frame_demand_mis_reports);
	}
	pw_show(frame_demand_mis_reports);
}
void hide_frame_demand_mis_reports()
{
	pw_hide(frame_demand_mis_reports);
}
int weekday()
{
	char ch[3]="%w",*wkday=NULL,query[100];
	sprintf(query,"select strftime('%s','now')",ch);
	wkday=exec_query_get_single_value(wkday,query,DB_PACK);
	printf("query is %s",query);
	sprintf(wday,"%s",wkday);
	free(wkday);
	return atoi(wday);
}

void no_demand()
{
weekday();
char *check=NULL,query[512];
sprintf(query,"select distinct Batch_type from Batch_info");
check=exec_query_get_single_value(check,query,DB_PACK);
	if(strcmp(check,"B1")==0)
	{
	
		if(atoi(wday)==4||atoi(wday)==5||atoi(wday)==6||atoi(wday)==0)
		{
		hide_demand_options_frame();
		show_frame_no_work_demand();
		}
		else
		{
	show_message_frame("Allowed on Thursday,Friday,\nSaturday,Sunday For \nBatch-1");	
		return;
		}
	}
	else
	{
	if(atoi(wday)==6||atoi(wday)==0||atoi(wday)==1||atoi(wday)==2||atoi(wday)==3)
		{
		hide_demand_options_frame();
		show_frame_no_work_demand();
		}
		else
		{
			show_message_frame("Allowed on Saturday,Sunday,\nMonday,Tuesday,\nWednesday For Batch-2");
		return;
		}
	}
}
void cb_misreport()
{
	hide_demand_options_frame();
	show_demand_mis_reports();
}
void cb_demand_aloc()
{	
	char *check1=NULL,*check2=NULL,query[1024];
	sprintf(query,"select max(created_date) from Batch_info");
	check1=exec_query_get_single_value(check1,query,DB_PACK);
	sprintf(query,"select Batch_type from Batch_info where created_date='%s'",check1);
	check2=exec_query_get_single_value(check2,query,DB_PACK);
	if(strcmp(check2,"B1")==0)
	{
		weekday();
		if(atoi(wday)==4 || atoi(wday)==5|| atoi(wday)==6 || atoi(wday)==0)
		{
			hide_demand_options_frame();
			show_demand_aloc_frame();
		}
		else
		{
			show_message_frame("Allowed on Thursday,Friday,\nSaturday,Sunday For \nBatch-1");
			return;
		}
	}
	else
	{
		weekday();
		if(atoi(wday)==6 || atoi(wday)==0|| atoi(wday)==1||atoi(wday)==2||atoi(wday)==3)
		{
			hide_demand_options_frame();
			show_demand_aloc_frame();
		}
		else
		{
			show_message_frame("Allowed on Saturday,Sunday,\nMonday,Tuesday \nWednesday For Batch-2");
			return;
		}
	}
	  free(check1);
        free(check2);
}
void cb_demand_grps()
{
	char *check1=NULL,*check2=NULL,query[1024];
	sprintf(query,"select max(created_date) from Batch_info");
	check1=exec_query_get_single_value(check1,query,DB_PACK);
	sprintf(query,"select Batch_type from Batch_info where created_date='%s'",check1);
	check2=exec_query_get_single_value(check2,query,DB_PACK);
	if(strcmp(check2,"B1")==0)
	{
		weekday();
		if(atoi(wday)==4 || atoi(wday)==5|| atoi(wday)==6 || atoi(wday)==0)
		{
			hide_demand_options_frame();
			show_demand_grps_frame();
		}
		else
		{
			show_message_frame("Allowed on Thursday,Friday,\nSaturday,Sunday For \nBatch-1");
			return;
		}
	}
	else
	{
		weekday();
		if(atoi(wday)==6 || atoi(wday)==0|| atoi(wday)==1||atoi(wday)==2 ||atoi(wday)==3)
		{
			hide_demand_options_frame();
			show_demand_grps_frame();
		}
		else
		{
			show_message_frame("Allowed on Saturday,Sunday,\nMonday,Tuesday,\nWednesday For Batch-2");
			return;
		}
	}
	free(check1);
	free(check2);
}
void cb_demandoptions_back()
{
	hide_demand_options_frame();
	show_fa_module();
}
pw_widget demand_options_frame,btn_demand_option[2],btn_demand_back;
void create_demand_options_frame()
{
	demand_options_frame =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, "BLUE" ,
			PW_CONFIG_END);
	pw_hide(demand_options_frame);

	btn_demand_option[0] = pw_create_button(demand_options_frame, 30, 25, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Demand Capture",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demand_grps,
			PW_CONFIG_END);
	btn_demand_option[1] = pw_create_button(demand_options_frame, 135, 25, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Work Allocation",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demand_aloc,
			PW_CONFIG_END);
	btn_demand_option[2] = pw_create_button(demand_options_frame, 30, 140, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "No Work Demand",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, no_demand,
			PW_CONFIG_END);
	btn_demand_option[3] = pw_create_button(demand_options_frame, 135, 140, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Demand History",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_misreport,
			PW_CONFIG_END);
	btn_demand_back = pw_create_button(demand_options_frame, 10, 220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_demandoptions_back,
			PW_CONFIG_END);
}
void show_demand_options()
{
	if(!demand_options_frame)
	{
		create_demand_options_frame();
		pw_show(demand_options_frame);
	}
	else
		pw_show(demand_options_frame);
}
void hide_demand_options_frame()
{
	pw_hide(demand_options_frame);
}

