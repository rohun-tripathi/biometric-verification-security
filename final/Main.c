// the page that opens up first
// author rohun tripati
// the one with four buttons

#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

#include"features_live.h"
#include"name.h"
#ifdef __arm__
#include <biometric.h>
#endif


pw_widget toplevel, pre_screen ,shop_butt, root_butt, supplier_butt, MIS_butt,butt_always_home;

void hide_pre_screen()
{
	pw_hide(pre_screen);
	return;
}

void show_frame_shop();
void show_frame_root();

// these will be created when the pages are created and the functions will become like the two above
void show_frame_mis();
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
Bool cb_supplier_screen()
{
	hide_pre_screen();
	show_frame_supp();	
	return False;
}
Bool cb_MIS_screen()
{
	hide_pre_screen();
	show_frame_mis();	
	return False;
}
void show_frame_home()
{
	
//page graphics begin
	pre_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	root_butt = pw_create_button(pre_screen, 70, 10, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/admin.png",
                        PW_NORMAL_IMAGE, "/root/PDS/admin.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_root_screen,
                        PW_CONFIG_END);

	shop_butt = pw_create_button(pre_screen, 70, 65 , 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/sale.png",
                        PW_NORMAL_IMAGE, "/root/PDS/sale.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_shop_screen,
                        PW_CONFIG_END);

	supplier_butt = pw_create_button(pre_screen, 70, 120, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/supplier.png",
                        PW_NORMAL_IMAGE, "/root/PDS/supplier.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_supplier_screen,
                        PW_CONFIG_END);

	MIS_butt = pw_create_button(pre_screen, 70, 175, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/Inspector.png",
                        PW_NORMAL_IMAGE, "/root/PDS/Inspector.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_MIS_screen,
                        PW_CONFIG_END);
}
void cb_topbar_button()
{
//	pw_hide(splash_screen);
//	pw_hide(bcust_screen);
//	pw_hide(order_screen);
//	pw_hide(id_screen);
	show_frame_home();
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

	butt_always_home = pw_create_button(toplevel, 160, 10, 80 ,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/home.png",
                        PW_NORMAL_IMAGE, "/root/PDS/home.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_topbar_button,
                        PW_CONFIG_END);

	show_frame_home();

	pw_eventloop();
	pw_exit();
	return (0);
}
