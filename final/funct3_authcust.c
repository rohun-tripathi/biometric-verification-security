//customer authentication befors order page



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

char userid[20] = {"guest"} ;
char name[20] = {"guest"} ;
char univuserid[20];
pw_widget toplevel, bcust_screen ,bcust_butt_tryag, bcust_butt_back , rb_benificary_bcust, bcust_label;

void show_frame_order(char * userid, char * name);
void show_frame_home();
void show_frame_idnumberpage();

void hide_bcust_screen()
{
	pw_hide(bcust_screen);
}
void cb_order_page()
{
	hide_bcust_screen();
	show_frame_order(userid,name);
	return;
}
Bool cb_back_screen_bcust()
{
	hide_bcust_screen();
	show_frame_idnumberpage();
	return False;
}

Bool cb_try_again_bcust()
{
	pw_set_text(bcust_label,"please try again");
	pw_radiobutton_deselect(rb_benificary_bcust);
	pw_set_bgcolor(rb_benificary_bcust, GREEN);
	return False;
		
}
void display_wait_msg(char * text);
void hide_check_screen();

//function for authentication
long int person_auth(char * name, char * univuserid,int fps);
void custmr_auth()
{
	char customer[20]= {"customer"};
	int fps=0;
	long int retval = person_auth(customer,univuserid,fps);

	if (retval==1)
	{
		printf("Yes authenticated, going to next page\n");
		char text[30] = {"Customer Authentication Complete"};
		display_wait_msg(text);
		sleep(2); //add functionalities
		hide_check_screen();

		cb_order_page();
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
		
void show_frame_bcust(char * ext_userid,char * ext_name)
{
////////copying the incoming stuff
	printf("reached show_frame_bcust for authentication\n");
	if(ext_name!=NULL)
	{
	strcpy(userid,ext_userid);
	strcpy(univuserid,ext_userid);
	strcpy(name,ext_name);
	}
	
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
	bcust_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);

	char message[50];
	sprintf(message,"Select Radibutton And Press Thumb to Sensor, '%s' ", name);
	bcust_label = pw_create_label(bcust_screen, 20, 5, 200, 50,
			PW_TEXT,message,
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
                        PW_BGCOLOR, GREEN ,
			PW_CONFIG_END);
	
	bcust_butt_tryag = pw_create_button(bcust_screen, 110, 120, 80, 20,
                        PW_ACTIVE_IMAGE, "/root/PDS/tryag.png",
                        PW_NORMAL_IMAGE, "/root/PDS/tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again_bcust,
                        PW_CONFIG_END);

	bcust_butt_back = pw_create_button(bcust_screen, 20, 120, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_back_screen_bcust,
                        PW_CONFIG_END);
	rb_benificary_bcust = pw_create_radiobutton(bcust_screen,35, 70, 20, 20, NULL, 
			PW_BGCOLOR, GREEN, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, custmr_auth,
			PW_NA);

	return ;
}
