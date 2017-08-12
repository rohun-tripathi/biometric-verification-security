// the page that has two buttons
// author rohun tripati
// the one after the authentication is passed by the admin
//this is followed by bioenroll.c

#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif


pw_widget toplevel, opt_screen ,kpr_optbt, cust_optbt;

void hide_opt_screen()
{
	pw_hide(opt_screen);
	return;
}

void show_frame_root3();
void show_frame_enrollcust();

Bool cb_kpr_enrl()
{
	hide_opt_screen();
	//show_frame_enrollkpr();
	show_frame_root3();	
	return False;
}

Bool cb_cust_enrl()
{
	hide_opt_screen();
	//show_frame_enrollcust();
	show_frame_enroll();	
	return False;
}

void show_frame_opt()
{
	
//page graphics begin
	opt_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	cust_optbt = pw_create_button(opt_screen, 75, 40, 90,36,
                        PW_ACTIVE_IMAGE, "custenrl.png",
                        PW_NORMAL_IMAGE, "custenrl.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_cust_enrl,
                        PW_CONFIG_END);

	kpr_optbt = pw_create_button(opt_screen, 70, 110, 100,36,
                        PW_ACTIVE_IMAGE, "kprenrl.png",
                        PW_NORMAL_IMAGE, "kprenrl.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_kpr_enrl,
                        PW_CONFIG_END);
	return;
}
