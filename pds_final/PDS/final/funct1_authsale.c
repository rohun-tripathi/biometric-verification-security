// the one where the sale starts/takes place
// come here using the sale button on the Main page
// goes to funct2
// components one combo box and one biometric scanner
//comes from main.c
// question what is being returned in &uid???? 

#include<sqlite3.h>
#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

#include <fcntl.h>
#include <sys/stat.h>

#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif

char univuserid[20];
pw_widget toplevel, splash_screen ,Butt_splash_try,Butt_splash_back;
pw_widget rb_benificary_splash, sale_label[3], combo_sale;

//the functions are for diaplaying wait or error message
void display_wait_msg(char * text);
void hide_check_screen();

//function for authentication
long int person_auth(char * name, char * univuserid,int fps);
void keeper_auth_splash()
{
	char shopkeeper[20]= {"shopkeeper"};
	int fps=0;
	long int retval = person_auth(shopkeeper,univuserid,fps);

	if (retval==1)
	{
		printf("Yes authenticated, going to next page\n");
		char text[40] = {"Shopkeeper Authentication Complete"};
		display_wait_msg(text);
		sleep(2); //add functionalities
		hide_check_screen();

		goto_frame_opt2();
	}
	else if(retval ==2)
	{
		printf("sensor time out\n");
		char text[30] = {"Sensor Time Out"};
		display_wait_msg(text);
		sleep(2); //add functionalities
		hide_check_screen();
	}
	else
	{
		printf("VERIFICATION FAILED\n");
		char text[30] = {"Verification Failed"};
		display_wait_msg(text);
		sleep(2); //add functionalities
		hide_check_screen();
	}
	

		
}
///// to move to next frame
void show_frame_home();
void hide_splash_screen()
{
	pw_hide(splash_screen);
}

void show_frame_opt2();
Bool back_splash_screen()
{
	hide_splash_screen();
	show_frame_home();
	return False;
}

Bool goto_frame_opt2()
{
	hide_splash_screen();
	show_frame_opt2();
	return False;
}
Bool cb_try_again_kpr()
{
	pw_set_text(sale_label[0],"please try again");
		pw_radiobutton_deselect(rb_benificary_splash);
		pw_set_bgcolor(rb_benificary_splash, LTGOLD);
	return False;		
}

Bool cb_home_screen()
{
	hide_splash_screen();
	show_frame_home();
	return False;
}

//the functions for the combobox
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
void combo_set_biometric()
{
	char * userid;
	unsigned int idusr;
	pw_combobox_get_selection(combo_sale,&userid,&idusr);
		printf("userid(in set_biomet) is %s\n",userid);

		strcpy(univuserid,userid);
	
	free(userid);
	return;
}

void show_frame_shop()
{
//page graphics begin
	splash_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, LTGOLD ,
                        PW_CONFIG_END);
	sale_label[0] = pw_create_label(splash_screen, 15, 10, 200, 40,
			PW_TEXT,"Shopkeeper Authentication Page",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	
	sale_label[1] = pw_create_label(splash_screen, 20, 55, 120, 40,
			PW_TEXT,"Shopkeeper User ID: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);

	sale_label[2] = pw_create_label(splash_screen, 110, 120, 100, 40,
			PW_TEXT,"Shopkeeper Biometric authentication",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	combo_sale= pw_create_combobox(splash_screen, 140, 60, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
		
	char query[512];
	sprintf(query, "select user_id from shop_keepers");
	exec_query_load_into_combo(combo_sale, query, DB_PACK);
	pw_combobox_selection_intimate(combo_sale,combo_set_biometric);

	rb_benificary_splash = pw_create_radiobutton(splash_screen,50, 120, 20, 20, NULL, 
			PW_BGCOLOR, LTGOLD, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_auth_splash,
			PW_NA);

	Butt_splash_try = pw_create_button(splash_screen, 110, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/tryag.png",
                        PW_NORMAL_IMAGE, "/root/PDS/tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again_kpr,
                        PW_CONFIG_END);

///////////////////////////////////////////we need to change the functionality of the button under here later ryt now it goes to home
	Butt_splash_back = pw_create_button(splash_screen, 15, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, back_splash_screen,
                        PW_CONFIG_END);
	return ;
}
