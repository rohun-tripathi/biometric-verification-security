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
pw_widget toplevel, supp3_screen ,supp3_butt_back,supp3_butt_update,supp3_butt_submit;
pw_widget supp3_label[3], supp3_combo; //may be used
pw_widget supp3_lbox_stock,supp3_entry;

int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
Bool cb_entry_num(pw_widget w, pw_event e);

int exec_query_load_into_lbox_trriple(pw_widget lbox, char *query, char *database)
{
	sqlite3 *dbhandle=NULL;
	int rval=0, nRows=0, nCols=0, i;
	char *szErrMsg=NULL;
	char **qResults=NULL;

	i = sqlite3_open(database, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	printf("query is %s\n", query);

	rval = sqlite3_get_table(dbhandle, query, &qResults, &nRows, &nCols, &szErrMsg);
	printf("after query  row = %d col = %d \n", nRows, nCols);
	if (rval != SQLITE_OK) {
		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return (-1);
	}
	
	for(i=0; i<nRows; i++)
	{
				pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+3)))], 0);
				pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+4)))], 1);
				pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+5)))], 2);
	}
	pw_update(lbox, 1);
	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);
	return 0;
}

void sel_supp3_lbox_stock()
{
	char *str=NULL;
	int idx = pw_tlistbox_get_selection(supp3_lbox_stock);
	pw_tlistbox_get_element(supp3_lbox_stock, &str,idx, 0);

	//txtlb_get_element(lbox_savealocated_grps, &str, idx, 0);
	if(idx==-1)
	{
		return  ;
	}
//	sel_aloc_wrk[0]='\0';
//	strcpy(sel_aloc_wrk,str);
	printf("selected workcode to del is %s\n",str);
	//pw_set_text(supp3_label[1],"'%s'",str);
	free(str);
	return  ;
}

void supp3_combo_function()
{
	pw_set_text(supp3_entry,"");
return;
}

void supp3_update_cb()
{
	char * product;
	char entered[20];
	char query[512];
	sqlite3 * dbhandle = NULL;
	unsigned int idprod;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	pw_combobox_get_selection(supp3_combo,&product,&idprod);
	strcpy(entered,pw_get_text(supp3_entry));

	printf("selection is (in update) is %s and quantity %s\n",product,entered);

	if(strcmp(product,"") != 0 && strcmp(entered,"") != 0)
	{

	pw_tlistbox_clear(supp3_lbox_stock);

	sprintf(query,"update stocktemp set quantity = '%s' where product = '%s' ",entered, product);
	printf("query is : %s ",query);
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
		
	sprintf(query,"select product, code, quantity from stocktemp ");
	printf("query is : %s ",query);
	exec_query_load_into_lbox_trriple(supp3_lbox_stock,query, DB_PACK);

	pw_update(supp3_lbox_stock,1);
	}
//		strcpy(univuserid,userid);
	
	free(product);
	return;
}
void show_frame_supp4(int type);
void supp3_hide_screen()
{
	pw_hide(supp3_screen);
	return;
}

void supp3_submit_cb()
{
	supp3_hide_screen();
	show_frame_supp4(1);
	return;
}

void show_frame_supp3()
{

//page graphics begin
	supp3_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, LTGOLD ,
                        PW_CONFIG_END);


	supp3_label[0] = pw_create_label(supp3_screen, 10, 5, 200, 40,
			PW_TEXT,"Select Product to Add To Stock: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	
	supp3_combo= pw_create_combobox(supp3_screen, 20, 55, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	
	char query[1024];
	sprintf(query, "select product from stocktemp");
	exec_query_load_into_combo(supp3_combo, query, DB_PACK);
	pw_combobox_selection_intimate(supp3_combo,supp3_combo_function);

	supp3_entry = pw_create_entry(supp3_screen,100, 55 ,100, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, LTGOLD,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	supp3_butt_update = pw_create_button(supp3_screen, 15, 90, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/update.png",
                        PW_NORMAL_IMAGE, "/root/PDS/update.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, supp3_update_cb,
                        PW_CONFIG_END);

	
	supp3_butt_submit = pw_create_button(supp3_screen, 110, 90, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, supp3_submit_cb,
                        PW_CONFIG_END);	
	
	supp3_label[1] = pw_create_label(supp3_screen, 15, 110, 230, 30,
			PW_TEXT,"The Stock Brought: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);


	supp3_lbox_stock =pw_create_tlistbox(supp3_screen, 0, 140 , 240, 65,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(supp3_lbox_stock,80);
	pw_tlistbox_addcolumn(supp3_lbox_stock,80);
	pw_tlistbox_addcolumn(supp3_lbox_stock,80);

	pw_update(supp3_lbox_stock,1);

//sqlite stuff

	sqlite3 *dbhandle=NULL;
	pw_tlistbox_clear(supp3_lbox_stock);
	sqlite3_open(DB_PACK, &dbhandle);

	sprintf(query,"CREATE TABLE stocktemp(product  varchar(15),code varchar(15), quantity int(10))");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"update stocktemp set quantity = '0' ");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	
	sprintf(query,"select product, code, quantity from stocktemp ");
	exec_query_load_into_lbox_trriple(supp3_lbox_stock,query, DB_PACK);

	pw_update(supp3_lbox_stock,1);

///////sqlite stuff ends

//	pw_tlistbox_selection_intimate(supp3_lbox_stock,sel_supp3_lbox_stock);
	pw_tlistbox_set_selection_type(supp3_lbox_stock,PW_TLISTBOX_SINGLE_SELECTION);
			
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
