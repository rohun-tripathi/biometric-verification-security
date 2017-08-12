// the 3rd in the sequence for the root thing used for customer enrollment first
// this comes from root2 and to root4_cust
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

char cust_userid[20];
pw_widget toplevel, cust_root3_screen ,cust_butt_root3_try, cust_butt_root3_back;
pw_widget cust_rb_benificary_root3, cust_root3_label[3], cust_root3_entry;

void show_frame_home();
void cust_show_frame_register(char * cust_userid);
void show_frame_opt();

void cust_hide_root3_screen()
{
	pw_hide(cust_root3_screen);
}

void cust_cb_register_root3(char * userid)
{
	cust_hide_root3_screen();
	cust_show_frame_register(userid);
	return;
}
//enrollment part
int keeper_enroll_root3_extra(char* userid, char * name);
void cust_keeper_enroll_root3()
{
	strcpy(cust_userid,pw_get_text(cust_root3_entry));

	char custstr[20] = {"customer"};

	int cust_retval = keeper_enroll_root3_extra(cust_userid,custstr);
	printf("ret value is= %d\n", cust_retval);

	if(cust_retval == 0)
	{
		//enroll_flag[i] = ENROLLED;
		pw_set_bgcolor(cust_rb_benificary_root3, "green");
		cust_cb_register_root3(cust_userid);
		//if(validate_mandatory_enroll_flag() == 1)
		//	pw_enable(Butt_beneficary_enroll);
	}
	else
	{
		//enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(cust_rb_benificary_root3, VIOLET);
		sleep(1);
		pw_radiobutton_deselect(cust_rb_benificary_root3);
		//pw_disable(Butt_beneficary_enroll);
	}
}

//enrollment done

Bool cb_entry_num(pw_widget w, pw_event e);

Bool cust_cb_try_again_root3()
{
	pw_set_text(cust_root3_label,"Please Try Again");
	pw_radiobutton_deselect(cust_rb_benificary_root3);
	pw_set_bgcolor(cust_rb_benificary_root3, GREY);
	return False;
}

Bool cust_cb_root3_back()
{
	cust_hide_root3_screen();
	show_frame_opt();
	return False;
}

void cust_show_frame_root3()
{
//page graphics begin
	cust_root3_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	cust_root3_label[0] = pw_create_label(cust_root3_screen, 15, 10, 240, 40,
			PW_TEXT,"Customer Enrollment Page",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);
	
	cust_root3_label[1] = pw_create_label(cust_root3_screen, 20, 55, 120, 40,
			PW_TEXT,"Customer User ID: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);


	cust_root3_label[2] = pw_create_label(cust_root3_screen, 110, 120, 100, 40,
			PW_TEXT,"Customer Biometric enrollment:",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);

	cust_root3_entry = pw_create_entry(cust_root3_screen, 140, 60, 70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, GREY,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	cust_rb_benificary_root3 = pw_create_radiobutton(cust_root3_screen,50, 120, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cust_keeper_enroll_root3,
			PW_NA);

	cust_butt_root3_try = pw_create_button(cust_root3_screen, 110, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/tryag.png",
                        PW_NORMAL_IMAGE, "/root/PDS/tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cust_cb_try_again_root3,
                        PW_CONFIG_END);

///////////////////////////////////////////we need to change the functionality of the button under here later ryt now it takes to home
	cust_butt_root3_back = pw_create_button(cust_root3_screen, 15, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cust_cb_root3_back,
                        PW_CONFIG_END);

	return ;
}
