// the one being used cb_splash_screem not working

#include<pwrap2.h>

#include<stdlib.h>
#include<string.h>

#include"features.h"


Bool cb_entry_num(pw_widget w, pw_event e);


pw_widget toplevel,splash_screen,Butt_splash;
pw_widget id_screen, id_submit_button, id_entry,id_label;

void show_frame_order();
 
void hide_id_screen()
{
	pw_hide(id_screen);
	return;
}

Bool chng_id_page()
{
	char name[20];

	strcpy(name,pw_get_text(id_entry));
	if(atoi(name)==1)
	{
		hide_id_screen();
		show_frame_order();
	}

	else
	{
		pw_set_text(id_label,"please try again");
	}		 
	
	return False;
}


void show_frame_idnumberpage()
{
	id_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);
//submit the id 
	id_submit_button = pw_create_button(id_screen,55, 90, 50,20,
                        PW_ACTIVE_IMAGE, "ADD.png",
                        PW_NORMAL_IMAGE, "ADD.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, chng_id_page,
                        PW_CONFIG_END);
	
	id_entry = pw_create_entry(id_screen,55, 50,70, 20,
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_num,
			PW_CONFIG_END);

	id_label = pw_create_label(id_screen, 55, 10, 160, 20,
			PW_TEXT,"Enter ur id",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	



	//pw_eventloop();
	return;

}	

