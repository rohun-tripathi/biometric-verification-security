//introducing sqlite
#include<sqlite3.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pwrap2.h>

//#include "funct2.c"
#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif


pw_widget toplevel, root_screen ,root_submit_button, root_user_entry, root_pass_entry, root_label;

Bool cb_entry_num(pw_widget w, pw_event e);


//////////declaring functions already in header is not necessary

///// to move to next frame
void hide_root_screen()
{
	pw_hide(root_screen);
}
void show_frame_enroll();


Bool chng_root_page()
{
	char name[20];
	char pass[20];

	strcpy(name,pw_get_text(root_user_entry));
	strcpy(pass,pw_get_text(root_pass_entry));
	

	if((atoi(name)==1) && atoi(pass)==1)
	{
		hide_root_screen();
		show_frame_enroll();
	}

	else
	{
		pw_set_text(root_label,"please try again");
	}		 
	
	return False;
}


void show_frame_root()
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
	root_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);
	//submit the pass andd user 
	root_submit_button = pw_create_button(root_screen,55, 90, 50,20,
                        PW_ACTIVE_IMAGE, "ADD.png",
                        PW_NORMAL_IMAGE, "ADD.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, chng_root_page,
                        PW_CONFIG_END);
	
	root_user_entry = pw_create_entry(root_screen,55, 30,70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	root_pass_entry = pw_create_entry(root_screen,55, 55,70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	root_label = pw_create_label(root_screen, 55, 5, 160, 20,
			PW_TEXT,"Enter ur id",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	return ;
}
