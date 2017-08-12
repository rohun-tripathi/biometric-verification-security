// the one where the sale starts/takes place
// come here using the sale button on the Main page
// goes to funct2
// components one combo box and one biometric scanner

#include<sqlite3.h>
#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>
#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif

pw_widget toplevel, splash_screen ,Butt_splash_try,Butt_splash_home;
pw_widget rb_benificary_splash, sale_label[3];

//////////declaring functions already in header is not necessary

///// to move to next frame
void show_frame_home();

void show_frame_register();

void hide_splash_screen()
{
	pw_hide(splash_screen);
}
	
///// for biometric scan enrollment
int scan_template(char *fname);
int keeper_enroll(int fin);

void keeper_enroll_splash()
{
	int i;

	for(i=0; i<3; i++)		//used to retrive which finger has called this
	{
		if(pw_radiobutton_status(rb_benificary_splash[i]))
		{
			printf("Selected saleperson = %d \n", i+1);
			break;
		}
	}

	int retval = keeper_enroll(i);
	printf("ret value is= %d\n", retval);

	if(retval == 0)
	{
		//enroll_flag[i] = ENROLLED;
		pw_set_bgcolor(rb_benificary_splash[i], "green");
		//if(validate_mandatory_enroll_flag() == 1)
		//	pw_enable(Butt_beneficary_enroll);
	}
	else
	{
		//enroll_flag[i] = NOT_ENROLLED;
		pw_set_bgcolor(rb_benificary_splash[i], VIOLET);
		//pw_disable(Butt_beneficary_enroll);
	}
	
	return;
}

////timebeing
void show_frame_idnumberpage();
Bool cb_splash_screen()
{
	hide_splash_screen();
	show_frame_idnumberpage();
	return False;
}

Bool cb_try_again_kpr()
{
	pw_set_text(sale_label[0],"please try again");
	int i = 0;
	for (i=0;i<3;i++)
	{
		pw_radiobutton_deselect(rb_benificary_splash[i]);
		pw_set_bgcolor(rb_benificary_splash[i], GREY);
	}
	return False;
		
}

Bool cb_home_screen()
{
	hide_splash_screen();
	show_frame_home();
	return False;
}
void show_frame_shop()
{
//page graphics begin


	splash_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	sale_label[0] = pw_create_label(splash_screen, 15, 10, 240, 20,
			PW_TEXT,"Shopkeeper Authentication",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);
	
	sale_label[1] = pw_create_label(splash_screen, 20, 60, 120, 20,
			PW_TEXT,"Shopkeeper User ID : ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);


	sale_label[2] = pw_create_label(splash_screen, 20, 120, 100, 40,
			PW_TEXT,"Shopkeeper Biometric authentication :",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);
	combo_sale= pw_create_combobox(splash_screen, 140, 60, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
		
		pw_combobox_insert(combo_pd,"w1",True);
		pw_combobox_insert(combo_pd,"r2",True);
		pw_combobox_selection_intimate(combo_pd,combo_change_code);

	rb_benificary_splash = pw_create_radiobutton(splash_screen,160, 120, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_enroll_splash,
			PW_NA);

	Butt_splash_try = pw_create_button(splash_screen, 90, 180, 80,20,
                        PW_ACTIVE_IMAGE, "tryag.png",
                        PW_NORMAL_IMAGE, "tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again_kpr,
                        PW_CONFIG_END);







///////////////////////////////////////////we need to change the functionality of the button under here later
	Butt_splash_home = pw_create_button(splash_screen, 15, 180, 60,20,
                        PW_ACTIVE_IMAGE, "submit.png",
                        PW_NORMAL_IMAGE, "submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_splash_screen,
                        PW_CONFIG_END);

	return ;
}
