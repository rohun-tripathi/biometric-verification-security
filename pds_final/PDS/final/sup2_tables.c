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
pw_widget toplevel, supp2_screen ,supp2_butt_back;
pw_widget supp2_label[3], supp2_combo; //may be used
pw_widget lbox_stock;

int exec_query_load_into_lbox_double(pw_widget lbox, char *query, char *database)
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
	printf("after query loding into lbox double       row = %d col = %d \n", nRows, nCols);
	if (rval != SQLITE_OK) {
		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return (-1);
	}
	
	for(i=0; i<nRows; i++)
	{
				pw_tlistbox_insertend(lbox, qResults[(i+(i+2))], 0);
				pw_tlistbox_insertend(lbox, qResults[(i+(i+3))], 1);
				
	}
	pw_update(lbox, 1);
	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);
	return 0;
}

void sel_lbox_stock()
{
	char *str=NULL;
	int idx = pw_tlistbox_get_selection(lbox_stock);
	pw_tlistbox_get_element(lbox_stock, &str,idx, 0);

	//txtlb_get_element(lbox_savealocated_grps, &str, idx, 0);
	if(idx==-1)
	{
		return  ;
	}
//	sel_aloc_wrk[0]='\0';
//	strcpy(sel_aloc_wrk,str);
	printf("selected workcode to del is %s\n",str);
	pw_set_text(supp2_label[1],"'%s'",str);
	free(str);
	return  ;
}

void show_frame_supp2()
{

//page graphics begin
	supp2_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, LTGOLD ,
                        PW_CONFIG_END);


	lbox_stock =pw_create_tlistbox(supp2_screen, 0, 50 , 240, 80,
			PW_BGCOLOR, "#F5DEBC",
			PW_FONT, PW_BOLD_FONT,
			PW_CONFIG_END);
	pw_tlistbox_addcolumn(lbox_stock,120);
	pw_tlistbox_addcolumn(lbox_stock,120);

	pw_update(lbox_stock,1);

//sqlite stuff
	char query[1024];
	pw_tlistbox_clear(lbox_stock);

	sprintf(query,"select product, quantity from stock ");

	exec_query_load_into_lbox_double(lbox_stock,query, DB_PACK);

	pw_update(lbox_stock,1);

///////sqlite stuff ends

	pw_tlistbox_selection_intimate(lbox_stock,sel_lbox_stock);

	pw_tlistbox_set_selection_type(lbox_stock,PW_TLISTBOX_SINGLE_SELECTION);

	supp2_label[0] = pw_create_label(supp2_screen, 15, 10, 200, 40,
			PW_TEXT,"Select Product To Add To Stock: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	
	supp2_label[1] = pw_create_label(supp2_screen, 20, 270, 120, 40,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);



			
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
