// the 3rd in the sequence for the root thing used for inspectors enrollment first
// this comes from root2 and to root4_ins
// this takes the user id of the person and enrolls him, much like in the verification page.

#include<sqlite3.h>
#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>
#include<string.h>
#include"features_live.h"

#include<unistd.h>		//contains the sleep function. used only for that
#ifdef __arm__
#include <biometric.h>
#endif

char ins_userid[20];
pw_widget toplevel, ins_root3_screen ,ins_butt_root3_try, ins_butt_root3_back;
pw_widget ins_rb_benificary_root3, ins_root3_label[3], ins_root3_entry;

void show_frame_home();
void ins_show_frame_register(char * ins_userid);
void show_frame_opt();

void ins_hide_root3_screen()
{
	pw_hide(ins_root3_screen);
}

void ins_cb_register_root3(char * userid)
{
	ins_hide_root3_screen();
	ins_show_frame_register(userid);
	return;
}
//enrollment part
int keeper_enroll_root3_extra(char* userid, char * name);
void ins_keeper_enroll_root3()
{
	strcpy(ins_userid,pw_get_text(ins_root3_entry));

	char ins_str[20] = {"inspector"};

	int ins_retval = keeper_enroll_root3_extra(ins_userid,ins_str);
	printf("ret value is= %d\n", ins_retval);

	if(ins_retval == 0)
	{
		//enroll_flag[i] = ENROLLED;
		pw_set_bgcolor(ins_rb_benificary_root3, "green");
		ins_cb_register_root3(ins_userid);
		//if(validate_mandatory_enroll_flag() == 1)
		//	pw_enable(Butt_beneficary_enroll);
	}
	else
	{
		//enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(ins_rb_benificary_root3, VIOLET);
		sleep(1);
		pw_radiobutton_deselect(ins_rb_benificary_root3);
		//pw_disable(Butt_beneficary_enroll);
	}
}

//enrollment done

Bool cb_entry_num(pw_widget w, pw_event e);

Bool ins_cb_try_again_root3()
{
	pw_set_text(ins_root3_label,"Please Try Again");
	pw_radiobutton_deselect(ins_rb_benificary_root3);
	pw_set_bgcolor(ins_rb_benificary_root3, GREY);
	return False;
}

Bool ins_cb_root3_back()
{
	ins_hide_root3_screen();
	show_frame_opt();
	return False;
}

void ins_show_frame_root3()
{
//page graphics begin
	ins_root3_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	ins_root3_label[0] = pw_create_label(ins_root3_screen, 15, 10, 240, 40,
			PW_TEXT,"Inspectors Enrollment Page",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);
	
	ins_root3_label[1] = pw_create_label(ins_root3_screen, 20, 55, 120, 40,
			PW_TEXT,"Inspectors User ID: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);


	ins_root3_label[2] = pw_create_label(ins_root3_screen,110, 120, 100, 40,
			PW_TEXT,"Inspectors Biometric enrollment:",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);

	ins_root3_entry = pw_create_entry(ins_root3_screen, 140, 60, 70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, GREY,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	ins_rb_benificary_root3 = pw_create_radiobutton(ins_root3_screen,50, 120, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, ins_keeper_enroll_root3,
			PW_NA);

	ins_butt_root3_try = pw_create_button(ins_root3_screen, 110, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/tryag.png",
                        PW_NORMAL_IMAGE, "/root/PDS/tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, ins_cb_try_again_root3,
                        PW_CONFIG_END);

///////////////////////////////////////////we need to change the functionality of the button under here later ryt now it takes to home
	ins_butt_root3_back = pw_create_button(ins_root3_screen, 15, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, ins_cb_root3_back,
                        PW_CONFIG_END);

	return ;
}
