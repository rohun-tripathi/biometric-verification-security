#include "types.h"
#include <autils.h>

void show_demand_grps_frame();
void hide_demand_grps_frame();
void show_demand_options();
void hide_demanded_grp_details_frame();
int exec_query_load_into_lbox_single(pw_widget lbox_muster_att_grpdet , char *query, char *database);
int exec_query_load_into_lbox_double(pw_widget lbox_muster_att_grpdet , char *query, char *database);
void show_jobcard_check_frame();
void txtlb_get_element(pw_widget w, char **s, int idx, int column);
char *exec_query_get_single_value(char *data, char *query, char *database);
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
void show_progress_frame();
void hide_progress_frame();
char get_date[15];
 char *demanded_groupid=NULL;
static pw_widget  check_list=NULL;
static pw_widget *checkbox=NULL;

pw_widget demand_grps_frame,combo_demand_hab,lbl_demand_grps[4],lbox_demand_grps,Butt_demand_grps[4],txt_entry_grp_nam,Butt_grpnam_search,demanded_grp_details_frame,lbl_demanded_grp_detail[6],txt_entry_days,Butt_demand_grpsdetail[4],lbox_grpmembers,lbl_frmdt,lbl_todt,rb_all,rb_sel,combo_demand_grptype,check_all,lbl_recepit,textentry_demand_id;
char sel_to_dt[12],sel_frm_dt[12],jcno[100][100],jcname[100][100];
char dt_mand_pan_id[10],sel_demand_hab[50],dmnd_frmdt[15],dmnd_todt[15];
pw_widget lbl_apps_dt,lbl_apps_frmdt;
int  Gps_Test(double *lat, double *lon);

char *get_sel_frm_dt()
{
	return sel_frm_dt;
}
char *get_sel_to_dt()
{
	return sel_to_dt;
}
int del_temp_table()
{
	char query[1024];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query,"delete from temp_demand_grpdetails");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);
	return 0;
}

char max_frmdt[15],max_todt[15];
int get_grp_maxdates(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(max_frmdt, argv[0]);
	strcpy(max_todt, argv[1]);
	printf("max_todt is %s\n",max_todt);
	return 0;
}
int get_grp_details(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(jcno[no_worker_found], argv[0]);
	no_worker_found++;
	return 0;
}

char sel_grp_type[20],demand_type[40][40];
int get_tempdemand_details(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(jcno[no_worker_found], argv[0]);
	strcpy(demand_type[no_worker_found], argv[1]);
	no_worker_found++;
	return 0;
}
pw_widget frame_demand_jc_details,Butt_demand_jcback,lbox_demand_jc_details,Butt_demand_jcsave;
char sel_demand_id[7],sel_demanded_grp[150];
char demand_jc[20],sel_jcchk[25];
char demand_group_id[14],batch_check1[3];
void  groups_to_demand();
int get_demandjc_save()
{
	char query[1024];
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	sel_demand_id[0]='\0';
	strcpy(sel_demand_id,pw_get_text(textentry_demand_id));
	printf("sel demandid is %s\n",sel_demand_id);
	if(strcmp(sel_demand_id,"")==0)
	{
		show_message_frame("ENTER Receipt No.");
		return -1 ;
	}
	else if (strlen(sel_demand_id)!=6)
	{
		show_message_frame("Receipt No. Mustbe 6 Digit");
		pw_entry_clear(textentry_demand_id);
		return -1 ;
	}
	else if(strcmp(sel_demand_id,"00000")==0)
	{
		show_message_frame("Receipt No. NOT tobe Zero's");
		pw_entry_clear(textentry_demand_id);
		return -1 ;
	}
	char *chk_qty=NULL,query1[1024];

	chk_qty=strpbrk(pw_get_text(textentry_demand_id),"-*+.abcdefghijklmnopqrstuvwxyz");
	printf("chk_qty is %s\n",chk_qty);
	while(chk_qty != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(textentry_demand_id);
		return 0;
	}
	free(chk_qty);
	char recepit_id[9];
	char *pch=NULL,year_buff[5]="";
	recepit_id[0]='\0';
	printf("current date is %s\n",get_date);
	pch=strtok(get_date, "-");
	printf("buffer date is %s\n",pch);
	strncpy(year_buff,pch+2,2);
	printf("recepit_id is %s\n",year_buff);
	strcat(recepit_id,year_buff);
	strcat(recepit_id,sel_demand_id);
	printf("recepit id is %s\n",recepit_id);
	//TODO:check recepit id from demand table
	char *recept_cnt=NULL,*recept_cnt1=NULL,*recept_cnt2=NULL;
	sprintf(query,"select ifnull(count(*),'0') from Demand_Group_details where recepit_id='%s' and group_name!='%s'",recepit_id,sel_demanded_grp);
	recept_cnt = exec_query_get_single_value(recept_cnt, query, DB_PACK);
	sprintf(query1,"select ifnull(count(*),'0') from mcc_demand_group_details where substr(receipt_no,7,8)='%s' and group_id='%s'",recepit_id,demand_group_id);
	recept_cnt1 = exec_query_get_single_value(recept_cnt1, query1, DB_PACK);
	sprintf(query,"select ifnull(count(*),'0') from work_allocation_details  where substr(recepit_no,7,8)='%s' and group_id!='%s'",recepit_id,demand_group_id);
	recept_cnt2= exec_query_get_single_value(recept_cnt1, query, DB_PACK);
	if(atoi(recept_cnt)>=1 || atoi(recept_cnt1)>=1||atoi(recept_cnt2)>=1)
	{
		show_message_frame("Duplicate Receipt No.");
		return 0 ;
	}
	free(recept_cnt);
	free(recept_cnt1);
	free(recept_cnt2);
	show_progress_frame("SAVING PLEASE WAIT");
	double lat, lon;
	Gps_Test(&lat, &lon);
	char demand_date1[25]="";
	int hour=0, min=0, sec=0, day1=0, month1=0, year1=0;

	char *demand_date=NULL;
	au_get_time(&hour, &min, &sec, &day1, &month1, &year1);
	//sprintf(demand_date1, "%02d-%02d-%04d %02d:%02d:%02d", day, month, year,hour,min,sec);
	sprintf(demand_date1, "%04d-%02d-%02d %02d:%02d:%02d",year1,month1,day1,hour,min,sec);
	 if(strcmp(batch_check1,"B1")==0)
	{
	sprintf(query,"select date('now','weekday 0','-3 days')");
	demand_date=exec_query_get_single_value(demand_date,query,DB_PACK);
	}
	else
	{
	sprintf(query,"select date('now','weekday 3','-4 days')");
	demand_date=exec_query_get_single_value(demand_date,query,DB_PACK);
	}
	printf("demand_date is %s\n",demand_date);
	memset(jcno,'\0',sizeof(jcno));
	memset(demand_type,'\0',sizeof(demand_type));
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	printf("group id is %s\n",demand_group_id);
	sprintf(query, "select jc_info,demand_type from temp_demand_grpdetails ");
	printf("query of temp1details %s\n",query);
	no_worker_found=0;
	sqlite3_exec(dbhandle, query, get_tempdemand_details, NULL, NULL);
	printf("query of gredetails %s\n",query);
	for(i=0;i<no_worker_found;i++)
	{
		sprintf(query,"insert into Demand_Group_details(group_id,group_name,group_type,Householdcode,Demanddays,recepit_id,fromdate,todate,demanddate,Gpslongitude,Gpslatitude,status,allocated_status,demand_type,application_date) values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%lf', '%lf','%s','%s','%s','%s')",demand_group_id,sel_demanded_grp,sel_grp_type,jcno[i],"14",recepit_id,dmnd_frmdt,dmnd_todt,demand_date1,lat,lon,"N","N",demand_type[i],demand_date);
		printf("\n query_insert is %s\n",query);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	sel_demanded_grp[0]='\0';       
	pw_tlistbox_remove_selection_all(lbox_demand_grps);
	del_temp_table();
	hide_progress_frame();
	sqlite3_close(dbhandle);        
	pw_hide(frame_demand_jc_details);	
	pw_show(demand_grps_frame);
	 groups_to_demand();
	show_message_frame("Successfuly Saved");

	printf("demand_jc in demand capture aftersave is %s\n",demand_jc);
	return 0;
}
int sel_jc_list()
{
	char query[1024];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	pw_tlistbox_clear(lbox_demand_jc_details);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from temp_demand_grpdetails where rowid not in(select max(rowid) from temp_demand_grpdetails  group by jc_info)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query, "select distinct substr(household_code,15,4)||worker_code||'-'|| name from household_details hh ,temp_demand_grpdetails tdg where hh.household_code||hh.worker_code = tdg.jc_info and group_type='%s' and  is_active='Y' and group_name='%s'",sel_grp_type,sel_demanded_grp);

	exec_query_load_into_lbox_single(lbox_demand_jc_details, query, DB_PACK);
	pw_update(lbox_demand_jc_details, 1);
	sqlite3_close(dbhandle);
	return 0;
}
void get_demandjc_back()
{
	sel_demanded_grp[0]='\0';
	sel_frm_dt[0]='\0';
	sel_to_dt[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_demand_grps);
	del_temp_table();
	pw_hide(frame_demand_jc_details);
	pw_show(demand_grps_frame);
}
void create_demand_jc_details()
{
	frame_demand_jc_details = pw_create_frame(toplevel, 0, 15, 240, 260, PW_NA);

	lbl_recepit =  pw_create_label(frame_demand_jc_details, 0, 5, 90, 20,
			PW_TEXT, "Receipt No:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	textentry_demand_id =  pw_create_entry(frame_demand_jc_details, 90, 5, 60, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_num,
			PW_CONFIG_END);

	pw_focus(textentry_demand_id);

	lbox_demand_jc_details = pw_create_tlistbox(frame_demand_jc_details, 0, 56, 240, 160,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_set_selection_type(lbox_demand_jc_details,PW_TLISTBOX_SINGLE_SELECTION);
	Butt_demand_jcback = pw_create_button(frame_demand_jc_details, 10, 220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demandjc_back,
			PW_CONFIG_END);
	Butt_demand_jcsave = pw_create_button(frame_demand_jc_details, 140, 220, 60, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "SAVE",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demandjc_save,
			PW_CONFIG_END);
}
char buf[15];
void show_demand_jc()
{
	if(frame_demand_jc_details==NULL)
		create_demand_jc_details();
	pw_show(frame_demand_jc_details);
	pw_entry_clear(textentry_demand_id);
	buf[0]='\0';
	sel_jc_list();
}
int get_demand_view(pw_widget w, pw_event e)
{
	char query[1024];
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	/*if(strcmp(sel_frm_dt,"")==0) 
	{
		show_message_frame("SELECT FROM DATE");
		return 0;
	}*/
	char *jc_cnt=NULL;
	sprintf(query,"select ifnull(count(*),'0') from temp_demand_grpdetails");
	jc_cnt =exec_query_get_single_value(jc_cnt, query, DB_PACK);
	if(atoi(jc_cnt)==0)
	{
		show_message_frame("Select DATA ");
		return 0;
	}
	free(jc_cnt);

	show_demand_jc();
	return 0;
}
int cb_grp_sel()
{
	char query[1024];
	del_temp_table();



/*	if(strcmp(sel_frm_dt,"")==0)
	{
		show_message_frame("SELECT FROM DATE");
		return -1 ;


	}
*/	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	show_progress_frame("Processing Please Wait...");
	sprintf(query,"delete from temp_demand_grpdetails");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);
	return 0;
}
int cb_grp_all(pw_widget w, pw_event e)
{
	char query[1024];
	/*if(strcmp(sel_frm_dt,"")==0)
	{
		show_message_frame("SELECT FROM DATE");
		pw_checkbutton_deselect(check_all);
		return -1 ;
	}*/
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	char *demand_grp_chk=NULL,*demand_grpunmet_check=NULL;
			sprintf(query,"select count(*) from mcc_demand_group_details where allocated_days  between 0 and 7 and group_id='%s'",demand_group_id);
		demand_grpunmet_check = exec_query_get_single_value(demand_grpunmet_check, query, DB_PACK);
		if(atoi(demand_grpunmet_check)>=1)
		{
			show_message_frame("SOME JOBCARD HAS \nPENDING DEMAND");
			pw_checkbutton_deselect(check_all);
            		pw_set_bgcolor(w, "white");
			return 0;
		}
		free(demand_grpunmet_check);

	sprintf(query,"select ifnull(count(*),'0') from mcc_demand_group_details where (select ifnull(max(demand_todate),0) from mcc_demand_group_details where group_id='%s')>='%s' and group_id='%s'",demand_group_id,dmnd_frmdt,demand_group_id);

	demand_grp_chk = exec_query_get_single_value(demand_grp_chk, query, DB_PACK);
	if(atoi(demand_grp_chk)>=1)
	{
		show_message_frame("Already Demanded in MCC");
		pw_checkbutton_deselect(check_all);
		pw_set_bgcolor(w, "white");
		return 0;
	}
	free(demand_grp_chk);
	sprintf(query,"select  ifnull(max(fromdate),'0'),ifnull(max(todate),'0') from Demand_Group_details where group_id='%s' and status in('N','Y')", demand_group_id);
	sqlite3_exec(dbhandle, query, get_grp_maxdates, NULL, NULL);
	printf("query is %s\n",query);
	char *demand_cnt_chk=NULL;
	sprintf(query,"select (case when '%s' <= '%s' then 'NOK' else 'OK' end)",dmnd_frmdt,max_todt);
	printf("query of case %s\n",query);
	demand_cnt_chk = exec_query_get_single_value(demand_cnt_chk, query, DB_PACK);

	if(strcmp(demand_cnt_chk,"NOK")==0)
	{
		show_message_frame("Already Demanded in \n Selected Period");
		pw_checkbutton_deselect(w);
		return 0;
	}
	free(demand_cnt_chk);
	show_demand_jc();
	show_progress_frame("Processing Please Wait...");
	sprintf(query, "select JobCard_id|| worker_code from (select distinct substr(household_code,1,18) as Jobcard_id,worker_code as worker_code  from household_details  where is_active='Y' and group_name='%s') as a where jobcard_id not in(select substr(jobcard_no,1,18)from wage_info where no_days>=150)",sel_demanded_grp);

	no_worker_found=0;	
	sqlite3_exec(dbhandle, query, get_grp_details, NULL, NULL);
	printf("query of gredetails %s\n",query);
	for(i=0;i<no_worker_found;i++)
	{
		sprintf(query,"insert into temp_demand_grpdetails(jc_info,demand_type) values('%s','%s')",jcno[i],"Y");
		printf("\n query_insert is %s\n",query);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	sel_jc_list();
	hide_progress_frame();
	sqlite3_close(dbhandle);	
	return 0;
}
int del_jc_sel()
{
	char query[1024];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	printf("demand_jcdel is %s\n",demand_jc);
	printf("jobcard is %s\n",sel_jcchk);

	sprintf(query,"delete from temp_demand_grpdetails where jc_info='%s%s'",demand_jc,sel_jcchk);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("delete query is %s\n",query);
	sqlite3_close(dbhandle);

	return 0;
}
int save_jc_sel()
{
	char query[1024];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	printf("demand_jcsave is %s\n",demand_jc);
	printf("jobcard is %s\n",sel_jcchk);
	sprintf(query,"insert into temp_demand_grpdetails(jc_info,demand_type) values('%s%s','%s')",demand_jc,sel_jcchk,"N");
	printf("\n query_insert is %s\n",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);
	return 0;
}
int check_box_callback(pw_widget w, pw_event e)
{
	char query[1024];
	/*if(strcmp(sel_frm_dt,"")==0)
	{
		show_message_frame("SELECT FROM DATE");
		pw_checkbutton_deselect(w);
		return -1 ;
	}*/
	char seljc[20];
	seljc[0]='\0';
	if (pw_checkbutton_status(w)) {
		pw_set_bgcolor(w, "blue");
		strcpy(seljc,pw_get_text(w));
		strncpy(sel_jcchk,seljc,11);
		printf("sel_jcck is %s\n",sel_jcchk);
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
			printf("Unable to open db\n");
		sprintf(query,"select ifnull(max(fromdate),'0'),ifnull(max(todate),'0') from Demand_Group_details where Householdcode like '%%%s%%' and status in('N','Y') and group_id='%s'", sel_jcchk,demand_group_id);
		sqlite3_exec(dbhandle, query, get_grp_maxdates, NULL, NULL);
		printf("query is %s\n",query);
		sqlite3_close(dbhandle);
		char *demand_cnt_chk=NULL;
		sprintf(query,"select (case when '%s' <= '%s' then 'NOK' else 'OK' end)",dmnd_frmdt,max_todt);
		printf("query of case %s\n",query);
		demand_cnt_chk = exec_query_get_single_value(demand_cnt_chk, query, DB_PACK);
		if(strcmp(demand_cnt_chk,"NOK")==0)
		{
			show_message_frame("Already Demanded in \n Selected Period");
			pw_checkbutton_deselect(w);
			pw_set_bgcolor(w, "white");
			return 0;
		}
		char *demand_grp_chk=NULL;
		sprintf(query,"select ifnull(count(*),'0') from mcc_demand_group_details where (select ifnull(max(demand_todate),0) from mcc_demand_group_details where household_code||worker_code like '%%%s%%' and group_id='%s')>='%s' and household_code||worker_code like '%%%s%%' and group_id='%s'",sel_jcchk,demand_group_id,dmnd_frmdt,sel_jcchk,demand_group_id);
		demand_grp_chk = exec_query_get_single_value(demand_grp_chk, query, DB_PACK);
		if(atoi(demand_grp_chk)>=1)
		{
			show_message_frame("Already Demanded in MCC");
			pw_checkbutton_deselect(w);
			pw_set_bgcolor(w, "white");
			return 0;
		}
		char *alloc_days_check=NULL;
		sprintf(query,"select count(*) from mcc_demand_group_details where allocated_days  between 0 and 7 and household_code||worker_code like '%%%s%%' and group_id='%s'",sel_jcchk,demand_group_id);
		alloc_days_check = exec_query_get_single_value(alloc_days_check, query, DB_PACK);
		if(atoi(alloc_days_check)>=1)
		{
			show_message_frame("JOBCARD HAS UNMET\nDEMAND");
			pw_checkbutton_deselect(w);
			pw_set_bgcolor(w, "white");
			return 0;
		}
		free(alloc_days_check);
		free(demand_grp_chk);
		free(demand_cnt_chk);
		save_jc_sel();
	} else {
		seljc[0]='\0';
		pw_set_bgcolor(w, "white");
		strcpy(seljc,pw_get_text(w));
		strncpy(sel_jcchk,seljc,11);
		printf("sel_jcck del is %s\n",sel_jcchk);
		printf("demand_jc1 is %s\n",demand_jc);
		del_jc_sel();
	}

	return 0;
}
void allocate_check_list( int count)
{
	if (checkbox) {
		free(checkbox);
		checkbox = NULL;
	}
	checkbox = (pw_widget *)malloc (sizeof(pw_widget) * count);
	printf("count in ckeck box is %d\n",count);
}

int no_worker_found;
char jbno[80][80];
int get_details(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(jbno[no_worker_found], argv[0]);
	printf("jobcardno is %s\n",jbno[no_worker_found]);
	no_worker_found++;
	return 0;
}
char check_jc[100][100],seljcno[100][100];
void update_check_list( void )
{
	char query[1024];
	int i;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	no_worker_found=0;

	//sprintf(query, "select distinct substr(JobCard_id,15,4) ||worker_code ||'-'|| substr(name,1,19) from (select distinct substr(household_code,1,18) as Jobcard_id,worker_code as worker_code,name  from household_details  where is_active='Y' and group_id='%s') as a where jobcard_id not in(select substr(jobcard_no,1,18)from wage_info where no_days>=150)",demand_group_id);
	sprintf(query, "select distinct substr(JobCard_id,10,9) ||worker_code ||'-'|| substr(name,1,17) from (select distinct substr(household_code,1,18) as Jobcard_id,worker_code as worker_code,name  from household_details  where is_active='Y' and group_id='%s') as a where jobcard_id not in(select substr(jobcard_no,1,18)from wage_info where no_days>=150)",demand_group_id);
	sqlite3_exec(dbhandle, query, get_details, NULL, NULL);
	printf("query is %s",query);
	printf("no_worker_found %d\n",no_worker_found);

	for (i=0; i < no_worker_found; i++ ){
		sprintf(check_jc[i], "%s", jbno[i]);
		printf("i value is %d\n",i);
		printf("check_jc[i] is %s\n",check_jc[i]);
	}
	allocate_check_list(no_worker_found);
	for (i = 0 ; i < no_worker_found; i++ ) {
		checkbox[i] = pw_create_checkbutton(check_list, 0, 0, 240, 20,
				PW_BORDER, PW_RECTANGLE_BORDER,
				PW_TEXT, check_jc[i],
				PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, check_box_callback,
				PW_NA);
		pw_wlistbox_insertend(check_list, checkbox[i]);
	}
	sqlite3_close(dbhandle);
}
char tmp_deman_frdt[20],tmp_deman_todt[20],tmp_deman_count[10];
int get_demand_dates(void *parg, int argc, char **argv, char **colnames)
{
	tmp_deman_count[0]='\0';
	tmp_deman_todt[0]='\0';
	tmp_deman_frdt[0]='\0';
	strcpy(tmp_deman_count,argv[0]);
	strcpy(tmp_deman_frdt,argv[1]);
	strcpy(tmp_deman_todt,argv[2]);
	return 0;
}

/*int cb_from_date(pw_widget widget, pw_event e)
{
	char query_frmdt[1024];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query_frmdt,"select distinct count(*),ifnull(fromdate,0),ifnull(todate,0) from Demand_Group_details where substr(demanddate,1,10)=current_date and group_name='%s'",sel_demanded_grp);
	printf("query is %s",query_frmdt);
	sqlite3_exec(dbhandle, query_frmdt, get_demand_dates, NULL, NULL);
	if(atoi(tmp_deman_count)>0)
	{
		pw_set_text(lbl_frmdt,tmp_deman_frdt);
		pw_set_text(lbl_todt,tmp_deman_todt);
		strcpy(sel_frm_dt,tmp_deman_frdt);
		strcpy(sel_to_dt,tmp_deman_todt);
		return 0;
	}

	int day_of_week,year=0,month=0,day=0;
	char buf[15],buf1[15];
	buf[0]='\0';
	buf1[0]='\0';
#ifdef __arm__
	day_of_week = pw_create_calendar(demanded_grp_details_frame, 47, 77, &day, &month, &year, PW_NA);
	printf("inside from date\n");
	snprintf(buf, 32, "%d-%02d-%02d", year,month,day);
	snprintf(buf1, 32, "%02d-%02d-%d", day,month,year);
	printf("\n sel frm date is %s\n",buf1);
	char *db_date=NULL,*ck_frmdt=NULL,*chk_day=NULL;
	char weekday[3]="%w";	
	sprintf(query_frmdt,"select strftime('%s','%s')",weekday,buf);
	chk_day=exec_query_get_single_value(chk_day,query_frmdt,DB_PACK);
	printf("query is %s",query_frmdt);
	if(strcmp(chk_day,"0")==0)
	{
		show_message_frame("FROM DATE is SUNDAY");
		pw_set_text(lbl_frmdt, "");
		return 0;
	}
	if((strcmp(chk_day,"5")==0) || (strcmp(chk_day,"4")==0))
	{

		sprintf(query_frmdt,"select (case when '%s'<(select date('now')) then 'NOK' else 'OK' end)",buf);
		printf("query of case %s\n",query_frmdt);
		ck_frmdt = exec_query_get_single_value(ck_frmdt, query_frmdt, DB_PACK);

		if(strcmp(ck_frmdt,"NOK")==0)
		{
			show_message_frame("PAST DATE");
			pw_set_text(lbl_frmdt, "");
			return 0;
		}
	}
	printf("inside from date1\n");
	sprintf(query_frmdt,"select (case when '%s'<(select date('now','-1 day')) then 'NOK' else 'OK' end)",buf);
	printf("query of case %s\n",query_frmdt);
	ck_frmdt = exec_query_get_single_value(ck_frmdt, query_frmdt, DB_PACK);

	if(strcmp(ck_frmdt,"NOK")==0)
	{
		show_message_frame("PAST DATE");
		return 0;
	}
	sprintf(query_frmdt,"select (case when '%s'>(select date('now','6 day')) then 'NOK' else 'OK' end)",buf);
	printf("query of case %s\n",query_frmdt);
	ck_frmdt = exec_query_get_single_value(ck_frmdt, query_frmdt, DB_PACK);

	if(strcmp(ck_frmdt,"NOK")==0)
	{
		show_message_frame("FROMDATE > 7 Days");
		return 0;
	}

	pw_set_text(lbl_frmdt, buf1);
	sel_frm_dt[0]='\0';
	strcpy(sel_frm_dt,buf);

	sprintf(query_frmdt,"SELECT date( julianday(date('%s'))+13)",sel_frm_dt);
	db_date = exec_query_get_single_value(db_date, query_frmdt, DB_PACK);
	printf("demanded to date is %s\n",db_date);
	sel_to_dt[0]='\0';
	char *demand_todt=NULL;
	strcpy(sel_to_dt,db_date);
	sprintf(query_frmdt,"select strftime('%s','%s')",date_format,sel_to_dt);
	demand_todt = exec_query_get_single_value(demand_todt, query_frmdt, DB_PACK);

	printf("\n sel to date is %s\n",sel_to_dt);
	printf("\n sel buf date is %s\n",demand_todt);
	pw_set_text(lbl_todt,demand_todt);
	free(db_date);
	free(demand_todt);
	free(chk_day);
#endif
	return 0;
}*/
void get_demandgrps_detail_back()
{
	pw_wlistbox_clear(check_list);
	pw_tlistbox_remove_selection_all(lbox_demand_grps);
	hide_demanded_grp_details_frame();
	sel_frm_dt[0]='\0';
	sel_to_dt[0]='\0';
	sel_demanded_grp[0]='\0';
	pw_show(demand_grps_frame);
}
void create_demanded_grp_details_frame()
{
	demanded_grp_details_frame = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	lbl_demanded_grp_detail[0] = pw_create_label(demanded_grp_details_frame,00, 00, 239,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_demanded_grp_detail[2] = pw_create_label(demanded_grp_details_frame,10, 20,80,15,
			PW_TEXT, "From Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, BLUE,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_frmdt = pw_create_label(demanded_grp_details_frame, 10, 35, 85, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_from_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	lbl_demanded_grp_detail[2] = pw_create_label(demanded_grp_details_frame,130, 20,80,15,
			PW_TEXT, "To    Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, BLUE,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_todt = pw_create_label(demanded_grp_details_frame,130, 35,85,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	check_all = pw_create_checkbutton(demanded_grp_details_frame,03, 56, 40, 15,
			PW_BGCOLOR, BLUE ,
			PW_FGCOLOR, "white",
			PW_TEXT, "ALL",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_grp_all,
			PW_NA);
	check_list = pw_create_wlistbox(demanded_grp_details_frame, 0, 73, 240, 160, PW_NA);
	Butt_demand_grpsdetail[0] = pw_create_button(demanded_grp_details_frame, 10, 240, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demandgrps_detail_back,
			PW_CONFIG_END);
	Butt_demand_grpsdetail[2] = pw_create_button(demanded_grp_details_frame, 140, 240, 80, 29,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Demand View",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_demand_view,
			PW_CONFIG_END);
}
void show_demanded_grp_details()
{
	if(demanded_grp_details_frame==NULL)
		create_demanded_grp_details_frame();
	pw_wlistbox_clear(check_list);
	update_check_list();
	pw_show(demanded_grp_details_frame);
	printf("group_name is %s\n",sel_demanded_grp);
	pw_set_text(lbl_demanded_grp_detail[0], sel_demanded_grp);
	pw_checkbutton_deselect(check_all);
	
	sel_frm_dt[0]='\0';
	sel_to_dt[0]='\0';
}
void hide_demanded_grp_details_frame()
{
	pw_hide(demanded_grp_details_frame);
}
int date_tokenizer(char *sel_frmdt,char *sel_todt)
{
        char *frm_dt=NULL,*to_dt=NULL,query[256];
        sprintf(query,"select strftime('%s','%s')",date_format,sel_frmdt);
        frm_dt = exec_query_get_single_value(frm_dt, query, DB_PACK);
        sprintf(query,"select strftime('%s','%s')",date_format,sel_todt);
        to_dt = exec_query_get_single_value(to_dt, query, DB_PACK);
        pw_set_text(lbl_frmdt,frm_dt);
        pw_set_text(lbl_todt,to_dt);
        free(frm_dt);
        free(to_dt);
	return 0;
}

void get_select_grp()
{
	printf("demand_jc in demand capture callback is %s\n",demand_jc);
	if(strcmp(pw_get_text(combo_demand_hab)," ")==0)
	{
		show_message_frame("SELECT HABITATION");
		return ;
	}

	if(strcmp(pw_get_text(combo_demand_grptype)," ")==0)
	{
		show_message_frame("SELECT GROUPTYPE");
		return ;
	}
	int i=pw_tlistbox_get_selection(lbox_demand_grps);
	if(i==-1)
	{
		show_message_frame("SELECT GROUP NAME");
		return ;
	}
	char *fromdate=NULL, *todate=NULL,*hh_cnt=NULL,*partial_dem_check=NULL;
	char *dem_unmetcheck=NULL,*dem_check=NULL,query[1000];
	if(strcmp(batch_check1,"B1")==0)
	{
		sprintf(query,"select date('now','weekday 0','+1 day')");
		fromdate=exec_query_get_single_value(fromdate, query, DB_PACK);
		strcpy(dmnd_frmdt,fromdate);
		sprintf(query,"select date('%s','+13 days')",fromdate);
		todate=exec_query_get_single_value(todate, query, DB_PACK);
		strcpy(dmnd_todt,todate);

		sprintf(query,"select count(*) from household_details where group_id='%s' and is_active='Y'",demand_group_id);
		hh_cnt=exec_query_get_single_value(hh_cnt,query,DB_PACK);
		sprintf(query,"select count(*) from mcc_demand_group_details where group_id='%s' and allocated_days<=0",demand_group_id);
		dem_unmetcheck=exec_query_get_single_value(dem_unmetcheck,query,DB_PACK);
		if(atoi(dem_unmetcheck)>0)
		{
			if(atoi(dem_unmetcheck)==atoi(hh_cnt))
			{
				show_message_frame("Group has Unmet Demand");
				return  ;
			}
		}

		sprintf(query,"select count(*) from Demand_Group_details where group_name='%s' and  fromdate='%s' and todate='%s' and status not in('R','R1')",sel_demanded_grp,dmnd_frmdt,dmnd_todt);
		dem_check=exec_query_get_single_value(dem_check,query,DB_PACK);
		if(atoi(dem_check)>0)
		{
			show_message_frame("Group Already Demanded \nfor the Period");
			return  ;
		}

		sprintf(query,"select count(*) from mcc_demand_group_details where group_id='%s' and allocated_days between 1 and 7",demand_group_id);
		partial_dem_check =exec_query_get_single_value(partial_dem_check,query,DB_PACK);
		if(atoi(partial_dem_check)>0)
		{		
				show_message_frame("Partial Demand Exist \n for the Group");
				return  ;
	
		}
	}
	else
	{
		sprintf(query,"select date('now','weekday 4')");
		fromdate=exec_query_get_single_value(fromdate, query, DB_PACK);
		strcpy(dmnd_frmdt,fromdate);
		sprintf(query,"select date('%s','+13 days')",fromdate);
		todate=exec_query_get_single_value(todate, query, DB_PACK);
		strcpy(dmnd_todt,todate);

		sprintf(query,"select count(*) from household_details where group_id='%s' and is_active='Y'",demand_group_id);
		hh_cnt=exec_query_get_single_value(hh_cnt,query,DB_PACK);

		sprintf(query,"select count(*) from mcc_demand_group_details where group_id='%s' and allocated_days<=0",demand_group_id);
		dem_unmetcheck=exec_query_get_single_value(dem_unmetcheck,query,DB_PACK);
		if(atoi(dem_unmetcheck)>0)
		{
			if(atoi(dem_unmetcheck)==atoi(hh_cnt))
			{

				show_message_frame("Group has Unmet Demand");
				return  ;
			}
		}

		sprintf(query,"select count(*) from Demand_Group_details where group_name='%s' and  fromdate='%s' and todate='%s' and status not in('R','R1')",sel_demanded_grp,dmnd_frmdt,dmnd_todt);
		dem_check=exec_query_get_single_value(dem_check,query,DB_PACK);

		if(atoi(dem_check)>0 )
		{
			show_message_frame("Group Already Demanded \nfor the Period");
			return  ;
		}

		sprintf(query,"select count(*) from mcc_demand_group_details where group_id='%s' and allocated_days between 1 and 7",demand_group_id);
		partial_dem_check =exec_query_get_single_value(partial_dem_check,query,DB_PACK);
		if(atoi(partial_dem_check)>0)
		{
		//	if(atoi(partial_dem_check)!=atoi(hh_cnt))
		//	{
				show_message_frame("Partial Demand Exist \n for the Group");
				return  ;

		//	}
		}
	}
	del_temp_table();
	hide_demand_grps_frame();
	show_demanded_grp_details();
	date_tokenizer(dmnd_frmdt,dmnd_todt);
	free(fromdate);free(todate);free(dem_check);
	free(dem_unmetcheck);free(hh_cnt);free(partial_dem_check);
}
void lbox_demand_grp_sel(pw_widget wdgt, int idx)
{
	char *demanded_grpname=NULL;
	char query[1024];
	pw_tlistbox_get_element(lbox_demand_grps,&demanded_grpname,idx,0);
	txtlb_get_element(lbox_demand_grps,&demanded_grpname,idx,0);

	if(idx==-1)
	{
		return ;
	}
	sel_demanded_grp[0]='\0';
	strcpy(sel_demanded_grp,demanded_grpname);
	printf("to tag grp name is %s\n",sel_demanded_grp);
	demand_group_id[0]='\0';
	memset(demand_group_id, 0, 1);
	sprintf(query,"select distinct group_id from household_details where group_name='%s'",sel_demanded_grp);
	demanded_groupid=exec_query_get_single_value(demanded_groupid, query, DB_PACK);
	sprintf(demand_group_id,"%s",demanded_groupid);
	printf("groupid for taggrp is %s\n",demand_group_id);
	
	free(demanded_groupid);
	free(demanded_grpname);
	return ;
}

char sel_demandgrpnam[200];
int get_search_grpnam()
{
	char query[1024];
	if(strcmp(pw_get_text(combo_demand_hab)," ")==0)
	{
		show_message_frame("SELECT HABITATION");
		return -1;
	}

	if(strcmp(pw_get_text(combo_demand_grptype)," ")==0)
	{
		show_message_frame("SELECT GROUPTYPE");
		return -1;
	}

	char *srch_chk_qty=NULL,*search_cnt=NULL;

	srch_chk_qty=strpbrk(pw_get_text(txt_entry_grp_nam),"-*+.");
	printf("srch_chk_qty is %s\n",srch_chk_qty);
	while(srch_chk_qty != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(txt_entry_grp_nam);
		return 0;
	}

	free(srch_chk_qty);

	strcpy(sel_demandgrpnam,pw_get_text(txt_entry_grp_nam));
	sprintf(query,"select  count(*) from household_details where group_name like '%%%s%%' and  group_type='%s' order by group_name",sel_demandgrpnam,sel_grp_type);
	search_cnt = exec_query_get_single_value(search_cnt, query, DB_PACK);
	if(atoi(search_cnt)==0)
	{
		show_message_frame("NO DATA");
		pw_entry_clear(txt_entry_grp_nam);
		return 0;
	}
	free(search_cnt);
	pw_tlistbox_clear(lbox_demand_grps);
	sprintf(query,"select  distinct group_name from household_details where group_name like '%%%s%%' and  group_type='%s' order by group_name",sel_demandgrpnam,sel_grp_type);
	exec_query_load_into_lbox_single(lbox_demand_grps, query, DB_PACK);
	pw_update(lbox_demand_grps, 1);

	return 0;
}
void groups_to_demand()
{
        char query[1024];
        pw_tlistbox_clear(lbox_demand_grps);
if(strcmp(batch_check1,"B1")==0)
        sprintf(query, "select distinct group_name from household_details where group_type='%s' and group_name NOT IN(select distinct group_name from Demand_Group_details where application_date=(select date('now','weekday 0','-3 days')) and status!='R') group by group_name",sel_grp_type);
        else
        sprintf(query, "select distinct group_name from household_details where group_type='%s' and group_name NOT IN(select distinct group_name from Demand_Group_details where application_date=(select date('now','weekday 3','-4 days')) and status!='R') group by group_name",sel_grp_type);
        exec_query_load_into_lbox_single(lbox_demand_grps, query, DB_PACK);
        pw_update(lbox_demand_grps, 1);
}

void combo_demand_grptypechange(pw_widget widget, int index)
{
	char query[500];
        char *grp_type=NULL,*batch_chk=NULL;
        unsigned idx;
        pw_combobox_get_selection(combo_demand_grptype,&grp_type,&idx);
        strcpy(sel_grp_type,grp_type);
        free(grp_type);
        pw_tlistbox_clear(lbox_demand_grps);
        batch_check1[0]='\0';
        sprintf(query,"select distinct Batch_type from Batch_info");
        batch_chk=exec_query_get_single_value(batch_chk, query, DB_PACK);
        strcpy(batch_check1,batch_chk);
        free(batch_chk);
        groups_to_demand();

}
void combo_demand_habchange(pw_widget widget, int index)
{
	char query[1024];
	char *demandhabitname=NULL;
	unsigned idx;
	pw_combobox_get_selection(combo_demand_hab,&demandhabitname,&idx);
	strcpy(sel_demand_hab,demandhabitname);
//	strncpy(demandhabit_code,sel_demand_hab,5);
//	printf("habit code is %s\n",demandhabit_code);
	sprintf(demand_jc,"%s",dt_mand_pan_id);
	//sprintf(demand_jc,"%s%s",dt_mand_pan_id,demandhabit_code);
	printf("demand_jc values is %s\n",demand_jc);
	free(demandhabitname);
	pw_combobox_clear(combo_demand_grptype);
	sprintf(query,"select distinct group_type from household_details where group_type in ('GENERAL','SSS-T','DISABLED')");
	exec_query_load_into_combo(combo_demand_grptype, query, DB_PACK);
}
int demand_hab()
{
	char query[1024];
	char *dt_mand_pan_code=NULL;
	sprintf(query,"select substr(jc_code,1,9) from jc_info");
	dt_mand_pan_code = exec_query_get_single_value(dt_mand_pan_code, query, DB_PACK);
	sprintf(dt_mand_pan_id,"%s",dt_mand_pan_code);
	pw_combobox_clear(combo_demand_hab);
	pw_set_text(combo_demand_hab,"--Select--");
	sprintf(query,"select distinct Habitation_Code||'-'||habitation_name from tmp_Habitation_Master ");
	exec_query_load_into_combo(combo_demand_hab, query, DB_PACK);
	free(dt_mand_pan_code);
	return 0;
}
void get_demandgrps_back()
{
	hide_demand_grps_frame();
	show_demand_options();
}
pw_widget txt_grpnam_srch;
void create_demand_grps_frame()
{
	demand_grps_frame = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	pw_hide(demand_grps_frame);

	lbl_demand_grps[0]= pw_create_label(demand_grps_frame,00, 00, 85,20,
			PW_TEXT, "Habitation:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_demand_hab = pw_create_combobox(demand_grps_frame, 85, 00, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_demand_hab,combo_demand_habchange);

	lbl_demand_grps[1]= pw_create_label(demand_grps_frame,00, 22, 85,20,
			PW_TEXT, "SSS-Type",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_demand_grptype = pw_create_combobox(demand_grps_frame, 85, 22, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_demand_grptype,combo_demand_grptypechange);

	lbl_demand_grps[2]= pw_create_label(demand_grps_frame,00, 44, 60,25,
			PW_TEXT, "Group Name",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	txt_entry_grp_nam = pw_create_entry(demand_grps_frame, 60, 44, 90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	pw_focus(txt_entry_grp_nam);


	txt_entry_grp_nam = pw_create_entry(demand_grps_frame, 60, 44, 100, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_alpha,
			PW_CONFIG_END);
	pw_focus(txt_entry_grp_nam);

	Butt_grpnam_search = pw_create_button(demand_grps_frame, 160, 43,76, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SEARCH,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_search_grpnam,
			PW_CONFIG_END);

	lbl_demand_grps[3]= pw_create_label(demand_grps_frame,00, 69, 240,15,
			PW_TEXT, "Group Names",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);


	lbox_demand_grps = pw_create_tlistbox(demand_grps_frame, 0, 85, 239, 138,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_selection_intimate(lbox_demand_grps,lbox_demand_grp_sel);
	pw_tlistbox_set_selection_type(demand_grps_frame,PW_TLISTBOX_SINGLE_SELECTION);
	Butt_demand_grps[0] = pw_create_button(demand_grps_frame, 10, 235, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_demandgrps_back,
			PW_CONFIG_END);
	Butt_demand_grps[2]= pw_create_button(demand_grps_frame, 150, 235, 70, 30,
			PW_TEXT, "Demand Capture",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_select_grp,
			PW_CONFIG_END);
}
void show_demand_grps_frame()
{
	if(!demand_grps_frame)
	{
		create_demand_grps_frame();
		pw_show(demand_grps_frame);
	}
	else
		pw_show(demand_grps_frame);
	pw_set_text(combo_demand_hab,"--Select--");
	demand_hab();
	pw_combobox_clear(combo_demand_grptype);
	pw_tlistbox_clear(lbox_demand_grps);

}
void hide_demand_grps_frame()
{
	pw_hide(demand_grps_frame);
}
