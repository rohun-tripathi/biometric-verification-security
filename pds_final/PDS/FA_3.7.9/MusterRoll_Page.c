#include "types.h"
#include<string.h>
#include "string_list.h"
#include "functions.h"
#include<time.h>
#ifdef __arm__
#include<autils.h>
#include<ctype.h>
#endif

void hide_frame_get_muster();
void show_frame_get_muster();
void show_fa_module();
void show_progress_frame();
void hide_progress_frame();
void show_frame_normallogin_page();
char *exec_query_get_single_value(char *data, char *query, char *database);
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
int exec_query_load_into_lbox_single(pw_widget lbox_muster_att_grpdet , char *query, char *database);
int exec_query_load_into_lbox_double(pw_widget lbox_muster_att_grpdet , char *query, char *database);
void show_text_frame();
void hide_text_frame();
void txtlb_get_element(pw_widget w, char **s, int idx, int column);
void hide_frame_muster_id();
void show_frame_identify_must_atten();
int exec_query_load_fp_to_db(char *query, char *groupname);
void close_sqlite(sqlite3 *dbhandle);
int enroll_template_into_fps(int fps, unsigned char *template, int size);
void hide_frame_muster_attendance();
void hide_frame_tag_group();
void show_musterid_frame();
void show_frame_webcam_creation_ntenrll();
char query[1024],sel_groupname[150],group_id[16],muster_id[13],sel_notenrldjob_id[20]; 
char sel_from_date[11], sel_to_date[11];
pw_widget label_muster_id[5],label_muster[15],Butt_muster_mustr_normal;
char *get_work_code()
{
	return (char *)pw_get_text(label_muster_id[1]);
}
char *get_muster_id()
{
	return muster_id;
}

char *get_from_date()
{
	return sel_from_date;
}
char *get_to_date()
{
	return sel_to_date;
}
char *get_group_id()
{
	return group_id;
}
char *get_jobid_ntenrll()
{
	return sel_notenrldjob_id;
}
char *get_grp_name_ntenrll()
{
	return sel_groupname;
}

pw_widget frame_muster_attendance,label_muster_att_grpname,lbox_muster_att_grpdet,Butt_mus_atten,lbox_muster_wktag_grps,frame_get_muster,Butt_muster_all,label_muster_att_grpcnt;
pw_widget frame_muster_id,label_set_musterid,Butt_muster_id_submit,Butt_muster_id_grp,Butt_muster_id_back; 
void cb_mus_atten_back()
{
	hide_frame_muster_attendance();
	pw_show(frame_muster_id);
	return;
}
void lbox_grp_muster(pw_widget wdgt, int idx)
{
	char *str=NULL;
	pw_tlistbox_get_element(lbox_muster_att_grpdet,&str,idx,0);
	txtlb_get_element(lbox_muster_att_grpdet,&str,idx,0);
	pw_tlistbox_remove_selection_all(lbox_muster_att_grpdet);
	free(str);
	return;
}

void create_muster_attendance_frame(void)
{
	frame_muster_attendance= pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	pw_hide(frame_muster_attendance);
	label_muster_att_grpname =  pw_create_label(frame_muster_attendance, 3,05,236, 30,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET ,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	label_muster[14] =  pw_create_label(frame_muster_attendance, 0, 40, 160, 15,
			PW_TEXT, "JobCard Count(MCC):",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	label_muster_att_grpcnt =  pw_create_label(frame_muster_attendance, 160, 40, 80, 15,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,VIOLET ,
			PW_FGCOLOR, "Green",
			PW_CONFIG_END);

	lbox_muster_att_grpdet = pw_create_tlistbox(frame_muster_attendance, 0, 55, 240, 160,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	pw_tlistbox_addcolumn(lbox_muster_att_grpdet, 160);

	pw_tlistbox_selection_intimate(lbox_muster_att_grpdet,lbox_grp_muster);

	Butt_mus_atten = pw_create_button(frame_muster_attendance, 40, 230,34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_mus_atten_back,
			PW_CONFIG_END);
	return;
}

void show_frame_muster_attendance(void)
{
	if(!frame_muster_attendance)
	{
		create_muster_attendance_frame();
		pw_show(frame_muster_attendance);
	}
	else
		pw_show(frame_muster_attendance);
	pw_tlistbox_clear(lbox_muster_att_grpdet);
	char *tot_jc_cnt=NULL;
	//sprintf(query, "SELECT substr(a.household_code,15,6)||a.worker_code,b.name  FROM mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and substr(A.muster_id,7,6)='%s'", sel_groupname,muster_id);
	sprintf(query, "SELECT substr(a.household_code,15,6)||a.worker_code,b.name  FROM mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and A.muster_id='%s'", sel_groupname,muster_id);

	exec_query_load_into_lbox_double(lbox_muster_att_grpdet, query, DB_PACK);
	//sprintf(query, "SELECT count(*) FROM mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and substr(A.muster_id,7,6)='%s'", sel_groupname,muster_id);
	sprintf(query, "SELECT count(*) FROM mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and A.muster_id='%s'", sel_groupname,muster_id);
	tot_jc_cnt = exec_query_get_single_value(tot_jc_cnt, query, DB_PACK);
	pw_set_text(label_muster_att_grpname, sel_groupname );
	pw_set_text(label_muster_att_grpcnt, tot_jc_cnt);
	free(tot_jc_cnt);
	return;
}

void hide_frame_muster_attendance(void)
{
	pw_hide(frame_muster_attendance);
}

int insert_template_to_sensor(char *nrega_id, char *wname, char *groupname, sqlite3 *dbhandle, int fps)
{
	int i;
	unsigned char *template;
	char fname[256];
	unsigned long id;
	int imgFD=0, fsz=0, count=0;
	struct stat fst;
	for(i=1; i<=10; i++)
	{
		id = 0;
		fsz = 0;

		sprintf(fname, "/home/Backup/%s_%d.bir", nrega_id, i);

		printf("fname is %s\n",fname);
		imgFD = open(fname, O_RDONLY); //Needs clarification
		if(imgFD > 0)
		{
			printf("imgFD = %d \n", imgFD);
			fstat(imgFD, &fst);
			fsz = fst.st_size;

			if (fsz <= 0)

			{
				printf("Invalid Data \n");
			}

			else

			{
				template = (unsigned char *)malloc(sizeof(unsigned char)*(fsz+1));
				read(imgFD, template, fsz);
				id = enroll_template_into_fps(fps, template, fsz);
				free(template);
			}
			close(imgFD);
			printf("\n fname is %s size %d id = %lu \n", fname, fsz, id);

			if(id > 0)

			{
				count++;
				sprintf(query, "insert into wg_gpname_bio_info (sid, nrega_id, name, grname) values ('%lu', '%s', '%s', '%s')",
						id, nrega_id, wname, groupname);

				sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
				printf("\n insert query into wg_gpname is %s\n",query);
			}
		}
	}
	printf("Number of finger prints enrolled into sensor for nrega_id:%s is %d\n", nrega_id, count);
	return 1;
}

int exec_query_load_fp_to_db(char *query, char *groupname)
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

	//	system("date;sync");

	printf("\n No. of rows/JC is %d\n", nRows);
	show_progress_frame("Loading  into sensor ...");

	for(i=0; i<nRows; i++)

	{
		insert_template_to_sensor(qResults[(i+(i+2))], qResults[(i+(i+3))], groupname, dbhandle, fps);
	}

	sqlite3_free_table(qResults);
	hide_progress_frame();
	printf("ctrl in  -- 3 \n");
	//system("date;sync");
	close_sqlite(dbhandle);
	return 1;
}

int Load_fp_muster()
{
	char *grpname=NULL;
	pw_tlistbox_get_element(lbox_muster_wktag_grps , &grpname, pw_tlistbox_get_selection(lbox_muster_wktag_grps), 0);

	//Load only mccallocated jobcards from a group
	//sprintf(query, "SELECT a.household_code||a.worker_code,b.name  FROM mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and substr(A.muster_id,7,6)='%s' ", grpname,muster_id);
	sprintf(query, "SELECT a.household_code||a.worker_code,b.name  FROM mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and A.muster_id='%s' ", grpname,muster_id);

	printf("\n grpname is  %s\n", grpname);
	printf("\n guery in Load_fp_muster is  %s\n", query);
	exec_query_load_fp_to_db(query, grpname);
	printf("entering into load part");
	free(grpname);

	return 0;
}
int get_birs_del()
{
	show_progress_frame("Deleting FP from Sensor");
	char fp_delquery[512];
	int fps;
	sqlite3 *dbhandle;
	sqlite3_open(DB_PACK, &dbhandle);
	sprintf(fp_delquery,"delete from wg_gpname_bio_info");
	sqlite3_exec(dbhandle,fp_delquery, NULL, NULL, NULL);
	printf("fp_delquery is %s\n",fp_delquery);
	sprintf(fp_delquery,"delete from load_groups_details");
	sqlite3_exec(dbhandle, fp_delquery, NULL, NULL,NULL);

	//Deleting the old data.
	fps = init_fps_in_iso();
	remove_all_templates_from_fps(fps);
	bio_uninit(fps);
	sqlite3_close(dbhandle);	
	hide_progress_frame();
	return 0;
}
int load_birs(char *sel_groupname)
{
	char *load_cnt=NULL,*tot_bir_cnt=NULL;
	sprintf(query,"select ifnull(count(*),'0') from wg_gpname_bio_info");
	tot_bir_cnt =exec_query_get_single_value(tot_bir_cnt, query, DB_PACK);
	if(atoi(tot_bir_cnt)>=8500)
	{
		show_progress_frame("Refreshing Sensor ");
		get_birs_del();
		hide_progress_frame();
		return 0;
	}
	sprintf(query, "select ifnull(count(*),'0') from wg_gpname_bio_info where grname='%s'",sel_groupname);
	load_cnt = exec_query_get_single_value(load_cnt, query, DB_PACK);
	if(atoi(load_cnt)!=0)
	{
		pw_hide(frame_muster_id);
	}
	else
	{
		//Deleting the old data.
		fps = init_fps_in_iso();
		//	remove_all_templates_from_fps(fps);
		show_progress_frame("Loading to Sensor...");
		Load_fp_muster();
		pw_flush_eventloop();
		hide_progress_frame();
		uninit_fps(fps);
	}
	free(load_cnt);
	free(tot_bir_cnt);
	return 0;
}
void show_frame_not_enrolled();
pw_widget frame_not_enrold_photo,lbl_not_enrolled[3],lbox_not_enrolled_membs,butt_notenrld_photo_muster[5],lbox_identify_100_days1,Butt_photo_100_days_back1,frame_identify_100_execeed1,lbl_identity_100_1,label_identify_100_days1;

void cb_100_days_exceded_back1()
{
	pw_hide(frame_identify_100_execeed1);
	show_frame_not_enrolled();
}
void create_frame_identify_100_execeed1()
{
	frame_identify_100_execeed1 =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_identify_100_execeed1);

	lbl_identity_100_1 =  pw_create_label(frame_identify_100_execeed1 , 0, 05, 50, 25,
			PW_TEXT, "Group Name:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_identify_100_days1 =  pw_create_label(frame_identify_100_execeed1 , 50, 05, 189, 25,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbox_identify_100_days1 =pw_create_tlistbox(frame_identify_100_execeed1,0,35, 239, 180,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	pw_tlistbox_addcolumn(lbox_identify_100_days1, 150);

	Butt_photo_100_days_back1 = pw_create_button(frame_identify_100_execeed1, 0, 225, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_100_days_exceded_back1,
			PW_CONFIG_END);

	return;
}
void cb_identify_must_atten_100_exce1()
{
	char *cnt_100_days=NULL;	

	sprintf(query,"select ifnull(count(*),'0') from (select household_code as Jobcard_id,Name from household_details  where is_active='Y' and group_name='%s') as a where jobcard_id in(select jobcard_no from wage_info where no_days>='150')",sel_groupname);

	cnt_100_days=exec_query_get_single_value(cnt_100_days, query, DB_PACK);	

	if(atoi(cnt_100_days)==0)
	{
		show_message_frame("NO DATA");
		return ;
	}
	free(cnt_100_days);
	if(!frame_identify_100_execeed1)
	{
		create_frame_identify_100_execeed1();
		pw_show(frame_identify_100_execeed1);
	}
	else
		pw_show(frame_identify_100_execeed1);	

	pw_set_text(label_identify_100_days1 ,sel_groupname);	
	pw_tlistbox_clear(lbox_identify_100_days1);
	sprintf(query,"select JobCard_id as jobcard_id,name from (select substr(household_code,14,4) as Jobcard_id,Name from household_details  where is_active='Y' and group_name='%s') as a where jobcard_id in(select substr(jobcard_no,14,4) from wage_info where no_days>='150')",sel_groupname);
	printf("\n query is %s\n",query);

	exec_query_load_into_lbox_double(lbox_identify_100_days1, query, DB_PACK);

	pw_update(lbox_identify_100_days1,1);

	return ;
}
void bio_verify(pw_widget w, pw_event e)
{
	pw_hide(frame_not_enrold_photo);
	show_frame_identify_must_atten();
}
void cb_photo_notenrolled_back()
{
	pw_hide(frame_not_enrold_photo);
	show_musterid_frame();
}
void phot_cr_lbox_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
}
void  phot_cr_lbox_press()
{
	char *str=NULL;
	int idx = pw_tlistbox_get_selection(lbox_not_enrolled_membs);
	pw_tlistbox_get_element(lbox_not_enrolled_membs, &str,idx, 0);

	txtlb_get_element(lbox_not_enrolled_membs, &str, idx, 0);
	if(idx==-1)
	{
		return ;
	}
	show_text_frame(str);
	free(str);
	return ;
}

void lbox_seljcid_no()
{
	char *str1=NULL;
	int idx = pw_tlistbox_get_selection(lbox_not_enrolled_membs);
	pw_tlistbox_get_element(lbox_not_enrolled_membs, &str1,idx, 0);
	txtlb_get_element(lbox_not_enrolled_membs, &str1, idx, 0);
	if(idx==-1)
	{
		return ;
	}
	sprintf(sel_notenrldjob_id,"%s",str1);
	printf("\n selected photo job card is %s\n", sel_notenrldjob_id);
	free(str1);
	return ;
}
char msg1[50];
int cb_identify_photo_atten_exce1()
{
	printf("\n coming to photo_muster code \n");

	char *count_job=NULL,*ph_cnt_exists=NULL,*no_days_serv_ph=NULL,*no_days_pos_ph=NULL,*count_payorder_ph=NULL;

	char ph_hh_code[20],pos_days_ph[20]; 

	if(strcmp(sel_notenrldjob_id,"")==0)
	{
		show_message_frame("SELECT JOBCARD ");
		return 0 ;
	}

	sprintf(query,"select ifnull(count(*),0) from muster_attendance where substr(created_date,1,10)=current_date and jobcardno='%s' ",sel_notenrldjob_id);

	count_job= exec_query_get_single_value(count_job, query, DB_PACK);

	if(atoi(count_job)>0)
	{
		show_message_frame("e-MUSTER TAKEN");
		sel_notenrldjob_id[0]='\0';	
		return 0 ;
	}
	free(count_job);
	memset(pos_days_ph,'\0',sizeof(pos_days_ph));

	memset(ph_hh_code,'\0',sizeof(ph_hh_code));

	strncpy(ph_hh_code,sel_notenrldjob_id,18);

	printf("\n coming sel_photojob_id is %s\n",ph_hh_code);

	sprintf(query,"select ifnull(count(*),'0') from wage_info where jobcard_no='%s'",ph_hh_code);

	ph_cnt_exists=exec_query_get_single_value(ph_cnt_exists, query, DB_PACK);
	if(atoi(ph_cnt_exists)==0)
	{
		pw_hide(frame_not_enrold_photo);
		show_frame_webcam_creation_ntenrll();
		return 0;
	}
	else
	{
		sprintf(query,"select distinct no_days from wage_info where jobcard_no='%s'",ph_hh_code);
		no_days_serv_ph=exec_query_get_single_value(no_days_serv_ph, query, DB_PACK);
		sprintf(query, "select ifnull(count(*),'0') from Wage_Payorder_Info where jobcardno='%s'",ph_hh_code);
		count_payorder_ph = exec_query_get_single_value(count_payorder_ph, query, DB_PACK);

		if(atoi(count_payorder_ph)>0)
		{
			sprintf(query,"select distinct worked_days from wage_payorder_info where jobcardno='%s'",ph_hh_code);
			no_days_pos_ph=exec_query_get_single_value(no_days_pos_ph,query, DB_PACK);
			sprintf(pos_days_ph,"%s",no_days_pos_ph);
		}
		else
		{
			sprintf(pos_days_ph,"%s","0");
		}

		printf("\n no_days_serv_ph ,no_days_pos_ph is %s %s\n",no_days_serv_ph,pos_days_ph);

		if((atoi(no_days_serv_ph) >= 150)||(atoi(pos_days_ph) >= 150))
		{
			msg1[0]='\0';
			sprintf(msg1,"150 Days Exceeded For\n %s",ph_hh_code);
			show_message_frame(msg1);
			return 0;
		}
		else if(atoi(no_days_serv_ph)>=135)
		{
			pw_hide(frame_not_enrold_photo);
			show_frame_webcam_creation_ntenrll();
		}					
		else
		{

			pw_hide(frame_not_enrold_photo);	
			show_frame_webcam_creation_ntenrll();

		}

		free(count_payorder_ph);
		free(ph_cnt_exists);
		free(no_days_serv_ph);
		free(no_days_pos_ph);	
	}
	return 0;

}
void create_frame_not_enrold_photo(void)
{
	frame_not_enrold_photo =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_not_enrold_photo);
	lbl_not_enrolled[0] =  pw_create_label(frame_not_enrold_photo , 05, 00, 230, 25,
			PW_TEXT, "",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CONFIG_END);
	lbl_not_enrolled[2] =  pw_create_label(frame_not_enrold_photo , 00, 27, 230, 15,
			PW_TEXT, "Photo Muster List",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbox_not_enrolled_membs =pw_create_tlistbox(frame_not_enrold_photo,0,42, 239, 153,
			PW_BGCOLOR, "#F5DEBC",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,phot_cr_lbox_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,phot_cr_lbox_press,
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_not_enrolled_membs, 150);
	pw_tlistbox_selection_intimate(lbox_not_enrolled_membs,lbox_seljcid_no);
	butt_notenrld_photo_muster[1] = pw_create_button(frame_not_enrold_photo ,25 , 197 , 77, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, "150 Days   exceeded",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_identify_must_atten_100_exce1,
			PW_CONFIG_END);
	butt_notenrld_photo_muster[2] = pw_create_button(frame_not_enrold_photo ,167 , 197 , 70, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, "Photo     muster ",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_identify_photo_atten_exce1,
			PW_CONFIG_END);

	butt_notenrld_photo_muster[0] = pw_create_button(frame_not_enrold_photo, 25, 233, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_photo_notenrolled_back,
			PW_CONFIG_END);
	butt_notenrld_photo_muster[3] = pw_create_button(frame_not_enrold_photo ,167 , 233 , 70, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, "B-eMMS",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, bio_verify,
			PW_CONFIG_END);	

	return;

}
int get_unenrolled_wgseekrs()
{
	pw_tlistbox_clear(lbox_not_enrolled_membs);
	//sprintf(query,"select JobCard_id as jobcard_id,name from (select a.household_code||a.worker_code as Jobcard_id,b.name from mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and substr(A.muster_id,7,6)='%s') as a where jobcard_id not in (select  distinct nrega_id from wg_gpname_bio_info where grname='%s' union select jobcardno from muster_attendance where group_name='%s'  and substr(created_date,1,10)=current_date)",sel_groupname,muster_id,sel_groupname,sel_groupname);
	sprintf(query,"select JobCard_id as jobcard_id,name from (select a.household_code||a.worker_code as Jobcard_id,b.name from mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and A.muster_id='%s') as a where jobcard_id not in (select  distinct nrega_id from wg_gpname_bio_info where grname='%s' union select jobcardno from muster_attendance where group_name='%s'  and substr(created_date,1,10)=current_date)",sel_groupname,muster_id,sel_groupname,sel_groupname);
	exec_query_load_into_lbox_double(lbox_not_enrolled_membs, query, DB_PACK);
	pw_update(lbox_not_enrolled_membs,1);
	return 0;
}

void show_frame_not_enrolled()
{
	if(!frame_not_enrold_photo)
	{
		create_frame_not_enrold_photo();	
		pw_show(frame_not_enrold_photo);
	}	
	else
		pw_show(frame_not_enrold_photo);
	
		show_progress_frame("Processing Please Wait");
	pw_set_text(lbl_not_enrolled[0],sel_groupname);
	sel_notenrldjob_id[0]='\0';
	get_unenrolled_wgseekrs();
	hide_progress_frame();
}

int get_muster_attendance(pw_widget w, pw_event e)
{
	char *not_enrolled_count=NULL;
	sprintf(query,"select count(*) from (select household_code||worker_code as Jobcard_id,Name from household_details  where is_active='Y' and group_name='%s') as a where jobcard_id not in (select distinct nrega_id from wg_gpname_bio_info where grname='%s')",sel_groupname,sel_groupname);
	not_enrolled_count=exec_query_get_single_value(not_enrolled_count, query, DB_PACK);
	hide_frame_muster_id();
	if(atoi(not_enrolled_count)>=0)
	{
		
		load_birs(sel_groupname);
		show_frame_not_enrolled();	
		return 0;
	}
	else
	{
		show_frame_not_enrolled();	
		return 0;
	}
	free(not_enrolled_count);
}

void cb_muster_grp_details()
{
	pw_hide(frame_muster_id);
	show_frame_muster_attendance();
	return ;
}

void get_muster_id_back()
{
	hide_frame_muster_id();
	sel_groupname[0]='\0';
	group_id[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_muster_wktag_grps);
	pw_show(frame_get_muster);
	return;
}

void create_musterid_frame(void)
{
	frame_muster_id= pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	pw_hide(frame_muster_id);
	label_muster[8] =  pw_create_label(frame_muster_id, 0, 0, 60, 15,
			PW_TEXT, "Work Id:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_muster_id[1] =pw_create_label(frame_muster_id, 60, 0, 180, 15,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	label_muster[9] =  pw_create_label(frame_muster_id, 0, 17, 45, 60,
			PW_TEXT, "Work Name:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_muster_id[2] =pw_create_label(frame_muster_id, 45, 17, 195, 60,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_muster[10] =  pw_create_label(frame_muster_id, 0, 80, 90, 20,
			PW_TEXT, "Work Status:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	label_muster_id[3]=pw_create_label(frame_muster_id, 90, 80, 159, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_muster[11] =  pw_create_label(frame_muster_id, 0, 105, 45, 30,
			PW_TEXT, "Group Name:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_muster_id[4]=pw_create_label(frame_muster_id, 45, 105, 195, 30,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	label_muster[12] =  pw_create_label(frame_muster_id, 0, 140, 70, 20,
			PW_TEXT, "Muster Id:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_set_musterid=pw_create_label(frame_muster_id, 70, 140, 140, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "green",
			PW_CONFIG_END);

	Butt_muster_id_submit = pw_create_button(frame_muster_id, 150, 225, 72, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SUBMIT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_attendance,
			PW_CONFIG_END);

	Butt_muster_id_grp = pw_create_button(frame_muster_id, 64, 215, 66, 35,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Group Details",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_muster_grp_details,
			PW_BGCOLOR, "white", PW_CONFIG_END);

	Butt_muster_id_back = pw_create_button(frame_muster_id, 10, 215, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_id_back,
			PW_CONFIG_END);
	return;
}
char wc_value[20],sel_frmdt[11],sel_todt[11],workname[150],sel_wrkdesc[150];
void show_musterid_frame()
{
	if(!frame_muster_id)
	{
		create_musterid_frame();
		pw_show(frame_muster_id);
	}
	else
		pw_show(frame_muster_id);
	pw_set_text(label_muster_id[1], wc_value);
	pw_set_text(label_muster_id[2], workname);
	pw_set_text(label_muster_id[4], sel_groupname);
	pw_set_text(label_muster_id[3], sel_wrkdesc);
	pw_set_text(label_set_musterid,"");
	char *mid=NULL,*nmd_cnt=NULL;
	//sprintf(query,"select distinct ifnull(substr(muster_id,7,6),'0') from mcc_work_allocation_details where work_code='%s' and group_id='%s' and substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",wc_value,group_id,sel_frmdt,sel_todt);
	sprintf(query,"select ifnull(count(*),'0') from mcc_work_allocation_details  where work_code='%s' and group_id='%s' and substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s' and  allocation_type='NMD'",wc_value,group_id,sel_frmdt,sel_todt);
	nmd_cnt =  exec_query_get_single_value(nmd_cnt, query, DB_PACK);
	if(atoi(nmd_cnt)>0)
	{
		sprintf(query,"select distinct ifnull(muster_id,'0') from mcc_work_allocation_details where work_code='%s' and group_id='%s' and substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s' and allocation_type='NMD'",wc_value,group_id,sel_frmdt,sel_todt);
		mid = exec_query_get_single_value(mid, query, DB_PACK);
		sprintf(muster_id, "%s",mid);
		printf("muster_id i of NMD is %s\n",muster_id);
		free(nmd_cnt);
	}
	else
	{

		sprintf(query,"select distinct ifnull(muster_id,'0') from mcc_work_allocation_details where work_code='%s' and group_id='%s' and substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",wc_value,group_id,sel_frmdt,sel_todt);
		mid = exec_query_get_single_value(mid, query, DB_PACK);
		sprintf(muster_id, "%s",mid);
		printf("muster_id is %s\n",muster_id);
		free(nmd_cnt);
	}
	if(strcmp(muster_id, "")==0)
	{
		pw_show(frame_get_muster);
		show_message_frame("Download From MCC \n WorkAllocation Details");
		return ;
	}
	pw_set_text(label_set_musterid,muster_id);
	free(mid);
}
void hide_frame_muster_id(void)
{
	pw_hide(frame_muster_id);
	return;
}

pw_widget combo_muster_hab,txt_entry_wrk_id,lbox_muster_wktag_grps,Butt_wrkid_search,label_muster_frmdate[2],label_muster_todate[2],Butt_muster_mustr,Butt_muster_mustr_back,Butt_muster_tag;

char sel_habname[50],sel_workcode[8],dist_mand_pan_id[10],selected_wrkid[20];
void get_musterid(void)
{
	if(strcmp(sel_habname, "")==0)
	{
		show_message_frame("SELECT HABITATION");
		return ;
	}

	if(strcmp(sel_workcode, "")==0)
	{
		show_message_frame("SELECT WORK ID");
		return ;
	}

	if(strcmp(sel_groupname, "")==0)
	{
		show_message_frame("SELECT GROUP NAME");
		return ;
	}
	if(strcmp(group_id, "0")==0)
	{
		show_message_frame("Not Exist in Master Table");
		group_id[0]='\0';
		return ;

	}
	hide_frame_get_muster();
	show_musterid_frame();
	return ;
}

int Tokenizer(char *sel_frmdt,char *sel_todt)
{
	char *frm_dt=NULL,*to_dt=NULL;
	sprintf(query,"select strftime('%s','%s')",date_format,sel_frmdt);
	frm_dt = exec_query_get_single_value(frm_dt, query, DB_PACK);
	sprintf(query,"select strftime('%s','%s')",date_format,sel_todt);
	to_dt = exec_query_get_single_value(to_dt, query, DB_PACK);
	pw_set_text(label_muster_frmdate[0],frm_dt);
	pw_set_text(label_muster_todate[1], to_dt);
	free(frm_dt);
	free(to_dt);
	return 0;
}

int get_alldetails_callback(void *parg, int argc, char **argv, char **colnames)
{
	sel_from_date[0]='\0';
	sel_to_date[0]='\0';
	strcpy(sel_from_date, argv[0]);
	printf("sel_from_date, is %s\n",sel_from_date);
	strcpy(sel_to_date, argv[1]);
	return(0);
}
void lbox_seltag_grp()
{
	sqlite3 *dbhandle=NULL;

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	char *str1=NULL,*groupid=NULL;
	int idx = pw_tlistbox_get_selection(lbox_muster_wktag_grps);
	pw_tlistbox_get_element(lbox_muster_wktag_grps, &str1,idx, 0);
	txtlb_get_element(lbox_muster_wktag_grps, &str1, idx, 0);
	if(idx==-1)
	{
		return ;
	}

	sel_groupname[0]='\0';
	strcpy(sel_groupname,str1);

	char *grpcntchk =NULL;
	sprintf(query,"select ifnull(count(*),'0') from household_details where group_name='%s'",sel_groupname);
	grpcntchk = exec_query_get_single_value(grpcntchk,query,DB_PACK);
	if(atoi(grpcntchk)==0)
	{
		show_message_frame("Download Household Details \n From MCC");
		return ;
	}
	free(grpcntchk);
	group_id[0]='\0';
	memset(group_id, 0, 1);
	sprintf(query,"select distinct ifnull(group_id,0) from household_details where group_name='%s'",sel_groupname);
	groupid=exec_query_get_single_value(groupid, query, DB_PACK);
	sprintf(group_id,"%s",groupid);
	free(str1);
	free(groupid);
	pw_set_text(label_muster_frmdate[0],"");
	pw_set_text(label_muster_todate[1],"");
	sprintf(query,"select distinct substr(alloc_from_date,1,10),substr(alloc_to_date,1,10) from mcc_work_allocation_details where work_code='%s' and group_id='%s' and substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",wc_value,group_id,sel_frmdt,sel_todt);
	sqlite3_exec(dbhandle, query, get_alldetails_callback, NULL, NULL);
	printf("dates query %s\n",query);
	sqlite3_close(dbhandle);
	Tokenizer(sel_from_date,sel_to_date);
}
int get_wrknm_wrkdesc(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(workname,argv[0]);
	strcpy(sel_wrkdesc,argv[1]);
	return(0);
}

char *mon_date=NULL,*day_test=NULL,*frm_date=NULL,*mon_date1=NULL,*frm_date1=NULL;
char sel_day_test[10],total_value[15];
char habit_code[15]="",village_code[15]="";
void combobox_muster(pw_widget widget, int index)
{
	unsigned idx;
	char *workcode=NULL;

	show_progress_frame("PROCESSING PLEASE WAIT");

	pw_combobox_get_selection(combo_muster_wkcode , &workcode, &idx);

	strcpy(sel_workcode, workcode);

	free(workcode);

	sprintf(wc_value, "%s%s", total_value, sel_workcode);
	printf("wc value is %s\n",wc_value);

	sqlite3 *dbhandle=NULL;

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select work_name,description from work_registration where work_code='%s'", wc_value);
	sqlite3_exec(dbhandle, query, get_wrknm_wrkdesc, NULL, NULL);
	printf("work desc is %s\n",query);
	pw_tlistbox_clear(lbox_muster_wktag_grps);
	sel_groupname[0]='\0';
	//sprintf(query,"select distinct group_name from mcc_work_allocation_details where work_code='%s' and substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",wc_value,sel_frmdt,sel_todt);
	sprintf(query,"select distinct a.group_name from groups_distinct a,mcc_work_allocation_details b where a.group_id=b.group_id  and work_code='%s' and substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",wc_value,sel_frmdt,sel_todt);
	exec_query_load_into_lbox_single(lbox_muster_wktag_grps, query, DB_PACK);
	sqlite3_close(dbhandle);
	hide_progress_frame();
	return;
}

int get_search_wrkid()
{
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	if(strcmp(sel_habname, "")==0)
	{
		show_message_frame("SELECT HABITATION");
		return -1;
	}

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
	sprintf(query,"select  count(*) from mcc_work_allocation_details where work_code like '%%%s%%' and substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",selected_wrkid,sel_frmdt,sel_todt);

	search_cnt = exec_query_get_single_value(search_cnt, query, DB_PACK);
	if(atoi(search_cnt)==0)
	{
		show_message_frame("NO DATA");
		pw_entry_clear(txt_entry_wrk_id);
		return 0;
	}

	sprintf(query,"select distinct substr(work_code, 13, 6) from mcc_work_allocation_details where  work_code like '%%%s%%' and  substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",selected_wrkid,sel_frmdt,sel_todt);

	pw_combobox_clear(combo_muster_wkcode);
	pw_set_text(combo_muster_wkcode,"--Select--");
	pw_entry_clear(txt_entry_wrk_id);
	exec_query_load_into_combo(combo_muster_wkcode, query, DB_PACK);
	free(search_cnt);
	return 0;
}
char *exec_query_get_single_value(char *data, char *query, char *database);

int fp_del_check()
{
	sqlite3 *dbhandle=NULL;

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	char *del_fp_cnt=NULL;
	sprintf(query,"select ifnull(count(*),'0') from FP_Sensor_info");
	del_fp_cnt = exec_query_get_single_value(del_fp_cnt,query,DB_PACK);
	if(atoi(del_fp_cnt)==0)
	{
		get_birs_del();
		sprintf(query,"insert into FP_Sensor_info(next_del_date)values('%s')",sel_todt);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	}
	else
	{
		char *next_deldt=NULL,*ck_deldt=NULL;
		sprintf(query,"select ifnull(next_del_date,'0') from FP_Sensor_info");
		next_deldt = exec_query_get_single_value(next_deldt, query, DB_PACK);

		sprintf(query,"select (case when '%s'>current_date then 'NOK' else 'OK' end)",next_deldt);
		ck_deldt = exec_query_get_single_value(ck_deldt, query, DB_PACK);

		free(next_deldt);
		if(strcmp(ck_deldt,"OK")==0)
		{
			get_birs_del();
			sprintf(query,"update FP_Sensor_info set next_del_date='%s',delete_date=current_date",sel_todt);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
			printf("update query1 = %s \n", query);
		}

		free(ck_deldt);
	}
	free(del_fp_cnt);
	sqlite3_close(dbhandle);
	return 0;
}
extern char date[15];
void combobox_muster_habchange(pw_widget widget, int index)
{
	char *habitationname=NULL;
	unsigned idx;
	pw_combobox_get_selection(combo_muster_hab,&habitationname,&idx);
	strcpy(sel_habname,habitationname);
	free(habitationname);

	strncpy(village_code,sel_habname,3);
	printf("village code is %s\n",village_code);
	strncpy(habit_code,sel_habname+3,2);
	printf("habit code is %s\n",habit_code);
	sprintf(total_value,"%s%s",dist_mand_pan_id,village_code);
	printf("total_value is %s",total_value);

	char *batch_status=NULL,sel_batch_status[2];

	sprintf(query,"select Batch_type from Batch_info");
	batch_status= exec_query_get_single_value(batch_status,query, DB_PACK);
	sprintf(sel_batch_status, "%s", batch_status);
	free(batch_status);
	day_test =exec_query_get_single_value(day_test,"select strftime('%w','now')",DB_PACK);
	sprintf(sel_day_test, "%s", day_test);
	free(day_test);

	if(strcmp(sel_batch_status,"B1")==0)
	{
		if(atoi(sel_day_test)==1)
		{
			sprintf(query, "select date('now')");
			mon_date = exec_query_get_single_value(mon_date, query, DB_PACK);
			sprintf(sel_frmdt, "%s", mon_date);
			free(mon_date);
			sprintf(query,"select date('now','weekday 0')");
			mon_date1=exec_query_get_single_value(mon_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", mon_date1);
			free(mon_date1);
			sprintf(query,"select distinct substr(work_code, 13, 6) from mcc_work_allocation_details where substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",sel_frmdt,sel_todt);
			pw_combobox_clear(combo_muster_wkcode);
			pw_set_text(combo_muster_wkcode,"--Select--");
			exec_query_load_into_combo(combo_muster_wkcode, query, DB_PACK);
			fp_del_check();
		}

		else
		{
			sprintf(query, "select date('now','weekday 1','-7 days')");
			frm_date = exec_query_get_single_value(frm_date, query, DB_PACK);

			sprintf(sel_frmdt, "%s", frm_date);
			free(frm_date);

			sprintf(query,"select date('now','weekday 0')");
			frm_date1=exec_query_get_single_value(mon_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", frm_date1);
			free(frm_date1);
			sprintf(query,"select distinct substr(work_code, 13, 6) from mcc_work_allocation_details where substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",sel_frmdt,sel_todt);
			pw_combobox_clear(combo_muster_wkcode);
			pw_set_text(combo_muster_wkcode,"--Select--");
			exec_query_load_into_combo(combo_muster_wkcode, query, DB_PACK);
			fp_del_check();
		}
	}

	else
	{
		printf("sel_day_test1 is %s\n",sel_day_test);
		if(atoi(sel_day_test)==4)
		{
			printf("sel_day_test is %s\n",sel_day_test);
			sprintf(query, "select date('now')");
			mon_date = exec_query_get_single_value(mon_date, query, DB_PACK);
			sprintf(sel_frmdt, "%s", mon_date);
			free(mon_date);

			sprintf(query,"select date('%s','weekday 3')",sel_frmdt);
			mon_date1=exec_query_get_single_value(mon_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", mon_date1);
			free(mon_date1);
			sprintf(query,"select distinct substr(work_code, 13, 6) from mcc_work_allocation_details where substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",sel_frmdt,sel_todt);
			pw_combobox_clear(combo_muster_wkcode);
			pw_set_text(combo_muster_wkcode,"--Select--");
			exec_query_load_into_combo(combo_muster_wkcode, query, DB_PACK);
			fp_del_check();
		}

		else
		{

			sprintf(query, "select date('now','weekday 4','-7 days')");
			frm_date = exec_query_get_single_value(frm_date, query, DB_PACK);

			sprintf(sel_frmdt, "%s", frm_date);
			free(frm_date);

			sprintf(query,"select date('%s','weekday 3')",sel_frmdt);
			frm_date1=exec_query_get_single_value(mon_date1, query, DB_PACK);
			sprintf(sel_todt, "%s", frm_date1);
			free(frm_date1);
			sprintf(query,"select distinct substr(work_code, 13, 6) from mcc_work_allocation_details where substr(alloc_from_date,1,10) >='%s' and substr(alloc_to_date,1,10) <='%s'",sel_frmdt,sel_todt);
			pw_combobox_clear(combo_muster_wkcode);
			pw_set_text(combo_muster_wkcode,"--Select--");
			exec_query_load_into_combo(combo_muster_wkcode, query, DB_PACK);
			fp_del_check();
		}
	}
	return ;
}

void get_muster_back()
{
	hide_frame_get_muster();
	sel_habname[0]='\0';
	sel_workcode[0]='\0';
	sel_groupname[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_muster_wktag_grps);
	pw_entry_clear(txt_entry_wrk_id);
	show_fa_module();
	return;
}
int hab_details()
{
	char *dist_mand_pan_code=NULL ;

	sprintf(query,"select substr(jc_code,1,9) from jc_info");

	dist_mand_pan_code = exec_query_get_single_value(dist_mand_pan_code, query, DB_PACK);

	sprintf(dist_mand_pan_id,"%s",dist_mand_pan_code);
	sprintf(total_value,"%s",dist_mand_pan_code);
	pw_combobox_clear(combo_muster_hab);
	pw_set_text(combo_muster_hab,"--Select--");
	sprintf(query,"select distinct Habitation_Code||'-'||habitation_name from tmp_Habitation_Master ");
	exec_query_load_into_combo(combo_muster_hab, query, DB_PACK);
	free(dist_mand_pan_code);
	return 0;
}
void get_muster_normal()
{
	hide_frame_get_muster();
	show_frame_normallogin_page();
}
void create_frame_get_muster(void)
{
	frame_get_muster = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_get_muster);

	label_muster[0] =  pw_create_label(frame_get_muster,00, 00, 85,20,
			PW_TEXT, "Habitation:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_muster_hab = pw_create_combobox(frame_get_muster, 85, 00, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_selection_intimate(combo_muster_hab,combobox_muster_habchange);

	label_muster[1] =  pw_create_label(frame_get_muster, 0, 23, 75, 20,
			PW_TEXT, "WORK ID:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	txt_entry_wrk_id =  pw_create_entry(frame_get_muster, 85, 23, 60, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	pw_focus(txt_entry_wrk_id);

	Butt_wrkid_search = pw_create_button(frame_get_muster, 160, 23,76, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SEARCH,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_search_wrkid,
			PW_CONFIG_END);
	label_muster[2] =  pw_create_label(frame_get_muster, 0, 50, 80, 20,
			PW_TEXT, "WORK ID:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	combo_muster_wkcode = pw_create_combobox(frame_get_muster, 85, 50, 80, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_muster_wkcode ,combobox_muster);
	label_muster[4] =  pw_create_label(frame_get_muster, 0, 192, 80, 20,
			PW_TEXT, "From Date:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	label_muster_frmdate[0] =  pw_create_label(frame_get_muster, 80,192, 200, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	label_muster[5] =  pw_create_label(frame_get_muster, 0, 215, 80, 20,
			PW_TEXT, "To     Date:",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	label_muster_todate[1] =  pw_create_label(frame_get_muster, 80, 215, 160, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);
	label_muster[6] =  pw_create_label(frame_get_muster,0, 75, 120, 15,
			PW_TEXT, "Tagged Groups",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "GREEN",
			PW_CONFIG_END);

	lbox_muster_wktag_grps = pw_create_tlistbox(frame_get_muster, 0, 90, 239, 100,
			PW_BGCOLOR, "#FFFACD",
			PW_FONT, PW_BOLD_FONT,
			//			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,grpchk_cr_lbox_release,
			//                        PW_CALLBACK, PW_BUTTON_PRESS_EVENT,grpchk_cr_lbox_press,
			PW_CONFIG_END);
	pw_tlistbox_selection_intimate(lbox_muster_wktag_grps,lbox_seltag_grp);
	pw_tlistbox_set_selection_type(frame_get_muster,PW_TLISTBOX_SINGLE_SELECTION);

	Butt_muster_mustr = pw_create_button(frame_get_muster, 146, 235, 86, 27,
			PW_TEXT, "e-Musters",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_musterid,
			PW_CONFIG_END);
	Butt_muster_mustr_normal = pw_create_button(frame_get_muster, 55, 235, 80, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_TEXT, "Normal Mode",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_normal,
			PW_CONFIG_END);	
	Butt_muster_mustr_back = pw_create_button(frame_get_muster, 10, 235, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, get_muster_back,
			PW_CONFIG_END);
	return;
}

void show_frame_get_muster(void)
{
	show_progress_frame("PLEASE WAIT....");
	if (!frame_get_muster)
	{
		create_frame_get_muster();
		pw_show(frame_get_muster);
	}
	else
		pw_show(frame_get_muster);
	pw_set_text(combo_muster_hab,"--Select--");
	pw_set_text(label_muster_frmdate[0],"");
	pw_set_text(label_muster_todate[1],"");
	hab_details();
	pw_combobox_clear(combo_muster_wkcode);
	pw_tlistbox_clear(lbox_muster_wktag_grps);
	sel_groupname[0]='\0';
	hide_progress_frame();
	return;
}
void hide_frame_get_muster(void)
{
	pw_hide(frame_get_muster);
}
