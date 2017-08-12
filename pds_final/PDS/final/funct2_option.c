// the page that has two buttons: stock-Demand or sale/transaction of goods
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

pw_widget toplevel, opt2_screen ,opt2_butt_demand, opt2_butt_cust;

void show_frame_dem1();
void show_frame_supp4(int type);
void show_frame_idnumberpage();
void opt2_hide_screen()
{
	pw_hide(opt2_screen);
	return;
}
Bool opt2_demand_cb()
{
	opt2_hide_screen();
	show_frame_supp4(2);	
	return False;
}
Bool opt2_cust_cb()
{
	opt2_hide_screen();
	show_frame_idnumberpage();	
	return False;
}
void show_frame_opt2()
{
//page graphics begin
	opt2_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	opt2_butt_demand = pw_create_button(opt2_screen, 15, 85, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/dem.png",
                        PW_NORMAL_IMAGE, "/root/PDS/dem.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, opt2_demand_cb,
                        PW_CONFIG_END);

	opt2_butt_cust = pw_create_button(opt2_screen, 125, 85, 100,40,
                        PW_ACTIVE_IMAGE, "/root/PDS/salemod.png",
                        PW_NORMAL_IMAGE, "/root/PDS/salemod.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, opt2_cust_cb,
                        PW_CONFIG_END);

	return;
}
