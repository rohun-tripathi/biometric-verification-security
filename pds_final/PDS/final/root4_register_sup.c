// the one being used cb_splash_screem not working

#include<pwrap2.h>

#include<sqlite3.h>

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"features_live.h"

char sup_user_id[20] = {"\0"};
Bool cb_entry_num(pw_widget w, pw_event e);
 Bool cb_entry_alpha(pw_widget w, pw_event e);

pw_widget toplevel,sup_reg_screen,sup_reg_butt,sup_back_butt, sup_reg_entry_name, sup_reg_entry_userid, sup_reg_entry_age, sup_reg_entry_gender, sup_reg_label[4];

pw_widget sup_donereg_screen, sup_donereg_label;
void show_frame_home();
void display_wait_msg(char * text);
void hide_check_screen();

void sup_hide_reg_screen()
{
	pw_hide(sup_reg_screen);
	return;
}
void sup_show_frame_root3();

void sup_back_reg_page()
{
	sup_hide_reg_screen();
	sup_show_frame_root3();
}	
Bool sup_chng_reg_page()
{
	char sup_name[20] = {"\0"};
	char sup_age[20] = {"\0"};
	char sup_gender[20] = {"\0"};

	strcpy(sup_name,pw_get_text(sup_reg_entry_name));
	strcpy(sup_age,pw_get_text(sup_reg_entry_age));
	strcpy(sup_gender,pw_get_text(sup_reg_entry_gender));
	printf("\n:::sup age is :::::%s::::\n",sup_age);
	if(strcmp(sup_name,"")==0|| strcmp(sup_user_id,"")==0|| strcmp(sup_gender,"")==0|| strcmp(sup_age,"")==0)
	{
			pw_set_text(sup_reg_label,"please fill all and try again");
	}
	else
	{
			
////////////////sqlite stuff begins ////////////


	char query[512];
	int i =0;	
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	
	sprintf(query,"CREATE TABLE suppliers(name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	i=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"delete from suppliers where User_Id = '%s'",sup_user_id);
	printf("query is ::::::::::%s::::",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);


	sprintf(query,"insert into suppliers (user_id,name,age,gender)values ('%s', '%s', '%s', '%s')",sup_user_id,sup_name,sup_age,sup_gender);
	printf("query is ::::::::::%s::::",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);

/////////data work^ is done	

//////////exit this and return to home

	char text[30] = {"Registered Supplier"};
	display_wait_msg(text);
	sleep(2); //add functionalities
	hide_check_screen();
	
	sup_hide_reg_screen();
	show_frame_home();
	}		 
	return False;
}

void sup_show_frame_register(char * sup_ext_userid)
{
	strcpy(sup_user_id,sup_ext_userid);
	sup_reg_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);
//submit the id 
	sup_reg_label[0] = pw_create_label(sup_reg_screen, 25, 5, 200, 40,
			PW_TEXT,"Enter Personal Details(To Be Enrolled)",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);

	sup_reg_label[1] = pw_create_label(sup_reg_screen, 30, 50, 70, 20,
			PW_TEXT,"Name: ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	sup_reg_label[2] = pw_create_label(sup_reg_screen, 30, 80, 70, 20,
			PW_TEXT,"Age: ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	sup_reg_label[3] = pw_create_label(sup_reg_screen, 30, 110, 70, 20,
			PW_TEXT,"Gender:",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	sup_reg_entry_name = pw_create_entry(sup_reg_screen,115, 50,90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_alpha,
			PW_CONFIG_END);
	
	sup_reg_entry_age = pw_create_entry(sup_reg_screen,115, 80,90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	
	sup_reg_entry_gender = pw_create_entry(sup_reg_screen,115,110,90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_alpha,
			PW_CONFIG_END);

	sup_reg_butt = pw_create_button(sup_reg_screen,25, 160, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, sup_chng_reg_page,
                        PW_CONFIG_END);

	sup_back_butt = pw_create_button(sup_reg_screen,25, 190, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, sup_back_reg_page,
                        PW_CONFIG_END);
	return;
}
