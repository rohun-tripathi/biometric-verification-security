
#include<pwrap2.h>

#include "funct.h"



pw_widget toplevel, splash_screen ,Butt_splash;
/*
void show_frame_identity(void)
{
	if(! frame_optionspage)
	{
		create_frame_optionspage();
		pw_show(frame_optionspage);
	}
	else
		pw_show(frame_optionspage);
}

*/

//////////declaring functions already in header is not necessary

//void hide_splash_screen();
//Bool cb_splash_screen();

int main(int ac, char **av)
{
	pw_init(PW_SUPPORT_ALCHEMY);

        toplevel = pw_create_window(240, 270,
                        PW_TITLE, "Shopkeeper Application",
                        PW_BGCOLOR, BLUE ,
                        PW_CONFIG_END);

	pw_alchemy_command(toplevel,PW_WM_SET_STATUS_MSG,"powered by APOnline",20);
	splash_screen = pw_create_frame(toplevel, 0, 0, 240, 200,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);
	Butt_splash = pw_create_button(splash_screen,
 50, 100, 50,30,
                        PW_ACTIVE_IMAGE, "ADD.png",
                        PW_NORMAL_IMAGE, "ADD.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_splash_screen,
                        PW_CONFIG_END);

	pw_eventloop();
	
	pw_exit();

	return (0);


}
