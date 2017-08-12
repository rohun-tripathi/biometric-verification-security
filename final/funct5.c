#include<pwrap2.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"features_live.h"

pw_widget toplevel,id_screen;
pw_widget order_screen, order_submit_button, order_entry,order_label;
pw_widget combo_pd,combo_qty,order_label_code, order_label_pd, order_label_quantity, order_label_rate, order_labelo_rate, order_labelo_code, order_labelo_price, order_label_price, check_screen, check_label;

pw_widget done_screen,done_label_price,done_labelo_price, done_Butt, done_label;
unsigned int price;

void show_frame_home();

void hide_done_screen()
{
	pw_hide(done_screen);
}

Bool cb_done_screen()
{
	hide_done_screen();
	show_frame_home();
	return False;
}

void show_frame_done( unsigned int price )
{
	
	char price2[20];
	done_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);
	

	done_label_price = pw_create_label(done_screen, 10, 20, 70, 20,
			PW_TEXT,"Price",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);
	sprintf(price2,"%d",price);

	done_labelo_price = pw_create_label(done_screen, 100, 20, 70, 20,
			PW_TEXT, price2,
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	done_label = pw_create_label(done_screen, 10, 50, 180, 35,
			PW_TEXT,"Make Payment to complete transaction",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);
	done_Butt = pw_create_button(done_screen, 10, 120, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/home.png",
                        PW_NORMAL_IMAGE, "/root/PDS/home.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_done_screen,
                        PW_CONFIG_END);
	return;
}
