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


pw_widget toplevel, enroll_screen ,enroll_butt_tryag, enroll_butt_home, enroll_label; 
pw_widget rb_benificary_enroll[3], shpkpr_no_label[3];
pw_widget enroll_cscreen ,enroll_cbutt_tryag, enroll_cbutt_home,rb_benificary_cenroll, enroll_clabel;
	
///// for biometric scan enrollment
int scan_template(char *fname);
void show_frame_register();
void show_frame_home();
int keeper_enroll(int fin);

void hide_enroll_screen()
{
	pw_hide(enroll_screen);
}
void cb_register_page()
{
	hide_enroll_screen();
	show_frame_register();
	return;
}
void keeper_enroll_bio()
{
	int i;
	for(i=0; i<3; i++)		//used to retrive which finger has called this
	{
		if(pw_radiobutton_status(rb_benificary_enroll[i]))
		{
			printf("Selected saleperson = %d \n", i+1);
			break;
		}
	}	

	int retval = keeper_enroll(i);
	printf("ret value is= %d\n", retval);

	if(retval == 0)
	{
		//enroll_flag[i] = ENROLLED;
		pw_set_bgcolor(rb_benificary_enroll[i], "green");
		cb_register_page();
		//if(validate_mandatory_enroll_flag() == 1)
		//	pw_enable(Butt_beneficary_enroll);
	}
	else
	{
		//enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(rb_benificary_enroll[i], VIOLET);
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
	int i = 0;
	for (i=0;i<3;i++)
	{
		pw_radiobutton_deselect(rb_benificary_enroll[i]);
		pw_set_bgcolor(rb_benificary_enroll[i], GREY);
	}
	return False;
		
}
		
void show_frame_enrollkpr()
{

////////////////sqlite stuff


	char query[512];
	int j =0;	
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	
	sprintf(query,"CREATE TABLE shop_keepers(name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	
	j=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

//	this one is useless as the i will always get a 1 value at completion of query
//	printf("::::::::::::::::::%d::::::::::::::::::",i);

//page graphics begin
	enroll_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	enroll_label = pw_create_label(enroll_screen, 20, 5, 200, 40,
			PW_TEXT,"Choose radiobutton and press thumb to sensor",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
                        PW_BGCOLOR, GREY ,
			PW_CONFIG_END);
	
	enroll_butt_tryag = pw_create_button(enroll_screen, 20, 160, 75,25,
                        PW_ACTIVE_IMAGE, "tryag.png",
                        PW_NORMAL_IMAGE, "tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again,
                        PW_CONFIG_END);

	enroll_butt_home = pw_create_button(enroll_screen, 20, 195, 50,18,
                        PW_ACTIVE_IMAGE, "home.png",
                        PW_NORMAL_IMAGE, "home.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_enroll_screen,
                        PW_CONFIG_END);

	rb_benificary_enroll[0] = pw_create_radiobutton(enroll_screen,50, 50, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_enroll_bio,
			PW_NA);
	shpkpr_no_label[0] = pw_create_label(enroll_screen, 25, 50, 200, 40,
			PW_TEXT,"1: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
                        PW_BGCOLOR, GREY ,
			PW_CONFIG_END);

	rb_benificary_enroll[1] = pw_create_radiobutton(enroll_screen,50, 75, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_enroll_bio,
			PW_NA);
	shpkpr_no_label[1] = pw_create_label(enroll_screen, 25, 75, 200, 40,
			PW_TEXT,"2: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
                        PW_BGCOLOR, GREY ,
			PW_CONFIG_END);	

	rb_benificary_enroll[2] = pw_create_radiobutton(enroll_screen,50, 100, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_enroll_bio,
			PW_NA);
	shpkpr_no_label[2] = pw_create_label(enroll_screen, 25, 100, 200, 40,
			PW_TEXT,"3: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
                        PW_BGCOLOR, GREY ,
			PW_CONFIG_END);
	return;
}


void show_frame_enrollcust()
{
/*	
////////////////sqlite stuff

	char query[512];
	int i =0;	
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	
	sprintf(query,"CREATE TABLE customers(name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	
	i=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

//	this one is useless as the i will always get a 1 value at completion of query
//	printf("::::::::::::::::::%d::::::::::::::::::",i);


//page graphics begin
	enroll_cscreen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	enroll_clabel = pw_create_label(enroll_screen, 20, 5, 200, 40,
			PW_TEXT,"Choose radibutton and press thumb to sensor",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
                        PW_BGCOLOR, GREY ,
			PW_CONFIG_END);
	
	enroll_cbutt_tryag = pw_create_button(enroll_screen, 20, 90, 75,25,
                        PW_ACTIVE_IMAGE, "tryag.png",
                        PW_NORMAL_IMAGE, "tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again,
                        PW_CONFIG_END);

	enroll_cbutt_home = pw_create_button(enroll_screen, 20, 130, 50,18,
                        PW_ACTIVE_IMAGE, "home.png",
                        PW_NORMAL_IMAGE, "home.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_enroll_screen,
                        PW_CONFIG_END);
	rb_benificary_cenroll = pw_create_radiobutton(enroll_screen,50, 50, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_enroll_bio,
			PW_NA);

	*/return;
}
