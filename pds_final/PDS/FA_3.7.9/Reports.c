#include "types.h"
#include "string_list.h"
pw_widget label_report[5],edit_muster_id,txt_entry_jcno,Butt_jcno_search,lab_issue,combo_issue_type;

pw_widget frame_wage_payorder_report,combo_wage_payorder_hab,combo_wg_pay,label_get_payorder_detail[1],Butt_wg_payorder_back,Btn_wrksr_view,lab_wc,frame_fp_info,frame_ntfp_info,frame_grp_info;
char comp_value[20],buf_dt[15];

pw_widget Butt_date_report,lbox_mcc_alloca_details,combo_mcc_date_report,frame_mccwrk_option_report,frame_mccwrk_date_report,lbl_option_report[10],frame_master_tables_count,label_master_cnt[4],label_get_master_cnt[4],Butt_option_report[5],Btn_Fpcnt_view,Butt_master_cnt_back,Butt_fp_all_del,frame_view_muster_option,Butt_view_emusters,Butt_view_muster_back,lbox_gpa_report,Butt_gpa_reportback,Btn_wrks_id_submit,lbl_day_wise,dt_entry;
int insert_template_to_sensor();
void close_sqlite(sqlite3 *dbhandle);
int Load_fp_group();
int mem_info_upload();
int au_get_device_id();
void show_progress_frame();
void hide_frame_mccwrk_option_report();
void show_frame_mccwrk_date_report();
void show_frame_mccwrk_option_report();
void hide_frame_mccwrk_date_report();
void hide_frame_bemms_options();
void show_frame_mis_report();
void hide_frame_mis_report();
void hide_progress_frame();
void hide_frame_fp_info();
void show_frame_ntfp_info();
void hide_frame_ntfp_info();
void show_frame_fp_info();
void show_frame_grp_info();
void hide_frame_grp_info();
char *exec_query_get_single_value(char *data, char *query, char *database);
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
void hide_frame_wage_payorder_report();
void hide_frame_master_tables_count();
void show_mccwrk_aaloc_report();
int exec_query_load_into_lbox_double();
int txtlb_get_element();
int au_get_storage_info();
int exec_query_load_into_lbox_single();
void show_text_frame(char *msg);
void hide_text_frame();
void hide_frame_view_muster_option();
void show_frame_view_muster_option();
void show_frame_bemms_options();
void show_frame_pos_info();
void hide_frame_pos_info();
void hide_frame_memory_info();
void show_frame_master_tables_count();
void hide_frame_works_information();
int get_muster_check_details();
int get_birs_del();
void hide_frame_pos_updates();
int gp_test();
void hide_frame_muster_work_report();
void hide_frame_muster_grp_report();
void show_frame_muster_work_report();
void hide_frame_muster_grp_mem_report();
void hide_frame_muster_det_report();

pw_widget frame_muster_check_report,combo_muster_chk,lbox_muster_chk,Butt_muster_chk[12],frame_muster_check_details,lbox_muster_details,frame_muster_group_details,lbox_muster_grpdet,lbl_muster_chk[13],Butt_view_wrk_det,frame_muster_duplicate_details,lbl_dup_chk[1],lbox_dup_chk,Butt_dup_back,lbox_works_chk,lab_work,combo_wrk_type,delete_fp;

char selected_elem[50];

void combo_wg_payorder_days(pw_widget widget, int index)
{
	char *jobno,*wg_wkd_days=NULL,wg_paywrked[10],*count_wg_payorder=NULL;
	unsigned idx;
	char query[512],sel_jobcardno[20],jc_pay_value[20];
	sel_jobcardno[0]='\0';
	jc_pay_value[0]='\0';
	pw_combobox_get_selection(combo_wg_pay, &jobno, &idx);
	strcpy(sel_jobcardno, jobno);
	free(jobno);
	sprintf(jc_pay_value,"%s%s",comp_value,sel_jobcardno);

	sprintf(query, "select ifnull(count(*),'0') from Wage_Info where substr(jobcard_no,15,4) like '%s' order by jobcard_no",sel_jobcardno);

	count_wg_payorder = exec_query_get_single_value(count_wg_payorder, query, DB_PACK);

	if(atoi(count_wg_payorder)==0)
	{
		pw_set_text(label_get_payorder_detail[1], "0");
	}
	if(atoi(count_wg_payorder)==1)
	{
		sprintf(query,"select ifnull(no_days,'0') from Wage_Info where substr(jobcard_no,15,4) like '%%%s%%'",sel_jobcardno);
		wg_wkd_days = exec_query_get_single_value(wg_wkd_days, query, DB_PACK);

		sprintf(wg_paywrked,"%s",wg_wkd_days);
		pw_set_text(label_get_payorder_detail[1], wg_paywrked);
		free(wg_wkd_days);

	}
	if(atoi(count_wg_payorder)>1)
	{
		show_message_frame("WRONG DATA");
		return ;
	}
	free(count_wg_payorder);
	return;
}
char dist_mand_pan_id[15],sel_habname[50];
void combobox_wg_pyoder(pw_widget widget, int index)
{
	char habit_code[10]="",query[1024];
	char *habitationname=NULL;
	unsigned idx;
	pw_combobox_get_selection(combo_wage_payorder_hab,&habitationname,&idx);
	strcpy(sel_habname,habitationname);
	free(habitationname);
	strncpy(habit_code,sel_habname,5);

	sprintf(comp_value,"%s%s",dist_mand_pan_id,habit_code);
	printf("comp_value is %s\n",comp_value);
	pw_combobox_clear(combo_wg_pay);
	sprintf(query,"select  distinct substr(Household_code,15,4) from household_details where substr(Household_code,1,14)='%s' and is_active='Y' order by household_code ",comp_value);
	exec_query_load_into_combo(combo_wg_pay, query, DB_PACK);
	return ;
}
char *grpcnt=NULL;
int master_tables_count()
{
	char tot_hh_cnt[30],tot_wrks_cnt[30],tot_sss_cnt[30],query[125];
	char  *hh_cnt=NULL,*wrks_cnt=NULL,*sss_cnt=NULL;
	sprintf(query, "select ifnull(count(*),0) from household_details where Is_Active='Y' ");

	hh_cnt = exec_query_get_single_value(hh_cnt, query, DB_PACK);
	sprintf(tot_hh_cnt,"%s",hh_cnt);
	pw_set_text(label_get_master_cnt[0], tot_hh_cnt);
	free(hh_cnt);

	sprintf(query, "select ifnull(count(*),0) from Work_Registration ");

	wrks_cnt = exec_query_get_single_value(wrks_cnt, query, DB_PACK);
	sprintf(tot_wrks_cnt,"%s",wrks_cnt);
	pw_set_text(label_get_master_cnt[1], tot_wrks_cnt);
	free(wrks_cnt);
	sprintf(query, "select ifnull(count(distinct group_id),'0') from groups_allocation ");
	sss_cnt = exec_query_get_single_value(sss_cnt, query, DB_PACK);
	sprintf(tot_sss_cnt,"%s",sss_cnt);
	pw_set_text(label_get_master_cnt[2], tot_sss_cnt);
	free(sss_cnt);
	sprintf(query,"select ifnull(count(distinct grname),'0') from wg_gpname_bio_info");
	grpcnt=exec_query_get_single_value(grpcnt, query, DB_PACK);
	pw_set_text(label_get_master_cnt[3],grpcnt);
	return 0;
}
pw_widget frame_works_information,lbl_wrks_info[4],lbl_fp_info[4],lbl_ntfp_info[4],viewgrpdt,deletefp,combo_wrk_type,loadbir,wrk_id_entry,lbl_grp_info[7],viewntenroll,Btn_wrkid_search,lbox_wrks_det,Btn_wrks_cnt_back,Btn_fp_cnt_back,Btn_ntfp_cnt_back,Btn_grp_cnt_back,lbox_fp_det,lbox_ntfp_det,lbox_grp_det;
char sel_wrktype_cnt[5];
void  combobox_wrktype_change(pw_widget widget, int index)
{
	char *wrktype=NULL,*wrktype_cnt=NULL,query[512];
	unsigned idx;
	pw_combobox_get_selection(combo_wrk_type,&wrktype,&idx);

	sprintf(query, "select ifnull(count(*),0) from Work_Registration where description='%s'",wrktype);

	wrktype_cnt = exec_query_get_single_value(wrktype_cnt, query, DB_PACK);
	sprintf(sel_wrktype_cnt,"%s",wrktype_cnt);
	pw_set_text(lbl_wrks_info[2], sel_wrktype_cnt);

	sprintf(query,"select distinct work_code,work_name from Work_Registration where description='%s'",wrktype);
	pw_tlistbox_clear(lbox_wrks_det);
	exec_query_load_into_lbox_double(lbox_wrks_det, query, DB_PACK);
	pw_update(lbox_wrks_det,1);
	free(wrktype);
	free(wrktype_cnt);
	return ;
}
int work_type_details()
{
	char query[512];
	sprintf(query,"select distinct description from Work_Registration");
	exec_query_load_into_combo(combo_wrk_type, query, DB_PACK);
	return 0;
}
void cb_wks_name_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
	return;
}
int cb_wks_name_press()
{
	char *wrkname;
	int idx = pw_tlistbox_get_selection(lbox_wrks_det);
	pw_tlistbox_get_element(lbox_wrks_det, &wrkname,idx, 1);

	txtlb_get_element(lbox_wrks_det, &wrkname, idx, 1);
	if(idx==-1)
	{
		return -1;
	}
	show_text_frame(wrkname);
	free(wrkname);
	return 0;
}
int get_srch_wrkid()
{
	char *srch_cnt=NULL;
	char sel_wrkid[18],query[512];
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	if(strcmp(pw_get_text(combo_wrk_type), "")==0)
	{
		show_message_frame("SELECT WORK TYPE");
		return -1;
	}
	char *srch_chk_wrkid=NULL;

	srch_chk_wrkid=strpbrk(pw_get_text(wrk_id_entry),"-*+.");
	printf("srch_chk_wrkid is %s\n",srch_chk_wrkid);
	while(srch_chk_wrkid != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(wrk_id_entry);
		return 0;
	}
	free(srch_chk_wrkid);
	strcpy(sel_wrkid,pw_get_text(wrk_id_entry));
	sprintf(query,"select count(*)  from Work_Registration where work_code like '%%%s%%'",sel_wrkid);

	srch_cnt = exec_query_get_single_value(srch_cnt, query, DB_PACK);
	if(atoi(srch_cnt)==0)
	{
		show_message_frame("NO DATA");
		pw_entry_clear(wrk_id_entry);
		return 0;
	}
	sprintf(query,"select distinct work_code,work_name from Work_Registration where work_code like '%%%s%%'",sel_wrkid);
	pw_tlistbox_clear(lbox_wrks_det);
	exec_query_load_into_lbox_double(lbox_wrks_det, query, DB_PACK);
	pw_update(lbox_wrks_det,1);
	free(srch_cnt);
	return 0;
}
int exec_query_load_fp_to_sensor(char *query, char *groupname)
{
	sqlite3 *dbhandle;

	int rval=0, nRows=0, nCols=0, i;

	char *szErrMsg=NULL;

	char **qResults=NULL;

	sqlite3_open(DB_PACK, &dbhandle);

	show_progress_frame("Processing ...");
	rval = sqlite3_get_table(dbhandle, query, &qResults, &nRows, &nCols, &szErrMsg);

	if (rval != SQLITE_OK) {

		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);

		sqlite3_free(szErrMsg);
		hide_progress_frame();
		return (-1);
	}

	printf("ctrl in  -- 2 \n");
	system("date;sync");

	printf("\n No. of rows/JC is %d\n", nRows);
	show_progress_frame("Loading  into sensor ...");

	for(i=0; i<nRows; i++)

	{
		insert_template_to_sensor(qResults[(i+(i+2))], qResults[(i+(i+3))], groupname, dbhandle, fps);
	}
	sqlite3_free_table(qResults);
	hide_progress_frame();
	printf("ctrl in  -- 3 \n");
	system("date;sync");
	close_sqlite(dbhandle);
	return 1;
}
int Load_fp_group()
{
	char *grpname,query[512];
	pw_tlistbox_get_element(lbox_ntfp_det , &grpname, pw_tlistbox_get_selection(lbox_ntfp_det), 0);
	printf("grpname in lbox delete is %s\n",grpname);
	sprintf(query, "select household_code||worker_code as Jobcard_id, Name from household_details  where is_active='Y'  and group_name='%s' ", grpname);

	printf("\n grpname is  %s\n", grpname);
	printf("\n guery in Load_fp_muster is  %s\n", query);
	exec_query_load_fp_to_sensor(query, grpname);
	printf("entering into load part");
	free(grpname);
	return 0;
}
void get_wrkscnt_back(void)
{
	hide_frame_works_information();
	show_frame_master_tables_count();
	return;
}
void get_fp_back()
{
	hide_frame_fp_info();
	show_frame_master_tables_count();
}
void viewnot()
{
	hide_frame_fp_info();
	show_frame_ntfp_info();
}
void get_ntfp_back()
{
	hide_frame_ntfp_info();
	show_frame_fp_info();
}
int not_enrolled_grp()
{
	char *notenrgrpcnt=NULL,query[512];
	sprintf(query,"select distinct group_name from household_details where group_name NOT IN(select distinct grname from wg_gpname_bio_info)");
	pw_tlistbox_clear(lbox_ntfp_det);
	exec_query_load_into_lbox_single(lbox_ntfp_det, query, DB_PACK);
	pw_update(lbox_ntfp_det,1);
	sprintf(query,"select count(distinct group_name) from household_details where group_name NOT IN(select distinct grname from wg_gpname_bio_info)");
	notenrgrpcnt=exec_query_get_single_value(notenrgrpcnt, query, DB_PACK);
	pw_set_text(lbl_ntfp_info[1],notenrgrpcnt);
	free(notenrgrpcnt);

	return 0;
}
int get_grpload_birs()
{
	fps = init_fps_in_iso();
	show_progress_frame("Loading to Sensor...");
	Load_fp_group();
	pw_flush_eventloop();
	hide_progress_frame();
	uninit_fps(fps);
	not_enrolled_grp();	
	return 0;
}
void create_frame_ntfp_info()
{
	frame_ntfp_info=pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	pw_hide(frame_ntfp_info);
	lbl_ntfp_info[0] =  pw_create_label(frame_ntfp_info,00, 05,220,20,
			PW_TEXT, "Total Groups NOT Enrolled :",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_ntfp_info[1] =  pw_create_label(frame_ntfp_info,200, 05,30,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbox_ntfp_det = pw_create_tlistbox(frame_ntfp_info, 0, 40, 240, 160,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	Btn_fp_cnt_back = pw_create_button(frame_ntfp_info, 30,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_ntfp_back,
			PW_CONFIG_END);

	loadbir=pw_create_button(frame_ntfp_info,130,230,65,35,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT,"LOAD BIRS",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_grpload_birs,
			PW_CONFIG_END);
}
void show_frame_ntfp_info(void)
{
	if(!frame_ntfp_info)
	{
		create_frame_ntfp_info();
		pw_show(frame_ntfp_info);
	}
	else
		pw_show(frame_ntfp_info);
	not_enrolled_grp();
	return;
}
void hide_frame_ntfp_info(void)
{
	pw_hide(frame_ntfp_info);
	return;
}

int viewgroupdetails()
{
	char *sss=NULL;
	int idx = pw_tlistbox_get_selection(lbox_fp_det);
	pw_tlistbox_get_element(lbox_fp_det, &sss,idx, 0);
	txtlb_get_element(lbox_fp_det, &sss, idx, 0);
	if(idx==-1)
	{
		show_message_frame("SELECT A GROUP");
		return 0;
	}
	hide_frame_fp_info();
	show_frame_grp_info();
	return 0;
}
void get_grp_back()
{
	hide_frame_grp_info();
	show_frame_fp_info();
}
void create_frame_grp_info()
{
	frame_grp_info=pw_create_frame(toplevel,0,0,240,270,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);

	lbl_grp_info[0] =  pw_create_label(frame_grp_info,00, 00,200,20,
			PW_TEXT, "WageSeekers Enrolled :",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_grp_info[1] =  pw_create_label(frame_grp_info,200, 00,30,20,
			PW_TEXT, " ",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_grp_info[2] =  pw_create_label(frame_grp_info,00, 20,220,20,
			PW_TEXT, "WageSeekers NotEnrolled:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_grp_info[3] =  pw_create_label(frame_grp_info,200, 20,30,20,
			PW_TEXT, " ",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_grp_info[4] =  pw_create_label(frame_grp_info,00, 55,110,20,
			PW_TEXT, "Job Card NO",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_grp_info[5] =  pw_create_label(frame_grp_info,170, 55,70,20,
			PW_TEXT, "FP Count",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbox_grp_det = pw_create_tlistbox(frame_grp_info, 0, 70, 240, 150,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_grp_det,30);
	pw_update(lbox_grp_det,1);
	pw_tlistbox_set_selection_type(lbox_grp_det,PW_TLISTBOX_SINGLE_SELECTION);
	Btn_grp_cnt_back = pw_create_button(frame_grp_info, 30,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_grp_back,
			PW_CONFIG_END);
}
char sel_fpgrpname[70];
void lbox_fp_det_sel()
{
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	char *str1=NULL;
	int idx = pw_tlistbox_get_selection(lbox_fp_det);
	pw_tlistbox_get_element(lbox_fp_det, &str1,idx, 0);
	txtlb_get_element(lbox_fp_det, &str1, idx, 0);
	if(idx==-1)
	{
		return  ;
	}
	sel_fpgrpname[0]='\0';
	strcpy(sel_fpgrpname,str1);
	free(str1);
	sqlite3_close(dbhandle);

	return  ;
}
void show_frame_grp_info(void)
{
	if(!frame_grp_info)
	{
		create_frame_grp_info();
		pw_show(frame_grp_info);
	}
	else
		pw_show(frame_grp_info);
	pw_tlistbox_clear(lbox_grp_det);
	char *enroll1=NULL,*enroll2=NULL,query[512];
	sprintf(query,"select distinct(nrega_id),count(nrega_id) from wg_gpname_bio_info where grname='%s' group by nrega_id",sel_fpgrpname);
	exec_query_load_into_lbox_double(lbox_grp_det, query, DB_PACK);
	sprintf(query,"select count(distinct(nrega_id)) from wg_gpname_bio_info");
	enroll1=exec_query_get_single_value(enroll1, query, DB_PACK);
	sprintf(query,"select distinct(count(household_code||worker_code)) from household_details where household_code||worker_code NOT IN( select distinct(nrega_id) from wg_gpname_bio_info)");
	enroll2=exec_query_get_single_value(enroll2, query, DB_PACK);
	pw_set_text(lbl_grp_info[1],enroll1);
	pw_set_text(lbl_grp_info[3],enroll2);
	free(enroll1);
	free(enroll2);
	return;

}
/*char min_fpid[20],max_fpid[20],fp_grp[150];
  int get_fpid(void *parg, int argc, char **argv, char **colnames)
  {
  strcpy(min_fpid, argv[0]);
  printf("\n min_fpid is %s\n", min_fpid);
  strcpy(max_fpid, argv[1]);
  printf("\n max_fpid is %d\n", atoi(max_fpid));
  return(0);
  }
  int deletefpgrp()
  {
  int idx = pw_tlistbox_get_selection(lbox_fp_det);
  if(idx==-1)
  {
  show_message_frame("Select Group Name");
  return 0;
  }
  show_progress_frame("Deleteing");
  char query[500];
  sqlite3 *dbhandle=NULL;
  sqlite3_open(DB_PACK, &dbhandle);
  if (dbhandle == NULL)
  printf("Unable to open db\n");
//unsigned long int strtoul (const char* str, char** endptr, int base)--conver char to unsigned long
unsigned long minfp=0,maxfp=0;
minfp = strtoul(min_fpid, NULL, 0);
maxfp = strtoul(max_fpid, NULL, 0);
printf("minfp is %lu\n",minfp);
int fps = init_fps_in_iso();

int retfpdel = bio_delete_multiple_templates(fps,(unsigned long *)minfp,(unsigned long *)maxfp);
printf("retfpdel is %d\n",retfpdel);
bio_uninit(fps);
printf("\n :%s:\n",fp_grp);
sprintf(query,"delete from wg_gpname_bio_info where grname='%s'",fp_grp);	
sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
printf("delete query is %s\n",query);
sprintf(query,"select distinct grname from wg_gpname_bio_info");
pw_tlistbox_clear(lbox_fp_det);
exec_query_load_into_lbox_single(lbox_fp_det, query, DB_PACK);
pw_update(lbox_fp_det,1);
sqlite3_close(dbhandle);
hide_progress_frame();
return 0;
}
void fp_cr_lbox_release(pw_widget wdgt, int idx)
{
hide_text_frame();
}

void fp_cr_lbox_press()
{
char *str=NULL;
int idx = pw_tlistbox_get_selection(lbox_fp_det);
pw_tlistbox_get_element(lbox_fp_det, &str,idx, 0);

txtlb_get_element(lbox_fp_det, &str, idx, 0);
if(idx==-1)
{
return ;
}
fp_grp[0]='\0';
strcpy(fp_grp,str);
char query[125];
sqlite3 *dbhandle=NULL;
sqlite3_open(DB_PACK, &dbhandle);
if (dbhandle == NULL)
printf("Unable to open db\n");
sprintf(query, "select min(sid),max(sid) from wg_gpname_bio_info where grname='%s'",str);
printf("query is %s\n", query);
sqlite3_exec(dbhandle, query, get_fpid, NULL, NULL);
sqlite3_close(dbhandle);

//show_text_frame(str);
free(str);
return ;
}
	*/
void hide_frame_grp_info(void)
{
	pw_hide(frame_grp_info);
	return;
}
void create_frame_fp_info()
{
	frame_fp_info=pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	pw_hide(frame_fp_info);
	lbl_fp_info[0] =  pw_create_label(frame_fp_info,00, 200,170,20,
			PW_TEXT, "Total Groups Enrolled :",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_fp_info[2] =  pw_create_label(frame_fp_info,170, 200,15,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbox_fp_det = pw_create_tlistbox(frame_fp_info, 0, 40, 240, 150,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	pw_tlistbox_selection_intimate(lbox_fp_det,lbox_fp_det_sel);
	pw_tlistbox_set_selection_type(frame_fp_info,PW_TLISTBOX_SINGLE_SELECTION);

	lbl_fp_info[1] =  pw_create_label(frame_fp_info,00,00, 150,20,
			PW_TEXT, "Groups Not Enrolled",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_fp_info[3] =  pw_create_label(frame_fp_info,150,00, 30,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	viewntenroll =  pw_create_button(frame_fp_info,180, 00, 55,25,
			PW_TEXT, "VIEW",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, viewnot,
			PW_CONFIG_END);

	Btn_fp_cnt_back = pw_create_button(frame_fp_info, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_fp_back,
			PW_CONFIG_END);
	viewgrpdt = pw_create_button(frame_fp_info, 170,230, 45, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT,"VIEW",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, viewgroupdetails,
			PW_CONFIG_END);
	/*	delete_fp = pw_create_button(frame_fp_info, 150,230, 64, 30,
		PW_TEXT, "Delete FP",
		PW_FONT, PW_BOLD_FONT,
		PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, deletefpgrp,
		PW_CONFIG_END);*/
}
void show_frame_fp_info(void)
{
	if(!frame_fp_info)
		create_frame_fp_info();
	pw_show(frame_fp_info);
	char query[512],*notenrgrpcnt=NULL;
	sprintf(query,"select distinct grname from wg_gpname_bio_info");
	pw_tlistbox_clear(lbox_fp_det);
	exec_query_load_into_lbox_single(lbox_fp_det, query, DB_PACK);
	pw_update(lbox_fp_det,1);
	pw_set_text(lbl_fp_info[2],grpcnt);
	sprintf(query,"select count(distinct group_name) from household_details where group_name NOT IN(select distinct grname from wg_gpname_bio_info)");
	notenrgrpcnt=exec_query_get_single_value(notenrgrpcnt, query, DB_PACK);
	pw_set_text(lbl_fp_info[3],notenrgrpcnt);
	free(notenrgrpcnt);

	return;
}
void hide_frame_fp_info(void)
{
	pw_hide(frame_fp_info);
	return;
}

void create_frame_works_information()
{
	frame_works_information = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);

	pw_hide(frame_works_information);

	lbl_wrks_info[0] =  pw_create_label(frame_works_information,00, 00, 85,20,
			PW_TEXT, "Work Type",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_wrk_type = pw_create_combobox(frame_works_information, 85, 00, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_wrk_type,combobox_wrktype_change);
	lbl_wrks_info[1] =  pw_create_label(frame_works_information,00, 23, 155,18,
			PW_TEXT, "Works Count",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_wrks_info[2] =  pw_create_label(frame_works_information,155, 23, 85,18,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);


	lbl_wrks_info[3] =  pw_create_label(frame_works_information, 0, 45, 75, 20,
			PW_TEXT, "WORK ID",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	wrk_id_entry =  pw_create_entry(frame_works_information, 85, 45, 60, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	pw_focus(wrk_id_entry);

	Btn_wrkid_search = pw_create_button(frame_works_information, 160, 45,76, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SEARCH,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_srch_wrkid,
			PW_CONFIG_END);

	lbox_wrks_det = pw_create_tlistbox(frame_works_information, 0, 73, 240, 150,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wks_name_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT, cb_wks_name_press,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_wrks_det, 55);

	Btn_wrks_cnt_back = pw_create_button(frame_works_information, 30,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_wrkscnt_back,
			PW_CONFIG_END);

	Btn_wrks_id_submit = pw_create_button(frame_works_information, 150, 230, 72, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SUBMIT,
			//                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wrkid_sbmt,
			PW_CONFIG_END);


	return;
}
void show_frame_works_information(void)
{
	if(!frame_works_information)
	{
		create_frame_works_information();
		pw_show(frame_works_information);
	}
	else
		pw_show(frame_works_information);
	pw_hide(Btn_wrks_id_submit);
	pw_combobox_clear(combo_wrk_type);
	pw_tlistbox_clear(lbox_wrks_det);

	pw_set_text(combo_wrk_type,"--Select--");
	work_type_details();
	return;
}
void hide_frame_works_information(void)
{
	pw_hide(frame_works_information);
	return;
}
void get_tot_fp_details()
{
	hide_frame_master_tables_count();
	show_frame_fp_info();
}
void get_tot_wrks_details()
{
	hide_frame_master_tables_count();
	show_frame_works_information();
	return;
}
void get_master_back()
{
	hide_frame_master_tables_count();
	show_frame_pos_info();
}
void create_frame_master_tables_count(void)
{
	frame_master_tables_count =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_master_tables_count);
	label_master_cnt[0] =  pw_create_label(frame_master_tables_count,00, 30, 140,20,
			PW_TEXT, "House Hold Count:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	label_get_master_cnt[0] =  pw_create_label(frame_master_tables_count, 140, 30, 50, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_master_cnt[1] =  pw_create_label(frame_master_tables_count,00, 70, 140,25,
			PW_TEXT, "Works Count:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	label_get_master_cnt[1] =  pw_create_label(frame_master_tables_count, 140, 70, 50, 25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	Btn_wrksr_view= pw_create_button(frame_master_tables_count, 185,70,50, 30,
			PW_TEXT, "View",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_tot_wrks_details,
			PW_CONFIG_END);

	label_master_cnt[2] =  pw_create_label(frame_master_tables_count,00, 120, 140,20,
			PW_TEXT, "SSS Groups Count:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	label_get_master_cnt[2] =  pw_create_label(frame_master_tables_count, 140, 120, 50, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_master_cnt[3] =  pw_create_label(frame_master_tables_count,00, 160, 140,25,
			PW_TEXT, "HH FP Sensor count:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	label_get_master_cnt[3] =  pw_create_label(frame_master_tables_count, 140, 160, 50, 25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	Btn_Fpcnt_view = pw_create_button(frame_master_tables_count, 185,160,50, 30,
			PW_TEXT, "View",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_tot_fp_details,
			PW_CONFIG_END);

	Butt_master_cnt_back = pw_create_button(frame_master_tables_count, 20,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_master_back,
			PW_CONFIG_END);
	Butt_fp_all_del = pw_create_button(frame_master_tables_count, 124, 225, 76, 35,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "DELETE ALL FP",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_birs_del,
			PW_CONFIG_END);

	return;
}
int get_jcno(pw_widget widget, int index)
{
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	char *srch_jcno=NULL;
	char sel_jcno[20],query[512];
	srch_jcno=strpbrk(pw_get_text(txt_entry_jcno),"-*+.");
	printf("srch_jcno is %s\n",srch_jcno);
	while(srch_jcno != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(txt_entry_jcno);
		return 0;
	}
	free(srch_jcno);
	strcpy(sel_jcno,pw_get_text(txt_entry_jcno));
	if(strcmp(sel_habname,"")==0)

	{
		show_message_frame("SELECT HABITATION");
		return 0;
	}

	pw_combobox_clear(combo_wg_pay);
	char *hh_count=NULL;
	sprintf(query,"select count(*) from household_details where is_active='Y' and substr(Household_code,15,4) like '%%%s%%'",sel_jcno);

	hh_count = exec_query_get_single_value(hh_count, query, DB_PACK);

	if(atoi(hh_count)>0)
	{	
		sprintf(query,"select distinct substr(Household_code,15,4) from household_details where is_active='Y' and  substr(Household_code,15,4) like '%%%s%%' order by substr(Household_code,15,4)",sel_jcno);

		pw_combobox_clear(combo_wg_pay);
		exec_query_load_into_combo(combo_wg_pay, query, DB_PACK);
	}
	else
	{
		show_message_frame("NO DATA");
		return 0;
	}
	free(hh_count);

	return 0;
}
void get_wgreport_back()
{
	hide_frame_wage_payorder_report();
	show_frame_bemms_options();
	return;
}
void create_frame_wage_payorder_report(void)
{
	frame_wage_payorder_report =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_wage_payorder_report);
	label_report[0] =  pw_create_label(frame_wage_payorder_report,00, 15, 85,21,
			PW_TEXT, "Habitation:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	combo_wage_payorder_hab = pw_create_combobox(frame_wage_payorder_report, 85, 15, 150,21,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_wage_payorder_hab, (void *)combobox_wg_pyoder);

	label_report[1] =  pw_create_label(frame_wage_payorder_report, 00, 51, 110, 20,
			PW_TEXT, "HouseHold No",
			PW_BGCOLOR, VIOLET,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	txt_entry_jcno =  pw_create_entry(frame_wage_payorder_report,110, 51, 53,20 ,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_num,
			PW_CONFIG_END);
	pw_focus(txt_entry_jcno);
	/* Adding jobcard search to find workeddays for a jobcard
	   modified by 508316*/
	Butt_jcno_search = pw_create_button(frame_wage_payorder_report, 166, 56, 76, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SEARCH,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_jcno,
			PW_CONFIG_END);
	label_report[2] =  pw_create_label(frame_wage_payorder_report, 0, 95, 110, 20,
			PW_TEXT, "HouseHold No",
			PW_BGCOLOR, VIOLET,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	combo_wg_pay = pw_create_combobox(frame_wage_payorder_report, 110, 95, 85, 20,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_NA);
	pw_combobox_selection_intimate(combo_wg_pay, combo_wg_payorder_days);
	label_report[4] =  pw_create_label(frame_wage_payorder_report,00, 130, 125,25,
			PW_TEXT, "Worked Days:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	label_get_payorder_detail[1] =  pw_create_label(frame_wage_payorder_report, 125, 130, 100, 25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	Butt_wg_payorder_back = pw_create_button(frame_wage_payorder_report, 30,210, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_wgreport_back,
			PW_CONFIG_END);
	return;
}
int wage_hab_details()
{
	char total_value[20],query[125];
	char *dist_mand_pan_code=NULL,*hab_name=NULL ;
	sprintf(query,"select substr(jc_code,1,9) from jc_info");
	dist_mand_pan_code = exec_query_get_single_value(dist_mand_pan_code, query, DB_PACK);
	sprintf(dist_mand_pan_id,"%s",dist_mand_pan_code);
	pw_combobox_clear(combo_wage_payorder_hab);
	pw_set_text(combo_wage_payorder_hab,"--Select--");
	sprintf(query,"select Habitation_Code||'-'||habitation_name from tmp_Habitation_Master ");
	sprintf(total_value,"%s",dist_mand_pan_code);
	hab_name = exec_query_get_single_value(hab_name, query, DB_PACK);
	exec_query_load_into_combo(combo_wage_payorder_hab, query, DB_PACK);
	free(dist_mand_pan_code);
	free(hab_name);
	return 0;
}
void show_frame_master_tables_count()
{
	if(!frame_master_tables_count)
	{
		create_frame_master_tables_count();
		pw_show(frame_master_tables_count);
	}
	else
		pw_show(frame_master_tables_count);
	master_tables_count();
	return;
}
void hide_frame_master_tables_count(void)
{
	pw_hide(frame_master_tables_count);
}
void show_frame_wage_payorder_report(void)
{
	if(!frame_wage_payorder_report)
	{
		create_frame_wage_payorder_report();
		pw_show(frame_wage_payorder_report);
	}
	else
		pw_show(frame_wage_payorder_report);
	wage_hab_details();
	pw_set_text(label_get_payorder_detail[1], " ");
	return;
}
void hide_frame_wage_payorder_report(void)
{
	pw_hide(frame_wage_payorder_report);
	return;
}
char mandal_name[50],panchayat_name[50];
int get_gpinfo(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(mandal_name, argv[0]);
	printf("mandalname is %s\n",mandal_name);
	strcpy(panchayat_name, argv[1]);
	return(0);
}
pw_widget frame_memory_info,lbl_mem_info[5],lbl_get_mem_info[5],btn_mem_info_back,btn_mem_info_upload;
int total,bal_mem;
char free_mem[4],devid[15],fa_no[2];
int get_mem_info()
{
	sqlite3 *dbhandle=NULL;
	char query[256],*pos_userid=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query,"select ifnull(mandal,0),ifnull(panchayat,0) from jc_info ");
	sqlite3_exec(dbhandle, query, get_gpinfo, NULL, NULL);

	sprintf(query,"select substr(jc_code,10,1) from jc_info");
	pos_userid = exec_query_get_single_value(pos_userid, query, DB_PACK);
	sprintf(fa_no,"%s",pos_userid);
	printf("fa no is %s",fa_no);
	free(pos_userid);
	au_get_storage_info (&total, &bal_mem);
	au_get_device_id(&devid);
	printf("device id is %s\n",devid);
	sprintf(free_mem,"%d",bal_mem);
	printf("free memory = %s \n ", free_mem);
	char *batch_type=NULL;
	sprintf(query,"select ifnull(Batch_type,'0') from Batch_info");
	batch_type = exec_query_get_single_value(batch_type, query, DB_PACK);

	if(strcmp(batch_type,"B1")==0)
	{
		pw_set_text(lbl_get_mem_info[4],"MONDAY");
	}
	else
	{
		pw_set_text(lbl_get_mem_info[4],"THURSDAY");
	}
	free(batch_type);
	sqlite3_close(dbhandle);
	return 0;
}

char vers_no[8];
int cb_meminfo_upload()
{
	ret= gp_test();
	if(ret<0)
	{
		show_message_frame("NOT CONNECTED TRY AGAIN");
	}
	else
	{
		mem_info_upload(fa_no,free_mem,vers_no,devid);
		return 0;
	}
	return 0;
}
void cb_mem_info_back(void)
{
	hide_frame_memory_info();
	show_frame_pos_info();
	return;
}
void create_frame_memory_info(void)
{
	frame_memory_info =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_memory_info);

	lbl_mem_info[0] = pw_create_label(frame_memory_info,01, 10, 80, 25,
			PW_TEXT, "Mandal",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_get_mem_info[0]= pw_create_label(frame_memory_info, 100, 10, 159, 25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "green",
			PW_CONFIG_END);
	lbl_mem_info[1] = pw_create_label(frame_memory_info,01, 55, 80, 25,
			PW_TEXT, "Panchayt",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_get_mem_info[1]= pw_create_label(frame_memory_info, 100, 55, 159, 25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "green",
			PW_CONFIG_END);

	lbl_mem_info[2] = pw_create_label(frame_memory_info,01, 95, 100, 20,
			PW_TEXT, "POS User",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_get_mem_info[2]= pw_create_label(frame_memory_info, 100, 100, 159, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "green",
			PW_CONFIG_END);
	lbl_mem_info[3] = pw_create_label(frame_memory_info,01, 140, 140, 25,
			PW_TEXT, "Free Memory(MB)",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_get_mem_info[3]= pw_create_label(frame_memory_info, 140, 140, 80, 25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "green",
			PW_CONFIG_END);

	lbl_mem_info[4] = pw_create_label(frame_memory_info,01, 185, 100, 25,
			PW_TEXT, "Batch Type",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_get_mem_info[4]= pw_create_label(frame_memory_info, 100, 185, 100, 25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "green",
			PW_CONFIG_END);

	btn_mem_info_back = pw_create_button(frame_memory_info, 40, 225,34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_mem_info_back ,
			PW_CONFIG_END);
	btn_mem_info_upload = pw_create_button(frame_memory_info, 140, 225,60, 27,
			PW_TEXT, "UPLOAD",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_meminfo_upload,
			PW_CONFIG_END);
	return;
}

void show_frame_memory_info(void)
{
	if(! frame_memory_info)
	{
		create_frame_memory_info();
		pw_show(frame_memory_info);
	}
	else
		pw_show(frame_memory_info);
	get_mem_info();
	pw_set_text(lbl_get_mem_info[0],mandal_name);
	pw_set_text(lbl_get_mem_info[1],panchayat_name);
	pw_set_text(lbl_get_mem_info[2],fa_no);
	pw_set_text(lbl_get_mem_info[3],free_mem);
	return;
}

void hide_frame_memory_info()
{
	pw_hide(frame_memory_info);
	return;
}

void cb_wc_report()
{
	hide_frame_pos_info();
	//	show_mccwrk_aaloc_report();
	show_frame_mccwrk_option_report();
}
void cb_mem_info()
{
	hide_frame_pos_info();
	show_frame_memory_info();
	return;
}
void get_tables_count()
{
	hide_frame_pos_info();
	show_frame_master_tables_count();	
	return;
}
void  get_wage_payorder()
{
	hide_frame_bemms_options();
	show_frame_wage_payorder_report();
	return;	
}
void get_pos_details_back()
{
	hide_frame_mis_report();
	pw_show(frame_optionspage);
	return;
}
void cb_grp_details_back()
{
	pw_hide(frame_muster_group_details);
	pw_tlistbox_remove_selection_all(lbox_muster_chk);
	pw_show(frame_muster_check_details);
	return;
}
void get_muster_finalcheck_back()
{
	pw_hide(frame_muster_check_details);
	selected_elem[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_muster_chk);
	pw_show(frame_muster_check_report);
	return;
}
void cb_muster_date_report()
{
	hide_frame_mccwrk_option_report();
	show_frame_mccwrk_date_report();
}
void cb_work_allc_report()
{
	hide_frame_mccwrk_option_report();
	show_mccwrk_aaloc_report();
}
void cb_option_back()
{
	hide_frame_mccwrk_option_report();
	show_frame_pos_info();
}

void create_frame_mccwrk_option_report()
{
	frame_mccwrk_option_report = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_mccwrk_option_report);
	Butt_option_report[1] = pw_create_button(frame_mccwrk_option_report, 50, 50,150, 40,
			PW_TEXT,"Date Wise Report",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_muster_date_report , 
			PW_CONFIG_END);
	Butt_option_report[2] = pw_create_button(frame_mccwrk_option_report, 50, 150,150, 40,
			PW_TEXT,"Work Wise Report",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_work_allc_report , 
			PW_CONFIG_END);
	Butt_option_report[3] = pw_create_button(frame_mccwrk_option_report, 40, 225,34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_option_back , 
			PW_CONFIG_END);



}
void show_frame_mccwrk_option_report()
{
	if(!frame_mccwrk_option_report)
		create_frame_mccwrk_option_report();
	pw_show(frame_mccwrk_option_report);

}
void hide_frame_mccwrk_option_report()
{
	pw_hide(frame_mccwrk_option_report);
}
char from_dt[12];
int get_date_mcc(pw_widget widget, pw_event e)
{
	char  from1_dt[12],query[512],*batch_check=NULL,*string_var="%w",*to_dt=NULL,*check_wc_count=NULL;
	int day_of_week1,year1=0,month1=0,day1=0;
	from_dt[0]='\0';
	from1_dt[0]='\0';
	day_of_week1 = pw_create_calendar(frame_mccwrk_date_report, 17, 17, &day1, &month1, &year1, PW_NA);
	snprintf(from_dt, 32, "%d-%02d-%02d", year1,month1,day1);
	snprintf(from1_dt, 32, "%02d-%02d-%d", day1,month1,year1);
	printf("selected from date is %s\n",from_dt);

	sprintf(query,"select distinct Batch_type from Batch_info");
	batch_check=exec_query_get_single_value(batch_check,query,DB_PACK);
	if(strcmp(batch_check,"B1")==0)
	{
		char *ck_dt=NULL;
		sprintf(query," select strftime('%s','%s');",string_var,from_dt);
		printf("query of case %s\n",query);
		ck_dt = exec_query_get_single_value(ck_dt, query, DB_PACK);

		if(strcmp(ck_dt,"1")==0)
		{
			pw_set_text(lbl_option_report[2],from_dt);
			sprintf(query,"select date('%s','+6 days')",from_dt);
			to_dt=exec_query_get_single_value(to_dt,query,DB_PACK);
			pw_set_text(lbl_option_report[4],to_dt);
			sprintf(query,"select count(*) from mcc_work_allocation_details where substr(alloc_from_date,1,10)='%s'",from_dt);
			check_wc_count=exec_query_get_single_value(check_wc_count,query,DB_PACK);
			if(strcmp(check_wc_count,"0")==0)
			{
				show_message_frame("No WorkCodes");
			}
			else
			{
				sprintf(query,"select distinct work_code from mcc_work_allocation_details where substr(alloc_from_date,1,10)='%s'",from_dt);
				exec_query_load_into_lbox_single(lbox_mcc_alloca_details, query, DB_PACK);
				pw_update(lbox_mcc_alloca_details, 1);	
			}
		}
		else
		{
			show_message_frame("Select Monday Only");
			pw_tlistbox_remove_selection_all(lbox_mcc_alloca_details);
			pw_set_text(lbl_option_report[2],"");
			pw_set_text(lbl_option_report[4],"");
			pw_tlistbox_clear(lbox_mcc_alloca_details);
			return 0;
		}		
		free(ck_dt);


	}
	else
	{
		char *ck_dt=NULL;
		sprintf(query," select strftime('%s','%s');",string_var,from_dt);
		printf("query of case %s\n",query);
		ck_dt = exec_query_get_single_value(ck_dt, query, DB_PACK);

		if(strcmp(ck_dt,"4")==0)
		{
			pw_set_text(lbl_option_report[2],from_dt);
			sprintf(query,"select date('%s','+6 days')",from_dt);
			to_dt=exec_query_get_single_value(to_dt,query,DB_PACK);
			pw_set_text(lbl_option_report[4],to_dt);
			sprintf(query,"select count(*) from mcc_work_allocation_details where substr(alloc_from_date,1,10)='%s'",from_dt);
			check_wc_count=exec_query_get_single_value(check_wc_count,query,DB_PACK);
			if(strcmp(check_wc_count,"0")==0)
			{
				show_message_frame("No WorkCodes");
			}
			else
			{
				sprintf(query,"select distinct work_code from mcc_work_allocation_details where substr(alloc_from_date,1,10)='%s'",from_dt);
				exec_query_load_into_lbox_single(lbox_mcc_alloca_details, query, DB_PACK);
				pw_update(lbox_mcc_alloca_details, 1);	
			}

		}
		else
		{
			show_message_frame("Select Thursday Only");
			pw_tlistbox_remove_selection_all(lbox_mcc_alloca_details);
			pw_set_text(lbl_option_report[2],"");
			pw_set_text(lbl_option_report[4],"");
			pw_tlistbox_clear(lbox_mcc_alloca_details);
			return 0;
		}
		free(ck_dt);
	}
	return 0;
}
void get_date_back()
{
	hide_frame_mccwrk_date_report();
	show_frame_mccwrk_option_report();
}
int cb_mcc_date_report()
{
	pw_combobox_clear(combo_mcc_date_report);
	char *str=NULL,sel_work_code[20],query[512];
	int idx = pw_tlistbox_get_selection(lbox_mcc_alloca_details);
	pw_tlistbox_get_element(lbox_mcc_alloca_details, &str,idx, 0);
	txtlb_get_element(lbox_mcc_alloca_details, &str, idx, 0);
	if(idx==-1)
	{
		return -1;
	}

	strcpy(sel_work_code,str);
	free(str);
	sprintf(query,"select distinct a.group_name from household_details a,mcc_work_allocation_details b where b.work_code='%s' and substr(alloc_from_date,1,10)='%s' and a.group_id=b.group_id",sel_work_code,from_dt);
	exec_query_load_into_combo(combo_mcc_date_report,query, DB_PACK);
	return 0;
}
void create_frame_mccwrk_date_report()
{
	frame_mccwrk_date_report = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_mccwrk_date_report);
	lbl_option_report[1]=pw_create_label(frame_mccwrk_date_report,15, 05, 80, 15,
			PW_TEXT, "From Date", 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP, 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_option_report[2]=pw_create_label(frame_mccwrk_date_report,15, 25, 80, 20,
			PW_TEXT, " ", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_date_mcc, 
			PW_BGCOLOR, "white",
			PW_FGCOLOR, "black", 
			PW_CONFIG_END);
	lbl_option_report[3]=pw_create_label(frame_mccwrk_date_report,130, 05, 80, 15,
			PW_TEXT, "To Date", 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP, 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);
	lbl_option_report[4]=pw_create_label(frame_mccwrk_date_report,130, 25, 80, 20,
			PW_TEXT, " ", 
			PW_BGCOLOR, "white",
			PW_FGCOLOR, "black", 
			PW_CONFIG_END);
	lbox_mcc_alloca_details = pw_create_tlistbox(frame_mccwrk_date_report, 00, 50, 235, 130,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_mcc_date_report,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	lbl_option_report[5]= pw_create_label(frame_mccwrk_date_report,00, 190, 50,30,
			PW_TEXT, "Grp Name",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_mcc_date_report = pw_create_combobox(frame_mccwrk_date_report, 51, 190, 189,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	Butt_date_report= pw_create_button(frame_mccwrk_date_report, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_date_back,
			PW_CONFIG_END);
}
void show_frame_mccwrk_date_report()
{
	if(!frame_mccwrk_date_report)
		create_frame_mccwrk_date_report();
	pw_show(frame_mccwrk_date_report);
	pw_set_text(lbl_option_report[2],"");
	pw_set_text(lbl_option_report[4],"");
	pw_tlistbox_clear(lbox_mcc_alloca_details);
	pw_combobox_clear(combo_mcc_date_report);

}
void hide_frame_mccwrk_date_report()
{
	pw_hide(frame_mccwrk_date_report);
}
void create_frame_muster_group_details(void)
{
	frame_muster_group_details = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_muster_group_details);
	lbl_muster_chk[10] =  pw_create_label(frame_muster_group_details, 05, 05, 230, 35, 
			PW_TEXT, "", 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "#3b3b3b", 
			PW_CONFIG_END);

	lbox_muster_grpdet = pw_create_tlistbox(frame_muster_group_details, 0, 50, 240, 160,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_muster_grpdet, 160);

	Butt_muster_chk[3] = pw_create_button(frame_muster_group_details, 40, 225,34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_grp_details_back , 
			PW_CONFIG_END);
	return;
}
char mis_group_id[16];
int get_group_details()
{
	char query[512];
	pw_tlistbox_clear(lbox_muster_grpdet);
	sprintf(query,"select distinct substr(jobcardno,15,6),member_name from muster_attendance where  group_id ='%s' and  group_name='%s' and substr(created_date,1,10)='%s'",mis_group_id,selected_elem,buf_dt);
	exec_query_load_into_lbox_double(lbox_muster_grpdet, query, DB_PACK);
	return 0;
}
char query[1024];
int get_sbmt_musterid()
{
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");

	if(strcmp(pw_get_text(edit_muster_id),"")==0)
	{
		show_message_frame("ENTER MUSTER ID");
		return -1 ;
	}
	else if (strlen(pw_get_text(edit_muster_id))!=6)
	{
		show_message_frame("MUSTERID MUST BE 6 DIGIT");
		pw_entry_clear(edit_muster_id);
		return -1 ;
	}
	else if(strcmp(pw_get_text(edit_muster_id),"00000")==0)
	{
		show_message_frame("MUSTER ID NOT BE ZERO");
		pw_entry_clear(edit_muster_id);
		return -1 ;
	}
	char *chk_qty=NULL,*count_edit_mstrid=NULL;

	chk_qty=strpbrk(pw_get_text(edit_muster_id),"-*+.");
	printf("chk_qty is %s\n",chk_qty);
	while(chk_qty != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(edit_muster_id);
		return 0;
	}
	sprintf(query,"select count(*) from muster_id where muster_id='%s'",pw_get_text(edit_muster_id));

	count_edit_mstrid = exec_query_get_single_value(count_edit_mstrid, query, DB_PACK);

	if(atoi(count_edit_mstrid)!=0)
	{
		show_message_frame("MUSTER ID ALREADY EXIST");
		pw_entry_clear(edit_muster_id);
		printf("\n muster_id after MUSTER ID ALREADY EXIST is %s\n",pw_get_text(edit_muster_id));
		return -1 ;
	}
	free(count_edit_mstrid);

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	sprintf(query, "update muster_attendance set muster_id = '%s', status='%s' where group_id ='%s' and group_name='%s' and substr(created_date,1,10)= '%s' and status in ('M','N')",pw_get_text(edit_muster_id),"N",mis_group_id,selected_elem,buf_dt);

	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	printf("query1 is %s\n", query);
	sprintf(query, "update muster_id set muster_id = '%s' where  group_id='%s' and substr(created_date,1,10)= '%s'",pw_get_text(edit_muster_id),mis_group_id,buf_dt);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("query1 is %s\n", query);

	show_message_frame("MUSTER ID UPDATED");
	sqlite3_close(dbhandle);
	return 0;
}
char sel_muster_id[6];
int get_musterid_edit()
{
	pw_show(edit_muster_id);
	pw_focus(edit_muster_id);
	pw_show(Butt_muster_chk[3]);
	pw_set_text(edit_muster_id,sel_muster_id);
	return 0;
}
char crbar_cnt[10];
int get_crowbar_details()
{
	if(atoi(crbar_cnt)==0)
	{
		show_message_frame("NO DATA");
		return -1;
	}
	if (!frame_muster_group_details)
	{
		create_frame_muster_group_details();
		pw_show(frame_muster_group_details);
	}
	else
		pw_show(frame_muster_group_details);

	pw_set_text(lbl_muster_chk[10],selected_elem);
	pw_tlistbox_clear(lbox_muster_grpdet);

	sprintf(query,"select distinct substr(jobcardno,15,6),member_name from muster_attendance where group_id ='%s' and  group_name='%s' and substr(created_date,1,10)='%s' and crowbar='Y'",mis_group_id,selected_elem,buf_dt);
	exec_query_load_into_lbox_double(lbox_muster_grpdet, query, DB_PACK);
	return 0;
}
void get_muster_grp_details(void)
{
	if (!frame_muster_group_details)
	{
		create_frame_muster_group_details();
		pw_show(frame_muster_group_details);
	}
	else
		pw_show(frame_muster_group_details);
	pw_set_text(lbl_muster_chk[10],selected_elem);//bl_muster_chk[10]
	get_group_details();	
	return;
}
void get_tot_grp_details()
{
	if (!frame_muster_group_details)
	{
		create_frame_muster_group_details();
		pw_show(frame_muster_group_details);
	}
	else
		pw_show(frame_muster_group_details);

	pw_set_text(lbl_muster_chk[10],selected_elem);//bl_muster_chk[10]
	pw_tlistbox_clear(lbox_muster_grpdet);

	sprintf(query,"select distinct substr(household_code, 15, 4)||worker_code as Jobcard_id, Name from household_details  where is_active='Y'  and group_name='%s' union select substr(JobCardNo, 15, 6) as Jobcard_Id, Member_Name as Name from tmp_group_members where group_name='%s' and Is_Active='Y'",selected_elem,selected_elem);
	exec_query_load_into_lbox_double(lbox_muster_grpdet, query, DB_PACK);
	return ;
}
void create_frame_muster_check_details(void)
{
	frame_muster_check_details = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);	
	pw_hide(frame_muster_check_details);


	lbl_muster_chk[4] =  pw_create_label(frame_muster_check_details, 03,05, 235, 35,
			PW_TEXT, "",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FGCOLOR, "#3b3b3b",
			PW_CONFIG_END);

	lbl_muster_chk[5] =  pw_create_label(frame_muster_check_details ,0, 60, 115, 30,
			PW_TEXT, "Total Members:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_muster_chk[6] =  pw_create_label(frame_muster_check_details, 115,60, 60, 30,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END); 

	Butt_muster_chk[11]= pw_create_button(frame_muster_check_details, 180,60, 50, 30,
			PW_TEXT, "View",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_tot_grp_details,
			PW_CONFIG_END);

	lbl_muster_chk[7] =  pw_create_label(frame_muster_check_details ,0, 95, 115, 25,
			PW_TEXT, "Total Present:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_muster_chk[8]=  pw_create_label(frame_muster_check_details, 115, 95, 60, 25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FGCOLOR,"white",
			PW_BGCOLOR,VIOLET,
			PW_CONFIG_END);

	Butt_muster_chk[4]= pw_create_button(frame_muster_check_details, 180,95, 50, 30,
			PW_TEXT, "View",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_grp_details,
			PW_CONFIG_END);

	lbl_muster_chk[9] =  pw_create_label(frame_muster_check_details, 0, 170, 115, 30,
			PW_TEXT, "CrowBarCount:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_muster_chk[10]= pw_create_label(frame_muster_check_details, 115, 170, 60, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FGCOLOR,"white",
			PW_BGCOLOR,VIOLET,
			PW_CONFIG_END);
	Butt_muster_chk[5]= pw_create_button(frame_muster_check_details, 180,170, 50, 30,
			PW_TEXT, "View",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_crowbar_details,
			PW_CONFIG_END);
	lbl_muster_chk[11] =  pw_create_label(frame_muster_check_details, 0, 135, 115, 30,
			PW_TEXT, "Muster Id",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_muster_chk[12]= pw_create_label(frame_muster_check_details, 115, 135, 60, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FGCOLOR,"white",
			PW_BGCOLOR,VIOLET,
			PW_CONFIG_END);
	edit_muster_id =  pw_create_entry(frame_muster_check_details, 115, 135, 60, 30,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	Butt_muster_chk[6]= pw_create_button(frame_muster_check_details, 180,135, 50, 30,
			PW_TEXT, "EDIT",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_musterid_edit,
			PW_CONFIG_END);

	Butt_muster_chk[10]= pw_create_button(frame_muster_check_details, 40,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_finalcheck_back,
			PW_CONFIG_END);

	Butt_muster_chk[3] = pw_create_button(frame_muster_check_details, 140, 230, 72, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SUBMIT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_sbmt_musterid,
			PW_CONFIG_END);
	return;
}
char wg_cnt[10];
int get_people_present()
{
	char *wgseeker_count=NULL,*crowbar_count=NULL;

	sprintf(query,"select count(distinct jobcardno) from muster_attendance where  group_id='%s' and group_name='%s' and substr(created_date,1,10)='%s'",mis_group_id,selected_elem,buf_dt);

	wgseeker_count = exec_query_get_single_value(wgseeker_count, query, DB_PACK);
	sprintf(wg_cnt,"%s",wgseeker_count);

	sprintf(query,"select count(distinct jobcardno) from muster_attendance where  group_id='%s' and group_name='%s' and crowbar='Y' and substr(created_date,1,10)='%s'",mis_group_id,selected_elem,buf_dt);

	crowbar_count = exec_query_get_single_value(wgseeker_count, query, DB_PACK);
	sprintf(crbar_cnt,"%s",crowbar_count);
	free(crowbar_count);
	free(wgseeker_count);
	return 0;
}
char tot_cnt[10];
int get_total_members()
{
	char *total_count=NULL,*muster_id=NULL,*cnt_musterid=NULL;

	sprintf(query,"select sum(count) count from (select count(*) count from household_details hd where hd.group_name='%s' and hd.is_active='Y' union select count(*) count from tmp_group_members hd,tmp_group_master gm where gm.group_name='%s'  and is_active='Y' and hd.group_name=gm.group_name)",selected_elem,selected_elem);	

	total_count = exec_query_get_single_value(total_count, query, DB_PACK);	

	sprintf(tot_cnt,"%s",total_count);

	sprintf(query,"select distinct muster_id from muster_attendance where group_id='%s' and group_name='%s' and substr(created_date,1,10)= '%s'",mis_group_id,selected_elem,buf_dt);

	muster_id =exec_query_get_single_value(muster_id, query, DB_PACK);

	sprintf(sel_muster_id,"%s",muster_id);

	sprintf(query,"select ifnull(count(*),'0') from muster_attendance where group_id='%s' and group_name='%s'and muster_id='%s' and substr(created_date,1,10)='%s' and status in('M','N')",mis_group_id,selected_elem,sel_muster_id,buf_dt);

	cnt_musterid =exec_query_get_single_value(cnt_musterid, query, DB_PACK);

	if(atoi(cnt_musterid)==0)
	{
		pw_hide(Butt_muster_chk[6]);
	}
	else
	{
		pw_show(Butt_muster_chk[3]);
	}
	free(muster_id);
	free(total_count);
	free(cnt_musterid);
	return 0;
}
char sel_dt[15];
void get_muster_chk_count()
{
	strcpy(sel_dt,pw_get_text(dt_entry));
	printf("sel_date is %s\n",sel_dt);
	char *pstr, p1[20];
	pstr=strtok(sel_dt, "-");
	char *x1=pstr;
	pstr=strtok(NULL, "-");
	char *y1=pstr;
	strcpy(p1, y1);
	pstr=strtok(NULL, "-");

	sprintf(buf_dt, "%s-%s-%s", pstr, p1, x1);

	printf("sel_bufdate is %s\n",buf_dt);
	if(strcmp(selected_elem,"")==0)
	{
		show_message_frame("SELECT GROUP");
		return  ;
	}
	if (!frame_muster_check_details)
	{
		create_frame_muster_check_details();
		pw_show(frame_muster_check_details);	
	}
	else
		pw_show(frame_muster_check_details);	
	pw_show(Butt_muster_chk[6]);
	pw_hide(edit_muster_id);
	pw_hide(Butt_muster_chk[3]);
	char *mis_groupid=NULL;
	memset(mis_group_id, 0, 1);

	sprintf(query,"select distinct group_id from household_details where group_name='%s'",selected_elem);

	mis_groupid=exec_query_get_single_value(mis_groupid, query, DB_PACK);
	sprintf(mis_group_id,"%s",mis_groupid);
	free(mis_groupid);

	crbar_cnt[0]='\0';
	get_total_members();
	get_people_present();
	pw_set_text(lbl_muster_chk[4],selected_elem);	
	pw_set_text(lbl_muster_chk[6],tot_cnt);
	pw_set_text(lbl_muster_chk[8],wg_cnt);	
	pw_set_text(lbl_muster_chk[10],crbar_cnt);
	pw_set_text(lbl_muster_chk[12],sel_muster_id);
	pw_set_text(edit_muster_id,sel_muster_id);
	return ;
}
char issue[10];
int cb_wrk_status_cng()
{
	sqlite3 *dbhandle=NULL;

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	if(strcmp(selected_elem,"")==0)
	{
		show_message_frame("SELECT WORK CODE");
		return 0 ;
	}
	sprintf(query, "update muster_attendance set status='%s' where  work_code='%s' and status='%s'","N",selected_elem,"W") ;
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("query1 is %s\n", query);

	sprintf(query,"select distinct work_code from muster_attendance where status ='%s' and substr(created_date,1,10) ='%s'",issue,buf_dt);
	pw_tlistbox_clear(lbox_muster_chk);
	exec_query_load_into_lbox_single(lbox_muster_chk, query, DB_PACK);
	pw_update(lbox_muster_chk,1);
	sqlite3_close(dbhandle);

	return 0;
}
void cb_muster_cr_lbox_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
}
int cb_muster_cr_lbox_press()
{
	char *str;
	int idx = pw_tlistbox_get_selection(lbox_muster_chk);
	pw_tlistbox_get_element(lbox_muster_chk, &str,idx, 0);
	txtlb_get_element(lbox_muster_chk, &str, idx, 0);
	if(idx==-1)
	{
		return -1;
	}
	strcpy(selected_elem,str);
	show_text_frame(str);
	free(str);
	printf("selected_elem is %s\n",selected_elem);
	return 0;
}
int get_work_check_details()
{
	sprintf(query,"select distinct work_code from muster_attendance where substr(created_date,1,10)=current_date");
	pw_tlistbox_clear(lbox_works_chk);
	exec_query_load_into_lbox_single(lbox_works_chk, query, DB_PACK);
	pw_update(lbox_works_chk,1);
	return 0;
}
void get_view_muster(void)
{
	hide_frame_view_muster_option();
	show_frame_bemms_options();
	return;

}
char sel_wrk_code[20];
void get_muster_check_back()
{
	sel_wrk_code[0]='\0';
	selected_elem[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_muster_chk);	
	pw_hide(frame_muster_check_report);
	show_frame_view_muster_option();
}

void combobox_wrk_sel(pw_widget widget, int index)
{
	unsigned idx;
	char *wrk_type;
	pw_combobox_get_selection(combo_wrk_type,&wrk_type,&idx);
	strcpy(sel_wrk_code,wrk_type);
	free(wrk_type);
	printf("\n sel_wrk_code is %s\n",sel_wrk_code);

	strcpy(sel_dt,pw_get_text(dt_entry));
	printf("sel_date is %s\n",sel_dt);
	char *pstr, p1[20];
	pstr=strtok(sel_dt, "-");
	char *x1=pstr;
	pstr=strtok(NULL, "-");
	char *y1=pstr;
	strcpy(p1, y1);
	pstr=strtok(NULL, "-");
	sprintf(buf_dt, "%s-%s-%s", pstr, p1, x1);

	sprintf(query,"select distinct group_name from muster_attendance where work_code ='%s' and substr(created_date,1,10) ='%s'",sel_wrk_code,buf_dt);
	pw_tlistbox_clear(lbox_muster_chk);
	exec_query_load_into_lbox_single(lbox_muster_chk, query, DB_PACK);
	pw_update(lbox_muster_chk,1);
}
void combobox_issue(pw_widget widget, int index)
{
	char sel_issue[30]="";
	char *issue_type,*cnt_day_issue=NULL;
	unsigned idx;
	issue[0]='\0';
	sel_dt[0]='\0';
	pw_combobox_get_selection(combo_issue_type,&issue_type,&idx);
	strcpy(sel_issue,issue_type);
	free(issue_type);
	strncpy(issue,sel_issue,1);

	printf("issue is %s\n",issue);
	strcpy(sel_dt,pw_get_text(dt_entry));
	printf("sel_date is %s\n",sel_dt);
	char *pstr, p1[20];
	pstr=strtok(sel_dt, "-");
	char *x1=pstr;
	pstr=strtok(NULL, "-");
	char *y1=pstr;
	strcpy(p1, y1);
	pstr=strtok(NULL, "-");
	sprintf(buf_dt, "%s-%s-%s", pstr, p1, x1);

	sprintf(query,"select ifnull(count(*),'0') from muster_attendance where substr(created_date,1,10)='%s' and status='%s'",buf_dt,issue);

	cnt_day_issue = exec_query_get_single_value(cnt_day_issue, query, DB_PACK);

	if(atoi(cnt_day_issue)==0)
	{
		pw_show(lbox_muster_chk);
		pw_show(Butt_muster_chk[0]);
		pw_hide(Butt_muster_chk[2]);
		show_message_frame("NO ISSUES");
		pw_tlistbox_clear(lbox_muster_chk);
		get_muster_check_details();
		return ;
	}
	else if (strcmp(issue,"F")==0)
	{
		pw_hide(lbox_muster_chk);
		pw_hide(Butt_muster_chk[0]);
		pw_hide(Butt_muster_chk[2]);
		sprintf(query,"select distinct work_code from muster_attendance where status ='%s' and substr(created_date,1,10) ='%s'",issue,buf_dt);
		pw_tlistbox_clear(lbox_muster_chk);
		exec_query_load_into_lbox_single(lbox_muster_chk, query, DB_PACK);
		pw_update(lbox_muster_chk,1);
		return;
	}
	else if (strcmp(issue,"E")==0)
	{
		pw_hide(lbox_muster_chk);
		pw_hide(Butt_muster_chk[0]);
		pw_hide(Butt_muster_chk[2]);
		sprintf(query,"select distinct jobcardno from muster_attendance where status ='%s' and substr(created_date,1,10) ='%s'",issue,buf_dt);
		pw_tlistbox_clear(lbox_muster_chk);
		exec_query_load_into_lbox_single(lbox_muster_chk, query, DB_PACK);
		pw_update(lbox_muster_chk,1);

	}
	else
	{
		pw_show(lbox_muster_chk);
		pw_show(Butt_muster_chk[0]);
		pw_hide(Butt_muster_chk[2]);
		sprintf(query,"select distinct group_name from muster_attendance where status ='%s' and substr(created_date,1,10) ='%s'",issue,buf_dt);
		pw_tlistbox_clear(lbox_muster_chk);
		exec_query_load_into_lbox_single(lbox_muster_chk, query, DB_PACK);
		pw_update(lbox_muster_chk,1);
		return;
	}
	free(cnt_day_issue);
	return  ;
}
int get_entry_date(pw_widget widget, pw_event e)
{
	int day_of_week1,year1,month1,day1;
	char buf1_dt[15];
	buf_dt[0]='\0';

	buf1_dt[0]='\0';

	day_of_week1 = pw_create_calendar(frame_muster_check_report, 37, 77, &day1, &month1, &year1, PW_NA);

	snprintf(buf_dt, 32, "%d-%02d-%02d", year1,month1,day1);

	snprintf(buf1_dt, 32, "%02d-%02d-%d", day1,month1,year1);

	pw_set_text(dt_entry,buf1_dt);

	pw_set_text(combo_issue_type,"--Select--");

	char *cnt_day=NULL;

	sprintf(query,"select ifnull(count(*),'0') from muster_attendance where substr(created_date,1,10)='%s'",buf_dt);

	cnt_day = exec_query_get_single_value(cnt_day, query, DB_PACK);

	if(atoi(cnt_day)==0)
	{
		show_message_frame("NO e-MUSTERS");
		pw_tlistbox_clear(lbox_muster_chk);
		return -1;
	}
	free(cnt_day);
	pw_tlistbox_clear(lbox_muster_chk);
	sprintf(query,"select distinct group_name from muster_attendance where substr(created_date,1,10)='%s'",buf_dt);
	exec_query_load_into_lbox_single(lbox_muster_chk, query, DB_PACK);
	return 0;	
}
void create_frame_muster_check_report(void)
{
	frame_muster_check_report = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_muster_check_report);


	lab_issue =  pw_create_label(frame_muster_check_report,00, 10, 50,25,
			PW_TEXT, "Issue Type:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_issue_type = pw_create_combobox(frame_muster_check_report,50, 10, 180,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_issue_type,combobox_issue);
	lbl_day_wise =  pw_create_label(frame_muster_check_report,0, 40, 115, 20,
			PW_TEXT, "Date",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "yellow",
			PW_CONFIG_END);

	dt_entry  =     pw_create_label(frame_muster_check_report , 40, 40, 80, 25,
			PW_BGCOLOR, "white",
			PW_FGCOLOR, "black",
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_entry_date,
			PW_CONFIG_END);


	lbl_muster_chk[1] =  pw_create_label(frame_muster_check_report,0, 70, 120, 15,
			PW_TEXT, "Group Names",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbox_muster_chk = pw_create_tlistbox(frame_muster_check_report, 0, 86, 235, 134,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_muster_cr_lbox_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_muster_cr_lbox_press,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);


	Butt_muster_chk[0] = pw_create_button(frame_muster_check_report, 134,230, 100, 30,
			PW_TEXT, "View Details",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_chk_count,
			PW_CONFIG_END);

	Butt_muster_chk[1]= pw_create_button(frame_muster_check_report, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_check_back,
			PW_CONFIG_END);
	Butt_muster_chk[2] = pw_create_button(frame_muster_check_report, 64,230, 60, 30,
			PW_TEXT, "Update",
			PW_FONT, PW_BOLD_FONT,

			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wrk_status_cng,
			PW_CONFIG_END);
	return ;		
}
int get_muster_check_details()
{
	pw_tlistbox_clear(lbox_muster_chk);
	sprintf(query,"select distinct group_name from muster_attendance where substr(created_date,1,10)=current_date");
	exec_query_load_into_lbox_single(lbox_muster_chk, query, DB_PACK);
	return 0;
}
extern char date[15];
int cb_emuster()
{
	hide_frame_view_muster_option();
	if (!frame_muster_check_report)
	{
		create_frame_muster_check_report();
		pw_show(frame_muster_check_report);
	}
	else
		pw_show(frame_muster_check_report);
	pw_hide(Butt_muster_chk[2]);
	pw_set_text(dt_entry,date);	
	pw_combobox_clear(combo_issue_type);
	pw_set_text(combo_issue_type,"--Select--");
	pw_combobox_insert(combo_issue_type, "Future Dates", False);
	pw_combobox_insert(combo_issue_type, "Exceeded 150 Days JC", False);
	get_muster_check_details();
	return 0;
}
pw_widget  Butt_wrk_chk[7],frame_muster_work_report,lbl_wrk,lbl_grp, lbox_grp_chk,frame_muster_grp_report,lbl_frm_dt1,lbl_frm_dt,lbl_to_dt,lbl_to_dt1,frame_muster_grp_mem_report,lbl_grp_mem,lbox_mem_chk,frame_muster_det_report,lbl_wrk_cnt,lbl_wrk_cnt1,lbl_grp_cnt,lbl_grp_cnt1,lbl_wage_cnt,lbl_wage_cnt1, Butt_view_mus_det,lbl1;
char chk_work_code[20],frm_dt[15],to_dt[15],chk_grp[50],chk_group_id[15],count_wrks[10],count_grps[10],count_wage[10];
void get_work_check_back()
{
	pw_tlistbox_remove_selection_all(lbox_works_chk);
	hide_frame_muster_work_report();
	show_frame_view_muster_option();	
}
void get_grp_check_back()
{
	pw_tlistbox_remove_selection_all(lbox_grp_chk);
	hide_frame_muster_grp_report();
	pw_show(frame_muster_work_report);
}
void cb_mem_details_back ()
{
	hide_frame_muster_grp_mem_report();
	pw_show(frame_muster_grp_report);
}
void get_must_check_back()
{
	count_wrks[0]='\0';
	count_grps[0]='\0';
	count_wage[0]='\0';
	hide_frame_muster_det_report();
	show_frame_view_muster_option();
}
void create_frame_muster_grp_mem_report()
{
	frame_muster_grp_mem_report = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_muster_grp_mem_report);	

	lbl_grp_mem =  pw_create_label(frame_muster_grp_mem_report, 05, 05, 230, 35,
			PW_TEXT, "",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "RED",
			PW_CONFIG_END);

	lbox_mem_chk = pw_create_tlistbox(frame_muster_grp_mem_report, 0, 50, 240, 160,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_mem_chk, 160);

	Butt_wrk_chk[4] = pw_create_button(frame_muster_grp_mem_report, 40, 225,34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_mem_details_back ,
			PW_CONFIG_END);
}
int get_grp_mem_details()
{
	if(strcmp(chk_grp,"")==0)
	{
		show_message_frame("Select GroupName");
		return 0 ;
	}

	hide_frame_muster_grp_report();
	if (!frame_muster_grp_mem_report)
	{
		create_frame_muster_grp_mem_report();
		pw_show(frame_muster_grp_mem_report);
	}
	else
		pw_show(frame_muster_grp_mem_report);

	pw_set_text(lbl_grp_mem,chk_grp);

	sprintf(query,"select distinct substr(jobcardno,15,6),member_name from muster_attendance where  group_name='%s' and substr(created_date,1,10)=current_date and work_code='%s' and group_id='%s'",chk_grp,chk_work_code,chk_group_id);
	pw_tlistbox_clear(lbox_mem_chk);
	exec_query_load_into_lbox_double(lbox_mem_chk, query, DB_PACK);	
	return 0;	
}

void cb_grps_cr_lbox_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
}

int cb_grps_cr_lbox_press()
{
	char *str1;
	int idx = pw_tlistbox_get_selection(lbox_grp_chk);
	pw_tlistbox_get_element(lbox_grp_chk, &str1,idx, 0);
	txtlb_get_element(lbox_grp_chk, &str1, idx, 0);
	if(idx==-1)
	{
		return -1;
	}
	strcpy(chk_grp,str1);
	printf("sel group name is %s\n",chk_grp);
	char *chk_grpid=NULL;
	sprintf(query,"select distinct group_id from household_details where group_name='%s'",chk_grp);
	chk_grpid=exec_query_get_single_value(chk_grpid, query, DB_PACK);
	sprintf(chk_group_id,"%s",chk_grpid);
	free(chk_grpid);	
	show_text_frame(str1);
	free(str1);
	return 0;
}
void create_frame_muster_grp_report()
{
	frame_muster_grp_report = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_muster_grp_report);

	lbl_grp       =  pw_create_label(frame_muster_grp_report,0,55, 120, 15,
			PW_TEXT, "Group Names",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbox_grp_chk = pw_create_tlistbox(frame_muster_grp_report, 0,71, 235, 145,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_grps_cr_lbox_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_grps_cr_lbox_press,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	pw_tlistbox_set_selection_type(frame_muster_work_report,PW_TLISTBOX_SINGLE_SELECTION);

	Butt_wrk_chk[2] = pw_create_button(frame_muster_grp_report, 114,230, 110, 30,
			PW_TEXT, "Group Details",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_grp_mem_details,
			PW_CONFIG_END);

	Butt_wrk_chk[3]= pw_create_button(frame_muster_grp_report, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_grp_check_back,
			PW_CONFIG_END);
}
int get_dt_det(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(frm_dt,argv[0]);
	strcpy(to_dt, argv[1]);
	return(0);
}

int get_grp_wrk_details()
{
	if(strcmp(chk_work_code,"")==0)
	{
		show_message_frame("Select WorkCode");
		return 0 ;	
	}

	hide_frame_muster_work_report();
	if (!frame_muster_grp_report)
	{
		create_frame_muster_grp_report();
		pw_show(frame_muster_grp_report);
	}
	else
		pw_show(frame_muster_grp_report);	

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	char query[512];

	sprintf(query,"select distinct group_name from muster_attendance where substr(created_date,1,10)=current_date and work_code='%s'",chk_work_code);

	pw_tlistbox_clear(lbox_grp_chk);
	exec_query_load_into_lbox_single(lbox_grp_chk, query, DB_PACK);

	sqlite3_close(dbhandle);
	return 0;
}
void cb_wrks_cr_lbox_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
}
int cb_wrks_cr_lbox_press()
{
	char *str1;
	int idx = pw_tlistbox_get_selection(lbox_works_chk);
	pw_tlistbox_get_element(lbox_works_chk, &str1,idx, 0);
	txtlb_get_element(lbox_works_chk, &str1, idx, 0);
	if(idx==-1)
	{
		return -1;
	}
	strcpy(chk_work_code,str1);
	printf("sel work code is %s\n",chk_work_code);
	show_text_frame(str1);
	free(str1);
	return 0;
}

void create_frame_muster_work_report(void)
{
	frame_muster_work_report = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_muster_work_report);

	lbl_wrk       =  pw_create_label(frame_muster_work_report,0,25, 120, 15,
			PW_TEXT, "Work Codes",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbox_works_chk = pw_create_tlistbox(frame_muster_work_report, 0, 50, 235, 145,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_wrks_cr_lbox_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_wrks_cr_lbox_press,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	pw_tlistbox_set_selection_type(frame_muster_work_report,PW_TLISTBOX_SINGLE_SELECTION);

	Butt_wrk_chk[0] = pw_create_button(frame_muster_work_report, 124,230, 100, 30,
			PW_TEXT, "Work Details",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_grp_wrk_details,
			PW_CONFIG_END);

	Butt_wrk_chk[1]= pw_create_button(frame_muster_work_report, 10,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_work_check_back,
			PW_CONFIG_END);
	return ;
}
int cb_wrk_details()
{
	char *cnt_wrk=NULL,query[125];
	sprintf(query,"select ifnull(count(*),'0') from muster_attendance where substr(created_date,1,10)=current_date");
	cnt_wrk=exec_query_get_single_value(cnt_wrk, query, DB_PACK);
	if(atoi(cnt_wrk)==0)
	{
		show_message_frame("NO DATA");
		return 0 ;		
	}
	free(cnt_wrk);
	hide_frame_view_muster_option();
	if (!frame_muster_work_report)
	{
		create_frame_muster_work_report();
		pw_show(frame_muster_work_report);
	}
	else
		pw_show(frame_muster_work_report);

	get_work_check_details();
	return 0;
}
void create_frame_muster_det_report()
{

	frame_muster_det_report = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_muster_det_report);

	lbl1 =  pw_create_label(frame_muster_det_report, 05, 05, 230, 35,
			PW_TEXT, "TODAYS e-Muster Report",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "RED",
			PW_CONFIG_END);	

	lbl_wrk_cnt       =  pw_create_label(frame_muster_det_report,0,65, 120, 15,
			PW_TEXT, "No Of Works:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_wrk_cnt1      = pw_create_label(frame_muster_det_report, 120, 58, 90, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FGCOLOR,"white",
			PW_BGCOLOR,VIOLET,
			PW_CONFIG_END);

	lbl_grp_cnt       =  pw_create_label(frame_muster_det_report,0,110, 120, 15,
			PW_TEXT, "No Of Groups:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_grp_cnt1      = pw_create_label(frame_muster_det_report, 120, 100, 90, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FGCOLOR,"white",
			PW_BGCOLOR,VIOLET,
			PW_CONFIG_END);

	lbl_wage_cnt       =  pw_create_label(frame_muster_det_report,0,155,210, 15,
			PW_TEXT, "No Of Wg(present):",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbl_wage_cnt1      = pw_create_label(frame_muster_det_report, 190, 148, 90, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FGCOLOR,"white",
			PW_BGCOLOR,VIOLET,
			PW_CONFIG_END);

	Butt_wrk_chk[6]= pw_create_button(frame_muster_det_report, 10,200, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_must_check_back,
			PW_CONFIG_END);
}
int get_cnt_all_det(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(count_wrks,argv[0]);
	strcpy(count_grps, argv[1]);
	strcpy(count_wage, argv[2]);
	return(0);
}
int cb_muster_details()
{
	char *cnt_all=NULL;

	sprintf(query,"select count(*) from muster_attendance where substr(created_date,1,10)=current_date");
	cnt_all=exec_query_get_single_value(cnt_all, query, DB_PACK);	
	if(atoi(cnt_all)==0)
	{
		show_message_frame("NO DATA");
		return 0 ;
	}
	free(cnt_all);
	hide_frame_view_muster_option();
	if (!frame_muster_det_report)
	{
		create_frame_muster_det_report();
		pw_show(frame_muster_det_report);
	}
	else
		pw_show(frame_muster_det_report);

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select count(distinct work_code),count(distinct group_name),count(distinct jobcardno) from muster_attendance where substr(created_date,1,10)=current_date");

	sqlite3_exec(dbhandle, query, get_cnt_all_det, NULL, NULL);

	pw_set_text(lbl_wrk_cnt1,count_wrks);
	pw_set_text(lbl_grp_cnt1,count_grps);
	pw_set_text(lbl_wage_cnt1,count_wage);
	sqlite3_close(dbhandle);	
	return 0;	
}

void  create_frame_view_muster_option()
{
	frame_view_muster_option = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_view_muster_option);
	Butt_view_emusters = pw_create_button(frame_view_muster_option, 30, 30, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "  Issue  e-Muster",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_emuster,
			PW_CONFIG_END);

	Butt_view_wrk_det = pw_create_button(frame_view_muster_option, 82, 145, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Work Details",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wrk_details,
			PW_CONFIG_END);

	Butt_view_mus_det = pw_create_button(frame_view_muster_option, 135, 30, 75, 75,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Todays    Glance e-Muster",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_muster_details,
			PW_CONFIG_END);

	Butt_view_muster_back= pw_create_button(frame_view_muster_option, 20,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_view_muster,
			PW_CONFIG_END);
	return;
}
void show_frame_view_muster_option(void)
{
	if(!frame_view_muster_option)
		create_frame_view_muster_option();
	pw_show(frame_view_muster_option);
	return;
}
void hide_frame_view_muster_option(void)
{
	pw_hide(frame_view_muster_option);
	return;
}
void get_muster_check_count()
{
	hide_frame_bemms_options();
	show_frame_view_muster_option();
	return;
}

void get_pos_info_back(void)
{
	hide_frame_pos_info();
	show_frame_mis_report();	
}
pw_widget frame_pos_info, btn_pos_info[4];
void create_frame_pos_info(void)
{
	frame_pos_info = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_pos_info);

	btn_pos_info[0] =pw_create_button(frame_pos_info, 30,20, 190, 30,
			PW_TEXT, "Master Tables Report",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_tables_count,
			PW_CONFIG_END);
	btn_pos_info[1] = pw_create_button(frame_pos_info, 30,90, 190, 30,
			PW_TEXT, "Memory Information ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_mem_info,
			PW_CONFIG_END);
	btn_pos_info[2] = pw_create_button(frame_pos_info, 30,160, 190, 30,
			PW_TEXT, "Muster-id Report(MCC)",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_wc_report,
			PW_CONFIG_END);

	btn_pos_info[4] =  pw_create_button(frame_pos_info, 30,220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_pos_info_back,
			PW_CONFIG_END);
	return;
}
void show_frame_pos_info(void)
{
	if(! frame_pos_info)
	{
		create_frame_pos_info();
		pw_show(frame_pos_info);
	}
	else
		pw_show(frame_pos_info);
	return;
}
void hide_frame_pos_info(void)
{
	pw_hide(frame_pos_info);
	return;
}
void cb_system_info(void)
{
	hide_frame_mis_report();
	show_frame_pos_info();
	return;
}
void get_misopttions_back(void)
{
	hide_frame_bemms_options();
	show_frame_mis_report();
	return;
}
pw_widget frame_bemms_options,btn_bemms_options[2];
void create_frame_bemms_options()
{
	frame_bemms_options = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_bemms_options);

	btn_bemms_options[0] =pw_create_button(frame_bemms_options, 30,50, 190, 45,
			PW_TEXT, "Veiw Muster ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_check_count,
			PW_CONFIG_END);
	btn_bemms_options[1] = pw_create_button(frame_bemms_options, 30,145, 190, 45,
			PW_TEXT, "HH Employment Report",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_wage_payorder,
			PW_CONFIG_END);

	btn_bemms_options[2] =pw_create_button(frame_bemms_options, 30,220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_misopttions_back,
			PW_CONFIG_END);
	return;
}
void show_frame_bemms_options(void)
{
	if(!frame_bemms_options)
	{
		create_frame_bemms_options();
		pw_show(frame_bemms_options);
	}
	else
		pw_show(frame_bemms_options);
	return;
}
void hide_frame_bemms_options(void)
{
	pw_hide(frame_bemms_options);
	return;
}
void cb_bemms(void)
{
	hide_frame_mis_report();
	show_frame_bemms_options();
	return;
}
pw_widget frame_mis_report,btn_mis_report[3];
void cb_kill_fa()
{
	system("killall -9 FA");
}
int cb_update()
{		
	char query[512];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query,"delete from download_status");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"insert into download_status(download_status)values('%s')","Y");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	show_message_frame("UPDATED");
	close_sqlite(dbhandle);
	return 0;
}

void create_frame_mis_report(void)
{
	frame_mis_report = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_mis_report);

	btn_mis_report[0] = pw_create_button(frame_mis_report, 82, 25, 75, 75,
			PW_TEXT, "Bemms   Report",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bemms,
			PW_CONFIG_END);
	btn_mis_report[1] = pw_create_button(frame_mis_report, 82, 140, 75, 75,
			PW_TEXT, "POS Report",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_system_info,
			PW_CONFIG_END);

	btn_mis_report[2] = pw_create_button(frame_mis_report, 160,230, 60, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Home",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_kill_fa,
			PW_CONFIG_END);
	btn_mis_report[4] = pw_create_button(frame_mis_report, 30,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_pos_details_back,
			PW_CONFIG_END);
	return;
}
void show_frame_mis_report(void)
{
	if(! frame_mis_report)
	{
		create_frame_mis_report();
		pw_show(frame_mis_report);
	}
	else
		pw_show(frame_mis_report);
	return;
}
void hide_frame_mis_report(void)
{
	pw_hide(frame_mis_report);
	return;
}
void  hide_frame_muster_work_report(void)
{
	pw_hide(frame_muster_work_report);
}
void hide_frame_muster_grp_report(void)
{
	pw_hide(frame_muster_grp_report);
}
void hide_frame_muster_grp_mem_report()
{
	pw_hide(frame_muster_grp_mem_report);
}
void hide_frame_muster_det_report()
{
	pw_hide(frame_muster_det_report);
}
