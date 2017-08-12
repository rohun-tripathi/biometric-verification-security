// the 2nd in the sequence for the main shopkeeper app
// including sql functionalities at 11:09 jun 13
// take the id number of the customer

#include<pwrap2.h>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>

#include"features_live.h"

int custid = 0;

Bool cb_entry_num(pw_widget w, pw_event e);

pw_widget toplevel,splash_screen,Butt_splash,back_butt;
pw_widget id_screen, id_submit_button, id_entry,id_label;

//code copied from db-utils to sqlfunct
int exec_query_check_valid_value(char *data, char *query, char *database);
char* exec_query_get_single_value(char *data, char *query, char *database);

void show_frame_bcust(char * userid, char * name);
void show_frame_shop();

void hide_id_screen()
{
	pw_hide(id_screen);
	return;
}
void back_id_page()
{
	hide_id_screen();
	show_frame_shop();
}
	
Bool chng_id_page()
{
	char userid[20];		//stores the id to cbe checked against
	strcpy(userid,pw_get_text(id_entry));
	char * name = NULL;

//	custid = atoi(userid);	//dont use this
#ifdef __arm__

////////////////sqlite stuff
	char query[512];
	int i,number;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);

	sprintf(query,"CREATE TABLE customers (name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	i=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"select count(*) from customers where user_id=%s",userid);
	number = exec_query_check_valid_value(name, query, DB_PACK);
	
	if(number>0)
	{
		sprintf(query,"select name from customers where user_id=%s",userid);
		name = exec_query_get_single_value(name, query, DB_PACK);
		printf("no of customers with userid: '%s' is count %d and name is '%s'  \n" ,userid ,number, name);
		hide_id_screen();
		show_frame_bcust(userid,name);
	}
	else
	{
		pw_set_text(id_label,"Please Try Again");
	}		 
	
	sqlite3_close(dbhandle);
/////////data work is done	

#else
	printf("\nalternate p-lace for change id page entered\n");
	char * name_sub = NULL;
	char * userid_sub = NULL;
	
	hide_id_screen();
	printf("id screen hidden\n");
	show_frame_bcust(NULL,NULL);
#endif
	return False;
}


void show_frame_idnumberpage()
{
	id_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);

	id_label = pw_create_label(id_screen, 25, 15, 200, 20,
			PW_TEXT,"Enter Customers ID No.",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);

	id_entry = pw_create_entry(id_screen,25, 45 ,100, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, GREEN,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	id_submit_button = pw_create_button(id_screen,115, 160, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, chng_id_page,
                        PW_CONFIG_END);
	back_butt = pw_create_button(id_screen,25, 160, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, back_id_page,
                        PW_CONFIG_END);
	
	//pw_eventloop();
	return;

}	
 	
