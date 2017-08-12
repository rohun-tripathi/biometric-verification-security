#include "types.h"
char *exec_query_get_single_value(char *data, char *query, char *database);
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
int exec_query_load_into_lbox_single(pw_widget lbox_muster_att_grpdet , char *query, char *database);
void show_demand_options();
void show_frame_no_work_demand();
void show_progress_frame();
void hide_progress_frame();

pw_widget frame_no_work_demand,lbl_no_demand_grp[5],lbox_no_demand_grp,txt_entry_grpnam,Butt_grp_nam_search,Butt_no_demand_grps[2],combo_no_demand_hab_grp,combo_no_demand_grptype,lbl_view_grp_details,frame_view_grp_details,Butt_view_grp_details[2],lbox_view_grp_details,checkall;
char check_jc[200][200];
static pw_widget  check_list=NULL;
static pw_widget *checkbox=NULL;
char tgrpid[100][100],tgrpname[300][300],batch_type[10];
int get_tempnd_details(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(tgrpid[no_worker_found], argv[0]);
	strcpy(tgrpname[no_worker_found], argv[1]);
	no_worker_found++;
	return 0;
}

int save_grp_details()
{
	char query[1024],*app_date=NULL;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	show_progress_frame("SAVING PLEASE WAIT");
	memset(tgrpid,'\0',sizeof(tgrpid));
	memset(tgrpname,'\0',sizeof(tgrpname));
	sprintf(query, "select grpid,grp_nam from temp_nodemand_grpdetails ");
	printf("query of temp1details %s\n",query);
	no_worker_found=0;
	sqlite3_exec(dbhandle, query, get_tempnd_details, NULL, NULL);
	printf("query of gredetails %s\n",query);
	if(strcmp(batch_type,"B1")==0)
        {
                sprintf(query,"select date('now','weekday 0','-3 days')");
                app_date=exec_query_get_single_value(app_date,query,DB_PACK);
        }
        else
        {
                sprintf(query,"select date('now','weekday 4','-5 days')");
                app_date=exec_query_get_single_value(app_date,query,DB_PACK);

        }

	for(i=0;i<no_worker_found;i++)
	{
		sprintf(query,"insert into no_work_demand(group_id,group_name,applicationdate) values('%s','%s','%s')",tgrpid[i],tgrpname[i],app_date);
		printf("\n query_insert is %s\n",query);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	free(app_date);
	sprintf(query,"delete from temp_nodemand_grpdetails");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	show_message_frame("Successfuly Saved");
	show_frame_no_work_demand();
	sqlite3_close(dbhandle);
	hide_progress_frame();
	return 0;
}
void get_no_demand_view_grps_back()
{
	pw_hide(frame_view_grp_details);
	show_frame_no_work_demand();
}
void create_frame_view_grpdetails()
{
	frame_view_grp_details = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	pw_hide(frame_view_grp_details);
	lbl_view_grp_details= pw_create_label(frame_view_grp_details,00, 10,240,15,
			PW_TEXT, "Group Names",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbox_view_grp_details= pw_create_tlistbox(frame_view_grp_details, 0, 36, 239, 158,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	Butt_view_grp_details[0] = pw_create_button(frame_view_grp_details, 10, 235, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_no_demand_view_grps_back,
			PW_CONFIG_END);
	Butt_view_grp_details[1]= pw_create_button(frame_view_grp_details, 150, 235, 70, 30,
			PW_TEXT,"Save",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, save_grp_details,
			PW_CONFIG_END);
}
void show_frame_view_grpdetails()
{
	if(!frame_view_grp_details)
	{
		create_frame_view_grpdetails();
		pw_show(frame_view_grp_details);
	}
	else
		pw_show(frame_view_grp_details);
}
int show_no_demand()
{
	char query[1024];
	pw_tlistbox_clear(lbox_view_grp_details);
	sprintf(query,"select distinct grp_nam from temp_nodemand_grpdetails");
	exec_query_load_into_lbox_single(lbox_view_grp_details, query, DB_PACK);
	pw_update(lbox_view_grp_details, 1);
	return 0;
}
void view_grp_details()
{
	char query[200],*rows=NULL,rw[20];
	if(strcmp(pw_get_text(combo_no_demand_hab_grp)," ")==0)
	{
		show_message_frame("Select habitation");
		return;
	}
	if(strcmp(pw_get_text(combo_no_demand_grptype)," ")==0)
	{
		show_message_frame("Select SSS-Type");
		return;
	}
	sprintf(query,"select ifnull(count(*),0) from temp_nodemand_grpdetails");
	rows=exec_query_get_single_value(rows,query,DB_PACK);
	sprintf(rw,"%s",rows);
	free(rows);
	if(atoi(rw)==0)
	{
		show_message_frame("Selcet group name");
		return;
	}
	pw_hide(frame_no_work_demand);
	show_frame_view_grpdetails();
	show_no_demand();
}
char selgrpname[200],query[1024];
int del_sel_grp()
{
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query,"delete from temp_nodemand_grpdetails where grp_nam='%s'",selgrpname);
	printf("del query is %s\n",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);
	return 0;
}
int save_sel_grp()
{
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	char nd_group_id[18],*nd_groupid=NULL;
	nd_group_id[0]='\0';
	memset(nd_group_id, 0, 1);
	sprintf(query,"select distinct group_id from household_details where group_name='%s'",selgrpname);
	nd_groupid=exec_query_get_single_value(nd_groupid, query, DB_PACK);
	sprintf(nd_group_id,"%s",nd_groupid);
	printf("groupid for taggrp is %s\n",nd_group_id);
	free(nd_groupid);
	sprintf(query,"insert into temp_nodemand_grpdetails(grpid,grp_nam) values('%s','%s')",nd_group_id,selgrpname);
	printf("\n query_insert is %s\n",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);
	return 0;
}
void get_no_demandgrps_back()
{
	pw_hide(frame_no_work_demand);
	show_demand_options();
}
int check_grp_callback(pw_widget w, pw_event e)
{
	selgrpname[0]='\0';
	if (pw_checkbutton_status(w)) {
		pw_set_bgcolor(w, "blue");
		strcpy(selgrpname,pw_get_text(w));
		printf("sel_grpname is %s\n",selgrpname);
		save_sel_grp();
	}
	else {
		selgrpname[0]='\0';
		pw_set_bgcolor(w, "white");
		strcpy(selgrpname,pw_get_text(w));
		printf("sel_jcck del is %s\n",selgrpname);
		del_sel_grp();
	}

	return 0;
}
int no_demand_hab()
{
	char query[500] ;
	pw_combobox_clear(combo_no_demand_hab_grp);
	pw_set_text(combo_no_demand_hab_grp,"--Select--");
	sprintf(query,"select distinct Habitation_Code||'-'||habitation_name from tmp_Habitation_Master ");
	exec_query_load_into_combo(combo_no_demand_hab_grp, query, DB_PACK);
	return 0;
}
void assign_check_list( int count)
{
	if (checkbox) {
		free(checkbox);
		checkbox = NULL;
	}
	checkbox = (pw_widget *)malloc (sizeof(pw_widget) * count);
	printf("count in ckeck box is %d\n",count);
}

char grpname[200][200];

int get_grpdetails(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(grpname[no_worker_found], argv[0]);
	printf("jobcardno is %s\n",grpname[no_worker_found]);
	no_worker_found++;
	return 0;
}
char nodemandhabit_code[10],sel_nodemand_hab[10],sel_grptype[20],query[1024];
void group_list( void )
{
	pw_wlistbox_clear(check_list);
	int i;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"delete from temp_nodemand_grpdetails");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	memset(grpname,'\0',sizeof(grpname));
	memset(check_jc,'\0',sizeof(check_jc));
	if(strcmp(batch_type,"B1")==0)
	{
	sprintf(query,"select distinct group_name from household_details where substr(household_code,10,5)='%s' and group_name NOT IN(select distinct group_name from Demand_Group_details where substr(application_date,1,10)=(select date('now','weekday 0','-3 days')) or substr(application_date,1,10)=(select date('now','weekday 0','-10 days')))  and group_type='%s' and group_name NOT IN(select group_name from no_work_demand where substr(applicationdate,1,10)=(select date('now','weekday 0','-3 days'))) order by group_name",nodemandhabit_code,sel_grptype);
	}
	else
	{
	sprintf(query,"select distinct group_name from household_details where substr(household_code,10,5)='%s' and group_name NOT IN(select distinct group_name from Demand_Group_details where substr(application_date,1,10)=(select date('now','weekday 4','-5 days')) or substr(application_date,1,10)=(select date('now','weekday 4','-12 days')))  and group_type='%s' and group_name NOT IN(select group_name from no_work_demand where substr(applicationdate,1,10)=(select date('now','weekday 4','-5 days'))) order by group_name",nodemandhabit_code,sel_grptype);
	}
	no_worker_found=0;
	sqlite3_exec(dbhandle, query, get_grpdetails, NULL, NULL);
	printf("query is %s",query);
	printf("no_worker_found %d\n",no_worker_found);

	for (i=0; i < no_worker_found; i++ ){
		sprintf(check_jc[i], "%s", grpname[i]);
		printf("i value is %d\n",i);
		printf("check_jc[i] is %s\n",check_jc[i]);
	}
	assign_check_list(no_worker_found);
	for (i = 0 ; i < no_worker_found; i++ ) {
		checkbox[i] = pw_create_checkbutton(check_list, 0, 0, 240, 20,
				PW_BORDER, PW_RECTANGLE_BORDER,
				PW_TEXT, check_jc[i],
				PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, check_grp_callback,
				PW_NA);
		pw_wlistbox_insertend(check_list, checkbox[i]);
	}
	sqlite3_close(dbhandle);
}
void combo_no_demand_grptypechange(pw_widget widget, int index)
{
	pw_wlistbox_clear(check_list);
	char *grp_type=NULL,*check=NULL;
	unsigned idx;
	pw_combobox_get_selection(combo_no_demand_grptype,&grp_type,&idx);
	strcpy(sel_grptype,grp_type);
	free(grp_type);
	sprintf(query,"select distinct Batch_type from Batch_info");
        check=exec_query_get_single_value(check,query,DB_PACK);
	strcpy(batch_type,check);
	free(check);
	group_list();
}
void combo_no_demand_habchange(pw_widget widget, int index)
{
	char *nodemandhabitname=NULL,query[250];
	unsigned idx;
	sel_nodemand_hab[0]='\0';
	pw_combobox_get_selection(combo_no_demand_hab_grp,&nodemandhabitname,&idx);
	strcpy(sel_nodemand_hab,nodemandhabitname);
	strncpy(nodemandhabit_code,sel_nodemand_hab,5);
	printf("habit code is %s\n",nodemandhabit_code);
	free(nodemandhabitname);
	pw_combobox_clear(combo_no_demand_grptype);
	sprintf(query,"select distinct group_type from household_details where group_type in ('GENERAL','SSS-T','DISABLED')");
	exec_query_load_into_combo(combo_no_demand_grptype, query, DB_PACK);
}
char gn[500][500],gid[500][500];	
int details(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(gn[no_worker_found], argv[0]);
	strcpy(gid[no_worker_found], argv[1]);
	printf("jobcardno is %s\n",grpname[no_worker_found]);
	no_worker_found++;
	return 0;
}

int grp_all(pw_widget w, pw_event e)
{
	if(strcmp(pw_get_text(combo_no_demand_hab_grp)," ")==0)
	{
		show_message_frame("Select habitation");
		pw_checkbutton_deselect(w);
		return 0;
	}
	if(strcmp(pw_get_text(combo_no_demand_grptype)," ")==0)
	{
		show_message_frame("Select SSS-Type");
		pw_checkbutton_deselect(w);
		return 0;
	}
	char *grp_cnt=NULL;
	if(strcmp(batch_type,"B1")==0)
        {
        sprintf(query,"select count(*) from household_details where substr(household_code,10,5)='%s' and group_name NOT IN(select distinct group_name from Demand_Group_details where substr(application_date,1,10)=(select date('now','weekday 0','-3 days')) or substr(application_date,1,10)=(select date('now','weekday 0','-10 days')))  and group_type='%s' and group_name NOT IN(select group_name from no_work_demand where substr(applicationdate,1,10)=(select date('now','weekday 0','-3 days'))) order by group_name",nodemandhabit_code,sel_grptype);
        }
        else
        {
        sprintf(query,"select count(*) from household_details where substr(household_code,10,5)='%s' and group_name NOT IN(select distinct group_name from Demand_Group_details where substr(application_date,1,10)=(select date('now','weekday 4','-5 days')) or substr(application_date,1,10)=(select date('now','weekday 4','-12 days')))  and group_type='%s' and group_name NOT IN(select group_name from no_work_demand where substr(applicationdate,1,10)=(select date('now','weekday 4','-5 days'))) order by group_name",nodemandhabit_code,sel_grptype);
	}
	grp_cnt=exec_query_get_single_value(grp_cnt,query,DB_PACK);
	if(atoi(grp_cnt)==0)
	{
	pw_checkbutton_deselect(w);
		show_message_frame("NO Groups");
		return 0;
	}
	free(grp_cnt);

	show_progress_frame("Processing Please Wait...");
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	memset(gid,'\0',sizeof(gid));
	memset(gn,'\0',sizeof(gn));
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query,"delete from temp_nodemand_grpdetails");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	if(strcmp(batch_type,"B1")==0)
        {
        sprintf(query,"select distinct group_name,group_id from household_details where substr(household_code,10,5)='%s' and group_name NOT IN(select distinct group_name from Demand_Group_details where substr(application_date,1,10)=(select date('now','weekday 0','-3 days')) or substr(application_date,1,10)=(select date('now','weekday 0','-10 days')))  and group_type='%s' and group_name NOT IN(select group_name from no_work_demand where substr(applicationdate,1,10)=(select date('now','weekday 0','-3 days'))) order by group_name",nodemandhabit_code,sel_grptype);
        }
        else
        {
        sprintf(query,"select distinct group_name,group_id from household_details where substr(household_code,10,5)='%s' and group_name NOT IN(select distinct group_name from Demand_Group_details where substr(application_date,1,10)=(select date('now','weekday 4','-5 days')) or substr(application_date,1,10)=(select date('now','weekday 4','-12 days')))  and group_type='%s' and group_name NOT IN(select group_name from no_work_demand where substr(applicationdate,1,10)=(select date('now','weekday 4','-5 days'))) order by group_name",nodemandhabit_code,sel_grptype);
	}	
	no_worker_found=0;
	sqlite3_exec(dbhandle, query, details, NULL, NULL);
	printf("query of gredetails %s\n",query);
	for(i=0;i<no_worker_found;i++)
	{
		sprintf(query,"insert into temp_nodemand_grpdetails(grpid,grp_nam) values('%s','%s')",gid[i],gn[i]);
		printf("\n query_insert is %s\n",query);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	sqlite3_close(dbhandle);
	pw_checkbutton_deselect(w);
	hide_progress_frame();
	pw_hide(frame_no_work_demand);
	show_frame_view_grpdetails();
	show_no_demand();
	return 0;
}
void create_frame_no_work_demand()
{
	frame_no_work_demand = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	pw_hide(frame_no_work_demand);
	lbl_no_demand_grp[0]= pw_create_label(frame_no_work_demand,00, 00, 85,20,
			PW_TEXT, "Habitation:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_no_demand_hab_grp= pw_create_combobox(frame_no_work_demand, 85, 00, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_no_demand_hab_grp,combo_no_demand_habchange);
	lbl_no_demand_grp[1]= pw_create_label(frame_no_work_demand,00, 22, 85,20,
			PW_TEXT, "SSS-Type",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_no_demand_grptype = pw_create_combobox(frame_no_work_demand, 85, 22, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_no_demand_grptype,combo_no_demand_grptypechange);
	checkall = pw_create_checkbutton(frame_no_work_demand,03, 45, 40, 15,
			PW_BGCOLOR, BLUE ,
			PW_FGCOLOR, "white",
			PW_TEXT, "ALL",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, grp_all,
			PW_NA);
	lbl_no_demand_grp[3]= pw_create_label(frame_no_work_demand,00, 60, 240,15,
			PW_TEXT, "Group Names",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	check_list = pw_create_wlistbox(frame_no_work_demand, 0, 75, 240, 150, PW_NA);

	Butt_no_demand_grps[0] = pw_create_button(frame_no_work_demand, 10, 235, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_no_demandgrps_back,
			PW_CONFIG_END);
	Butt_no_demand_grps[1]= pw_create_button(frame_no_work_demand, 140, 235, 70, 30,
			PW_TEXT,"view",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, view_grp_details,
			PW_CONFIG_END);
}	
void show_frame_no_work_demand()
{
	if(!frame_no_work_demand)
		create_frame_no_work_demand();
	pw_wlistbox_clear(check_list);
	no_demand_hab();
	pw_show(frame_no_work_demand);
	pw_combobox_clear(combo_no_demand_grptype);
	pw_set_text(combo_no_demand_hab_grp,"--Select--");
}
