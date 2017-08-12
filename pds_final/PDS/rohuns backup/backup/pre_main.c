
#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

//#include "funct2.c"
#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif


pw_widget toplevel, pre_screen ,shop_butt, root_butt;



//////////declaring functions already in header is not necessary

///// to move to next frame
void hide_pre_screen()
{
	pw_hide(pre_screen);
	return;
}

void show_frame_shop();
void show_frame_root();
	

Bool cb_shop_screen()
{
	hide_pre_screen();
	show_frame_shop();
	return False;
}

Bool cb_root_screen()
{
	hide_pre_screen();
	show_frame_root();
	return False;
}
void show_frame_home()
{
	
//page graphics begin
	pre_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	root_butt = pw_create_button(pre_screen, 50, 100, 50,30,
                        PW_ACTIVE_IMAGE, "ADD.png",
                        PW_NORMAL_IMAGE, "ADD.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_root_screen,
                        PW_CONFIG_END);

	shop_butt = pw_create_button(pre_screen, 150, 100, 50,30,
                        PW_ACTIVE_IMAGE, "ADD.png",
                        PW_NORMAL_IMAGE, "ADD.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_shop_screen,
                        PW_CONFIG_END);
}

int main(int ac, char **av)
{
	char query[512];
	pw_init(PW_SUPPORT_ALCHEMY);

        toplevel = pw_create_window(240, 270,
                        PW_TITLE, "Shopkeeper Application",
                        PW_BGCOLOR, BLUE ,
                        PW_CONFIG_END);

	pw_alchemy_command(toplevel,PW_WM_SET_STATUS_MSG,"powered by APOnline",20);

	show_frame_home();

	pw_eventloop();
	
	pw_exit();

	return (0);


}
