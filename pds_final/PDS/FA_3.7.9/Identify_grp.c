#include "types.h"
#include <fcntl.h>
#include <sys/stat.h>

void hide_frame_identify_must_atten();
void show_progress_frame();
void show_frame_webcam_creation();
int identify_finger_print();
void hide_progress_frame();
int cb_identify_must_atten();
char *exec_query_get_single_value(char *data, char *query, char *database);
int exec_query_load_into_lbox_single(pw_widget lbox_grp_cr, char *query, char *database);
void hide_frame_identify_must_photo();
int exec_query_load_into_lbox_double(pw_widget lbox, char *query, char *database);
void hide_frame_identify_must_photo();
void show_musterid_frame();
void hide_frame_identify_100_execeed();

int Muster_Photo(char *photo_jobcard,char *grpname,char *photoname);

char *get_sel_photojob_id();

char *get_grp_name();
char *get_grp_name_ntenrll();

char *get_photo_name();
int txtlb_get_element();

char sel_name[50],msg[50];

char sel_photojob_id[20],get_must_grpname[150];

char *get_sel_photojob_id()
{
	return sel_photojob_id;
}
char *get_grp_name()
{
	return get_must_grpname;
}
pw_widget frame_identify_must_atten, Butt_Identify_must_back, Butt_Identify ,label_identify_must_grp,lbox_identify_must_membs ,CrowButt ,Butt_Identify_photo,Butt_Identify_un_enroll,frame_identify_must_photo,lbox_identify_must_photo_membs,Butt_photo_muster,frame_identify_must_atten_photo,Butt_photo_Identify,Butt_photo_Identify_back,frame_photo_display,frame_get_muster,rb[4],lbl_identity[4],lbox_muster_wktag_grps,label_identify_must_group,Butt_Bio_FP,Butt_Identify_cnt_exceed;
pw_widget Butt_Bio_Identify;

pw_widget lbl_msg;
char *get_muster_id();
char *get_dist_limit();
char *get_work_code();
char *get_crow_bar_status();
char *get_from_date();
char *get_to_date();
char *get_sel_groupname();
char *get_combo_hab();
char *get_group_id();

void show_text_frame(char *msg);
void hide_text_frame();

int mark_attendance_for_verified_person(char *jobcardno, char *name, char *gname);
int  Gps_Test(double *lat, double *lon);

char query[1024];

void lbox_identify_change(pw_widget wdgt, int idx)
{
	char *mname;

	pw_tlistbox_get_element(lbox_identify_must_membs,&mname,idx,0);
	txtlb_get_element(lbox_identify_must_membs,&mname,idx,0);
	strcpy(sel_name,mname);
	free(mname);
	printf("\n copied mname is %s\n",sel_name);
	return;
}
char *get_crow_bar_status()
{
	if(pw_radiobutton_status(rb[3]) == 1 )
		return "Y";
	else
		return "N";
}
int  cb_muster_crowbar_submit()
{
	if(strcmp(sel_name,"")==0)
	{
		show_message_frame("SELECT ATTENDEE");
		return 0;
	}
	sqlite3 *dbhandle=NULL;

	char *totcount=NULL,*crbarcount=NULL;	
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}

	sprintf(query, "select ifnull(count(*),'0') from muster_attendance where substr(created_date,1,10)=current_date and group_name='%s'",get_must_grpname);
	totcount = exec_query_get_single_value(totcount, query, DB_PACK);

	sprintf(query, "select ifnull(count(*),'0') from muster_attendance where substr(created_date,1,10)=current_date and group_name='%s' and crowbar='Y'",get_must_grpname);
	crbarcount = exec_query_get_single_value(crbarcount, query, DB_PACK);
	if(atoi(totcount)==0)
	{
		show_message_frame("SELECT e-MUSTERS");
		return 0;
	}
	printf("yahoo crowbar val is %s\n",get_crow_bar_status());
	int res=strcmp(get_crow_bar_status(),"N");
	printf("\n ret is %d\n",ret);

	if(res==0)
	{
		sprintf(query, "update muster_attendance set crowbar='%s' where substr(created_date,1,10)=current_date and group_name='%s' and member_name='%s'",get_crow_bar_status(),get_must_grpname,sel_name) ;		

		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		printf("query1 is %s\n", query);

		show_message_frame("CROWBAR NOT TAKEN");
		return 0;
	}
	if(atoi(crbarcount)<floor((atoi(totcount))/2))
	{
		sprintf(query, "update muster_attendance set crowbar='%s'where substr(created_date,1,10)=current_date and group_name='%s' and member_name='%s'",get_crow_bar_status(),get_must_grpname,sel_name) ;
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		printf("query1 is %s\n", query);
		show_message_frame("CROWBAR TAKEN");
		return 0 ;
	}
	else
		show_message_frame("CROWBAR LIMIT EXCEEDS");
	sqlite3_close(dbhandle);
	return 0;
}
int cb_photo_muster()
{

	printf("\n coming to photo_muster code \n");

	char *count_job=NULL,*ph_cnt_exists=NULL,*no_days_serv_ph=NULL,*no_days_pos_ph=NULL,*count_payorder_ph=NULL;

	char ph_hh_code[20],pos_days_ph[20]; 

	if(strcmp(sel_photojob_id,"")==0)
	{
		show_message_frame("SELECT JOBCARD ");
		return 0 ;
	}

	sprintf(query,"select ifnull(count(*),0) from muster_attendance where substr(created_date,1,10)=current_date and jobcardno='%s' ",sel_photojob_id);

	count_job= exec_query_get_single_value(count_job, query, DB_PACK);

	if(atoi(count_job)>0)
	{
		show_message_frame("e-MUSTER TAKEN");
		sel_photojob_id[0]='\0';	
		return 0 ;
	}
	memset(pos_days_ph,'\0',sizeof(pos_days_ph));

	memset(ph_hh_code,'\0',sizeof(ph_hh_code));

	strncpy(ph_hh_code,sel_photojob_id,18);

	printf("\n coming sel_photojob_id is %s\n",ph_hh_code);

	sprintf(query,"select ifnull(count(*),'0') from wage_info where jobcard_no='%s'",ph_hh_code);

	ph_cnt_exists=exec_query_get_single_value(ph_cnt_exists, query, DB_PACK);
	if(atoi(ph_cnt_exists)==0)
	{
		hide_frame_identify_must_photo();
		show_frame_webcam_creation();
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
			msg[0]='\0';
			sprintf(msg,"150 Days Exceeded For\n %s",ph_hh_code);
			show_message_frame(msg);
			return 0;
		}
		else if(atoi(no_days_serv_ph)>=135)
		{
			hide_frame_identify_must_photo();
			show_frame_webcam_creation();
		}					
		else
		{

			hide_frame_identify_must_photo();	
			show_frame_webcam_creation();

		}

		free(count_payorder_ph);
		free(ph_cnt_exists);
		free(no_days_serv_ph);
		free(no_days_pos_ph);	
	}
	return 0;
}
void cb_phot_cr_lbox_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
	if(frame_photo_display)
		pw_hide(frame_photo_display);
}
void  cb_phot_cr_lbox_press()
{
	char *str;
	int idx = pw_tlistbox_get_selection(lbox_identify_must_photo_membs);
	pw_tlistbox_get_element(lbox_identify_must_photo_membs, &str,idx, 0);

	txtlb_get_element(lbox_identify_must_photo_membs, &str, idx, 0);
	if(idx==-1)
	{
		return ;
	}
	show_text_frame(str);
	free(str);
	return ;
}
void lbox_selpjc_no()
{
	char *str1;
	int idx = pw_tlistbox_get_selection(lbox_identify_must_photo_membs);
	pw_tlistbox_get_element(lbox_identify_must_photo_membs, &str1,idx, 0);

	txtlb_get_element(lbox_identify_must_photo_membs, &str1, idx, 0);
	if(idx==-1)
	{
		return ;
	}
	strcpy(sel_photojob_id, str1);
	printf("\n selected photo job card is %s\n", sel_photojob_id);
	return ;
}
char nrega_id[20], name[50], gname[50];
int get_jcdetails(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(nrega_id, argv[0]);
	strcpy(name, argv[1]);
	strcpy(gname,argv[2]);
	return(0);
}
unsigned char *read_data_from_file_with_size(unsigned char *data, char *path, int *fsz1)
{
	int imgFD=0;

	struct stat fst;

	int fsz=*fsz1;

	imgFD = open(path, O_RDONLY);

	fstat(imgFD, &fst);

	fsz = fst.st_size;

	if (fsz <= 0) {

		printf("read_data_from_file: File Size is -ve\n");

		close(imgFD);

		return NULL;

	}
	data = (char *)malloc(sizeof(int)*(fsz+1));

	if (data == NULL) {

		printf("read_data_from_file: Data allocation not possible\n");

		close(imgFD);

		return NULL;
	}
	bzero(data, fsz+1);

	read(imgFD, data, fsz);

	close(imgFD);

	data[fsz] = '\0';

	*fsz1 = fsz;
	return(data);
}
char min_id[5],max_id[5];
int get_sid(void *parg, int argc, char **argv, char **colnames)
{
	min_id[0]='\0';
	max_id[0]='\0';
	strcpy(min_id, argv[0]);
	printf("\n min_id is %s\n", min_id);
	strcpy(max_id, argv[1]);
	printf("\n max_id is %s\n", max_id);
	return(0);
}
unsigned long k;

int  rval1,m;
int ret_val1,retval;
char tmplFileName[256];
int jc_verify()
{
	unsigned char *tmplData=NULL;
	int tmplLen=0;
	sprintf(tmplFileName, "/tmp/template.dat");

	unlink(tmplFileName);

	show_progress_frame("Place Finger for verification");

	printf("Scaning the template.\n");
	fps = bio_init(FP_SENSOR);

	retval = bio_scan_template(fps, tmplFileName, 10, 0);

	printf("retval for scanning template is %d",retval);
	if (retval != 0) { //Scanning finger print failed
		hide_progress_frame();
		show_progress_frame("SENSOR TIME OUT...");
		bio_uninit(fps);
		sleep(2);
		hide_progress_frame();
		return -1;
	}

	tmplData = read_data_from_file_with_size(tmplData, tmplFileName, &tmplLen);

	for (k=atoi(min_id); k <= atoi(max_id); k++) {

		printf("k value is %lu\n", k);

		ret_val1 = bio_change_user_security_level(fps, k, 3);

		printf("\nret_rval1 fjc_verify is %d\n",ret_val1);

		rval1 = bio_verify_by_template_data(fps, &k, (unsigned char *)tmplData, tmplLen);

		printf("\nrval1 bio_verify is %d\n",rval1);
		if (rval1 == -1 && k==atoi(max_id)) { //Scanning finger print failed
			hide_progress_frame();
			show_progress_frame("VERIFICATION FAIL");
			sleep(2);
			hide_progress_frame();
			bio_uninit(fps);
			printf("Verification Failed pp \n");
			return -1;
		}

		if (rval1 == 0) {

			printf("User id1 in success = %lu \n", k);
			system("madplay /usr/local/audio/S2.mp3");
			show_progress_frame("SUCCESS");
			sleep(2);
			bio_uninit(fps);
			hide_progress_frame();


			sqlite3 *dbhandle=NULL;

			sqlite3_open(DB_PACK, &dbhandle);

			if (dbhandle == NULL)

				printf("Unable to open db\n");

			sprintf(query, "select nrega_id,name,grname from wg_gpname_bio_info where sid = '%lu';",k);

			printf("query is %s\n", query);

			sqlite3_exec(dbhandle, query, get_jcdetails, NULL, NULL);

			sqlite3_close(dbhandle);

			mark_attendance_for_verified_person(nrega_id, name, gname);

			pw_tlistbox_clear(lbox_identify_must_photo_membs);

			sprintf(query,"select nrega_id as nrega_id,name from (select distinct nrega_id,name from wg_gpname_bio_info where grname='%s') as a where nrega_id not in (select jobcardno from muster_attendance where group_name='%s' and substr(created_date,1,10)=current_date)",gname,gname);
			exec_query_load_into_lbox_double(lbox_identify_must_photo_membs, query, DB_PACK);

			pw_update(lbox_identify_must_photo_membs,1);

			sqlite3_close(dbhandle);
			pw_tlistbox_remove_selection_all(lbox_identify_must_photo_membs);
			free(tmplData);
			sel_photojob_id[0]='\0';
			return 0;
		}
	}
	bio_uninit(fps);

	free(tmplData);
	return -1;
}
int  identify_finger(char *jobcardno)
{
	//#ifdef __arm__
	char query[256];
	printf("jobcard in identify finger is %s",jobcardno);
	printf("uid_jc in identify finger is %s",jobcardno);

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query, "select min(sid),max(sid) from wg_gpname_bio_info where nrega_id='%s'",jobcardno);
	printf("query is %s\n", query);
	sqlite3_exec(dbhandle, query, get_sid, NULL, NULL);
	sqlite3_close(dbhandle);
	jc_verify();
	return 0;
}
void cb_bio_fp_release(pw_widget wdgt, int idx)
{
	hide_text_frame();
	return;

}
void cb_bio_fpcnt()
{
	printf("\n selected photo job card1 is %s\n", sel_photojob_id);
	if(strcmp(sel_photojob_id,"")==0)
	{
		show_message_frame("SELECT JOBCARD ");
		return ;
	}
	char *fp_cnt=NULL;
	sprintf(query,"select ifnull( distinct(count(sid)),'0') from wg_gpname_bio_info where nrega_id='%s'",sel_photojob_id);
	fp_cnt = exec_query_get_single_value(fp_cnt, query, DB_PACK);
	show_text_frame(fp_cnt);
	sleep(2);
	hide_text_frame();
	free(fp_cnt);
}
void cb_bio_verify()
{
	char *count_job=NULL;
	printf("\n selected photo job card1 is %s\n", sel_photojob_id);
	if(strcmp(sel_photojob_id,"")==0)
	{
		show_message_frame("SELECT JOBCARD ");
		return ;
	}
	sprintf(query,"select ifnull(count(*),0) from muster_attendance where substr(created_date,1,10)=current_date and jobcardno='%s' ",sel_photojob_id);
	count_job= exec_query_get_single_value(count_job, query, DB_PACK);

	if(atoi(count_job)>0)
	{
		show_message_frame("e-MUSTER TAKEN");
		sel_photojob_id[0]='\0'; 
		return ;
	}        
	unsigned long id1;
	show_progress_frame("Place Finger for verification ");
	id1 = identify_finger(sel_photojob_id);
	printf("id1 value after identify is %lu",id1);
	hide_progress_frame();
	return;
}
int get_unenrolled_wgseekers(char *groupname_photo,char *work_code)
{
	pw_tlistbox_clear(lbox_identify_must_photo_membs);
	sprintf(query,"select JobCard_id as jobcard_id,name from (select a.household_code||a.worker_code as Jobcard_id,b.name from mcc_work_allocation_details A,household_details B WHERE A.household_code||A.worker_code=B.household_code||B.worker_code and B.is_active='Y' and B.group_name='%s' and substr(A.muster_id,7,6)='%s') as a where jobcard_id not in (select  distinct nrega_id from wg_gpname_bio_info where grname='%s')",groupname_photo,get_muster_id(),groupname_photo);


	exec_query_load_into_lbox_double(lbox_identify_must_photo_membs, query, DB_PACK);
	pw_update(lbox_identify_must_photo_membs,1);
	return 0;
}
int photo_jc(char *groupname_photo,char *work_code)
{
	pw_tlistbox_clear(lbox_identify_must_photo_membs);
	sprintf(query,"select nrega_id as nrega_id,name from (select distinct nrega_id,name from wg_gpname_bio_info where grname='%s') as a where nrega_id not in (select jobcardno from muster_attendance where group_name='%s' and substr(created_date,1,10)=current_date)",groupname_photo,groupname_photo);
	exec_query_load_into_lbox_double(lbox_identify_must_photo_membs, query, DB_PACK);
	pw_update(lbox_identify_must_photo_membs,1);
	return 0;
}
void cb_photo_identify_back()
{
	hide_frame_identify_must_photo();
	sel_photojob_id[0]='\0';
	pw_show(frame_identify_must_atten);
//	show_frame_identify_must_atten();
} 
void create_frame_identify_must_photo(void)
{
	frame_identify_must_photo =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_identify_must_photo);
	lbl_identity[2] =  pw_create_label(frame_identify_must_photo , 0, 05, 50, 25,
			PW_TEXT, "Group Name:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	label_identify_must_grp =  pw_create_label(frame_identify_must_photo , 50, 05, 189, 25,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbox_identify_must_photo_membs =pw_create_tlistbox(frame_identify_must_photo,0,35, 239, 180,
			PW_BGCOLOR, "#F5DEBC",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_phot_cr_lbox_release,
			PW_CALLBACK, PW_BUTTON_PRESS_EVENT,cb_phot_cr_lbox_press,
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_identify_must_photo_membs, 150);
	pw_tlistbox_selection_intimate(lbox_identify_must_photo_membs,lbox_selpjc_no);

	Butt_photo_Identify = pw_create_button(frame_identify_must_photo , 157, 225 , 75, 35,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, " ",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_photo_muster,
			PW_CONFIG_END);

	Butt_Bio_FP = pw_create_button(frame_identify_must_photo , 38, 225 , 50, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, "   FP    Count",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_bio_fp_release,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_fpcnt,
			PW_CONFIG_END);


	Butt_Bio_Identify = pw_create_button(frame_identify_must_photo , 95, 225 , 55, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, "   BIO    VERIFY",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_verify,
			PW_CONFIG_END);

	Butt_photo_Identify_back = pw_create_button(frame_identify_must_photo, 0, 225, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_photo_identify_back,
			PW_CONFIG_END);

	return;

}

char total_count[10],date_id[20];
int get_count_date_callback(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(total_count, argv[0]);
	strcpy(date_id, argv[1]);
	return 0;
}

int attendance(char *jobcardno)
{

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	char *count=NULL ;
	char *count_date_id=NULL,*count_date_id1=NULL;
	double lat, lon;

	sprintf(query, "select count(*) from muster_attendance where substr(created_date,1,10)=current_date and jobcardno='%s'", jobcardno);
	count = exec_query_get_single_value(count, query, DB_PACK);
	if(atoi(count)>0)
	{
		pw_set_text(lbl_msg, "ALREADY VERIFIED.");
		//system("madplay /usr/local/audio/T1.mp3");
		pw_update(lbl_msg, 1);
		sleep(2);

		return -1;
	}
	free(count);
	Gps_Test(&lat, &lon);

	char *count1=NULL,sel_count[30];

	sprintf(query, "select count(*) from muster_attendance where group_name='%s' and work_code='%s' and current_date>='%s' and current_date<='%s'", gname, get_work_code(), get_from_date(), get_to_date());

	count1 = exec_query_get_single_value(count1, query, DB_PACK);

	sprintf(sel_count,"%s",count1);
	sprintf(query,"select ifnull(count(*),'0'),ifnull(date_id,'0') from muster_dates where work_code='%s' and from_date='%s' and to_date='%s'",get_work_code(),get_from_date(), get_to_date());


	sqlite3_exec(dbhandle, query, get_count_date_callback, NULL, NULL);

	if(atoi(total_count)==0)
	{
		sprintf(query,"insert into muster_dates(work_code,from_date, to_date) values('%s','%s','%s')",get_work_code(),get_from_date(), get_to_date());

		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		printf("query1 = %s \n", query);

		sprintf(query,"select ifnull(date_id,'0') from muster_dates where work_code='%s' and from_date='%s' and to_date='%s'",get_work_code(),get_from_date(), get_to_date());
		count_date_id1 =exec_query_get_single_value(count_date_id1, query, DB_PACK);

		sprintf(date_id,"%s",count_date_id1);

	}
	else
	{
		sprintf(query,"select ifnull(date_id,'0') from muster_dates where work_code='%s' and from_date='%s' and to_date='%s'",get_work_code(),get_from_date(), get_to_date());

		count_date_id =exec_query_get_single_value(count_date_id, query, DB_PACK);

		sprintf(date_id,"%s",count_date_id);
	}
	free(count_date_id);
	free(count_date_id1);
	char *cnt_muster_mid_e=NULL,*error;
	sprintf(query, "select ifnull(count(*),'0') from muster_id where work_code='%s' and  group_id='%s'and from_date='%s' and to_date='%s'",get_work_code(),get_group_id(),get_from_date(),get_to_date());

	cnt_muster_mid_e = exec_query_get_single_value(cnt_muster_mid_e, query, DB_PACK);

	if(atoi(cnt_muster_mid_e)==0)
	{
		sprintf(query, "insert into muster_id (work_code,group_id,muster_id,from_date,to_date)values('%s','%s','%s','%s','%s')",get_work_code(),get_group_id(),get_muster_id(),get_from_date(),get_to_date());
		int rc=	sqlite3_exec(dbhandle, query, NULL, NULL, &error);
		printf("rc value is %d\n",rc);
		if(rc!=0)
		{
			show_musterid_frame();
			show_message_frame("DATA MISSING in Muster_id Insert  ");
			return -1;
		}
	}

	printf("query before insert is %s\n", query);

	printf("\n date id is %s\n",date_id);

	printf("\n muster is %s\n",get_muster_id());

	sprintf(query, "insert into muster_attendance(jobcardno, member_name, group_name, muster_id, status, distance, work_code, crowbar, group_id, worker_type, Mate, Gpslongitude, Gpslatitude, skilledtype, from_date, to_date,Attendance_type,progress_code) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%lf', '%lf', '%s', '%s', '%s','%s','%s')", jobcardno, name, gname, get_muster_id(), "N", "N", get_work_code(), "N",get_group_id(), (strlen(jobcardno)>16)?"U":"S", "N", lon, lat, "U", get_from_date(), get_to_date(),"B",date_id);

	int ms = sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	 printf("ms save value is %d\n",ms);
                if(ms!=0)
                {
                        //show_musterid_frame();
                        show_message_frame("DATA MISSING Reopen FA ");
			 system("killall -9 FA");
                        return -1;
                }

	printf("\n query is %s\n",query);
	pw_tlistbox_insertend(lbox_identify_must_membs,name,0);
	pw_update(lbox_identify_must_membs,1);
	sqlite3_close(dbhandle);
	return 0;
}
int cb_identify_must_un_enrolled()
{

	char *not_enrolled_cnt=NULL;

	sprintf(query,"select count(*) from (select household_code||worker_code as Jobcard_id,Name from household_details  where is_active='Y' and group_name='%s' union select JobCardNo as Jobcard_Id,Member_Name as Name from tmp_group_members where group_name='%s' and Is_Active='Y') as a where jobcard_id not in (select nrega_id from wg_gpname_bio_info where grname='%s')",get_must_grpname,get_must_grpname,get_must_grpname);

	not_enrolled_cnt = exec_query_get_single_value(not_enrolled_cnt, query, DB_PACK);

	if(atoi(not_enrolled_cnt)==0)
	{
		pw_show(frame_identify_must_atten);
		show_message_frame("GO TO e-MUSTERS");
		return -1 ;
	}

	show_progress_frame("Processing Please Wait");

	free(not_enrolled_cnt);
	if(!frame_identify_must_photo)
	{
		create_frame_identify_must_photo();
		pw_show(frame_identify_must_photo);
	}
	else
		pw_show(frame_identify_must_photo);
	pw_hide(Butt_Bio_Identify);
	pw_hide(Butt_Bio_FP);
	pw_set_text(Butt_photo_Identify,"  Photo     musters-1");
	pw_set_text(label_identify_must_grp ,get_must_grpname);
	get_unenrolled_wgseekers(get_must_grpname,get_work_code());
	 hide_progress_frame();
	return 0;
}
int cb_identify_must_atten_photo()
{
	char *no_phto_cnt=NULL;
	sprintf(query,"select count(*) from (select household_code||worker_code as Jobcard_id,Name from household_details  where is_active='Y' and group_name='%s')  as a where jobcard_id not in (select jobcardno from muster_attendance where work_code='%s' and group_name='%s' and substr(created_date,1,10)=current_date)",get_must_grpname,get_work_code(),get_must_grpname);
	printf("photo capture jc is %s\n",query);
	no_phto_cnt = exec_query_get_single_value(no_phto_cnt, query, DB_PACK);

	if(atoi(no_phto_cnt)==0)
	{
		pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK, PW_MSGBOX_ICON_WARNING, "ALERT","NO JC PRESENT");
		pw_show(frame_identify_must_atten);
		return -1 ;
	}
	free(no_phto_cnt);
	if(!frame_identify_must_photo)
	{
		create_frame_identify_must_photo();
		pw_show(frame_identify_must_photo);
	}
	else
		pw_show(frame_identify_must_photo);
	pw_show(Butt_Bio_Identify);
	pw_show(Butt_Bio_FP);
	pw_set_text(Butt_photo_Identify,"  Photo    musters-2");
	pw_set_text(label_identify_must_grp ,get_must_grpname);
	photo_jc(get_must_grpname,get_work_code());
	return 0;
}
//To verify the wageseekers fingerprints based on groupselection
int mark_attendance_for_verified_person(char *jobcardno, char *name, char *gname)
{
	char *cnt_exists=NULL,*no_days_server=NULL,*wrked_days=NULL, *count_payorder=NULL,*no_days_pos_db=NULL;

	char hh_code[20],pos_days[10],tot_days_atten[10];

	memset(hh_code,'\0',sizeof(hh_code));

	memset(pos_days,'\0',sizeof(pos_days));

	memset(tot_days_atten,'\0',sizeof(tot_days_atten));

	strncpy(hh_code,jobcardno,18);

	printf("\n hh code is %s\n",hh_code);

	sprintf(query,"select ifnull(count(*),'0') from wage_info where jobcard_no='%s'",hh_code);
	cnt_exists=exec_query_get_single_value(cnt_exists, query, DB_PACK);

	if(atoi(cnt_exists)==0)
	{
		attendance(jobcardno);
		return 0;
	}
	else
	{
		sprintf(query,"select distinct no_days from wage_info where jobcard_no='%s'",hh_code);
		no_days_server=exec_query_get_single_value(no_days_server, query, DB_PACK);

		sprintf(query, "select ifnull(count(*),'0') from Wage_Payorder_Info where jobcardno='%s'",hh_code);
		count_payorder = exec_query_get_single_value(count_payorder, query, DB_PACK);

		if(atoi(count_payorder)>0)
		{
			sprintf(query,"select distinct worked_days from wage_payorder_info where jobcardno='%s'",hh_code);
			no_days_pos_db=exec_query_get_single_value(no_days_pos_db,query, DB_PACK);
			sprintf(pos_days,"%s",no_days_pos_db);
		}
		else
		{
			sprintf(pos_days,"%s","0");
		}


		if((atoi(no_days_server) >= 150)||(atoi(pos_days) >= 150))
		{
			msg[0]='\0';
			sprintf(msg,"150 Days Exceeded For\n %s",hh_code);	
			show_message_frame(msg);

			return 0;

		}
		else if(atoi(no_days_server)>=135)
		{

			sprintf(query, "select ifnull(count(*),'0') from Wage_Payorder_Info where jobcardno='%s'",hh_code);
			count_payorder = exec_query_get_single_value(count_payorder, query, DB_PACK);

			if(atoi(count_payorder)==0)
			{

				ret=attendance(jobcardno);
				printf("\n ret is %d\n",ret);
				if(ret==-1)
				{
					return -1;
				}
				sqlite3 *dbhandle=NULL;
				sqlite3_open(DB_PACK, &dbhandle);
				if (dbhandle == NULL)
					printf("Unable to open db\n");

				int total=atoi(no_days_server)+1;

				sprintf(tot_days_atten,"%d",total);			

				sprintf(query, "insert into Wage_Payorder_Info(jobcardno,Worked_days) values ('%s', '%s')",hh_code,tot_days_atten);
				sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

				sprintf(query,"select distinct Worked_days from wage_payorder_info where jobcardno='%s'",hh_code);
				wrked_days=exec_query_get_single_value(wrked_days, query, DB_PACK);

				msg[0]='\0';		
				sprintf(msg,"%s Days Completed\n%s",wrked_days,hh_code);
				printf("\n msg is %s\n",msg);
				show_message_frame(msg);
				free(wrked_days);
				sqlite3_close(dbhandle);
				return 0;			
			}
			else
			{

				ret=attendance(jobcardno);
				if(ret==-1)
				{
					return -1;
				}

				sqlite3 *dbhandle=NULL;
				sqlite3_open(DB_PACK, &dbhandle);
				if (dbhandle == NULL)
					printf("Unable to open db\n");
				sprintf(query, "update Wage_Payorder_Info set Worked_days=Worked_days+1 where jobcardno='%s'",hh_code);
				sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
				printf("\n update querry is %s\n",query);
				sprintf(query,"select distinct Worked_days from wage_payorder_info where jobcardno='%s'",hh_code);
				wrked_days=exec_query_get_single_value(wrked_days, query, DB_PACK);
				printf("\n wrked_days is %s\n",wrked_days);
				msg[0]='\0';
				sprintf(msg,"%s Days Completed\n%s",wrked_days,hh_code);
				show_message_frame(msg);
				free(wrked_days);
				sqlite3_close(dbhandle);
				return 0;
			}
		}
		else
		{
			attendance(jobcardno);
		}
		free(cnt_exists);
		free(no_days_server);
		free(count_payorder);
		free(no_days_pos_db);
	}

	return 0;
}
pw_timer timer;
int count=0, run=0;

int cb_identify_must_atten()
{
	unsigned long id;
	pw_set_text(lbl_msg, "Please Place Finger...");
	pw_update(lbl_msg, 1);

	printf("id before identify is %lu\n",id);
	id = identify_finger_print();
	printf("id value after identify is %lu",id);
	if(id > 0)
	{
		//		Modified by kr on 3rd Nov 2011
		sqlite3 *dbhandle=NULL;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
			printf("Unable to open db\n");
		sprintf(query, "select nrega_id,name,grname from wg_gpname_bio_info where sid = '%lu';",id);
		printf("query is %s\n", query);
		sqlite3_exec(dbhandle, query, get_jcdetails, NULL, NULL);
		mark_attendance_for_verified_person(nrega_id, name, gname);
		pw_show(lbox_identify_must_membs);
		sqlite3_close(dbhandle);
	}
	return 0;
}
Bool cb_fp_scan()
{
	cb_identify_must_atten();
	return True;
}
Bool cb_attendee_verify(pw_widget w, pw_event e)
{
	printf("inside verify\n");
	printf("\n run value is %d\n",run);
	if(!run)
	{
		fps = init_fps_in_iso();
		printf("\n entered into verify loop and run value is %d\n",run);
		run = 1;
		pw_set_text(Butt_Identify, "Stop");
		timer = pw_create_timer(100000, True, cb_fp_scan, NULL, NULL);
	}
	else
	{
		printf("ret of biouninti is %d\n",ret);
		printf("\n entered into stop loop and run value is %d\n",run);
		run = 0;
		bio_uninit(fps);

		pw_set_text(Butt_Identify, "e-Muster");
		pw_hide(lbl_msg);
		pw_destroy_timer(timer);
		pw_flush_eventloop();
	}
	return False;
}

pw_widget lbl_identity_100[1],frame_identify_100_execeed,label_identify_100_days,lbox_identify_100_days,Butt_photo_100_days_back;

void cb_100_days_exceded_back()
{
	hide_frame_identify_100_execeed();
	pw_show(frame_identify_must_atten);
}


void create_frame_identify_100_execeed()
{
	frame_identify_100_execeed =pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);

	pw_hide(frame_identify_100_execeed);

	lbl_identity_100[1] =  pw_create_label(frame_identify_100_execeed , 0, 05, 50, 25,
			PW_TEXT, "Group Name:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_identify_100_days =  pw_create_label(frame_identify_100_execeed , 50, 05, 189, 25,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbox_identify_100_days =pw_create_tlistbox(frame_identify_100_execeed,0,35, 239, 180,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);

	pw_tlistbox_addcolumn(lbox_identify_100_days, 150);

	Butt_photo_100_days_back = pw_create_button(frame_identify_100_execeed, 0, 225, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_100_days_exceded_back,
			PW_CONFIG_END);
	return;
}
void cb_identify_must_atten_100_exce()
{
	char *cnt_100_days=NULL;	

	sprintf(query,"select ifnull(count(*),'0') from (select household_code as Jobcard_id,Name from household_details  where is_active='Y' and group_name='%s') as a where jobcard_id in(select jobcard_no from wage_info where no_days>='150')",get_must_grpname);

	cnt_100_days=exec_query_get_single_value(cnt_100_days, query, DB_PACK);	

	if(atoi(cnt_100_days)==0)
	{
		show_message_frame("NO DATA");
		return ;
	}
	free(cnt_100_days);
	if(!frame_identify_100_execeed)
	{
		create_frame_identify_100_execeed();
		pw_show(frame_identify_100_execeed);
	}
	else
		pw_show(frame_identify_100_execeed);	

	pw_set_text(label_identify_100_days ,get_must_grpname);	
	pw_tlistbox_clear(lbox_identify_100_days);
	sprintf(query,"select JobCard_id as jobcard_id,name from (select substr(household_code,14,4) as Jobcard_id,Name from household_details  where is_active='Y' and group_name='%s') as a where jobcard_id in(select substr(jobcard_no,14,4) from wage_info where no_days>='150')",get_must_grpname);
	printf("\n query is %s\n",query);

	exec_query_load_into_lbox_double(lbox_identify_100_days, query, DB_PACK);

	pw_update(lbox_identify_100_days,1);

	return ;
}
void cb_identify_must_atten_back()
{
	uninit_fps(fps);
	hide_frame_identify_must_atten();
	get_grp_name_ntenrll()[0]='\0';
	get_must_grpname[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_muster_wktag_grps);
	pw_show(frame_get_muster);
	pw_update(frame_get_muster,1);
}
void create_frame_identify_must_atten(void)
{
	frame_identify_must_atten =pw_create_frame(toplevel, 0, 0, 240, 270, 
			PW_BGCOLOR,VIOLET , 
			PW_CONFIG_END);
	pw_hide(frame_identify_must_atten);
	label_identify_must_group =  pw_create_label(frame_identify_must_atten , 05, 00, 230, 25,
			PW_TEXT, "",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CONFIG_END);

	lbl_identity[0] =  pw_create_label(frame_identify_must_atten , 10, 26, 164, 15,
			PW_TEXT, "Attendees",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET ,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_msg = pw_create_label(frame_identify_must_atten,0, 120, 230,35,
			PW_TEXT, "",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "RED",
			PW_CONFIG_END);

	lbox_identify_must_membs =pw_create_tlistbox(frame_identify_must_atten,0,42, 239, 123,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_selection_intimate(lbox_identify_must_membs,lbox_identify_change);

	lbl_identity[1] =  pw_create_label(frame_identify_must_atten, 0, 170, 65, 20, 
			PW_TEXT, "CrowBar", 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET , 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	rb[3] = pw_create_radiobutton(frame_identify_must_atten, 
			70, 170, 60, 20, NULL, 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET , 
			PW_FGCOLOR, "white", 
			PW_TEXT, "Y", PW_NA);

	rb[4] = pw_create_radiobutton(frame_identify_must_atten, 
			160, 170, 60, 20, rb[3], 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "N", PW_NA);

	CrowButt =pw_create_button(frame_identify_must_atten, 95, 195, 65, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_TEXT, "Crowbar",
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "#3b3b3b", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_muster_crowbar_submit, 
			PW_BGCOLOR, "white", PW_CONFIG_END);

	Butt_Identify = pw_create_button(frame_identify_must_atten , 170, 195 , 70, 30, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER, 
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "#3b3b3b", 
			PW_TEXT, "e-Muster", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_attendee_verify, 
			PW_CONFIG_END);

	Butt_Identify_un_enroll = pw_create_button(frame_identify_must_atten , 95, 230, 62, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, "   NOT   Enrolled",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_identify_must_un_enrolled,
			PW_CONFIG_END);

	Butt_Identify_photo = pw_create_button(frame_identify_must_atten , 170, 230, 70, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, "  Photo  Muster",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_identify_must_atten_photo,
			PW_CONFIG_END);

	Butt_Identify_cnt_exceed = pw_create_button(frame_identify_must_atten , 10, 195, 75, 30,//150, 195 , 70, 30,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_TEXT, "150 Days Exceeded",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_identify_must_atten_100_exce,
			PW_CONFIG_END);


	Butt_Identify_must_back = pw_create_button(frame_identify_must_atten, 10, 230, 34, 27, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_identify_must_atten_back,
			PW_CONFIG_END);

	return;
}

void show_frame_identify_must_atten()
{
	if(!frame_identify_must_atten)
		create_frame_identify_must_atten();
	pw_show(frame_identify_must_atten);
	pw_tlistbox_clear(lbox_identify_must_membs);

	pw_update(lbox_identify_must_membs,1);
	printf("get_grp_name_ntenrll() is %s\n",get_grp_name_ntenrll());
	sprintf(get_must_grpname,"%s",get_grp_name_ntenrll());
	printf("groupname in iden =%s\n",get_must_grpname );

	sprintf(query,"select member_name from muster_attendance where group_name='%s' and substr(created_date,1,10)=current_date and work_code='%s' order by created_date desc",get_must_grpname,get_work_code());
	exec_query_load_into_lbox_single(lbox_identify_must_membs, query, DB_PACK);
//	pw_radiobutton_select(rb[4]);
	pw_set_text(label_identify_must_group,get_grp_name_ntenrll());
	return;
}

void hide_frame_identify_must_atten(void)
{
	pw_hide(frame_identify_must_atten);
	return;
}
void hide_frame_identify_must_photo(void)
{
	pw_hide(frame_identify_must_photo);
	return;
}
void hide_frame_identify_100_execeed(void)
{
	pw_hide(frame_identify_100_execeed);
	return ;
}
