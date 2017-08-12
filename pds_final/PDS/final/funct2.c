// the 2nd in the sequence for the main shopkeeper app
// including sql functionalities at 11:09 jun 13

#include<pwrap2.h>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>

#include"features_live.h"

int custid = 0;

Bool cb_entry_num(pw_widget w, pw_event e);

pw_widget toplevel,splash_screen,Butt_splash;
pw_widget id_screen, id_submit_button, id_entry,id_label;

//code copied from db-utils to sqlfunct
char *exec_query_get_single_value(char *data, char *query, char *database);

void show_frame_bcust();
void hide_id_screen()
{
	pw_hide(id_screen);
	return;
}
	
Bool chng_id_page()
{
	char userid[20]; 		//stores the numeric id entered
	strcpy(userid,pw_get_text(id_entry));
	custid = atoi(userid);

#ifdef __arm__

////////////////sqlite stuff


	char query[512];
	int i =0;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);

	char *cnt=NULL;
	
	sprintf(query,"CREATE TABLE shop_keepers1 (name  varchar(15),User_ID varchar(15), age int(2), Gender varchar(2))");
	i=sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"select name from shop_keepers where User_ID='%s'",userid);
	printf("query is (main part) %s\n",query);

	cnt = exec_query_get_single_value(cnt, query, DB_PACK);
       	
	printf("query is count %s\n",cnt);


	if(cnt != NULL)
	{
		hide_id_screen();

		show_frame_bcust(custid);
	}
	else
	{
		pw_set_text(id_label,"please try again");
	}		 
	
	sqlite3_close(dbhandle);
/////////data work is done	


#else
	hide_id_screen();
	show_frame_bcust(custid);
#endif
	return False;
}


void show_frame_idnumberpage()
{
	id_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);

	id_label = pw_create_label(id_screen, 25, 10, 200, 20,
			PW_TEXT,"Enter Customers ID No.",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);

	id_entry = pw_create_entry(id_screen,25, 45 ,70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, GREEN,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	id_submit_button = pw_create_button(id_screen,25, 160, 60,20,
                        PW_ACTIVE_IMAGE, "submit.png",
                        PW_NORMAL_IMAGE, "submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, chng_id_page,
                        PW_CONFIG_END);
	
	//pw_eventloop();
	return;

}	
 	
