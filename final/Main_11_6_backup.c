#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

//#include "funct2.c"
#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif


pw_widget toplevel, splash_screen ,Butt_splash, rb_benificary_splash;



//////////declaring functions already in header is not necessary

///// to move to next frame
void hide_splash_screen();
void show_frame_idnumberpage();
	
///// for biometric scan enrollment
int scan_template(char *fname);
void keeper_enroll();



Bool cb_splash_screen()
{
	hide_splash_screen();
	show_frame_idnumberpage();
	return False;
}


int main(int ac, char **av)
{
	pw_init(PW_SUPPORT_ALCHEMY);

        toplevel = pw_create_window(240, 270,
                        PW_TITLE, "Shopkeeper Application",
                        PW_BGCOLOR, BLUE ,
                        PW_CONFIG_END);

	pw_alchemy_command(toplevel,PW_WM_SET_STATUS_MSG,"powered by APOnline",20);
	splash_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	Butt_splash = pw_create_button(splash_screen, 50, 100, 50,30,
                        PW_ACTIVE_IMAGE, "ADD.png",
                        PW_NORMAL_IMAGE, "ADD.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_splash_screen,
                        PW_CONFIG_END);
	rb_benificary_splash = pw_create_radiobutton(splash_screen,50, 150, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_enroll,
			PW_NA);

	
	
	pw_eventloop();
	
	pw_exit();

	return (0);


}
