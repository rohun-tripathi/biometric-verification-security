//introducing sqlite
#include<sqlite3.h>

#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

//#include "funct2.c"
#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif


pw_widget toplevel, enroll_screen ,enroll_butt_tryag, enroll_butt_home , rb_benificary_enroll, enroll_label;

void show_frame_register();

//////////declaring functions already in header is not necessary

///// to move to next frame
void hide_enroll_screen()
{
	pw_hide(enroll_screen);
}

void show_frame_home();

int keeper_enroll(int fin);
	
///// for biometric scan enrollment
int scan_template(char *fname);

void cb_register_page()
{
	hide_enroll_screen();
	show_frame_register();
	return;
}
Bool cb_entry_num(pw_widget w, pw_event e);
void keeper_enroll_bio()
{
	int retval = keeper_enroll(1);
	
	printf("ret value is= %d\n", retval);

	if(retval == 0)
	{
		//enroll_flag[i] = ENROLLED;
		pw_set_bgcolor(rb_benificary_enroll, "green");
		cb_register_page();
		//if(validate_mandatory_enroll_flag() == 1)
		//	pw_enable(Butt_beneficary_enroll);
	}
	else
	{
		//enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(rb_benificary_enroll, VIOLET);
		//pw_disable(Butt_beneficary_enroll);
	}
}



Bool cb_enroll_screen()
{
	hide_enroll_screen();
	show_frame_home();
	return False;
}

Bool cb_try_again()
{
	pw_set_text(enroll_label,"please try again");
	pw_radiobutton_deselect(rb_benificary_enroll);
	pw_set_bgcolor(rb_benificary_enroll, GREY);
	return False;
		
}
		
void show_frame_enroll()
{
	
////////////////sqlite stuff


	char query[512];
	int i =0;	
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	
	sprintf(query,"CREATE TABLE shop_keepers(name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	
	i=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

//	this one is useless as the i will always get a 1 value at completion of query
//	printf("::::::::::::::::::%d::::::::::::::::::",i);




//page graphics begin
	enroll_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	enroll_label = pw_create_label(enroll_screen, 20, 5, 200, 40,
			PW_TEXT,"Choose radibutton and press thumb to sensor",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
                        PW_BGCOLOR, GREY ,
			PW_CONFIG_END);
	
	enroll_butt_tryag = pw_create_button(enroll_screen, 50, 80, 50,30,
                        PW_ACTIVE_IMAGE, "tryag.png",
                        PW_NORMAL_IMAGE, "tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again,
                        PW_CONFIG_END);

	enroll_butt_home = pw_create_button(enroll_screen, 50, 120, 50,30,
                        PW_ACTIVE_IMAGE, "home.png",
                        PW_NORMAL_IMAGE, "home..png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_enroll_screen,
                        PW_CONFIG_END);
	rb_benificary_enroll = pw_create_radiobutton(enroll_screen,50, 40, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_enroll_bio,
			PW_NA);

	return ;


}
