//introducing sqlite
#include<sqlite3.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pwrap2.h>
#include<unistd.h>

//#include "funct2.c"
#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif


pw_widget toplevel, root_screen ,root_submit_button, root_user_entry, root_pass_entry, root_label, root_label_user, root_label_pass, root_back_button;

Bool cb_entry_num(pw_widget w, pw_event e);


//////////declaring functions already in header is not necessary

///// to move to next frame
void hide_root_screen()
{
	pw_hide(root_screen);
}
void show_frame_enroll();
void show_frame_home();


Bool chng_root_page()
{
	char name[20];
	char pass[20];

	strcpy(name,pw_get_text(root_user_entry));
	strcpy(pass,pw_get_text(root_pass_entry));
	

	if(atoi(name)==1234)
	{
		if (atoi(pass)==1234)
		{
		hide_root_screen();
		show_frame_opt();
		return False;
		}
		else 
		{
			pw_set_text(root_label,"Incorrect Password: ");
			pw_entry_clear(root_pass_entry);
			return False;
		}
	}
	else
	{
		pw_entry_clear(root_pass_entry);
		pw_set_text(root_label,"Incorrect Username: ");
	}		 
	
	return False;
}
void back_root_page()
{
		hide_root_screen();
		show_frame_home();
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
	root_label = pw_create_label(root_screen, 20, 15, 200, 30,
			PW_TEXT,"Enter Credentials: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);	

	root_label_user = pw_create_label(root_screen, 20, 45, 100, 20,
			PW_TEXT,"Username: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	root_label_pass = pw_create_label(root_screen, 20, 75, 100, 20,
			PW_TEXT,"Password: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);

	root_user_entry = pw_create_entry(root_screen,110, 45,100, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	root_pass_entry = pw_create_entry(root_screen,110, 75,100, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	pw_entry_passwdchar( root_pass_entry, '*' );
	pw_focus(root_pass_entry);	
	pw_entry_clear(root_pass_entry);

	root_submit_button = pw_create_button(root_screen,20, 140, 70,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, chng_root_page,
                        PW_CONFIG_END);

	root_back_button = pw_create_button(root_screen,20, 170, 70,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, back_root_page,
                        PW_CONFIG_END);
	
	return ;
}
