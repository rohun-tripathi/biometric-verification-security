// the one being used cb_splash_screem not working

#include<pwrap2.h>

#include<sqlite3.h>

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"features_live.h"

char cust_user_id[20] = {"\0"};
Bool cb_entry_num(pw_widget w, pw_event e);
 Bool cb_entry_alpha(pw_widget w, pw_event e);

pw_widget toplevel,cust_reg_screen,cust_reg_butt,cust_back_butt, cust_reg_entry_name, cust_reg_entry_userid, cust_reg_entry_age, cust_reg_entry_gender, cust_reg_label[4],combo_reg_ins;

pw_widget cust_donereg_screen, cust_donereg_label;
void show_frame_home();
void display_wait_msg(char * text);
void hide_check_screen();

void cust_hide_reg_screen()
{
	pw_hide(cust_reg_screen);
	return;
}
void cust_show_frame_root3();
void cust_back_reg_page()
{
	cust_hide_reg_screen();
	cust_show_frame_root3();
}	
Bool cust_chng_reg_page()
{
	char cust_name[20] = {"\0"};
	char cust_age[20] = {"\0"};
	char cust_gender[20] = {"\0"};

	strcpy(cust_name,pw_get_text(cust_reg_entry_name));
	strcpy(cust_age,pw_get_text(cust_reg_entry_age));
	strcpy(cust_gender,pw_get_text(cust_reg_entry_gender));
	printf("\n:::cust age is :::::%s::::\n",cust_age);
	if(strcmp(cust_name,"")==0|| strcmp(cust_user_id,"")==0|| strcmp(cust_gender,"")==0|| strcmp(cust_age,"")==0)
	{
			pw_set_text(cust_reg_label,"please fill all and try again");
	}
	else
	{
			
////////////////sqlite stuff begins ////////////


	char query[512];
	int i =0;	
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	
	sprintf(query,"CREATE TABLE customers(name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	i=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"delete from customers where User_Id = '%s'",cust_user_id);
	printf("query is ::::::::::%s::::",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);


	sprintf(query,"insert into customers (user_id,name,age,gender)values ('%s', '%s', '%s', '%s')",cust_user_id,cust_name,cust_age,cust_gender);
	printf("query is ::::::::::%s::::",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);

/////////data work^ is done	

//////////exit this and return to home

	char text[30] = {"Registered Customer"};
	display_wait_msg(text);
	sleep(2); //add functionalities
	hide_check_screen();
	
	cust_hide_reg_screen();
	show_frame_home();
	}		 
	return False;
}

void cust_show_frame_register(char * cust_ext_userid)
{
	strcpy(cust_user_id,cust_ext_userid);
	cust_reg_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);
//submit the id 
	cust_reg_label[0] = pw_create_label(cust_reg_screen, 25, 5, 200, 40,
			PW_TEXT,"Enter Personal Details(To Be Enrolled)",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);

	cust_reg_label[1] = pw_create_label(cust_reg_screen, 30, 50, 70, 20,
			PW_TEXT,"Name: ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	cust_reg_label[2] = pw_create_label(cust_reg_screen, 30, 80, 70, 20,
			PW_TEXT,"Age: ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	cust_reg_label[3] = pw_create_label(cust_reg_screen, 30, 110, 70, 20,
			PW_TEXT,"Gender:",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	cust_reg_entry_name = pw_create_entry(cust_reg_screen,115, 50,90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_alpha,
			PW_CONFIG_END);
	
	cust_reg_entry_age = pw_create_entry(cust_reg_screen,115, 80,90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	
	cust_reg_entry_gender = pw_create_entry(cust_reg_screen,115,110,90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_alpha,
			PW_CONFIG_END);

	cust_reg_butt = pw_create_button(cust_reg_screen,25, 160, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cust_chng_reg_page,
                        PW_CONFIG_END);

	cust_back_butt = pw_create_button(cust_reg_screen,25, 190, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cust_back_reg_page,
                        PW_CONFIG_END);
	return;
}
