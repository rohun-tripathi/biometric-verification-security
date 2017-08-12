// the page that has two buttons: stock exchange or customer and sale
// author rohun tripati
// the one after the authentication is passed by the admin
// this is followed by customer id

#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif


pw_widget toplevel, mis2_screen ,mis2_butt_stock, mis2_butt_last,mis2_butt_today, mis2_butt_sinlast;

void mis2_hide_screen()
{
	pw_hide(mis2_screen);
	return;
}

void show_frame_mis3();
void show_frame_mis4();
void show_frame_mis5();
void show_frame_mis6();

Bool mis2_stock_cb()
{
	mis2_hide_screen();
	show_frame_mis3();	
	return False;
}
Bool mis2_last_cb()
{
	mis2_hide_screen();
	show_frame_mis4();	
	return False;
}
Bool mis2_today_cb()
{
	mis2_hide_screen();
	show_frame_mis5();	
	return False;
}
Bool mis2_sinlast_cb()
{
	mis2_hide_screen();
	show_frame_mis6();	
	return False;
}

void show_frame_mis2()
{
//page graphics begin
	mis2_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	mis2_butt_stock = pw_create_button(mis2_screen, 15, 45, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/stock.png",
                        PW_NORMAL_IMAGE, "/root/PDS/stock.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, mis2_stock_cb,
                        PW_CONFIG_END);

	mis2_butt_last = pw_create_button(mis2_screen, 125, 45, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/sinlast.png",
                        PW_NORMAL_IMAGE, "/root/PDS/sinlast.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, mis2_last_cb,
                        PW_CONFIG_END);

	mis2_butt_today = pw_create_button(mis2_screen, 125, 145, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/soldtoday.png",
                        PW_NORMAL_IMAGE, "/root/PDS/soldtoday.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, mis2_today_cb,
                        PW_CONFIG_END);

	mis2_butt_sinlast = pw_create_button(mis2_screen, 15, 145, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/soldsinlast.png",
                        PW_NORMAL_IMAGE, "/root/PDS/soldsinlast.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, mis2_sinlast_cb,
                        PW_CONFIG_END);
	return;
}
