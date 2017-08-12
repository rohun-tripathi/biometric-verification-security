// the one being used cb_splash_screem not working

#include<pwrap2.h>

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"features.h"

pw_widget toplevel,id_screen;
pw_widget order_screen, order_submit_button, order_entry,order_label;
pw_widget combo_pd,combo_qty,order_label_code, order_label_pd, order_label_quantity, order_label_rate, order_labelo_rate, order_labelo_code, order_labelo_price, order_label_price, check_screen, check_label;

unsigned int price;
char price2[20];
void show_frame_done();

void hide_check_screen() 
{
	pw_hide(check_screen);
}
void hide_order_screen()
{
	pw_hide(order_screen);
}
Bool show_done_payment()
{
	hide_check_screen();
	hide_order_screen();
	show_frame_done(price);
	return False;
}

void check_user_usage()
{
	check_screen = pw_create_frame(toplevel,0, 75 , 240, 155,
                        PW_BGCOLOR, GREEN ,
                        PW_CONFIG_END);

	check_label = pw_create_label(check_screen, 50, 50, 100, 40,
			PW_TEXT,"checking please wait",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREEN,
			PW_CONFIG_END);
	pw_show(check_screen);
	sleep(2);
	show_done_payment();			
	return;
}

Bool chng_order_page()
{
/*	char name[20];
	strcpy(name,pw_get_text(id_entry));
	if(atoi(name)==1)
	{*/
		hide_order_screen();
	/*	show_frame_order();
	}
	else
	{
		pw_set_text(id_label,"please try again");
	}*/		 
	return False;
}

void combo_change_price()
{
	char * quantity;
	unsigned int idqty;
	char * product;
	unsigned int idpd;
	
	int ten =10;
	
	pw_combobox_get_selection(combo_pd,&product,&idpd);
	//strcpy(sel_mandal_name,product);
	free(product);

	pw_combobox_get_selection(combo_qty,&quantity,&idqty);
	//strcpy(sel_mandal_name,product);
	//free(product);

	idqty++;
	idpd++;
	
	if(idqty<3 && idqty >0 && idpd<3 && idpd >0)
	{
		price = (idqty)*(idpd)*(ten);
		sprintf(price2,"%d",price);
		pw_set_text(order_labelo_price,price2);

	}
	else
	{
		pw_set_text(order_labelo_price,"");
	}
	free(quantity);
}

void combo_change_code()
{
	char * product;
	unsigned int idx;
	pw_combobox_get_selection(combo_pd,&product,&idx);
	//strcpy(sel_mandal_name,product);
	free(product);
	if (idx==0)
	{
		pw_set_text(order_labelo_code,"1234");
		pw_set_text(order_labelo_rate,"Rs 10/Kg");
	}
	else if (idx==1)
	{
		pw_set_text(order_labelo_code,"4321");
		pw_set_text(order_labelo_rate,"Rs 20/Kg");
	}	
	else
	{
		pw_set_text(order_labelo_code,"");
		pw_set_text(order_labelo_rate,"");
	}					
}
void show_frame_order()
{

	order_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, VIOLET ,
                        PW_CONFIG_END);
	order_label = pw_create_label(order_screen, 10, 10, 100, 20,
			PW_TEXT,"Order Please",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);

	order_label_pd = pw_create_label(order_screen, 10, 40, 70, 20,
			PW_TEXT,"Product",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	combo_pd= pw_create_combobox(order_screen, 100, 40, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_insert(combo_pd,"w1",True);
	pw_combobox_insert(combo_pd,"r2",True);
	pw_combobox_selection_intimate(combo_pd,combo_change_code);

	order_label_code = pw_create_label(order_screen, 10, 70, 70, 20,
			PW_TEXT,"Code",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);	
	order_labelo_code = pw_create_label(order_screen, 100, 70, 70, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	
	order_label_rate = pw_create_label(order_screen, 10, 100, 70, 20,
			PW_TEXT,"Rate",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	order_labelo_rate = pw_create_label(order_screen, 100, 100, 70, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	
	order_label_quantity = pw_create_label(order_screen, 10, 130, 70, 20,
			PW_TEXT,"Quantity",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	combo_qty= pw_create_combobox(order_screen, 100, 130, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_insert(combo_qty,"1 kg",False);
	pw_combobox_insert(combo_qty,"2 kg",False);
	pw_combobox_selection_intimate(combo_qty,combo_change_price);

	order_label_price = pw_create_label(order_screen, 10, 160, 70, 20,
			PW_TEXT,"Price",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	order_labelo_price = pw_create_label(order_screen, 100, 160, 70, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);

 
	order_submit_button = pw_create_button(order_screen,55, 190, 50,20,
                        PW_ACTIVE_IMAGE, "ADD.png",
                        PW_NORMAL_IMAGE, "ADD.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, check_user_usage
,
                        PW_CONFIG_END);
	
	


	//pw_eventloop();
	return;

}	
