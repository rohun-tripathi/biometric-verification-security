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


char *exec_query_get_single_value(char *data, char *query, char *database);

int amount[3];
pw_widget toplevel, supp6_screen ,supp6_butt_back, supp6_butt_submit;
pw_widget supp6_label[3], supp6_combo; //may be used
pw_widget supp6_lbox_stock, supp6_lbox_stocktot;

void show_frame_home();
int exec_query_load_into_lbox_trriple(pw_widget lbox, char *query, char *database);

void supp6_hide_screen()
{
	pw_hide(supp6_screen);
}
void supp6_home_cb()
{
	supp6_hide_screen();
	show_frame_home();
	return;
}

void add()
{
	char query[512];
	int i,number;
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);

sprintf(query,"delete from stocklast");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"insert into stocklast select a.product,a.code,a.quantity from stocktemp a");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"delete from stocktemp");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"insert into stocktemp select a.product,a.code,a.quantity + b.quantity from stock a, stocklast b where a.code = b.code ");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"delete from stock");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"insert into stock select a.product,a.code,a.quantity from stocktemp a");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"delete from stocktemp");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

sprintf(query,"insert into stocktemp select a.product,a.code,a.quantity from stocktempdefault a");
sqlite3_exec(dbhandle,query,NULL,NULL,NULL);

	sqlite3_close(dbhandle);
}

void show_frame_supp6()
{
	add();
	
//page graphics begin
	supp6_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, LTGOLD ,
                        PW_CONFIG_END);

	supp6_label[0] = pw_create_label(supp6_screen, 15, 10, 200, 40,
			PW_TEXT,"Total Amount Added This Time: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);

	supp6_lbox_stock =pw_create_tlistbox(supp6_screen, 0, 50 , 240, 60,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(supp6_lbox_stock,80);
	pw_tlistbox_addcolumn(supp6_lbox_stock,80);
	pw_tlistbox_addcolumn(supp6_lbox_stock,80);
	pw_update(supp6_lbox_stock,1);

//sqlite stuff
	char query[1024];
	pw_tlistbox_clear(supp6_lbox_stock);

	sprintf(query,"select product,code, quantity from stocklast ");
	exec_query_load_into_lbox_trriple(supp6_lbox_stock,query, DB_PACK);
	pw_update(supp6_lbox_stock,1);

///////sqlite stuff ends
	
	supp6_label[1] = pw_create_label(supp6_screen, 20, 115, 240, 30,
			PW_TEXT,"Total Amount In Stock: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
		
	supp6_lbox_stocktot =pw_create_tlistbox(supp6_screen, 0, 140 , 240, 60,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(supp6_lbox_stocktot,80);
	pw_tlistbox_addcolumn(supp6_lbox_stocktot,80);
	pw_tlistbox_addcolumn(supp6_lbox_stocktot,80);
	pw_update(supp6_lbox_stocktot,1);
//sqlite stuff;
	pw_tlistbox_clear(supp6_lbox_stocktot);

	sprintf(query,"select product,code, quantity from stock ");
	exec_query_load_into_lbox_trriple(supp6_lbox_stocktot,query, DB_PACK);
	pw_update(supp6_lbox_stocktot,1);

///////sqlite stuff ends
	
	supp6_butt_submit = pw_create_button(supp6_screen, 15, 210, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/home.png",
                        PW_NORMAL_IMAGE, "/root/PDS/home.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, supp6_home_cb,
                        PW_CONFIG_END);	
	
	return ;
}
