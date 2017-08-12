// the 3rd in the sequence for the root thing used for shopkeeper first
// this comes from root2 and to root4
// this takes the user id of the person and enrolls him, much like in the verification page.

#include<sqlite3.h>
#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>
#include<string.h>
#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif

char userid[20];
pw_widget toplevel, root3_screen ,butt_root3_try, butt_root3_home;
pw_widget rb_benificary_root3, root3_label[3], root3_entry;

void show_frame_home();
void show_frame_register(char * userid);

void hide_root3_screen()
{
	pw_hide(root3_screen);
}

void cb_register_root3(char * userid)
{
	hide_root3_screen();
	show_frame_register(userid);
	return;
}
//enrollment part
int keeper_enroll_root3_extra(char* userid);
void keeper_enroll_root3()
{
	strcpy(userid,pw_get_text(root3_entry));
	
	int retval = keeper_enroll_root3_extra(userid);
	printf("ret value is= %d\n", retval);

	if(retval == 0)
	{
		//enroll_flag[i] = ENROLLED;
		pw_set_bgcolor(rb_benificary_root3, "green");
		cb_register_root3(userid);
		sleep(1);		
		//if(validate_mandatory_enroll_flag() == 1)
		//	pw_enable(Butt_beneficary_enroll);
	}
	else
	{
		//enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(rb_benificary_root3, VIOLET);
		//pw_disable(Butt_beneficary_enroll);
	}
}

//enrollment done

Bool cb_entry_num(pw_widget w, pw_event e);

Bool cb_try_again_root3()
{
	pw_set_text(root3_label,"please try again");
	pw_radiobutton_deselect(rb_benificary_root3);
	pw_set_bgcolor(rb_benificary_root3, GREY);
	return False;
}

Bool cb_root3_home()
{
	hide_root3_screen;
	show_frame_home();
	return False;
}

void show_frame_root3()
{
//page graphics begin
	root3_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	root3_label[0] = pw_create_label(root3_screen, 15, 10, 240, 20,
			PW_TEXT,"Shopkeeper Enrollment",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);
	
	root3_label[1] = pw_create_label(root3_screen, 20, 60, 120, 40,
			PW_TEXT,"Shopkeeper User ID : ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);


	root3_label[2] = pw_create_label(root3_screen, 20, 120, 100, 40,
			PW_TEXT,"Shopkeeper Biometric enrollment :",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);

	root3_entry = pw_create_entry(root3_screen, 140, 60, 70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, GREY,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	rb_benificary_root3 = pw_create_radiobutton(root3_screen,160, 120, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_enroll_root3,
			PW_NA);

	butt_root3_try = pw_create_button(root3_screen, 90, 180, 80,20,
                        PW_ACTIVE_IMAGE, "tryag.png",
                        PW_NORMAL_IMAGE, "tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again_root3,
                        PW_CONFIG_END);

///////////////////////////////////////////we need to change the functionality of the button under here later ryt now it takes to home
	butt_root3_home = pw_create_button(root3_screen, 15, 180, 50,18,
                        PW_ACTIVE_IMAGE, "home.png",
                        PW_NORMAL_IMAGE, "home.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_root3_home,
                        PW_CONFIG_END);

	return ;
}
