// the one being used cb_splash_screem not working

#include<pwrap2.h>

#include<sqlite3.h>

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"features_live.h"

char user_id[20] = {"\0"};
Bool cb_entry_num(pw_widget w, pw_event e);
 

pw_widget toplevel,reg_screen,reg_butt, reg_entry_name, reg_entry_userid, reg_entry_age, reg_entry_gender, reg_label;

pw_widget donereg_screen, donereg_label;
void show_frame_home();
void display_wait_msg(char * text);
void hide_check_screen();

void hide_reg_screen()
{
	pw_hide(reg_screen);
	return;
}


Bool chng_reg_page()
{
	char name[20] = {"\0"};
	char age[20] = {"\0"};
	char gender[20] = {"\0"};

	strcpy(name,pw_get_text(reg_entry_name));
	strcpy(age,pw_get_text(reg_entry_age));
	strcpy(gender,pw_get_text(reg_entry_gender));
	printf("\n::::::::%s::::\n",age);
	if(strcmp(name,"")==0|| strcmp(user_id,"")==0|| strcmp(gender,"")==0|| strcmp(age,"")==0)
	{
			pw_set_text(reg_label,"please fill all and try again");
	}
	else
	{
			
////////////////sqlite stuff


	char query[512];
	int i =0;	
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	
	sprintf(query,"CREATE TABLE shop_keepers(name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	i=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"insert into shop_keepers (user_id,name,age,gender)values ('%s', '%s', '%s', '%s')",user_id,name,age,gender);
	printf("query is ::::::::::%s::::",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);

/////////data work is done	



//////////exit this and return to home

	char text[30] = {"Registered"};
	display_wait_msg(text);
	sleep(2); //add functionalities
	hide_check_screen();
	
	hide_reg_screen();
	show_frame_home();
	}		 
	return False;
}


void show_frame_register(char * ext_userid)
{
	strcpy(user_id,ext_userid);
	reg_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);
//submit the id 
	
	reg_label = pw_create_label(reg_screen, 25, 5, 200, 20,
			PW_TEXT,"Enter Details To Be Enrolled",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	reg_entry_name = pw_create_entry(reg_screen,55, 40,70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	
	reg_entry_age = pw_create_entry(reg_screen,55, 70,70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	
	reg_entry_gender = pw_create_entry(reg_screen,55,100,70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	reg_butt = pw_create_button(reg_screen,25, 150, 60,20,
                        PW_ACTIVE_IMAGE, "submit.png",
                        PW_NORMAL_IMAGE, "submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, chng_reg_page,
                        PW_CONFIG_END);
	//pw_eventloop();
	return;

}	

