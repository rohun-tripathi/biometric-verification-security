#include "types.h"
#include <autils.h>


pw_widget demand_aloc_frame,lbl_aloc_grps[6],lbl_wrktype_nam,combo_aloc_hab,lbox_alocated_grps,Butt_aloc_grps[2],combo_aloc_grptype,combo_aloc_wkcod,combo_aloc_wktype,demand_work_aloc_frame,lbl_demanded_grp_aloc[8],entry_aloc_days,lbl_aloc_frmdt,lbl_aloc_todt,Butt_aloc_addgrp,lbox_alocgrp_details,Butt_aloc_grpsdetail,lbl_demanded_getgrp_aloc[8];
pw_widget frame_view_allocation_details,lbl_view_alloc[5],lbox_savealocated_grps,lbl_get_view_alloc[7],Butt_savealoc_grp,Butt_savealoc_add,Butt_aloc_grpsdetailback,Butt_del_aloc_grp;
int work_codes();
void show_demand_options();
void hide_demand_aloc_frame();
void show_demand_aloc_frame(); 
void show_demand_work_aloc_frame();
void hide_demand_work_aloc_frame();
void show_progress_frame();
void hide_progress_frame();
int exec_query_load_into_lbox_qudraple(pw_widget lbox, char *query, char *database);
int exec_query_load_into_lbox_trriple(pw_widget lbox, char *query, char *database);
int exec_query_load_into_lbox_double(pw_widget lbox, char *query, char *database);
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
int exec_query_load_into_lbox_single(pw_widget combo, char *query, char *database);
char *exec_query_get_single_value(char *data, char *query, char *database);
void show_text_frame();
void hide_text_frame();
void txtlb_get_element(pw_widget w, char **s, int idx, int column);
int  Gps_Test(double *lat, double *lon);
void show_frame_view_allocation_details();
void hide_frame_view_allocation_details();
void hide_allcation_selection_period();

char sel_wrk_cod[20],select_alloc_days[10],buf_dt_alloc[15],sel_alloc_grp_name[100],sel_recepitno[10],rem_mandays[10],hh_wrk_code_alloc[40][40],dem_dt[35][35],recept_no[35][35],grp_id_alloc[16],sel_chk_batch[2],sel_aloc_grp_type[20];
char alloc_todtchk[15],alloc_type[40],st_check[2];
void get_alocgrps_detail_back()
{	
	pw_entry_clear(entry_aloc_days);
	pw_set_text(lbl_aloc_frmdt,"");
	pw_set_text(lbl_aloc_todt,"");
	select_alloc_days[0]='\0';
	buf_dt_alloc[0]='\0';
	sel_wrk_cod[0]='\0';
	sel_alloc_grp_name[0]='\0';
	  alloc_type[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_alocated_grps);	
	hide_demand_work_aloc_frame();
	pw_show(demand_aloc_frame);	
}
char dmnd_frmdt[15],dmnd_todt[15];
int get_recepit_details(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(dmnd_frmdt, argv[0]);
	printf("dmnd_frmdt is %s\n",dmnd_frmdt);
	strcpy(dmnd_todt, argv[1]);
	printf("dmnd_todt is %s\n",dmnd_todt);
	return 0;
}
int allocate_info()
{
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	char query[256];
	//sprintf(query,"select distinct fromdate,todate from Demand_Group_details where substr(recepit_id,3,6)='%s'",sel_recepitno);
	sprintf(query,"select distinct min(demand_frmdt),min(demand_todt) from tmp_demand_group_allocate");
	sqlite3_exec(dbhandle, query, get_recepit_details, NULL, NULL);
	printf("query is %s\n",query);
	sqlite3_close(dbhandle);
	return 0;
}
int del_temp_alloc()
{
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	char query[256];
	sprintf(query,"delete from tmp_demand_allocation");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);
	return 0;
}
pw_widget Butt_aloc_view;
int view_btn_chk()
{
	char query[125],*chk_viewcnt=NULL;
	sprintf(query,"select ifnull(count(*),'0') from tmp_demand_allocation");
	chk_viewcnt = exec_query_get_single_value(chk_viewcnt, query, DB_PACK);
	if(atoi(chk_viewcnt)==0)
	{
		pw_disable(Butt_aloc_view);
	}
	else
	{
		pw_enable(Butt_aloc_view);
	}
	free(chk_viewcnt);
	return 0;
}
char demand_fr_tmp[70][30],demand_to_tmp[70][30];
int get_hh_details_alloc(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(hh_wrk_code_alloc[no_worker_found], argv[0]);
	strcpy(demand_fr_tmp[no_worker_found], argv[1]);
	strcpy(demand_to_tmp[no_worker_found], argv[2]);
	no_worker_found++;
	return(0);
}
void get_savealoc_detail_back()
{
	pw_entry_clear(entry_aloc_days);
	pw_set_text(lbl_aloc_frmdt,"");
	pw_set_text(lbl_aloc_todt,"");
	select_alloc_days[0]='\0';
	buf_dt_alloc[0]='\0';
	hide_frame_view_allocation_details();
	pw_show(demand_work_aloc_frame);
	view_btn_chk();
}
void get_allocated_add()
{
get_savealoc_detail_back();
}

char sel_wrkaloc_todt[15],sel_aloc_wrk[15];
int alloc_days;
int work_alloc_details()
{
	char query[1024],date_only[15],*alocdays_cnt=NULL;
	sprintf(date_only,"%s","%d-%m-%Y");
	sprintf(query,"select ifnull(sum(alloc_days),'0') from tmp_demand_allocation");
	alocdays_cnt = exec_query_get_single_value(alocdays_cnt, query, DB_PACK);
	if(strcmp(alloc_type,"14 Days Allocation")==0)
        {
                printf("inside 14days if");
                if(atoi(alocdays_cnt)<14)
                {
                    pw_hide(Butt_savealoc_grp);    
			pw_show(Butt_savealoc_add);
                }
		 else
                {
		pw_hide(Butt_savealoc_add);   
                        pw_show(Butt_savealoc_grp);
                }

        }
        else 
	{
		if(atoi(alocdays_cnt)<7)
                {
		pw_hide(Butt_savealoc_grp);   
                        pw_show(Butt_savealoc_add);
                }
		 else
                {
                        pw_hide(Butt_savealoc_add);
                        pw_show(Butt_savealoc_grp);
                }

        }

	pw_tlistbox_clear(lbox_savealocated_grps);

	sprintf(query,"select  distinct substr(work_code, 13, 6),substr(strftime('%s',alloc_from_date),1,6)||substr(strftime('%s',alloc_from_date),9,2),substr(strftime('%s',alloc_to_date),1,6)||substr(strftime('%s',alloc_to_date),9,2),alloc_days from tmp_demand_allocation where group_name='%s' order by alloc_from_date,alloc_to_date",date_only,date_only,date_only,date_only,sel_alloc_grp_name);
	exec_query_load_into_lbox_qudraple(lbox_savealocated_grps,query, DB_PACK);
	pw_update(lbox_savealocated_grps,1);

	pw_set_text(lbl_get_view_alloc[3],alocdays_cnt);
	free(alocdays_cnt);
	return 0;
}
int cb_del_alloc_wrk()
{
	char query[256];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	if(strcmp(sel_aloc_wrk,"")==0)
	{
		show_message_frame("Select work code");
		return 0;
	}
	sprintf(query,"delete from tmp_demand_allocation where work_code like '%%%s%%'",sel_aloc_wrk);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("del query is %s\n",query);

	show_message_frame("DELETED Successfully");
	sel_aloc_wrk[0]='\0';
	work_alloc_details();
	sqlite3_close(dbhandle);
	return 0;
}
int no_record_found;
char tmp_work_code[10][20],tmp_allc_frmdate[10][20],tmp_allc_todate[10][20],tmp_allc_days[10][10];
int get_tmp_save_alloc(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(tmp_work_code[no_record_found],argv[0]);
	strcpy(tmp_allc_frmdate[no_record_found],argv[1]);
	strcpy(tmp_allc_todate[no_record_found],argv[2]);
	strcpy(tmp_allc_days[no_record_found],argv[3]);
	no_record_found++;
	return 0;
}

char min_tmp_dt[20],alloc_endtime[20];
int get_allocated_save()
{
	char alloc_date[25]="";
	int hour=0, min=0, sec=0, day1=0, month1=0, year1=0;
	au_get_time(&hour, &min, &sec, &day1, &month1, &year1);
	sprintf(alloc_endtime, "%02d:%02d:%02d",hour,min,sec);
	sprintf(alloc_date, "%04d-%02d-%02d %02d:%02d:%02d",year1,month1,day1,hour,min,sec);

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	double lat, lon;
	Gps_Test(&lat, &lon);
	int j;
	show_progress_frame("Saving Please Wait");
	char query[1024];
	memset(hh_wrk_code_alloc,'\0',sizeof(hh_wrk_code_alloc));
	memset(demand_fr_tmp,'\0',sizeof(demand_fr_tmp));
	memset(demand_to_tmp,'\0',sizeof(demand_to_tmp));
	sprintf(query,"select distinct jobcardno,demand_frmdt,demand_todt from tmp_demand_group_allocate where jobcardno NOT IN(select jobcardno from work_allocation_details where alloc_from_date='%s' UNION select household_code||worker_code from mcc_work_allocation_details where substr(alloc_from_date,1,10)='%s')",min_tmp_dt,min_tmp_dt);
	printf("\n query is %s\n",query);
	no_worker_found=0;
	sqlite3_exec(dbhandle, query, get_hh_details_alloc, NULL, NULL);
	memset(tmp_allc_frmdate,'\0',sizeof(tmp_allc_frmdate));
	memset(tmp_allc_frmdate,'\0',sizeof(tmp_allc_todate));
	memset(tmp_work_code,'\0',sizeof(tmp_work_code));
	memset(tmp_allc_days,'\0',sizeof(tmp_allc_days));
	sprintf(query,"select ifnull(work_code,0),ifnull(alloc_from_date,0),ifnull(alloc_to_date,0),ifnull(alloc_days,0) from tmp_demand_allocation where group_name='%s'",sel_alloc_grp_name);
	no_record_found=0;
	sqlite3_exec(dbhandle, query, get_tmp_save_alloc, NULL, NULL);
	for(i=0; i<no_worker_found; i++)
	{
		for(j=0; j<no_record_found; j++)//TODO:add sqlite erroe before insert
		{
			sprintf(query,"insert into work_allocation_details(work_code,group_name,group_id,jobcardno,demanded_days,alloc_days,alloc_from_date,alloc_to_date,demand_frm_dt,demand_to_dt,gps_long,gps_lat,status,allocation_date) values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%lf','%lf','%s','%s')",tmp_work_code[j],sel_alloc_grp_name,grp_id_alloc,hh_wrk_code_alloc[i],"12",tmp_allc_days[j],tmp_allc_frmdate[j],tmp_allc_todate[j],demand_fr_tmp[i],demand_to_tmp[i],lon,lat,"N",alloc_date);
			printf("\n insert query is %s\n",query);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
			sprintf(query,"update remaining_mandays_details set balance_mandays=(balance_mandays-'%s') where work_code='%s'",tmp_allc_days[j],tmp_work_code[j]);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
			printf("query1 is %s\n", query);
			sprintf(query,"update remaining_mandays_details set spent_mandays=(spent_mandays+'%s') where work_code='%s'",tmp_allc_days[j],tmp_work_code[j]);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
			printf("query1 is %s\n", query);
		}

	}
	get_alocgrps_detail_back();
	del_temp_alloc();
	hide_progress_frame();
	pw_show(demand_aloc_frame);
	sel_alloc_grp_name[0]='\0';
	show_message_frame("Work Allocation saved");
	sqlite3_close(dbhandle);
	return 0;
}

pw_widget frame_demand_allctn_view,lbl_demand_allocation_view[3],lbox_alocated_jcid,butt_view_allocation_back,butt_allocation_save;
void get_viewaloc_detail_back()
{
	pw_hide(frame_demand_allctn_view);
	pw_show(frame_view_allocation_details);
}


void create_frame_allocation_view()
{
	frame_demand_allctn_view = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	pw_hide(frame_demand_allctn_view);
	lbl_demand_allocation_view[0]=pw_create_label(frame_demand_allctn_view,00, 00,240,25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_demand_allocation_view[1]=pw_create_label(frame_demand_allctn_view,00, 30,180,15,
			PW_TEXT, "No of Wageseekers",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);
	lbl_demand_allocation_view[2]=pw_create_label(frame_demand_allctn_view,180, 30,60,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbox_alocated_jcid=pw_create_tlistbox(frame_demand_allctn_view, 0, 50, 240, 170,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_alocated_jcid,240);
	pw_update(lbox_alocated_jcid,1);
	butt_view_allocation_back=pw_create_button(frame_demand_allctn_view, 16, 230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_viewaloc_detail_back,
			PW_CONFIG_END);

	butt_allocation_save=pw_create_button(frame_demand_allctn_view, 130, 230, 60, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "SAVE",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_allocated_save,
			PW_CONFIG_END);
}
void show_fram_allocation_view()
{
	if(!frame_demand_allctn_view)
	{
		create_frame_allocation_view();
		pw_show(frame_demand_allctn_view);
	}
	else
		pw_show(frame_demand_allctn_view);
	pw_set_text(lbl_demand_allocation_view[0],sel_alloc_grp_name);
	printf("sel grp name is :%s:",sel_alloc_grp_name);
	char query3[1024],*tot_wg_cnt=NULL;
	pw_tlistbox_clear(lbox_alocated_jcid);
	sprintf(query3,"select distinct substr(household_code,15,4)||worker_code||'-'||name from household_details where household_code||worker_code IN (select distinct jobcardno from tmp_demand_group_allocate where jobcardno NOT IN(select jobcardno from work_allocation_details where alloc_from_date='%s' UNION select household_code||worker_code from mcc_work_allocation_details where substr(alloc_from_date,1,10)='%s'))",min_tmp_dt,min_tmp_dt);
	exec_query_load_into_lbox_single(lbox_alocated_jcid, query3, DB_PACK);
	pw_update(lbox_alocated_jcid,1);
	sprintf(query3,"select count(distinct jobcardno) from tmp_demand_group_allocate where jobcardno NOT IN(select jobcardno from work_allocation_details where alloc_from_date='%s' UNION select household_code||worker_code from mcc_work_allocation_details where substr(alloc_from_date,1,10)='%s')",min_tmp_dt,min_tmp_dt);

	tot_wg_cnt =exec_query_get_single_value(tot_wg_cnt,query3,DB_PACK);
	pw_set_text(lbl_demand_allocation_view[2],tot_wg_cnt);
	free(tot_wg_cnt);
}
char wrk_tmp[20],frm_tmp[10],to_tmp[10];
int get_date_wrkid(void *parg, int argc, char **argv, char **colnames)
{
        strcpy(wrk_tmp, argv[0]);
        strcpy(frm_tmp, argv[1]);
        strcpy(to_tmp,  argv[2]); 
        return 0;
}

void get_allocated_view()
{
		//TODO:completed spliting but this will move to save button
	char *buf_min_frmdt=NULL,query2[800],*date_check=NULL,*date_check1=NULL,*tmp_count=NULL;
	min_tmp_dt[0]='\0';
	sprintf(query2,"select min(alloc_from_date) from tmp_demand_allocation where group_name='%s'",sel_alloc_grp_name);
	buf_min_frmdt=exec_query_get_single_value(buf_min_frmdt,query2,DB_PACK);
	sprintf(min_tmp_dt,"%s",buf_min_frmdt);
	free(buf_min_frmdt);
	sprintf(query2,"select date('%s','+6 days')",min_tmp_dt);
	date_check=exec_query_get_single_value(date_check,query2,DB_PACK);
	 sprintf(query2,"select date('%s','+7 days')",min_tmp_dt);
	date_check1=exec_query_get_single_value(date_check,query2,DB_PACK);
	sprintf(query2,"select count(*) from tmp_demand_allocation where group_name='%s' and alloc_to_date='%s'",sel_alloc_grp_name,date_check);
	tmp_count=exec_query_get_single_value(tmp_count,query2,DB_PACK);
	if(atoi(tmp_count)==0)
	{
	sqlite3 *dbhandle=NULL;
        sqlite3_open(DB_PACK, &dbhandle);
        if (dbhandle == NULL)
                printf("Unable to open db\n");
	sprintf(query2,"select work_code,alloc_from_date,alloc_to_date from tmp_demand_allocation where group_name='%s' and '%s'  between alloc_from_date and alloc_to_date ",sel_alloc_grp_name,date_check);
	printf("query is %s",query2);
	sqlite3_exec(dbhandle, query2, get_date_wrkid, NULL, NULL);
	sprintf(query2,"delete from tmp_demand_allocation where group_name='%s' and  work_code='%s' and alloc_from_date='%s'  and alloc_to_date='%s'",sel_alloc_grp_name,wrk_tmp,frm_tmp,to_tmp);
	sqlite3_exec(dbhandle, query2, NULL, NULL, NULL);
	char *allocate_days=NULL,*allocate_days1=NULL;
	sprintf(query2,"select julianday('%s')-julianday('%s')",date_check,frm_tmp);
	allocate_days=exec_query_get_single_value(allocate_days,query2,DB_PACK);
	 sprintf(query2,"select julianday('%s')-julianday('%s')",to_tmp,date_check1);
        allocate_days1=exec_query_get_single_value(allocate_days1,query2,DB_PACK);
	sprintf(query2,"insert into tmp_demand_allocation (group_name,work_code,alloc_from_date,alloc_to_date,alloc_days) values('%s','%s','%s','%s','%d')",sel_alloc_grp_name,wrk_tmp,frm_tmp,date_check,atoi(allocate_days)+1);
	sqlite3_exec(dbhandle, query2, NULL, NULL, NULL);
	printf("\n\n\nquery is .....%s..\n\n",query2);
	sprintf(query2,"insert into tmp_demand_allocation (group_name,work_code,alloc_from_date,alloc_to_date,alloc_days) values('%s','%s','%s','%s','%d')",sel_alloc_grp_name,wrk_tmp,date_check1,to_tmp,atoi(allocate_days1)+1);
	sqlite3_exec(dbhandle, query2, NULL, NULL, NULL);
printf("\n\n\nquery is .....%s..\n\n",query2);	
	sqlite3_close(dbhandle);
	free(allocate_days);
        free(allocate_days1);
	}
	free(tmp_count);
	free(date_check1);
	free(date_check);
	pw_hide(frame_view_allocation_details);
	show_fram_allocation_view();
}
void sel_lbox_savealocated_grps()
{
	char *str=NULL;
	int idx = pw_tlistbox_get_selection(lbox_savealocated_grps);
	pw_tlistbox_get_element(lbox_savealocated_grps, &str,idx, 0);

	txtlb_get_element(lbox_savealocated_grps, &str, idx, 0);
	if(idx==-1)
	{
		return  ;
	}
	sel_aloc_wrk[0]='\0';
	strcpy(sel_aloc_wrk,str);
	printf("selected workcode to del is %s\n",sel_aloc_wrk);
	free(str);
	return  ;
}
void create_frame_view_allocation_details()
{
	frame_view_allocation_details = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	lbl_view_alloc[0] = pw_create_label(frame_view_allocation_details,00, 00,240,25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_view_alloc[1] = pw_create_label(frame_view_allocation_details,00, 30,70,15,
			PW_TEXT, "Wrk Code",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_view_alloc[2] = pw_create_label(frame_view_allocation_details,70, 30,60,15,
			PW_TEXT, "Frm Dt",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);
	lbl_view_alloc[3] = pw_create_label(frame_view_allocation_details,130, 30,60,15,
			PW_TEXT, "To Dt",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);
	lbl_view_alloc[4] = pw_create_label(frame_view_allocation_details,190, 30,60,15,
			PW_TEXT, "Days",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbox_savealocated_grps = pw_create_tlistbox(frame_view_allocation_details, 0, 45, 240, 100,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_savealocated_grps,67);
	pw_tlistbox_addcolumn(lbox_savealocated_grps,67);
	pw_tlistbox_addcolumn(lbox_savealocated_grps,20);
	pw_update(lbox_savealocated_grps,1);
	pw_tlistbox_selection_intimate(lbox_savealocated_grps,sel_lbox_savealocated_grps);

	pw_tlistbox_set_selection_type(lbox_savealocated_grps,PW_TLISTBOX_SINGLE_SELECTION);

	lbl_view_alloc[4] = pw_create_label(frame_view_allocation_details,00, 150,120,20,
			PW_TEXT, "Allocated Days",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_get_view_alloc[3] = pw_create_label(frame_view_allocation_details,120,150,90,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	Butt_del_aloc_grp = pw_create_button(frame_view_allocation_details, 60, 220, 70, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Delete",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_del_alloc_wrk,
			PW_CONFIG_END);

	Butt_savealoc_grp = pw_create_button(frame_view_allocation_details, 160, 220, 60, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "VIEW",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_allocated_view,
			PW_CONFIG_END);
	 Butt_savealoc_add = pw_create_button(frame_view_allocation_details, 160, 220, 60, 30,
                        PW_JUSTIFY, PW_JUSTIFY_CENTER,
                        PW_FONT, PW_BOLD_FONT,
                        PW_TEXT, "ADD",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_allocated_add,
                        PW_CONFIG_END);
	Butt_aloc_grpsdetailback = pw_create_button(frame_view_allocation_details, 16, 220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_savealoc_detail_back,
			PW_CONFIG_END);
}
void show_frame_view_allocation_details()
{
	if(!frame_view_allocation_details)
	{
		create_frame_view_allocation_details();
		pw_show(frame_view_allocation_details);
	}
	else
		pw_show(frame_view_allocation_details);
	pw_set_text(lbl_view_alloc[0],sel_alloc_grp_name);	

	work_alloc_details();
}

void hide_frame_view_allocation_details()
{
	pw_hide(frame_view_allocation_details);
}
void show_view_allocation_details()
{
	hide_demand_work_aloc_frame();
	show_frame_view_allocation_details();
}

pw_widget allcation_selection_period,lbl_allocation_selection_period[7],lbox_alocated_selection_period,Butt_aloc_selection_period[3];
int get_allocated_grp_details()
{
	printf("aloc todate in workalloca %s\n",sel_wrkaloc_todt);
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	if(strcmp(sel_wrk_cod,"")==0)
	{
		show_message_frame("SELECT WORKCODE");
		return 0;
	}
	if(strcmp(pw_get_text(entry_aloc_days),"")==0)
	{
		show_message_frame("ENTER NO OF DAYS");
		return 0;
	}
	if(strcmp(pw_get_text(entry_aloc_days),"00000")==0)
	{
		show_message_frame("WRONG DATA");
		return 0;
	}
	char *chk_entry=NULL;

	chk_entry =strpbrk(pw_get_text(entry_aloc_days),"-*+.0");
	printf("chk_entry is %s\n",chk_entry);
	while(chk_entry != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(entry_aloc_days);
		pw_set_text(lbl_aloc_frmdt,"");
		pw_set_text(lbl_aloc_todt,"");
		buf_dt_alloc[0]='\0';
		return 0;
	}
	free(chk_entry);

	char query[512];
	if(strcmp(alloc_type,"14 Days Allocation")==0)
	{
		printf("inside 14days if");
		if(atoi(pw_get_text(entry_aloc_days))>14)
		{
			show_message_frame("NO of Days 14-Exceeded");
			pw_entry_clear(entry_aloc_days);
			pw_set_text(lbl_aloc_frmdt,"");
			pw_set_text(lbl_aloc_todt,"");
			return 0;
		}
	}
	else
	{
		if(atoi(pw_get_text(entry_aloc_days))>7)
		{
			show_message_frame("NO of Days 7-Exceeded");
			pw_entry_clear(entry_aloc_days);
			pw_set_text(lbl_aloc_frmdt,"");
			pw_set_text(lbl_aloc_todt,"");
			return 0;
		}
	}

	memset(select_alloc_days,'\0',sizeof(select_alloc_days));
	strcpy(select_alloc_days,pw_get_text(entry_aloc_days)); 
	printf("select_alloc_days is %s\n",select_alloc_days);
	printf("sel_recepitno is %s\n",sel_recepitno);
	char *jc_cnt=NULL;
	sprintf(query,"select count(distinct jobcardno) from tmp_demand_group_allocate");
	jc_cnt=exec_query_get_single_value(jc_cnt, query, DB_PACK);
	alloc_days=(atoi(jc_cnt))*(atoi(select_alloc_days));
	free(jc_cnt);
	printf("\n alloc days=%d\n",alloc_days);
	printf("\n rem_mandays is %d\n",atoi(rem_mandays));

	if(alloc_days>atoi(rem_mandays))
	{       
		show_message_frame("Allocation Days Greater than\nRemaining Person days");
		pw_entry_clear(entry_aloc_days);
		pw_set_text(lbl_aloc_frmdt,"");
		pw_set_text(lbl_aloc_todt,"");
		buf_dt_alloc[0]='\0';
		return 0;
	}

	if(strcmp(buf_dt_alloc,"")==0)
	{
		show_message_frame("Select From Date");
		return 0;
	}

	char *count_in_tmp=NULL,*date_chek=NULL,*date_chek1=NULL;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	memset(tmp_allc_frmdate,'\0',sizeof(tmp_allc_frmdate));
	memset(tmp_allc_frmdate,'\0',sizeof(tmp_allc_todate));
	memset(tmp_work_code,'\0',sizeof(tmp_work_code));
	memset(tmp_allc_days,'\0',sizeof(tmp_allc_days));
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	sprintf(query,"select count(*) from tmp_demand_allocation");
	count_in_tmp = exec_query_get_single_value(count_in_tmp, query, DB_PACK);
	no_record_found=0;

	if(atoi(count_in_tmp)>0)
	{
		sprintf(query,"select ifnull(work_code,0),ifnull(alloc_from_date,0),ifnull(alloc_to_date,0),ifnull(alloc_days,0) from tmp_demand_allocation where group_name='%s'",sel_alloc_grp_name);

		printf("query is %s",query);
		sqlite3_exec(dbhandle, query,get_tmp_save_alloc, NULL, NULL);
		printf("no_worker_found is :%d:",no_record_found);
		for (i=0; i < no_record_found; i++ )
		{
			sprintf(query,"select (case when '%s' BETWEEN '%s' AND '%s' then 'NOK' else 'OK' end)",buf_dt_alloc,tmp_allc_frmdate[i],tmp_allc_todate[i]);
			printf("query is %s",query);
			date_chek=exec_query_get_single_value(date_chek, query, DB_PACK);
			if(strcmp(date_chek,"NOK")==0)
			{
				show_message_frame("Date Period Exists");
				return 0;
			}
			sprintf(query,"select (case when '%s' BETWEEN '%s' AND '%s' then 'NOK' else 'OK' end)",alloc_todtchk,tmp_allc_frmdate[i],tmp_allc_todate[i]);
			date_chek1=exec_query_get_single_value(date_chek1, query, DB_PACK);
			if(strcmp(date_chek1,"NOK")==0)
			{ 
				show_message_frame("Date Period Exists");
				return 0;
			}
			if(strcmp(tmp_work_code[i],sel_wrk_cod)==0)
			{
				show_message_frame("Work ID Exists");
				return 0;

			}
		}
	}
/*
	char *st_check=NULL;
	sprintf(query,"select Batch_type from Batch_info");
	st_check=exec_query_get_single_value(st_check,query,DB_PACK);
*/
	if(strcmp(st_check,"B2")==0)
	{
		char *saturday1=NULL,*check_saturday=NULL,*alloc_todtchk1=NULL;
		sprintf(query,"select date('%s','weekday 6')",alloc_todtchk);
		saturday1=exec_query_get_single_value(saturday1,query,DB_PACK);
		sprintf(query,"select case when '%s'=='%s' then 'OK' else 'NOK' end",alloc_todtchk,saturday1);
		check_saturday=exec_query_get_single_value(check_saturday,query,DB_PACK);
		if(strcmp(check_saturday,"OK")==0)
		{
			sprintf(query,"select date('%s','+1 day')",alloc_todtchk);
			alloc_todtchk1=exec_query_get_single_value(alloc_todtchk1,query,DB_PACK);

			sprintf(query,"insert into tmp_demand_allocation(group_name,work_code,alloc_from_date,alloc_to_date,alloc_days) values('%s','%s','%s','%s','%s')",sel_alloc_grp_name,sel_wrk_cod,buf_dt_alloc,alloc_todtchk1,pw_get_text(entry_aloc_days));
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);        
			sprintf(query,"update tmp_demand_allocation set alloc_days=alloc_days+1 where work_code='%s'",sel_wrk_cod);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		}
		else
		{
			sprintf(query,"insert into tmp_demand_allocation(group_name,work_code,alloc_from_date,alloc_to_date,alloc_days) values('%s','%s','%s','%s','%s')",sel_alloc_grp_name,sel_wrk_cod,buf_dt_alloc,alloc_todtchk,pw_get_text(entry_aloc_days));
			printf("query insert is %s",query);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		}
	}
	else
	{
		sprintf(query,"insert into tmp_demand_allocation(group_name,work_code,alloc_from_date,alloc_to_date,alloc_days) values('%s','%s','%s','%s','%s')",sel_alloc_grp_name,sel_wrk_cod,buf_dt_alloc,alloc_todtchk,pw_get_text(entry_aloc_days));
		printf("query insert is %s",query);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	sqlite3_close(dbhandle);        
	show_view_allocation_details();
	return 0;	
}
int cb_allocated_grp_viewdetails()
{
	hide_demand_work_aloc_frame();
	show_frame_view_allocation_details();
	return 0;
}
char sel_day_test[10],sel_day[10],wday[3];
int check_weekday()
{
	char ch[3]="%w",*wkday=NULL,query[100];
	sprintf(query,"select strftime('%s','now')",ch);
	wkday=exec_query_get_single_value(wkday,query,DB_PACK);
	printf("query is %s",query);
	sprintf(wday,"%s",wkday);
	free(wkday);
	return 0;
}
int cb_alocfrm_date()
{
	if(strcmp(sel_wrk_cod,"")==0)
	{
		show_message_frame("SELECT WORKCODE");
		return 0;		
	}	
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	char *alocdays_cnt=NULL,query[1024];
	sprintf(query,"select ifnull(sum(alloc_days),'0') from tmp_demand_allocation");
	alocdays_cnt = exec_query_get_single_value(alocdays_cnt, query, DB_PACK);
	if(strcmp(alloc_type,"14 Days Allocation")==0)
	{
		printf("inside 14days from dt");
		if(atoi(alocdays_cnt)>=14)
		{
			show_frame_view_allocation_details();
			return 0;
		}
	}
		else
		{
			if(atoi(alocdays_cnt)>=7)
			{
				show_frame_view_allocation_details();
				return 0;
			}
		}
	//}
	free(alocdays_cnt);
	int days=atoi(pw_get_text(entry_aloc_days));
	printf("days is %d\n",days);
	if(strcmp(pw_get_text(entry_aloc_days),"")==0)
	{
		show_message_frame("ENTER NO OF DAYS");
		return 0;
	}
	char *chk_entry=NULL;
	chk_entry=strpbrk(pw_get_text(entry_aloc_days),"-*+.0");
	printf("chk_entry is %s\n",chk_entry);
	while(chk_entry != NULL)
	{
		show_message_frame("WRONG VALUE");
		pw_entry_clear(entry_aloc_days);
		pw_set_text(lbl_aloc_frmdt,"");
		pw_set_text(lbl_aloc_todt,"");
		buf_dt_alloc[0]='\0';
		return 0;
	}
	free(chk_entry);

	char query_fromdt[512];
	int day_of_week_alloc,year_alloc=0,month_alloc=0,day_alloc=0;
	buf_dt_alloc[0]='\0';
	char buf1_dt_alloc[20];
	buf1_dt_alloc[0]='\0';
	printf("before cal");
	day_of_week_alloc = pw_create_calendar(demand_work_aloc_frame, 37, 77, &day_alloc, &month_alloc, &year_alloc, PW_NA);

	snprintf(buf_dt_alloc, 32, "%d-%02d-%02d", year_alloc,month_alloc,day_alloc);

	snprintf(buf1_dt_alloc, 32, "%02d-%02d-%d", day_alloc,month_alloc,year_alloc);
	char *status_frm_dt=NULL,*ck_frmdt=NULL;
	printf("from date is to buf_dt_alloc %s\n",buf_dt_alloc);

	sprintf(query_fromdt,"select (case when '%s'<=(select max(alloc_to_date) from work_allocation_details  where group_name='%s' and status in ('N','Y') then 'NOK' else 'OK' end)",buf_dt_alloc,sel_alloc_grp_name);
	printf("query of already allocatedcase %s\n",query_fromdt);
	ck_frmdt = exec_query_get_single_value(ck_frmdt, query_fromdt, DB_PACK);

	if(strcmp(ck_frmdt,"NOK")==0)
	{
		show_message_frame("Already allocation exist  \nin Selected Period");
		pw_set_text(lbl_aloc_frmdt,"");
		return 0;
	}


	sprintf(query_fromdt,"select (case when '%s'<(select date('now')) then 'NOK' else 'OK' end)",buf_dt_alloc);
	printf("query of case %s\n",query_fromdt);
	ck_frmdt = exec_query_get_single_value(ck_frmdt, query_fromdt, DB_PACK);

	if(strcmp(ck_frmdt,"NOK")==0)
	{
		show_message_frame("PAST DATE");
		pw_set_text(lbl_aloc_frmdt,"");
		return 0;
	}
	free(ck_frmdt);
	check_weekday();

	char *curnt_day=NULL;
	sprintf(query_fromdt,"select (case when current_date>='%s' then 'NOK' else 'OK' end)",buf_dt_alloc);
	curnt_day=exec_query_get_single_value(curnt_day,query_fromdt,DB_PACK);
	printf("query is %s",query_fromdt);
	if(strcmp(curnt_day,"NOK")==0)
	{
		show_message_frame("Should be greater than \n current date");
		pw_set_text(lbl_aloc_frmdt,"");
		pw_set_text(lbl_aloc_todt,"");
		buf_dt_alloc[0]='\0';
		return 0;
	}
	free(curnt_day);

	sprintf(query_fromdt,"select (case when '%s'<'%s' then 'NOK' else 'OK' end)",buf_dt_alloc,dmnd_frmdt);
	status_frm_dt = exec_query_get_single_value(status_frm_dt, query_fromdt, DB_PACK);
	printf("past date query is %s\n",query_fromdt);
	if(strcmp(status_frm_dt,"NOK")==0)
	{
		show_message_frame("Lessthan Demand Date");
		pw_entry_clear(entry_aloc_days);
		pw_set_text(lbl_aloc_frmdt,"");
		pw_set_text(lbl_aloc_todt,"");
		buf_dt_alloc[0]='\0';
		return 0;       
	}
	free(status_frm_dt);
	char *alloc_grp_chk=NULL;
	sprintf(query,"select ifnull(count(*),'0') from mcc_work_allocation_details where group_name='%s' and (select ifnull(max(alloc_to_date),0) from mcc_work_allocation_details  where group_name='%s')>='%s'",sel_alloc_grp_name,sel_alloc_grp_name,buf_dt_alloc);
	alloc_grp_chk = exec_query_get_single_value(alloc_grp_chk, query, DB_PACK);
	if(atoi(alloc_grp_chk)>=1)
	{
		show_message_frame("Already Allocated in MCC");
		pw_set_text(lbl_aloc_frmdt,"");
		buf_dt_alloc[0]='\0';
		return 0;
	}
	free(alloc_grp_chk);

	printf("from date is to buf1_dt_alloc %s\n",buf1_dt_alloc);
	pw_set_text(lbl_aloc_frmdt,buf1_dt_alloc);
	char *chk_batch=NULL,*day_test=NULL;
	sprintf(query_fromdt,"select ifnull(Batch_type,'0') from Batch_info");
	chk_batch = exec_query_get_single_value(chk_batch,query_fromdt, DB_PACK);

	printf("batch query is %s\n",query_fromdt);
	sprintf(sel_chk_batch, "%s", chk_batch);
	printf("sel_batchstatus is %s\n",sel_chk_batch);
	free(chk_batch);

	char weekday[3]="%w",*chk_day=NULL;
	sprintf(query_fromdt,"select strftime('%s','%s')",weekday,buf_dt_alloc);
	chk_day=exec_query_get_single_value(chk_day,query_fromdt,DB_PACK);
	printf("query is %s",query_fromdt);
	if(strcmp(chk_day,"0")==0)
	{
		show_message_frame("FROM DATE is SUNDAY");
		pw_set_text(lbl_aloc_frmdt,"");
		buf_dt_alloc[0]='\0';
		return 0;
	}
	free(chk_day);

	char *alloc_todt_buff=NULL,*string_var="%w";

	if(strcmp(alloc_type,"14 Days Allocation")==0)
	{
		if(strcmp(sel_chk_batch,"B1")==0)
		{
			char *ck_frmdt=NULL;
			sprintf(query_fromdt,"select (case when '%s'>(select date('now','weekday 0')) then 'OK' else 'NOK' end)",buf_dt_alloc);
			printf("query of case %s\n",query_fromdt);
			ck_frmdt = exec_query_get_single_value(ck_frmdt,query_fromdt, DB_PACK);
			if(strcmp(ck_frmdt,"NOK")==0)
			{
				show_message_frame("Alloc From_date>= Monday");
				pw_set_text(lbl_aloc_frmdt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			free(ck_frmdt);

			sprintf(query_fromdt,"select (case when '%s'<=(select date('now','weekday 0','+14 days')) then 'OK' else 'NOK' end)",buf_dt_alloc);
			printf("query of case %s\n",query_fromdt);
			ck_frmdt = exec_query_get_single_value(ck_frmdt,query_fromdt, DB_PACK);

			if(strcmp(ck_frmdt,"NOK")==0)
			{
				show_message_frame("ALLOC FROMDATE > 14days");
				pw_set_text(lbl_aloc_frmdt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			free(ck_frmdt);

			sprintf(query_fromdt,"SELECT date( julianday(date('%s'))+(%d-1))",buf_dt_alloc,days);
			alloc_todt_buff = exec_query_get_single_value(alloc_todt_buff,query_fromdt, DB_PACK);
			printf("demanded to date1 is %s\n",alloc_todt_buff);

			char *chk_todt1=NULL;
			sprintf(query_fromdt,"select (case when '%s'<=(select date('now','weekday 0','+14 days')) then 'OK' else 'NOK' end)",alloc_todt_buff);
			printf("query of case %s\n",query_fromdt);
			chk_todt1 = exec_query_get_single_value(chk_todt1,query_fromdt, DB_PACK);

			if(strcmp(chk_todt1,"NOK")==0)
			{
				show_message_frame("To Date Not  Within\n Batch-I Period");
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			free(chk_todt1);

			sprintf(query_fromdt,"select strftime('%s','%s')",string_var,buf_dt_alloc);
			day_test =exec_query_get_single_value(day_test,query_fromdt,DB_PACK);
			sprintf(sel_day_test, "%s", day_test);
			free(day_test);
			printf("sel_day_test is %s\n",sel_day_test);

			if((atoi(pw_get_text(entry_aloc_days))==14)&&!(atoi(sel_day_test)==1))
			{
				show_message_frame("Should be Monday Date");
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}

		}
		else
		{
			char *ck_frmdt=NULL;
			sprintf(query_fromdt,"select (case when '%s'>(select date('now','weekday 3')) then 'OK' else 'NOK' end)",buf_dt_alloc);
			printf("query of case %s\n",query_fromdt);
			ck_frmdt = exec_query_get_single_value(ck_frmdt,query_fromdt, DB_PACK);
			if(strcmp(ck_frmdt,"NOK")==0)
			{
				show_message_frame("From_date>= Thuresday");
				pw_set_text(lbl_aloc_frmdt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			free(ck_frmdt);

			sprintf(query_fromdt,"select (case when '%s'<=(select date('now','weekday 3','+14 days')) then 'OK' else 'NOK' end)",buf_dt_alloc);
			printf("query of case %s\n",query_fromdt);
			ck_frmdt = exec_query_get_single_value(ck_frmdt,query_fromdt, DB_PACK);

			if(strcmp(ck_frmdt,"NOK")==0)
			{
				show_message_frame("ALLOC FROMDATE > 14days");
				pw_set_text(lbl_aloc_frmdt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			free(ck_frmdt);
		sprintf(query_fromdt,"select strftime('%s','%s')",string_var,buf_dt_alloc);
			day_test =exec_query_get_single_value(day_test,query_fromdt,DB_PACK);
			sprintf(sel_day_test, "%s", day_test);
			free(day_test);
			printf("sel_day_test is %s\n",sel_day_test);

			if((atoi(pw_get_text(entry_aloc_days))==14)&&!(atoi(sel_day_test)==4))
			{
				show_message_frame("Should be Thuresday Date");
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			if(((atoi(pw_get_text(entry_aloc_days))>=4)&&(atoi(sel_day_test)<=7)) ||((atoi(pw_get_text(entry_aloc_days))>=2)&&(atoi(sel_day_test)>=5)))
			{
				sprintf(query_fromdt,"SELECT date( julianday(date('%s'))+(%d-1))",buf_dt_alloc,days);
				alloc_todt_buff = exec_query_get_single_value(alloc_todt_buff,query_fromdt, DB_PACK);
				printf("demanded to datey is %s\n",alloc_todt_buff);
			}

			else
			{
				sprintf(query_fromdt,"SELECT date( julianday(date('%s'))+(%d-1))",buf_dt_alloc,days);
				alloc_todt_buff = exec_query_get_single_value(alloc_todt_buff,query_fromdt, DB_PACK);
				printf("demanded to date2 is %s\n",alloc_todt_buff);
			}
			char *chk_todt1=NULL;
			sprintf(query_fromdt,"select (case when '%s'<=(select date('now','weekday 3','+14 days')) then 'OK' else 'NOK' end)",alloc_todt_buff);
			printf("query of case %s\n",query_fromdt);
			chk_todt1 = exec_query_get_single_value(chk_todt1,query_fromdt, DB_PACK);

			if(strcmp(chk_todt1,"NOK")==0)
			{
				show_message_frame("To Date Not  Within\n Batch-2 Period");
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			free(chk_todt1);
		}//else close for batch2
	}//else close for 14-days
	else
	{
		if(strcmp(sel_chk_batch,"B1")==0)
		{
			if((atoi(wday)==6)||(atoi(wday)==0))
			{
				char *ck_frmdt=NULL;
				sprintf(query_fromdt,"select (case when '%s'>(select date('now','+7 days')) then 'NOK' else 'OK' end)",buf_dt_alloc);
				printf("query of case %s\n",query_fromdt);
				ck_frmdt = exec_query_get_single_value(ck_frmdt,query_fromdt, DB_PACK);

				if(strcmp(ck_frmdt,"NOK")==0)
				{
					show_message_frame("ALLOC FROMDATE > 7days");
					pw_set_text(lbl_aloc_frmdt,"");
					buf_dt_alloc[0]='\0';
					return 0;
				}
				free(ck_frmdt);
				char *chk_todt1=NULL;
				sprintf(query_fromdt,"select (case when '%s'<=(select date('now','weekday 6','+1 day')) then 'OK' else 'NOK' end)",alloc_todt_buff);
				printf("query of case %s\n",query_fromdt);
				chk_todt1 = exec_query_get_single_value(chk_todt1,query_fromdt, DB_PACK);

				if(strcmp(chk_todt1,"NOK")==0)
				{
					show_message_frame("To Date  Not \n Within Batch-I Period");
					pw_set_text(lbl_aloc_frmdt,"");
					pw_set_text(lbl_aloc_todt,"");
					buf_dt_alloc[0]='\0';
					return 0;
				}
				free(chk_todt1);

			}                       
			else
			{
				char *chk_todt=NULL;
				sprintf(query_fromdt,"select (case when '%s'<(select date('now','weekday 6','+2 days'))  or '%s'>(select date('now','weekday 6','+7 days')) then 'NOK' else 'OK' end)",buf_dt_alloc,buf_dt_alloc);
				printf("query of case %s\n",query_fromdt);
				chk_todt = exec_query_get_single_value(chk_todt,query_fromdt, DB_PACK);

				if(strcmp(chk_todt,"NOK")==0)
				{
					show_message_frame("Alloc FromDate NOT Allowed");
					pw_set_text(lbl_aloc_frmdt,"");
					buf_dt_alloc[0]='\0';
					return 0;
				}
				free(chk_todt);

				sprintf(query_fromdt,"SELECT date( julianday(date('%s'))+(%d-1))",buf_dt_alloc,days);
				alloc_todt_buff = exec_query_get_single_value(alloc_todt_buff,query_fromdt, DB_PACK);
				printf("demanded to date1 is %s\n",alloc_todt_buff);

				char *chk_todt2=NULL;
				sprintf(query_fromdt,"select (case when '%s'<=(select date('%s','weekday 6','+1 day')) then 'OK' else 'NOK' end)",alloc_todt_buff,buf_dt_alloc);
				printf("query of case %s\n",query_fromdt);
				chk_todt2 = exec_query_get_single_value(chk_todt2,query_fromdt, DB_PACK);

				if(strcmp(chk_todt2,"NOK")==0)
				{
					show_message_frame("To Date  Not \n Within Batch-I Period");
					pw_set_text(lbl_aloc_frmdt,"");
					pw_set_text(lbl_aloc_todt,"");
					buf_dt_alloc[0]='\0';
					return 0;
				}
				free(chk_todt2);
			}

			if(atoi(pw_get_text(entry_aloc_days))>7) 
			{
				pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
				show_message_frame("No of Days Exceeded"); 
				pw_entry_clear(entry_aloc_days);
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				return 0;
			}
			sprintf(query_fromdt,"select strftime('%s','%s')",string_var,buf_dt_alloc);
			day_test =exec_query_get_single_value(day_test,query_fromdt,DB_PACK);
			sprintf(sel_day_test, "%s", day_test);
			free(day_test);
			printf("sel_day_test is %s\n",sel_day_test);

			if((atoi(pw_get_text(entry_aloc_days))==7)&&!(atoi(sel_day_test)==1))
			{
				show_message_frame("Should be Monday Date");
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				buf_dt_alloc[0]='\0';
				return 0;               
			}
			else
				sprintf(query_fromdt,"SELECT date( julianday(date('%s'))+(%d-1))",buf_dt_alloc,days);
			alloc_todt_buff = exec_query_get_single_value(alloc_todt_buff,query_fromdt, DB_PACK);
			printf("demanded to date1 is %s\n",alloc_todt_buff);
			char *chk_todt=NULL;
			sprintf(query_fromdt,"select (case when '%s'<=(select date('%s','weekday 6','+1 day')) then 'OK' else 'NOK' end)",alloc_todt_buff,buf_dt_alloc);
			printf("query of case %s\n",query_fromdt);
			chk_todt = exec_query_get_single_value(chk_todt,query_fromdt, DB_PACK);

			if(strcmp(chk_todt,"NOK")==0)
			{
				show_message_frame("To Date  Not \n Within Batch-I Period");
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			free(chk_todt);

		}                                             //Condition for Thursdays 
		else
		{
			if(atoi(pw_get_text(entry_aloc_days))>7)
			{
				pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
				show_message_frame("No of Days Exceeded");
				pw_entry_clear(entry_aloc_days);
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				return 0;
			}
			char *ck_frmdt=NULL,*chk_Tfrmdt=NULL;

			sprintf(query_fromdt,"SELECT date((julianday('now')+1),'weekday 4')");

			chk_Tfrmdt = exec_query_get_single_value(chk_Tfrmdt,query_fromdt,DB_PACK);

			sprintf(query_fromdt,"select (case when '%s'>='%s' then 'OK' else 'NOK' end)",buf_dt_alloc,chk_Tfrmdt);
			printf("query of thursaday's %s\n",query_fromdt);
			ck_frmdt = exec_query_get_single_value(ck_frmdt,query_fromdt, DB_PACK);

			if(strcmp(ck_frmdt,"NOK")==0)
			{
				show_message_frame(" From Date Should be \n Within Batch-2 Period");
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}

			char *ck_endfrmdt=NULL;
			sprintf(query_fromdt,"select (case when '%s'<=(select date('%s','6 day')) then 'OK' else 'NOK' end)",buf_dt_alloc,chk_Tfrmdt);
			printf("query of case %s\n",query_fromdt);
			ck_endfrmdt = exec_query_get_single_value(ck_endfrmdt,query_fromdt, DB_PACK);
			if(strcmp(ck_endfrmdt,"NOK")==0)
			{
				show_message_frame("ALLOC FROMDATE \n > Next Period");
				pw_set_text(lbl_aloc_frmdt,"");
				buf_dt_alloc[0]='\0';
				return 0;
			}
			free(ck_endfrmdt);
			free(ck_frmdt);
			sprintf(query_fromdt,"select strftime('%s','%s')",string_var,buf_dt_alloc);

			day_test =exec_query_get_single_value(day_test,query_fromdt,DB_PACK);
			sprintf(sel_day_test, "%s", day_test);
			free(day_test);
			printf("sel_day_test is %s\n",sel_day_test);

			if((atoi(pw_get_text(entry_aloc_days))==7)&&!(atoi(sel_day_test)==4))
			{
				show_message_frame("Should be Thursday's Date");
				pw_set_text(lbl_aloc_frmdt,"");
				pw_set_text(lbl_aloc_todt,"");
				buf_dt_alloc[0]='\0';
				return 0;

			}
			else
			{
				if(((atoi(pw_get_text(entry_aloc_days))>=4)&&(atoi(sel_day_test)<=7)) ||((atoi(pw_get_text(entry_aloc_days))>=2)&&(atoi(sel_day_test)>=5)))
				{
					sprintf(query_fromdt,"SELECT date( julianday(date('%s'))+(%d-1))",buf_dt_alloc,days);
					alloc_todt_buff = exec_query_get_single_value(alloc_todt_buff,query_fromdt, DB_PACK);
					printf("demanded to datey is %s\n",alloc_todt_buff);
				}

				else
				{
					sprintf(query_fromdt,"SELECT date( julianday(date('%s'))+(%d-1))",buf_dt_alloc,days);
					alloc_todt_buff = exec_query_get_single_value(alloc_todt_buff,query_fromdt, DB_PACK);
					printf("demanded to date2 is %s\n",alloc_todt_buff);
				}
				char *chk_todt1=NULL;
				sprintf(query_fromdt,"select (case when '%s'<=(select date((julianday('now')+6),'weekday 3')) then 'OK' else 'NOK' end)",alloc_todt_buff);
				printf("query of case %s\n",query_fromdt);
				chk_todt1 = exec_query_get_single_value(chk_todt1,query_fromdt, DB_PACK);

				if(strcmp(chk_todt1,"NOK")==0)
				{
					show_message_frame("To Date  Not \n Within Batch-2 Period");
					pw_set_text(lbl_aloc_frmdt,"");

					pw_set_text(lbl_aloc_todt,"");
					buf_dt_alloc[0]='\0';
					return 0;
				}
				free(chk_todt1);


			}
		}
	}
	char *lbl_todt=NULL;
	sel_wrkaloc_todt[0]='\0';
	strcpy(alloc_todtchk,alloc_todt_buff);
	sprintf(query_fromdt,"select strftime('%s','%s')",date_format,alloc_todt_buff);
	lbl_todt = exec_query_get_single_value(lbl_todt,query_fromdt, DB_PACK);
	sprintf(sel_wrkaloc_todt,"%s",alloc_todt_buff);
	printf("sel_wrkaloc_todt is %s\n",sel_wrkaloc_todt);
	pw_set_text(lbl_aloc_todt,lbl_todt);
	free(lbl_todt);

	return 0;
}

void combo_aloc_wrkcodchange(pw_widget widget, int index)
{
	char *work_code=NULL;
	unsigned idx;
	pw_combobox_get_selection(combo_aloc_wkcod,&work_code,&idx);
	strcpy(sel_wrk_cod,work_code);
	free(work_code);
	char *mandays_rem=NULL,*cnt_mandays_rem=NULL,*estm_days=NULL,query[512];

	sprintf(query,"select ifnull(count(*),'0') from remaining_mandays_details where work_code='%s'",sel_wrk_cod);
	cnt_mandays_rem = exec_query_get_single_value(cnt_mandays_rem, query, DB_PACK);

	if(atoi(cnt_mandays_rem)==0)
	{
		sprintf(query,"select distinct ifnull(estimated_days,'0') from Work_Registration where work_code='%s'",sel_wrk_cod);
		estm_days = exec_query_get_single_value(estm_days, query, DB_PACK);
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
			printf("Unable to open db\n");
		sprintf(query,"insert into remaining_mandays_details(work_code,tot_mandays,spent_mandays,balance_mandays) values('%s','%s','%s','%s')",sel_wrk_cod,estm_days,"0",estm_days);
		sqlite3_exec(dbhandle, query,NULL,NULL,NULL);
		sqlite3_close(dbhandle);
		pw_set_text(lbl_aloc_grps[5],estm_days);

		sprintf(rem_mandays,"%s",estm_days);
	}
	else
	{
		sprintf(query,"select distinct balance_mandays from remaining_mandays_details where work_code='%s'",sel_wrk_cod);
		mandays_rem = exec_query_get_single_value(mandays_rem, query, DB_PACK);
		pw_set_text(lbl_aloc_grps[5],mandays_rem);	
		sprintf(rem_mandays,"%s",mandays_rem);
	}
	free(cnt_mandays_rem);
	free(estm_days);
	free(mandays_rem);
	view_btn_chk();
	return ;
}
pw_widget txt_entry_srchwrk_id,Butt_allocwrkid_search;
char sel_habit_code[10],sel_wrk_type_name[150],sel_wrk_type_code[10];
void get_search_alocwrkid()
{
	char query[1024],*count=NULL,srchid[10];
	strcpy(srchid,pw_get_text(txt_entry_srchwrk_id));
	sprintf(query,"select count(*) from Work_Registration where work_type_name='%s' and work_type_code='%s' and description in('Sanctioned-Startup','In-Progress')  and work_code like '%%%s%%'",sel_wrk_type_name,sel_wrk_type_code,srchid);
	count=exec_query_get_single_value(count,query, DB_PACK);
	if(atoi(count)==0)
	{
		show_message_frame("NO DATA");
	}
	else
	{
		sprintf(query,"select DISTINCT work_code from Work_Registration where work_type_name='%s' and work_type_code='%s' and description in('Sanctioned-Startup','In-Progress')  and work_code like '%%%s%%'",sel_wrk_type_name,sel_wrk_type_code,pw_get_text(txt_entry_srchwrk_id));
		pw_combobox_clear(combo_aloc_wkcod);
		exec_query_load_into_combo(combo_aloc_wkcod, query, DB_PACK);
	}
}
int work_codes()
{
	char query[1024];
	//sprintf(query,"select distinct work_code from Work_Registration where work_type_name='%s' and work_type_code='%s' and description in('Sanctioned-Startup','In-Progress') and habitation_code='%s'",sel_wrk_type_name,sel_wrk_type_code,sel_habit_code);
	sprintf(query,"select distinct work_code from Work_Registration where work_type_name='%s' and work_type_code='%s' and description in('Sanctioned-Startup','In-Progress') ",sel_wrk_type_name,sel_wrk_type_code);
	pw_combobox_clear(combo_aloc_wkcod);
	pw_set_text(combo_aloc_wkcod,"--Select--");
	exec_query_load_into_combo(combo_aloc_wkcod, query, DB_PACK);
	return 0;
}
void cb_wrktypebtn_relese()
{
	show_btn_frame(sel_wrk_type_name);
	return ;
}
void combo_aloc_wrktypechange(pw_widget widget, int index)
{
	char *work_type=NULL;
	unsigned idx;
	pw_combobox_get_selection(combo_aloc_wktype,&work_type,&idx);
	strcpy(sel_wrk_type_name,work_type);
	free(work_type);
	char *work_type_code=NULL,query[1024];
	sel_wrk_type_code[0]='\0';
	//sprintf(query,"select distinct work_type_code from work_registration where work_type_name='%s' and habitation_code='%s'",sel_wrk_type_name,sel_habit_code);
	sprintf(query,"select distinct work_type_code from work_registration where work_type_name='%s'",sel_wrk_type_name);
	work_type_code = exec_query_get_single_value(work_type_code,query, DB_PACK);
	sprintf(sel_wrk_type_code,"%s",work_type_code);
	free(work_type_code);
	pw_set_text(lbl_wrktype_nam,sel_wrk_type_name);
	work_codes();

}

void create_demand_work_aloc_frame()
{
	demand_work_aloc_frame =  pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	lbl_demanded_getgrp_aloc[0] = pw_create_label(demand_work_aloc_frame,00, 00,240,25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);
	lbl_demanded_getgrp_aloc[2]= pw_create_label(demand_work_aloc_frame,00, 28, 50,25,
			PW_TEXT, "Work Type",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	combo_aloc_wktype =  pw_create_combobox(demand_work_aloc_frame, 50, 28, 185,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_aloc_wktype,combo_aloc_wrktypechange);

	lbl_wrktype_nam =  pw_create_button(demand_work_aloc_frame, 00, 53, 240, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_wrktypebtn_relese,
			PW_CONFIG_END);

	lbl_demanded_grp_aloc[0] =  pw_create_label(demand_work_aloc_frame, 0, 84, 85, 20,
			PW_TEXT, "Wrk code:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, BLUE ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	txt_entry_srchwrk_id =  pw_create_entry(demand_work_aloc_frame, 85, 84, 60, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	pw_focus(txt_entry_srchwrk_id);

	Butt_allocwrkid_search = pw_create_button(demand_work_aloc_frame, 160, 84,76, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SEARCH,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_search_alocwrkid,
			PW_CONFIG_END);
	lbl_demanded_grp_aloc[0]= pw_create_label(demand_work_aloc_frame ,00, 108, 50,25,
			PW_TEXT, "Work Code",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	combo_aloc_wkcod =  pw_create_combobox(demand_work_aloc_frame, 50, 107, 185,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_aloc_wkcod,combo_aloc_wrkcodchange);

	lbl_demanded_grp_aloc[0]= pw_create_label(demand_work_aloc_frame, 00, 133, 140,15,
			PW_TEXT, "Person days(rem):",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbl_aloc_grps[5]= pw_create_label(demand_work_aloc_frame, 140, 133, 50,15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_demanded_grp_aloc[1] = pw_create_label(demand_work_aloc_frame,00, 150,125,20,
			PW_TEXT, "No. of Days:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	entry_aloc_days = pw_create_entry(demand_work_aloc_frame,125, 150, 30, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	pw_focus(entry_aloc_days);

	lbl_demanded_grp_aloc[2] = pw_create_label(demand_work_aloc_frame,10, 172,80,15,
			PW_TEXT, "From Date",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, BLUE,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_aloc_frmdt = pw_create_label(demand_work_aloc_frame, 10, 187, 85, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	lbl_demanded_grp_aloc[2] = pw_create_label(demand_work_aloc_frame,130, 172,80,15,
			PW_TEXT, "To    Date:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, BLUE,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_aloc_todt = pw_create_label(demand_work_aloc_frame,130,187,85,20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	Butt_aloc_addgrp = pw_create_button(demand_work_aloc_frame, 140, 230, 80, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Work Allocate",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_allocated_grp_details,
			PW_CONFIG_END);

	Butt_aloc_view = pw_create_button(demand_work_aloc_frame, 60, 230, 60, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "View",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_allocated_grp_viewdetails,
			PW_CONFIG_END);

	Butt_aloc_grpsdetail = pw_create_button(demand_work_aloc_frame, 16, 230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_alocgrps_detail_back,
			PW_CONFIG_END);

}
void show_demand_work_aloc_frame()
{
	if(!demand_work_aloc_frame){
		create_demand_work_aloc_frame();
		pw_show(demand_work_aloc_frame);
	}
	else
		pw_show(demand_work_aloc_frame);
	allocate_info();
	printf("group name is %s\n",sel_alloc_grp_name);
	pw_set_text(lbl_demanded_getgrp_aloc[0],"");
	pw_set_text(lbl_wrktype_nam,"");
	pw_combobox_clear(combo_aloc_wkcod);
	pw_set_text(lbl_aloc_grps[5],"");
	pw_set_text(lbl_demanded_getgrp_aloc[0],sel_alloc_grp_name);
	char query[1024];
	if(strcmp(sel_aloc_grp_type,"DISABLED")==0)
	{
		pw_combobox_clear(combo_aloc_wktype);

		sprintf(query,"select distinct work_type_name from Work_Registration where work_type_code not in('217','218','220','223','312','302','314','284','243','247','251','311','375','255','294','362') and description in('Sanctioned-Startup','In-Progress') and  work_strategy in('P','Y') or work_type_code not in('03','030','032','08','09','141','16','18','40','55','069','071','205','206','049') and description='Sanctioned-Startup' and work_strategy='N' or description in('Sanctioned-Startup','In-Progress') and work_strategy='N' and work_type_code not in('038','72') or  work_strategy in('P','Y') and project_id in('12','66') and work_type_code in('311','375') ");

		exec_query_load_into_combo(combo_aloc_wktype,query,DB_PACK);
	}

	if((strcmp(sel_aloc_grp_type,"GENERAL")==0)||(strcmp(sel_aloc_grp_type,"SSS-T")==0)||(strcmp(sel_aloc_grp_type,"SSS-VA")==0))
	{
		pw_combobox_clear(combo_aloc_wktype);
		sprintf(query,"select distinct work_type_name from Work_Registration where work_type_code not in('217','218','220','223','312','302','314','284','243','247','251','255','294','068','295','311','375','353','362') and description in('Sanctioned-Startup','In-Progress') and  work_strategy in('P','Y') or work_type_code not in('03','030','032','08','09','141','16','18','40','55','069','071','205','206','049') and description='Sanctioned-Startup' and work_strategy='N' or work_type_code not in('038','72') and description in('Sanctioned-Startup','In-Progress') and work_strategy='N' or work_type_code in('311','375') and description in('Sanctioned-Startup','In-Progress') and  work_strategy in('P','Y') and project_id='12'");

		exec_query_load_into_combo(combo_aloc_wktype,query,DB_PACK);
	}

}
void hide_demand_work_aloc_frame()
{
	pw_hide(demand_work_aloc_frame);
}
void get_7days_allocation()
{
	alloc_type[0]='\0';
	strcpy(alloc_type,pw_get_text(Butt_aloc_selection_period[1]));
	printf("alloc_type is %s\n",alloc_type);
	//pw_hide(allcation_selection_period);
	hide_allcation_selection_period();
	show_demand_work_aloc_frame();
}
void get_14days_allocation()
{
	alloc_type[0]='\0';
	strcpy(alloc_type,pw_get_text(Butt_aloc_selection_period[2]));
	
	printf("alloc_type is %s\n",alloc_type);
	hide_allcation_selection_period();
	show_demand_work_aloc_frame();
}
void get_aloc_selection_period_back()
{
	pw_hide(allcation_selection_period);
	pw_show(demand_aloc_frame);
}
void create_allcation_selection_period()
{
	allcation_selection_period=pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	pw_hide(allcation_selection_period);
	lbl_allocation_selection_period[0]= pw_create_label(allcation_selection_period,00, 00,240,25,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);
	lbl_allocation_selection_period[1]= pw_create_label(allcation_selection_period,00, 27, 200,20,             
			PW_TEXT, "Total Registered W.S.:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_allocation_selection_period[2]= pw_create_label(allcation_selection_period,200, 27, 50,20,                                                        PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"GREEN",
			PW_CONFIG_END);

	lbl_allocation_selection_period[3]= pw_create_label(allcation_selection_period,00, 47, 240,20,                                                        PW_TEXT, "Pending Allocations",
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"RED",
			PW_CONFIG_END);
	lbl_allocation_selection_period[4]= pw_create_label(allcation_selection_period,00, 67, 140,20, 
			PW_TEXT, "Demand Periods",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_allocation_selection_period[5]= pw_create_label(allcation_selection_period,140, 67, 50,20,  
			PW_TEXT, "P.days",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	lbl_allocation_selection_period[6]= pw_create_label(allcation_selection_period,190,67, 50,20,
			PW_TEXT, "No.W.S.",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	lbox_alocated_selection_period = pw_create_tlistbox(allcation_selection_period,0,87,240,113,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_alocated_selection_period,35);
	pw_tlistbox_addcolumn(lbox_alocated_selection_period,35);
	pw_update(lbox_alocated_selection_period,1);
	Butt_aloc_selection_period[0] = pw_create_button(allcation_selection_period,10, 220, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_aloc_selection_period_back,
			PW_CONFIG_END);
	Butt_aloc_selection_period[1]= pw_create_button(allcation_selection_period,50, 220, 73, 35,
			PW_TEXT, "7 Days Allocation",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_7days_allocation,
			PW_CONFIG_END);
	Butt_aloc_selection_period[2]= pw_create_button(allcation_selection_period,145, 220, 73, 35,
			PW_TEXT, "14 Days Allocation",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_14days_allocation,
			PW_CONFIG_END);

}

void show_allcation_selection_period()
{
	show_progress_frame("PLEASE WAIT....");

	if(!allcation_selection_period){
		create_allcation_selection_period();
		pw_show(allcation_selection_period);
	}
	else
		pw_show(allcation_selection_period);
	pw_enable(Butt_aloc_selection_period[2]);
	hide_progress_frame();
	pw_set_text(lbl_allocation_selection_period[0],sel_alloc_grp_name);
	char query[1024],*count=NULL;
	sprintf(query,"select count(distinct(household_code||worker_code)) from household_details where group_name='%s' and  is_active='Y'",sel_alloc_grp_name);
	count = exec_query_get_single_value(count, query, DB_PACK);
	pw_set_text(lbl_allocation_selection_period[2],count);
	free(count);
	sprintf(query,"select count(*) from tmp_demand_group_allocate where allocated_days between 1 and 7");	
	  count = exec_query_get_single_value(count, query, DB_PACK);
	if(atoi(count)>0)
	{
		pw_disable(Butt_aloc_selection_period[2]);
	}
	free(count);
	pw_tlistbox_clear(lbox_alocated_selection_period);
	sprintf(query,"select substr(demand_frmdt,9,2)||substr(demand_frmdt,5,4)||substr(demand_frmdt,3,2)||'/'||substr(demand_todt,9,2)||substr(demand_todt,5,4)||substr(demand_todt,3,2),(case when allocated_days=0 then '14' else '7' end),count(*) from tmp_demand_group_allocate group by demand_frmdt,demand_todt");
	exec_query_load_into_lbox_trriple(lbox_alocated_selection_period, query, DB_PACK);
	pw_update(lbox_alocated_selection_period, 1);

}
void hide_allcation_selection_period()
{
	pw_hide(allcation_selection_period);
}
int get_aloc_selgrp()
{
	if(strcmp(sel_habit_code,"")==0)
	{
		show_message_frame("SELECT HABITATION");
		return 0;
	}
	if(strcmp(sel_aloc_grp_type,"")==0)
	{
		show_message_frame("SELECT SSS-TYPE");
		return 0;
	}


	if(strcmp(sel_alloc_grp_name,"")==0)
	{
		show_message_frame("SELECT GROUPNAME");
		return 0;
	}
	char *grp_chk=NULL;
	char *jc_cnt=NULL,query[1024];
		char *check=NULL;
	sprintf(query,"select  distinct ifnull(Batch_type,'0') from Batch_info");
	check = exec_query_get_single_value(check,query, DB_PACK);
	sprintf(sel_chk_batch, "%s", check);
	if(strcmp(sel_chk_batch,"B1")==0)
	sprintf(query,"select ifnull(count(*),'0') from work_allocation_details where group_name='%s' and status in('N','Y') and substr(allocation_date,1,10) between (select date('now','weekday 0','-3 days')) and (select date('now','weekday 0'))",sel_alloc_grp_name);
	else
	sprintf(query,"select ifnull(count(*),'0') from work_allocation_details where group_name='%s' and status in('N','Y') and substr(allocation_date,1,10) between (select date('now','weekday 3','-4 days')) and (select date('now','weekday 3'))",sel_alloc_grp_name);
	grp_chk =exec_query_get_single_value(grp_chk, query, DB_PACK);
	if(atoi(grp_chk)>0)
	{
		show_message_frame("Already Allocated in this week");
		sel_alloc_grp_name[0]='\0';
		pw_tlistbox_remove_selection_all(lbox_alocated_grps);
		return 0;
	}
	free(grp_chk);
			char *grp_id_alloc_buff=NULL;
	grp_id_alloc[0]='\0';
	sprintf(query,"select distinct group_id from household_details where group_name='%s'",sel_alloc_grp_name);
	grp_id_alloc_buff = exec_query_get_single_value(grp_id_alloc_buff, query, DB_PACK);       
	strcpy(grp_id_alloc,grp_id_alloc_buff);
	free(grp_id_alloc_buff);
	sqlite3 *dbhandle=NULL;
        sqlite3_open(DB_PACK, &dbhandle);
        if (dbhandle == NULL)
                printf("Unable to open db\n");
        sprintf(query,"delete from tmp_demand_group_allocate");
        sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"insert into tmp_demand_group_allocate select household_code||worker_code,substr(demand_fromdate,1,10),substr(demand_todate,1,10),allocated_days from mcc_demand_group_details where group_id='%s' and allocated_days between 0 and 7",grp_id_alloc);
	 sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"insert into tmp_demand_group_allocate select Householdcode,fromdate,todate,'%s' from Demand_Group_details where group_id='%s' and status='Y' and (application_date=(select date('now','weekday 0','-3 days')) or application_date=(select date('now','weekday 3','-4 days')))","0",grp_id_alloc);
         sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
        sqlite3_close(dbhandle);
		del_temp_alloc();
	free(jc_cnt);
	hide_demand_aloc_frame();
	show_allcation_selection_period();
	return 0;
}
void cb_grp_alloc_lbox_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
	return;
}

int demand_groups()
{
	char query[1024];
	pw_tlistbox_clear(lbox_alocated_grps);
//	sprintf(query,"select  group_name,12*count(*) from Demand_Group_details where group_type ='%s' and status='Y' and allocated_status in ('N','P')  group by group_name",sel_aloc_grp_type);
	sprintf(query,"select group_name from (select distinct group_name from  Demand_Group_details where status='Y' and group_type ='%s' and (application_date=(select date('now','weekday 0','-3 days')) or application_date=(select date('now','weekday 3','-4 days'))) union select distinct group_name from household_details where group_id IN(select distinct group_id from mcc_demand_Group_details where allocated_days between 0 and 7) and group_type ='%s')",sel_aloc_grp_type,sel_aloc_grp_type);
	exec_query_load_into_lbox_single(lbox_alocated_grps, query, DB_PACK);
	pw_update(lbox_alocated_grps, 1);
	return 0;
}
int cb_grp_alloc_lbox_press()
{
	char *str=NULL;
	int idx = pw_tlistbox_get_selection(lbox_alocated_grps);
	pw_tlistbox_get_element(lbox_alocated_grps, &str,idx, 0);
	txtlb_get_element(lbox_alocated_grps, &str, idx, 0);
	if(idx==-1)
	{
		return -1;
	}
	show_text_frame(str);
	strcpy(sel_alloc_grp_name,str);
	free(str);
/*	printf("before");	
	char query[1024];
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	sprintf(query,"update Demand_Group_details set allocated_status='C' where todate<=(select ifnull(max(substr(demand_todate,1,10)),0) from mcc_demand_group_details where group_id=(select distinct group_id from household_details where group_name='%s') and allocated_days>7) and group_name='%s'",sel_alloc_grp_name,sel_alloc_grp_name);	
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("update query is %s\n",query);
	//	demand_groups();
	sqlite3_close(dbhandle);
	printf("after");	*/

	return 0;
}



void combo_aloc_grptypechange(pw_widget widget, int index)
{
	char *aloc_grp_type=NULL;
	unsigned idx;
	pw_combobox_get_selection(combo_aloc_grptype,&aloc_grp_type,&idx);
	strcpy(sel_aloc_grp_type,aloc_grp_type);
	free(aloc_grp_type);
	pw_tlistbox_clear(lbox_alocated_grps);
	demand_groups();

}
char sel_aloc_habit_code[20];
void combo_aloc_habchange(pw_widget widget, int index)
{
	char *aloc_habitnam=NULL,query[512];
	unsigned idx;
	pw_combobox_get_selection(combo_aloc_hab,&aloc_habitnam,&idx);
	strcpy(sel_aloc_habit_code,aloc_habitnam);
	free(aloc_habitnam);
	strncpy(sel_habit_code,sel_aloc_habit_code,5);
	printf("habit code is %s\n",sel_habit_code);
	pw_combobox_clear(combo_aloc_grptype);

	sprintf(query,"select distinct group_type from household_details where group_type in ('GENERAL','SSS-T','DISABLED')");
	exec_query_load_into_combo(combo_aloc_grptype, query, DB_PACK);
	char *st_check1=NULL;
	sprintf(query,"select Batch_type from Batch_info");
	st_check1=exec_query_get_single_value(st_check1,query,DB_PACK);
	strcpy(st_check,st_check1);
	free(st_check1);
}
int aloc_hab()
{
	char query[125];
	pw_combobox_clear(combo_aloc_grptype);
	pw_set_text(combo_aloc_hab,"--Select--");
	sprintf(query,"select distinct Habitation_Code||'-'||habitation_name from tmp_Habitation_Master ");
	exec_query_load_into_combo(combo_aloc_hab, query, DB_PACK);

	return 0;
}
void get_alocgrps_back()
{
	sel_habit_code[0]='\0';
	sel_alloc_grp_name[0]='\0';
	sel_aloc_grp_type[0]='\0';
	sel_wrk_type_name[0]='\0';
	sel_wrk_cod[0]='\0';		
	pw_combobox_clear(combo_aloc_hab);
	pw_combobox_clear(combo_aloc_grptype);
	pw_tlistbox_clear(lbox_alocated_grps);	
	hide_demand_aloc_frame();
	show_demand_options();
}
void create_demand_aloc_frame()
{
	demand_aloc_frame = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE ,
			PW_CONFIG_END);
	pw_hide(demand_aloc_frame);
	lbl_aloc_grps[0]= pw_create_label(demand_aloc_frame,00, 00, 85,20,
			PW_TEXT, "Habitation",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	combo_aloc_hab = pw_create_combobox(demand_aloc_frame, 85, 00, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_aloc_hab,combo_aloc_habchange);

	lbl_aloc_grps[1]= pw_create_label(demand_aloc_frame,00, 25, 80,20,
			PW_TEXT, "SSS-Type",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_aloc_grptype = pw_create_combobox(demand_aloc_frame, 80, 25, 155,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_aloc_grptype,combo_aloc_grptypechange);

	lbl_aloc_grps[3]= pw_create_label(demand_aloc_frame,50, 50, 100,15,
			PW_TEXT, "GroupNames",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
/*	lbl_aloc_grps[4]= pw_create_label(demand_aloc_frame,190, 50, 54,15,
			PW_TEXT, "Ddays",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,BLUE,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);*/
	lbox_alocated_grps = pw_create_tlistbox(demand_aloc_frame, 0, 65, 240, 145,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_grp_alloc_lbox_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_grp_alloc_lbox_press,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
//	pw_tlistbox_addcolumn(lbox_alocated_grps,30);
	pw_update(lbox_alocated_grps,1);
	pw_tlistbox_set_selection_type(lbox_alocated_grps,PW_TLISTBOX_SINGLE_SELECTION);
	Butt_aloc_grps[0] = pw_create_button(demand_aloc_frame, 10, 230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_alocgrps_back,
			PW_CONFIG_END);
	Butt_aloc_grps[1]= pw_create_button(demand_aloc_frame, 140, 230, 60, 27,
			PW_TEXT, "NEXT",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_aloc_selgrp,
			PW_CONFIG_END);
}
void show_demand_aloc_frame()
{
	show_progress_frame("PLEASE WAIT....");

	if(!demand_aloc_frame){
		create_demand_aloc_frame();
		pw_show(demand_aloc_frame);
	}
	else
		pw_show(demand_aloc_frame);
	aloc_hab();
	hide_progress_frame();
}
void hide_demand_aloc_frame()
{
	pw_show(demand_aloc_frame);
}
