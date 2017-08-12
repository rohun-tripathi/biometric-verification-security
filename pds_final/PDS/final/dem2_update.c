// the one where the supply shows tables and u can update into how much u got
// come here after clearign authentication on supplier page
// goes to supp3 show_frame_supp3 wher shopkeeper's authentication is taken
// components tabels
// comes from sup1_auth.c

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

int amount[3];
pw_widget toplevel, dem2_screen ,dem2_butt_back, dem2_butt_submit;
pw_widget dem2_label[3], dem2_combo; //may be used
pw_widget dem2_lbox_stock;

void show_frame_dem1();
void show_frame_home();
int exec_query_load_into_lbox_trriple(pw_widget lbox, char *query, char *database);

//the functions are for diaplaying wait or error message
void display_wait_msg(char * text);
void hide_check_screen();

void dem2_hide_screen()
{
	pw_hide(dem2_screen);
}
void dem2_submit_cb()
{
	char query[512];
	int i,number;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);

sprintf(query,"CREATE TABLE demandtemp2 (product  varchar(15),code varchar(15), quantity int(10))");
sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

sprintf(query,"CREATE TABLE demand(product  varchar(15),code varchar(15), quantity int(10))");
sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

sprintf(query,"delete from demandtemp2");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"insert into demandtemp2 select a.product,a.code,a.quantity from demand a");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"delete from demand");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"insert into demand select a.product,a.code,a.quantity + b.quantity from demandtemp2 a, demandtemp b where a.code = b.code");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"update demandtemp set quantity = '0' ");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

	sqlite3_close(dbhandle);

	char text[40] = {"Demand Placed"};
	display_wait_msg(text);
	sleep(2); //add functionalities
	hide_check_screen();

	dem2_hide_screen();
	show_frame_home();

	return;
}
void dem2_back_cb()
{
	dem2_hide_screen();
	show_frame_dem1();
	return;
}
void show_frame_dem2()
{

//page graphics begin
	dem2_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, LTGOLD ,
                        PW_CONFIG_END);

	dem2_label[0] = pw_create_label(dem2_screen, 15, 10, 200, 40,
			PW_TEXT,"Confirm Added Quantity Demanded: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);

	dem2_lbox_stock =pw_create_tlistbox(dem2_screen, 0, 50 , 240, 80,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(dem2_lbox_stock,80);
	pw_tlistbox_addcolumn(dem2_lbox_stock,80);
	pw_tlistbox_addcolumn(dem2_lbox_stock,80);
	pw_update(dem2_lbox_stock,1);
//sqlite stuff
	char query[1024];
	pw_tlistbox_clear(dem2_lbox_stock);

	sprintf(query,"select product,code, quantity from demandtemp ");
	exec_query_load_into_lbox_trriple(dem2_lbox_stock,query, DB_PACK);
	pw_update(dem2_lbox_stock,1);

///////sqlite stuff ends
	
	dem2_butt_back = pw_create_button(dem2_screen, 15, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, dem2_back_cb,
                        PW_CONFIG_END);
	dem2_butt_submit = pw_create_button(dem2_screen, 110, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, dem2_submit_cb,
                        PW_CONFIG_END);	
	
	dem2_label[1] = pw_create_label(dem2_screen, 20, 270, 120, 40,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	return ;
}
