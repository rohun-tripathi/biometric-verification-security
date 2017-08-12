// the one being used cb_splash_screem not working

#include<pwrap2.h>

#include<sqlite3.h>

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"features_live.h"

char ins_user_id[20] = {"\0"};
Bool cb_entry_num(pw_widget w, pw_event e);
Bool cb_entry_alpha(pw_widget w, pw_event e); 

pw_widget toplevel,ins_reg_screen,ins_reg_butt,ins_back_butt, ins_reg_entry_name, ins_reg_entry_userid, ins_reg_entry_age, ins_reg_entry_gender, ins_reg_label[4] , combo_reg_ins;

pw_widget ins_donereg_screen, ins_donereg_label;
void show_frame_home();
void display_wait_msg(char * text);
void hide_check_screen();

void ins_hide_reg_screen()
{
	pw_hide(ins_reg_screen);
	return;
}
void ins_show_frame_root3();
void ins_back_reg_page()
{
	ins_hide_reg_screen();
	ins_show_frame_root3();
}	
Bool ins_chng_reg_page()
{
	char ins_name[20] = {"\0"};
	char ins_age[20] = {"\0"};
	char * ins_gender;
	int idgen;

	strcpy(ins_name,pw_get_text(ins_reg_entry_name));
	strcpy(ins_age,pw_get_text(ins_reg_entry_age));
	pw_combobox_get_selection(combo_reg_ins,&ins_gender,&idgen);
	
		
	printf("\n:::ins age is :::::%s::::\n",ins_age);
	if(strcmp(ins_name,"")==0|| strcmp(ins_user_id,"")==0|| strcmp(ins_gender,"")==0|| strcmp(ins_age,"")==0)
	{
			pw_set_text(ins_reg_label,"please fill all and try again");
	}
	else
	{
			
////////////////sqlite stuff begins ////////////


	char query[512];
	int i =0;	
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	
	sprintf(query,"CREATE TABLE inspectors(name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	i=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"delete from inspectors where User_Id = '%s'",ins_user_id);
	printf("query is ::::::::::%s::::",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);


	sprintf(query,"insert into inspectors (user_id,name,age,gender)values ('%s', '%s', '%s', '%s')",ins_user_id,ins_name,ins_age,ins_gender);
	printf("query is ::::::::::%s::::",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);

/////////data work^ is done	

//////////exit this and return to home

	char text[30] = {"Registered Inspectors"};
	display_wait_msg(text);
	sleep(2); //add functionalities
	hide_check_screen();
	
	ins_hide_reg_screen();
	show_frame_home();
	}		 
	return False;
}

void ins_show_frame_register(char * ins_ext_userid)
{
	strcpy(ins_user_id,ins_ext_userid);
	ins_reg_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);
//submit the id 
	ins_reg_label[0] = pw_create_label(ins_reg_screen, 25, 5, 200, 40,
			PW_TEXT,"Enter Personal Details(To Be Enrolled)",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);

	ins_reg_label[1] = pw_create_label(ins_reg_screen, 30, 50, 70, 20,
			PW_TEXT,"Name: ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	ins_reg_label[2] = pw_create_label(ins_reg_screen, 30, 80, 70, 20,
			PW_TEXT,"Age: ",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	ins_reg_label[3] = pw_create_label(ins_reg_screen, 30, 110, 70, 20,
			PW_TEXT,"Gender:",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	
	ins_reg_entry_name = pw_create_entry(ins_reg_screen,115, 50,90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_alpha,
			PW_CONFIG_END);
	
	ins_reg_entry_age = pw_create_entry(ins_reg_screen,115, 80,90, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);
	
	combo_reg_ins= pw_create_combobox(ins_reg_screen, 115, 110, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_insert(combo_reg_ins,"m",False);
	pw_combobox_insert(combo_reg_ins,"f",False);

	ins_reg_butt = pw_create_button(ins_reg_screen,25, 160, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, ins_chng_reg_page,
                        PW_CONFIG_END);

	ins_back_butt = pw_create_button(ins_reg_screen,25, 190, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, ins_back_reg_page,
                        PW_CONFIG_END);
	return;
}
