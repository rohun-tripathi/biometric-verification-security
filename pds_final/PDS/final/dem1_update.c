// the one where the supply shows tables and u can update into how much u got
// come here after clearign authentication on supplier page
// goes to supp4 show_frame_supp4 wher shopkeeper's authentication is taken
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
pw_widget toplevel, dem1_screen ,dem1_butt_back,dem1_butt_update,dem1_butt_submit;
pw_widget dem1_label[3], dem1_combo; //may be used
pw_widget dem1_lbox_stock,dem1_entry;

int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
Bool cb_entry_num(pw_widget w, pw_event e);
int exec_query_load_into_lbox_trriple(pw_widget lbox, char *query, char *database);

void dem1_combo_function()
{
	pw_set_text(dem1_entry,"");
	return;
}
void dem1_update_cb()
{
	char * product;
	char entered[20];
	char query[512];
	sqlite3 * dbhandle = NULL;
	unsigned int idprod;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	pw_combobox_get_selection(dem1_combo,&product,&idprod);
	strcpy(entered,pw_get_text(dem1_entry));

	printf("selection is (in update) is %s and quantity %s\n",product,entered);

	if(strcmp(product,"") != 0 && strcmp(entered,"") != 0)
	{

	sprintf(query,"update demandtemp set quantity = '%s' where product = '%s' ",entered, product);
	printf("query is : %s ",query);
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
		
	sprintf(query,"select product, code, quantity from demandtemp ");
	printf("query is : %s ",query);
	pw_tlistbox_clear(dem1_lbox_stock);
	exec_query_load_into_lbox_trriple(dem1_lbox_stock,query, DB_PACK);

	pw_update(dem1_lbox_stock,1);
	}
//		strcpy(univuserid,userid);
	
	free(product);
	return;
}
void show_frame_dem2();
void dem1_hide_screen()
{
	pw_hide(dem1_screen);
	return;
}
void dem1_submit_cb()
{
	dem1_hide_screen();
	show_frame_dem2();
	return;
}
////////////////add a back button if possible

void show_frame_dem1()
{

//page graphics begin
	dem1_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, LTGOLD ,
                        PW_CONFIG_END);


	dem1_label[0] = pw_create_label(dem1_screen, 10, 5, 200, 40,
			PW_TEXT,"Select Product to Demand: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	
	dem1_combo= pw_create_combobox(dem1_screen, 20, 55, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	
	char query[1024];
	sprintf(query, "select product from demandtemp");
	exec_query_load_into_combo(dem1_combo, query, DB_PACK);
	pw_combobox_selection_intimate(dem1_combo,dem1_combo_function);

	dem1_entry = pw_create_entry(dem1_screen,100, 55 ,100, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, LTGOLD,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	dem1_butt_update = pw_create_button(dem1_screen, 15, 90, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/update.png",
                        PW_NORMAL_IMAGE, "/root/PDS/update.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, dem1_update_cb,
                        PW_CONFIG_END);

	
	dem1_butt_submit = pw_create_button(dem1_screen, 110, 90, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, dem1_submit_cb,
                        PW_CONFIG_END);	
	
	dem1_label[1] = pw_create_label(dem1_screen, 15, 110, 230, 30,
			PW_TEXT,"Amount To be Requested: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);


	dem1_lbox_stock =pw_create_tlistbox(dem1_screen, 0, 140 , 240, 65,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(dem1_lbox_stock,80);
	pw_tlistbox_addcolumn(dem1_lbox_stock,80);
	pw_tlistbox_addcolumn(dem1_lbox_stock,80);

	pw_update(dem1_lbox_stock,1);

//sqlite stuff

	sqlite3 *dbhandle=NULL;
	pw_tlistbox_clear(dem1_lbox_stock);
	sqlite3_open(DB_PACK, &dbhandle);

	sprintf(query,"CREATE TABLE demandtemp(product  varchar(15),code varchar(15), quantity int(10))");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"update demandtemp set quantity = '0' ");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	
	sprintf(query,"select product, code, quantity from demandtemp ");
	exec_query_load_into_lbox_trriple(dem1_lbox_stock,query, DB_PACK);

	pw_update(dem1_lbox_stock,1);

///////sqlite stuff ends

//	pw_tlistbox_selection_intimate(dem1_lbox_stock,sel_dem1_lbox_stock);
	pw_tlistbox_set_selection_type(dem1_lbox_stock,PW_TLISTBOX_SINGLE_SELECTION);
			
/*	supp_label[2] = pw_create_label(supp_screen, 20, 120, 100, 40,
			PW_TEXT,"Supplier Biometric authentication:",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	supp_combo= pw_create_combobox(supp_screen, 140, 60, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
		
	char query[512];
	sprintf(query, "select user_id from suppliers");
	exec_query_load_into_combo(supp_combo, query, DB_PACK);
	pw_combobox_selection_intimate(supp_combo,supp_combo_set_biometric);

	supp_beneficiary = pw_create_radiobutton(supp_screen,160, 120, 20, 20, NULL, 
			PW_BGCOLOR, LTGOLD, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, supplier_auth,
			PW_NA);

	supp_butt_try = pw_create_button(supp_screen, 110, 180, 80,20,
                        PW_ACTIVE_IMAGE, "tryag.png",
                        PW_NORMAL_IMAGE, "tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again_supp,
                        PW_CONFIG_END);

///////////////////////////////////////////we need to change the functionality of the button under here later ryt now it goes to home
	supp_butt_back = pw_create_button(supp_screen, 15, 180, 80,20,
                        PW_ACTIVE_IMAGE, "back.png",
                        PW_NORMAL_IMAGE, "back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, back_supp_screen,
                        PW_CONFIG_END);
*/
	return ;
}
