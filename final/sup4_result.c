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

int univtype;
pw_widget toplevel, supp4_screen ,supp4_butt_back, supp4_butt_submit;
pw_widget supp4_label[3], supp4_combo; //may be used
pw_widget supp4_lbox_stock;

void show_frame_supp3();
void show_frame_dem1();
void show_frame_opt2();
int exec_query_load_into_lbox_trriple(pw_widget lbox, char *query, char *database);

void supp4_hide_screen()
{
	pw_hide(supp4_screen);
}
void show_frame_supp5();
void supp4_submit_cb()
{
	supp4_hide_screen();
	if(univtype == 1)
		show_frame_supp5();
	else if(univtype == 2)
		show_frame_dem1();
	return;
}
void supp4_back_cb()
{
	supp4_hide_screen();
	if(univtype == 1)
		show_frame_supp3();
	else if(univtype == 2)
		show_frame_opt2();
	return;
}
void show_frame_supp4(int ext_type)
{
	univtype = ext_type;
	

//page graphics begin
	supp4_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, LTGOLD ,
                        PW_CONFIG_END);

	char label[50];

	if(univtype == 1)
		strcpy(label,"Confirm Product Quantity To Be Added To Stock: "); 
	else if(univtype == 2)
		strcpy(label,"Amount already in Demand: ");
 
	supp4_label[0] = pw_create_label(supp4_screen, 15, 10, 200, 40,
			PW_TEXT, label, 
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);

	supp4_lbox_stock =pw_create_tlistbox(supp4_screen, 0, 50 , 240, 80,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(supp4_lbox_stock,80);
	pw_tlistbox_addcolumn(supp4_lbox_stock,80);
	pw_tlistbox_addcolumn(supp4_lbox_stock,80);
	pw_update(supp4_lbox_stock,1);
//sqlite stuff
	char query[1024];
	pw_tlistbox_clear(supp4_lbox_stock);
	if(univtype == 1)
		sprintf(query,"select product,code, quantity from stocktemp ");
	else if(univtype == 2)
		sprintf(query,"select product,code, quantity from demand ");
	
	exec_query_load_into_lbox_trriple(supp4_lbox_stock,query, DB_PACK);
	pw_update(supp4_lbox_stock,1);

///////sqlite stuff ends
	
	supp4_butt_back = pw_create_button(supp4_screen, 15, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, supp4_back_cb,
                        PW_CONFIG_END);
	supp4_butt_submit = pw_create_button(supp4_screen, 110, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, supp4_submit_cb,
                        PW_CONFIG_END);	
	
	supp4_label[1] = pw_create_label(supp4_screen, 20, 270, 120, 40,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	return ;
}
