#include "types.h"
#include<string.h>
#include "string_list.h"
#include "functions.h"
#include<time.h>
#ifdef __arm__
#include<autils.h>
#include<ctype.h>
#endif
void txtlb_get_element(pw_widget w, char **s, int idx, int column);
int exec_query_load_into_lbox_single(pw_widget lbox_muster_att_grpdet , char *query, char *database);
int exec_query_load_into_lbox_double(pw_widget lbox_muster_att_grpdet , char *query, char *database);
void show_progress_frame();
void hide_progress_frame();
void show_frame_get_muster();
void hide_frame_get_normal();
void show_frame_muster_update();
void hide_frame_muster_update();
pw_widget lbl_muster_update3,frame_muster_update,frame_get_normal,frame_get_normal,label_normal[15],combo_normal_hab,txt_entry_wrk_id,Butt_wrkid_search,combo_normal_wkcode,label_normal_frmdate[5],label_normal_todate[5],
	  lbox_normal_wktag_grps,Butt_normal_mustr,Butt_normal_mustr_back,txt_entry_muster_id,Butt_normal_mustr_view
	  ,lbl_muster_update1,lbl_muster_update2,lbox_muster_update,edit_muster_id,Butt_muster_update_edit,Butt_muster_update_back,Butt_muster_update_submit;
	  char *exec_query_get_single_value(char *data, char *query, char *database);
	  int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
	  char *normal_date=NULL,*from_date=NULL,*normal_date1=NULL,*from_date1=NULL;
	  char query[1024],sel_frmdt[11],sel_todt[11],selected_wrkid[10],sel_workcode[40],sel_groupname[500];
	  char tmp_hc[40][20],tmp_wc[20][20],tmp_gid[20][20];
	  int no_record_found;
void get_view_normal()
{
	char *count_nmd=NULL;
	sprintf(query,"select count(*) from mcc_work_allocation_details where allocation_type='NMD'");
	count_nmd=exec_query_get_single_value(count_nmd,query,DB_PACK);
	if(atoi(count_nmd)==0)
	{
		show_message_frame("No Records");
	}
	else
	{

		hide_frame_get_normal();
		show_frame_muster_update();
	}
}
int get_details_nmd(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(tmp_hc[no_record_found],argv[0]);
	strcpy(tmp_wc[no_record_found],argv[1]);
	strcpy(tmp_gid[no_record_found],argv[2]);
	printf("\n\n:::::::::::::::::%d:::::::\n\n",no_record_found);
	no_record_found++;
	return 0;
}
int get_save_normal()
{
	printf("inside save");

	//strcpy(sel_workcode, pw_get_text(combo_normal_wkcode));

	if(strcmp(sel_workcode,"")==0)
	{
		show_message_frame("SELECT WORK ID");
		return 0;
	}
	int idx,p;
	char *str1=NULL;
	idx = pw_tlistbox_get_selection(lbox_normal_wktag_grps);

	if(idx==-1)
	{
		show_message_frame("SELECT GROUP NAME");
		return 0;
	}
	pw_tlistbox_get_element(lbox_normal_wktag_grps, &str1,idx, 0);
	txtlb_get_element(lbox_normal_wktag_grps, &str1, idx, 0);
	sel_groupname[0]='\0';
	strcpy(sel_groupname,str1);

	free(str1);
	printf("workcode is %s",sel_workcode);
	printf(" group is %s",sel_groupname);
	if(strcmp(pw_get_text(txt_entry_muster_id),"")==0)
	{
		show_message_frame("ENTER MUSTER ID");
		return 0;
	}
	if (strlen(pw_get_text(txt_entry_muster_id))!=5)
	{
		show_message_frame("MUSTERID MUST BE 5 DIGIT");
		pw_entry_clear(txt_entry_muster_id);
		return 0;
	}
	char *srch_chk_qty2=NULL;
	srch_chk_qty2=strpbrk(pw_get_text(txt_entry_muster_id),"-*+.");
	printf("srch_chk_qty2 is %s\n",srch_chk_qty2);
	while(srch_chk_qty2 != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(txt_entry_muster_id);
		return 0;
	}
	char *muster_id_check1=NULL;
	sprintf(query,"select count(*) from mcc_work_allocation_details where muster_id like   '%%%s%%' and allocation_type='NMD'",pw_get_text(txt_entry_muster_id));
	muster_id_check1=exec_query_get_single_value(muster_id_check1,query,DB_PACK);
	printf("query is %s",query);
	/*sprintf(query,"select count(*) from muster_id where muster_id like   '%%%s%%'",pw_get_text(txt_entry_muster_id));
	  printf("query is %s",query);
	  muster_id_check2=exec_query_get_single_value(muster_id_check2,query,DB_PACK);
	 */if(atoi(muster_id_check1)>0)
	{
		show_message_frame("MUSTER ID EXISTS");
		return 0;
	}
	no_record_found=0;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select distinct household_code,worker_code,group_id from household_details where group_name='%s' and is_active='Y'",sel_groupname);

	printf("query is %s",query);
	sqlite3_exec(dbhandle, query, get_details_nmd,NULL,NULL);
	char *year1=NULL,*year2=NULL;
	sprintf(query,"select substr(date('now'),1,4)");
	year1=exec_query_get_single_value(year1,query,DB_PACK);
	sprintf(query,"select substr(date('now'),3,2)+1");
	year2=exec_query_get_single_value(year2,query,DB_PACK);

	show_progress_frame("PLEASE WAIT....");
	for(p=0; p<no_record_found; p++)
	{
		sprintf(query,"insert into mcc_work_allocation_details	(demand_id,work_code,group_id,household_code,worker_code,allocation_type,alloc_from_date,alloc_to_date,group_name,muster_id)	values('123','%s','%s','%s','%s','NMD','%s','%s','%s','%s'||'%s'||'%s')",sel_workcode,tmp_gid[p],tmp_hc[p],tmp_wc[p],sel_frmdt,sel_todt,sel_groupname,year1,year2,pw_get_text(txt_entry_muster_id));
		printf("\n insert query is %s\n",query);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}		

	free(muster_id_check1);
	//free(muster_id_check2);
	free(srch_chk_qty2);
	pw_entry_clear(txt_entry_muster_id);
	pw_tlistbox_clear(lbox_normal_wktag_grps);

	sprintf(query,"select distinct group_name from household_details where group_id NOT IN (select group_id from mcc_work_allocation_details  where  substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s')",sel_frmdt,sel_todt);

	printf("query is %s  ",query);
	exec_query_load_into_lbox_single(lbox_normal_wktag_grps, query, DB_PACK);
	hide_progress_frame();
	show_message_frame("Successfully Saved");
	return 0;
}
void combobox_normal(pw_widget widget, int index)
{

	char query1[1500];

	sqlite3 *dbhandle=NULL;

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sel_workcode[0]='\0';
	strcpy(sel_workcode, pw_get_text(combo_normal_wkcode)); 
	pw_tlistbox_clear(lbox_normal_wktag_grps);

	sprintf(query1,"select distinct group_name from household_details where group_id NOT IN (select group_id from mcc_work_allocation_details  where  substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s')",sel_frmdt,sel_todt);

	printf("query1 is %s  ",query1);
	exec_query_load_into_lbox_single(lbox_normal_wktag_grps, query1, DB_PACK);
	sqlite3_close(dbhandle);

	return;
}
int get_search_wrkid_normal()
{
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	char *srch_chk_qty=NULL,*search_cnt=NULL;
	srch_chk_qty=strpbrk(pw_get_text(txt_entry_wrk_id),"-*+.");
	printf("srch_chk_qty is %s\n",srch_chk_qty);
	while(srch_chk_qty != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(txt_entry_wrk_id);
		return 0;
	}
	free(srch_chk_qty);
	strcpy(selected_wrkid,pw_get_text(txt_entry_wrk_id));
	sprintf(query,"select  count(*) from Work_Registration where work_code like '%%%s%%' and work_type_code IN('325','326','327','328','329','330','331','332','333','334','335','336','337','338','340','341','342','346','347','348','349','350','351','352','353','354','355','356','357','358','359','360','361','362','363','364','365','366','367','369','370','371','372','373','374','375','376','377','378','379','380','381','382','383','384','385','386','387','389','390','391','392','393','394','395','396','397','398','399','402','403','404','405','406','407','408','409','412','413','416','417','418','419','420','421','422','423','424','425','426','427','428','429','430','432','433','434','435','437','438','439','441','442','443') and description in('Sanctioned-Startup','In-Progress')",selected_wrkid);
	search_cnt = exec_query_get_single_value(search_cnt, query, DB_PACK);
	if(atoi(search_cnt)==0)
	{
		show_message_frame("NO DATA");
		pw_entry_clear(txt_entry_wrk_id);
		return 0;
	}
	sprintf(query,"select distinct work_code from Work_Registration where  work_code like '%%%s%%'  and work_type_code IN('325','326','327','328','329','330','331','332','333','334','335','336','337','338','340','341','342','346','347','348','349','350','351','352','353','354','355','356','357','358','359','360','361','362','363','364','365','366','367','369','370','371','372','373','374','375','376','377','378','379','380','381','382','383','384','385','386','387','389','390','391','392','393','394','395','396','397','398','399','402','403','404','405','406','407','408','409','412','413','416','417','418','419','420','421','422','423','424','425','426','427','428','429','430','432','433','434','435','437','438','439','441','442','443') and description in('Sanctioned-Startup','In-Progress')",selected_wrkid );

	pw_combobox_clear(combo_normal_wkcode);
	pw_set_text(combo_normal_wkcode,"--Select--");
	pw_entry_clear(txt_entry_wrk_id);
	exec_query_load_into_combo(combo_normal_wkcode, query, DB_PACK);
	free(search_cnt);
	return 0;
}
void workid_normal()
{
	pw_combobox_clear(combo_normal_wkcode);
	pw_set_text(combo_normal_wkcode,"--Select--");
	//sprintf(query,"select distinct work_code from Work_Registration");
	sprintf(query,"select distinct work_code from Work_Registration where work_type_code IN('325','326','327','328','329','330','331','332','333','334','335','336','337','338','340','341','342','346','347','348','349','350','351','352','353','354','355','356','357','358','359','360','361','362','363','364','365','366','367','369','370','371','372','373','374','375','376','377','378','379','380','381','382','383','384','385','386','387','389','390','391','392','393','394','395','396','397','398','399','402','403','404','405','406','407','408','409','412','413','416','417','418','419','420','421','422','423','424','425','426','427','428','429','430','432','433','434','435','437','438','439','441','442','443') and description in('Sanctioned-Startup','In-Progress')");
	exec_query_load_into_combo(combo_normal_wkcode, query, DB_PACK);
	char *batch_status=NULL,sel_batch_status[5],*day_test1=NULL,sel_day_test1[10];
	sprintf(query,"select Batch_type from Batch_info");
	batch_status= exec_query_get_single_value(batch_status,query, DB_PACK);
	sprintf(sel_batch_status, "%s", batch_status);

	day_test1 =exec_query_get_single_value(day_test1,"select strftime('%w','now')",DB_PACK);
	sprintf(sel_day_test1, "%s", day_test1);

	if(strcmp(sel_batch_status,"B1")==0)
	{
		if(atoi(sel_day_test1)==1)
		{
			sprintf(query, "select date('now')");
			normal_date = exec_query_get_single_value(normal_date, query, DB_PACK);
			sprintf(sel_frmdt, "%s", normal_date);
			free(normal_date);
			sprintf(query,"select date('now','weekday 0')");
			normal_date1=exec_query_get_single_value(normal_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", normal_date1);
			free(normal_date1);
		}
		else
		{
			sprintf(query, "select date('now','weekday 1','-7 days')");
			from_date = exec_query_get_single_value(from_date, query, DB_PACK);
			sprintf(sel_frmdt, "%s", from_date);
			free(from_date);
			sprintf(query,"select date('now','weekday 0')");
			from_date1=exec_query_get_single_value(normal_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", from_date1);
			free(from_date1);
		}
	}
	else
	{
		printf("sel_day_test11 is %s\n",sel_day_test1);
		if(atoi(sel_day_test1)==4)
		{
			printf("sel_day_test1 is %s\n",sel_day_test1);
			sprintf(query, "select date('now')");
			normal_date = exec_query_get_single_value(normal_date, query, DB_PACK);
			sprintf(sel_frmdt, "%s", normal_date);
			free(normal_date);
			sprintf(query,"select date('%s','weekday 3')",sel_frmdt);
			normal_date1=exec_query_get_single_value(normal_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", normal_date1);
			free(normal_date1);
		} 

		else
		{
			sprintf(query, "select date('now','weekday 4','-7 days')");
			from_date = exec_query_get_single_value(from_date, query, DB_PACK);
			sprintf(sel_frmdt, "%s", from_date);
			free(from_date);
			sprintf(query,"select date('%s','weekday 3')",sel_frmdt);
			from_date1=exec_query_get_single_value(normal_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", from_date1);
			free(from_date1);
		}
	}
	free(batch_status);
	free(day_test1);
	return ;
}

void get_normal_back()
{
	hide_frame_get_normal();
	//show_frame_normallogin_page();
	show_frame_get_muster();
}

void create_frame_get_normal(void)
{
	frame_get_normal = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_get_normal);
	label_normal[1] =  pw_create_label(frame_get_normal, 0, 3, 75, 20,
			PW_TEXT, "Work Id:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	txt_entry_wrk_id =  pw_create_entry(frame_get_normal, 85, 3, 60, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	pw_focus(txt_entry_wrk_id);

	Butt_wrkid_search = pw_create_button(frame_get_normal, 160, 3,76, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SEARCH,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_search_wrkid_normal,
			PW_CONFIG_END);
	label_normal[2] =  pw_create_label(frame_get_normal, 0, 40, 80, 20,
			PW_TEXT, "Work Id:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	combo_normal_wkcode = pw_create_combobox(frame_get_normal, 60, 40, 185, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_normal_wkcode ,combobox_normal);
	label_normal[7] =  pw_create_label(frame_get_normal, 00, 65, 85, 20,
			PW_TEXT, "Muster Id:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	txt_entry_muster_id =  pw_create_entry(frame_get_normal, 85, 65, 60, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	label_normal[4] =  pw_create_label(frame_get_normal, 0, 192, 80, 20,
			PW_TEXT, "From Date:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	pw_hide(label_normal[4]);
	label_normal_frmdate[0] =  pw_create_label(frame_get_normal, 80,192, 200, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);
	pw_hide(label_normal_frmdate[0]);
	label_normal[5] =  pw_create_label(frame_get_normal, 0, 215, 80, 20,
			PW_TEXT, "To     Date:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	pw_hide(label_normal[5]);
	label_normal_todate[1] =  pw_create_label(frame_get_normal, 80, 215, 160, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);
	pw_hide(label_normal_todate[1]);
	label_normal[6] =  pw_create_label(frame_get_normal,0, 95, 120, 15,
			PW_TEXT, "Tagged Groups",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbox_normal_wktag_grps = pw_create_tlistbox(frame_get_normal, 0, 110, 239, 120,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	//pw_tlistbox_selection_intimate(lbox_normal_wktag_grps,lbox_seltag_grp);
	pw_tlistbox_set_selection_type(frame_get_normal,PW_TLISTBOX_SINGLE_SELECTION);

	Butt_normal_mustr = pw_create_button(frame_get_normal, 134, 235, 60, 27,
			PW_TEXT, "SAVE",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_save_normal,
			PW_CONFIG_END);

	Butt_normal_mustr_back = pw_create_button(frame_get_normal, 20, 235, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_normal_back,
			PW_CONFIG_END);
	Butt_normal_mustr_view = pw_create_button(frame_get_normal, 64, 235, 60, 27,
			PW_TEXT, "VIEW",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_view_normal,
			PW_CONFIG_END);
	return;
}

void show_frame_get_normal(void)
{
	show_progress_frame("PLEASE WAIT....");
	if (!frame_get_normal)
	{
		create_frame_get_normal();
		pw_show(frame_get_normal);
	}
	else
		pw_show(frame_get_normal);

	pw_tlistbox_clear(lbox_normal_wktag_grps);

	workid_normal();
	hide_progress_frame();
	return;
}
void hide_frame_get_normal(void)
{
	pw_hide(frame_get_normal);
}
char sel_groupname2[50];
char sel_musterid[50];
int cb_muster_update_submit()
{
	int idx;
	char *str2=NULL,*str3=NULL;
	idx = pw_tlistbox_get_selection(lbox_muster_update);

	if(idx==-1)
	{
		show_message_frame("SELECT GROUP NAME");
		return 0;
	}
	pw_tlistbox_get_element(lbox_muster_update, &str2,idx, 0);
	txtlb_get_element(lbox_muster_update, &str2, idx, 0);
	pw_tlistbox_get_element(lbox_muster_update, &str3,idx, 1);
	txtlb_get_element(lbox_muster_update, &str3, idx, 1);
	sel_musterid[0]='\0';	
	strcpy(sel_musterid,str3);
	sel_groupname2[0]='\0';
	strcpy(sel_groupname2,str2);

	if(strcmp(pw_get_text(edit_muster_id),"")==0)
	{
		show_message_frame("ENTER MUSTER ID");
		return 0;
	}
	if (strlen(pw_get_text(edit_muster_id))!=5)
	{
		show_message_frame("MUSTERID MUST BE 5 DIGIT");
		pw_entry_clear(edit_muster_id);
		return 0;
	}
	char *srch_chk_qty2=NULL;
	srch_chk_qty2=strpbrk(pw_get_text(edit_muster_id),"-*+.");
	printf("srch_chk_qty2 is %s\n",srch_chk_qty2);
	while(srch_chk_qty2 != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(edit_muster_id);
		return 0;
	}
	char *muster_id_check3=NULL,*muster_id_check4=NULL;
	sprintf(query,"select count(*) from mcc_work_allocation_details where muster_id like   '%%%s%%'",pw_get_text(edit_muster_id));
	muster_id_check3=exec_query_get_single_value(muster_id_check3,query,DB_PACK);
	printf("query is %s",query);
	sprintf(query,"select count(*) from muster_id where muster_id like   '%%%s%%'",pw_get_text(edit_muster_id));
	printf("query is %s",query);
	muster_id_check4=exec_query_get_single_value(muster_id_check4,query,DB_PACK);
	if(atoi(muster_id_check3)>0 || atoi(muster_id_check4)>0)
	{
		show_message_frame("MUSTER ID EXISTS");
		return 0;
	}
	sqlite3 *dbhandle;
	sqlite3_open(DB_PACK, &dbhandle);

	char *year3=NULL,*year4=NULL;
	sprintf(query,"select substr(date('now'),1,4)");
	year3=exec_query_get_single_value(year3,query,DB_PACK);
	sprintf(query,"select substr(date('now'),3,2)+1");
	year4=exec_query_get_single_value(year4,query,DB_PACK);
	sprintf(query,"update mcc_work_allocation_details set muster_id='%s%s%s' where group_name='%s' and allocation_type='NMD' and muster_id='%s%s%s'",year3,year4,pw_get_text(edit_muster_id),sel_groupname2,year3,year4,sel_musterid);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("\n update query in MCC %s\n",query);

	sprintf(query,"update muster_attendance set muster_id='%s%s%s' where group_name='%s'  and muster_id='%s%s%s'",year3,year4,pw_get_text(edit_muster_id),sel_groupname2,year3,year4,sel_musterid);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("\n update query is %s\n",query);
	pw_tlistbox_clear(lbox_muster_update);
	sprintf(query, "select distinct group_name,substr(muster_id,7,6) from mcc_work_allocation_details where allocation_type='NMD' and substr(alloc_from_date,1,10)='%s' and substr(alloc_to_date,1,10)='%s'",sel_frmdt,sel_todt);

	exec_query_load_into_lbox_double(lbox_muster_update, query, DB_PACK);
	show_message_frame("MUSTER-ID \n UPDATED");
	pw_entry_clear(edit_muster_id);
	sqlite3_close(dbhandle);
	return 0;
}

int get_muster_update_delete()
{
	sqlite3 *dbhandle;
	sqlite3_open(DB_PACK, &dbhandle);
	int idx;
	char *str3=NULL;
	idx = pw_tlistbox_get_selection(lbox_muster_update);

	if(idx==-1)
	{
		show_message_frame("SELECT GROUP NAME");
		return 0;
	}
	pw_tlistbox_get_element(lbox_muster_update, &str3,idx, 0);
	txtlb_get_element(lbox_muster_update, &str3, idx, 0);
	sel_groupname2[0]='\0';
	strcpy(sel_groupname2,str3);

	sprintf(query,"delete from mcc_work_allocation_details where group_name='%s' and allocation_type='NMD'",sel_groupname2);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	pw_tlistbox_clear(lbox_muster_update);
	sprintf(query, "select distinct group_name,substr(muster_id,7,6) from mcc_work_allocation_details where allocation_type='NMD' and substr(alloc_from_date,1,10)='%s' and substr(alloc_to_date,1,10)='%s'",sel_frmdt,sel_todt);

	exec_query_load_into_lbox_double(lbox_muster_update, query, DB_PACK);
	show_message_frame("GroupName \n Deleted");
	sqlite3_close(dbhandle);
	return 0;
}
void cb_muster_update_back()
{
	hide_frame_muster_update();
	show_frame_get_normal();
}
int muster_group_release()
{
	int idx;
	char *str4=NULL;
	idx = pw_tlistbox_get_selection(lbox_muster_update);

	if(idx==-1)
		return 0;

	pw_tlistbox_get_element(lbox_muster_update, &str4,idx, 0);
	txtlb_get_element(lbox_muster_update, &str4, idx, 0);
	sel_groupname2[0]='\0';
	strcpy(sel_groupname2,str4);

	char *count_group=NULL;
	sprintf(query,"select count(*) from muster_attendance where group_name='%s' and from_date='%s' and to_date='%s' and status='Y'",sel_groupname2,sel_frmdt,sel_todt);
	count_group=exec_query_get_single_value(count_group,query,DB_PACK);
	if(atoi(count_group)>0)
	{
		show_message_frame("Muster Attendance \n Taken Already");
		pw_tlistbox_remove_selection_all(lbox_muster_update);
		return 0;
	}
	return 0;

}
void cb_muster_update_edit()
{
	if(strcmp(sel_groupname2,"")==0)
	{
		show_message_frame("SELECT GROUPNAME");
		return ;
	}

	pw_show(edit_muster_id);
	pw_show(Butt_muster_update_submit);
	pw_focus(edit_muster_id);
	pw_show(lbl_muster_update3);
}
void create_frame_muster_update()
{
	frame_muster_update = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_muster_update);	
	lbl_muster_update3 =  pw_create_label(frame_muster_update, 00, 05, 70, 30,
			PW_TEXT, "Muster-Id",
			//PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_hide(lbl_muster_update3);	

	edit_muster_id =  pw_create_entry(frame_muster_update, 70, 05, 60, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	pw_hide(edit_muster_id);	
	Butt_muster_update_submit = pw_create_button(frame_muster_update, 165, 05,64, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Save", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_muster_update_submit ,
			PW_CONFIG_END);
	pw_hide(Butt_muster_update_submit);

	lbl_muster_update1 =  pw_create_label(frame_muster_update, 20, 40, 160, 15,
			PW_TEXT, "Group Name",
			//PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	lbl_muster_update2 =  pw_create_label(frame_muster_update, 115, 40, 150, 15,
			PW_TEXT, "Muster-Id",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	lbox_muster_update = pw_create_tlistbox(frame_muster_update, 0, 55, 240, 170,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,muster_group_release,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_muster_update, 60);

	Butt_muster_update_edit= pw_create_button(frame_muster_update, 75,230, 67, 30,
			PW_TEXT, "DELETE",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_update_delete,
			PW_CONFIG_END);

	Butt_muster_update_back = pw_create_button(frame_muster_update, 20, 230,34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_muster_update_back ,
			PW_CONFIG_END);
	Butt_muster_update_edit = pw_create_button(frame_muster_update, 165, 230,60, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "EDIT", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_muster_update_edit ,
			PW_CONFIG_END);
}
void show_frame_muster_update(void)
{
	show_progress_frame("PLEASE WAIT....");
	if (!frame_muster_update)
	{
		create_frame_muster_update();
		pw_show(frame_muster_update);
	}
	else
		pw_show(frame_muster_update);
	pw_tlistbox_clear(lbox_muster_update);
	sprintf(query, "select distinct group_name,substr(muster_id,7,6) from mcc_work_allocation_details where allocation_type='NMD' and substr(alloc_from_date,1,10)>='%s' and substr(alloc_to_date,1,10)<='%s'",sel_frmdt,sel_todt);
	exec_query_load_into_lbox_double(lbox_muster_update, query, DB_PACK);
	hide_progress_frame();
	return;
}
void hide_frame_muster_update(void)
{
	pw_hide(frame_muster_update);
}
