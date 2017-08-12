// the one where the inspector starts/takes place
// come here using the misly button on the Main page
// goes to mis2 show_frame_mis2
// components one combo box and one biometric scanner
// comes from main.c

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

char univuserid[20];
pw_widget toplevel, mis_screen ,mis_butt_try,mis_butt_back;
pw_widget mis_beneficiary, mis_label[3], mis_combo;

//the functions are for diaplaying wait or error message
void display_wait_msg(char * text);
void hide_check_screen();

//function for authentication
long int person_auth(char * name, char * univuserid,int fps);
void mis_auth()
{
	char inspector[20]= {"inspector"};
	int fps=0;
	long int retval = person_auth(inspector,univuserid,fps);

	if (retval==1)
	{
		printf("Yes authenticated, going to next page\n");
		char text[40] = {"Inspector Authentication Complete"};
		display_wait_msg(text);
		sleep(2); //add functionalities
		hide_check_screen();

		goto_frame_mis2();
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
///// to move to next frame
void show_frame_home();
void hide_mis_screen()
{
	pw_hide(mis_screen);
}

//////////////////change this////////////////////

void show_frame_mis2();
//////change this whn needed
Bool back_mis_screen()
{
	hide_mis_screen();
	show_frame_home();
	return False;
}

Bool goto_frame_mis2()
{
	hide_mis_screen();
	show_frame_mis2();
	return False;
}
Bool cb_try_again_mis()
{
	pw_set_text(mis_label[0],"please try again");
		pw_radiobutton_deselect(mis_beneficiary);
		pw_set_bgcolor(mis_beneficiary, LTGOLD);
	return False;		
}

//the functions for the combobox
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
void mis_combo_set_biometric()
{
	char * userid;
	unsigned int idusr;
	pw_combobox_get_selection(mis_combo,&userid,&idusr);

		printf("userid (in set_biomet) is %s\n",userid);

		strcpy(univuserid,userid);
	
	free(userid);
	return;
}

void show_frame_mis()
{
//page graphics begin
	mis_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, LTGOLD ,
                        PW_CONFIG_END);
	mis_label[0] = pw_create_label(mis_screen, 15, 10, 200, 40,
			PW_TEXT,"Inspector Authentication Page",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	
	mis_label[1] = pw_create_label(mis_screen, 20, 55, 120, 40,
			PW_TEXT,"Inspector User ID: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);

	mis_label[2] = pw_create_label(mis_screen,100, 120, 100, 40,
			PW_TEXT,"Inspector Biometric authentication",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, LTGOLD,
			PW_CONFIG_END);
	mis_combo= pw_create_combobox(mis_screen, 140, 60, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
		
	char query[512];
	sprintf(query, "select user_id from inspectors");
	exec_query_load_into_combo(mis_combo, query, DB_PACK);
	pw_combobox_selection_intimate(mis_combo,mis_combo_set_biometric);

	mis_beneficiary = pw_create_radiobutton(mis_screen,50, 120, 20, 20, NULL, 
			PW_BGCOLOR, LTGOLD, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, mis_auth,
			PW_NA);

	mis_butt_try = pw_create_button(mis_screen, 110, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/tryag.png",
                        PW_NORMAL_IMAGE, "/root/PDS/tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again_mis,
                        PW_CONFIG_END);

///////////////////////////////////////////we need to change the functionality of the button under here later ryt now it goes to home
	mis_butt_back = pw_create_button(mis_screen, 15, 180, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, back_mis_screen,
                        PW_CONFIG_END);
	return ;
}
