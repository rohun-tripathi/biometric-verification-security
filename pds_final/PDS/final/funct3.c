//what does this file do???



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

pw_widget toplevel, bcust_screen ,bcust_butt_tryag, bcust_butt_home , rb_benificary_bcust, bcust_label;

void show_frame_order();

//////////declaring functions already in header is not necessary

///// to move to next frame
void hide_bcust_screen()
{
	pw_hide(bcust_screen);
}

void show_frame_home();

//int custmr_enroll();
	
///// for biometric scan enrollment
int scan_template(char *fname);

void cb_order_page()
{
	hide_bcust_screen();
	show_frame_order();
	return;
}

void custmr_enroll_bio()
{
	int retval;
//	retval = custmr_enroll(custid);
	
	printf("ret value is= %d\n", retval);

	if(retval == 0)
	{
		//enroll_flag[i] = ENROLLED;
		pw_set_bgcolor(rb_benificary_bcust, "green");
		cb_order_page();
		//if(validate_mandatory_enroll_flag() == 1)
		//	pw_enable(Butt_beneficary_enroll);
	}
	else
	{
		//enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(rb_benificary_bcust, VIOLET);
		//pw_disable(Butt_beneficary_enroll);
	}
}
Bool cb_home_screen_bcust()
{
	hide_bcust_screen();
	show_frame_home();
	return False;
}

Bool cb_try_again_bcust()
{
	pw_set_text(bcust_label,"please try again");
	pw_radiobutton_deselect(rb_benificary_bcust);
	pw_set_bgcolor(rb_benificary_bcust, GREY);
	return False;
		
}
		
void show_frame_bcust(char * ext_userid,char * ext_name)
{
////////copying the incoming stuff
	if(ext_name!=NULL)
	{
	strcpy(userid,ext_userid);
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
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	char message[50];
	sprintf(message,"Choose radibutton and press thumb to sensor, '%s' ", name);
	bcust_label = pw_create_label(bcust_screen, 20, 5, 200, 40,
			PW_TEXT,message,
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
                        PW_BGCOLOR, GREY ,
			PW_CONFIG_END);
	
	bcust_butt_tryag = pw_create_button(bcust_screen, 20, 100, 75, 25,
                        PW_ACTIVE_IMAGE, "tryag.png",
                        PW_NORMAL_IMAGE, "tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again_bcust,
                        PW_CONFIG_END);

	bcust_butt_home = pw_create_button(bcust_screen, 20, 140, 50,17,
                        PW_ACTIVE_IMAGE, "home.png",
                        PW_NORMAL_IMAGE, "home.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_home_screen_bcust,
                        PW_CONFIG_END);
	rb_benificary_bcust = pw_create_radiobutton(bcust_screen,50, 55, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, custmr_enroll_bio,
			PW_NA);

	return ;
}
