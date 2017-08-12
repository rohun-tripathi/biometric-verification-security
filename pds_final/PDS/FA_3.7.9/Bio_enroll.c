#include "Bio-enroll.h"
#include "types.h"

pw_widget frame_bioenrollpage,  rb_benificary_fin[10] , label_benificary_frame , Butt_beneficary_enrollfp , Butt_beneficary_enroll , Butt_beneficary_back ;


void show_frame_beneficary_details();

void show_progress_frame();
void hide_progress_frame();
void show_frame_optionspage();
void hide_frame_bioenrollpage();

int insert_template_into_db();
	 int get_birs_del();
char *exec_query_get_single_value(char *data, char *query, char *database);

char *get_login_id();
char *enroll_name();
void cb_bio_enroll_back()
{
	hide_frame_bioenrollpage();	
	show_frame_beneficary_details(TAG_ENROLL, "Next");
	return;
}
char selected_job[30];

int enroll_insert()
{
	char query[1024];
	sqlite3 *dbhandle=NULL;
        sqlite3_open(DB_PACK, &dbhandle);
        if (dbhandle == NULL)
                printf("Unable to open db\n");

	sprintf(query,"insert into enrollment_master (jobcard_no,name,enroll_status,authorized_person) values('%s','%s','N','%s')",selected_job,enroll_name(),get_login_id());	
	 sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	 sqlite3_close(dbhandle);

	return 0;
}
void cb_enroll_into_db()
{
	insert_template_into_db(selected_job);
	printf("login id is %s\n",get_login_id());
	printf("name is %s\n",enroll_name());
	hide_frame_bioenrollpage();
	pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
			PW_MSGBOX_ICON_NOICON, "SUCCESS", "Enrollment Success");
	pw_update(frame_beneficary_details,1);
	enroll_insert();
	char *tot_bir_cnt=NULL,query[125];
	sprintf(query,"select ifnull(count(*),'0') from wg_gpname_bio_info");
        tot_bir_cnt =exec_query_get_single_value(tot_bir_cnt, query, DB_PACK);
        if(atoi(tot_bir_cnt)>0)
        {
	 get_birs_del();
	}
	free(tot_bir_cnt);
	pw_show(frame_beneficary_details);
	return ;
}
int scan_template(char *fname);

int scan_template_for_selected_finger(int fin)
{
	char fname[256];

#ifdef __AMIDA__
	sprintf(fname, "/tmp/%s_%d.bir",selected_job,fin);
	return(scan_template(fname));
#else
	return(0);
#endif
}

void reset_enroll_flag()
{
	int i;
	for(i=0; i<8; i++)
	{
		enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(rb_benificary_fin[i], VIOLET);
	}	return;
}
int validate_mandatory_enroll_flag()
{
	int i, count = 0;
	for(i=0; i<8; i++)
		if(enroll_flag[i] == NOT_ENROLLED)
			count++;
	printf("count in scanning %d\n",count);
	if(count <= 3){
		pw_enable(Butt_beneficary_enroll);                
		pw_redraw(Butt_beneficary_enroll);
	}                
	return(-1);

	return(1);
}
void cb_bio_enroll_enroll_fp()
{
	int i, retval;
	for(i=0; i<8; i++)		//used to retrive which finger has called this
	{
		if(pw_radiobutton_status(rb_benificary_fin[i]))
		{
			printf("Selected fin = %d \n", i+1);
			break;
		}
	}
	printf("\n fing i value is= %d\n", i);
	
	// control to  	
	retval = scan_template_for_selected_finger(i);

	printf("ret value is= %d\n", retval);

	if(retval == 0)
	{
		enroll_flag[i] = ENROLLED;
		pw_set_bgcolor(rb_benificary_fin[i], "green");
		if(validate_mandatory_enroll_flag() == 1)
			pw_enable(Butt_beneficary_enroll);
	}
	else
	{
		enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(rb_benificary_fin[i], VIOLET);
		pw_disable(Butt_beneficary_enroll);
	}
}
void create_frame_bioenrollpage(void)
{
	frame_bioenrollpage = pw_create_frame(toplevel, 0, 0, 240, 270, 
			PW_BGCOLOR, VIOLET , 
			PW_CONFIG_END);

	pw_hide(frame_bioenrollpage);

	label_benificary_frame = pw_create_label(frame_bioenrollpage, 0, 10, 240, 210, 
			PW_BGIMAGE_FILE, BUTT_FINGER, 
			PW_CONFIG_END);

	Butt_beneficary_enroll = pw_create_button(frame_bioenrollpage, 170, 230, 60, 30, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "#3b3b3b", 
			PW_TEXT, "Save", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_enroll_into_db, 
			PW_CONFIG_END);

	Butt_beneficary_back = pw_create_button(frame_bioenrollpage, 10, 230, 34, 27, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK, 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_back, 
			PW_CONFIG_END);

	rb_benificary_fin[0] = pw_create_radiobutton(frame_bioenrollpage,100, 30, 20, 20, NULL, 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_enroll_fp,
			PW_NA);

	rb_benificary_fin[1] = pw_create_radiobutton(frame_bioenrollpage, 70, 30, 20, 20,
			rb_benificary_fin[0], 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_enroll_fp,
			PW_NA);

	rb_benificary_fin[2] = pw_create_radiobutton(frame_bioenrollpage,45, 30, 20, 20,
			rb_benificary_fin[0], 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_enroll_fp,
			PW_NA);

	rb_benificary_fin[3] = pw_create_radiobutton(frame_bioenrollpage,20, 30, 20, 20,
			rb_benificary_fin[0], 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_enroll_fp,
			PW_NA);
	rb_benificary_fin[4] = pw_create_radiobutton(frame_bioenrollpage, 125, 30, 20, 20,
			rb_benificary_fin[0], 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_enroll_fp,
			PW_NA);

	rb_benificary_fin[5] = pw_create_radiobutton(frame_bioenrollpage,152, 30, 20, 20,
			rb_benificary_fin[0], 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_enroll_fp,
			PW_NA);

	rb_benificary_fin[6] = pw_create_radiobutton(frame_bioenrollpage,175, 30, 20, 20,
			rb_benificary_fin[0], 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_enroll_fp,
			PW_NA);

	rb_benificary_fin[7] = pw_create_radiobutton(frame_bioenrollpage,200, 30, 20, 20,
			rb_benificary_fin[0], 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "", 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_bio_enroll_enroll_fp,
			PW_NA);
	return;
}

int show_frame_bioenrollpage(char *jobcardno)
{
	if(! frame_bioenrollpage)
	{
		create_frame_bioenrollpage();
		pw_show(frame_bioenrollpage);
	}
	else
		pw_show(frame_bioenrollpage);

	pw_update(frame_bioenrollpage, 1);
	pw_raise(frame_bioenrollpage);
	pw_disable(Butt_beneficary_enroll);
	reset_enroll_flag();

	strcpy(selected_job, jobcardno);
	printf("\n selected job card is %s\n",selected_job);
	return (0);
}
void hide_frame_bioenrollpage(void)
{
	pw_hide(frame_bioenrollpage);
}

