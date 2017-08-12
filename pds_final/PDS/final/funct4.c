// actual order after verification is placed
/* calculates price and leads to payment and check page
* go to funct5
* come from func3
*/

#include<pwrap2.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif

char userid[20];
char name[20];

pw_widget toplevel,id_screen;
pw_widget order_screen, order_submit_button,order_back_button, order_entry,order_label;
pw_widget combo_pd,combo_qty,order_label_code, order_label_pd, order_label_quantity, order_label_rate, order_labelo_rate, order_labelo_code, order_labelo_price, order_label_price, check_screen, check_label;

unsigned int price;
char price2[20];
void show_frame_done();
void show_frame_bcust(char* userid,char * name);

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

	hide_order_screen();
	show_frame_done(price);
	return False;
}


void back_user_usage()
{
	hide_check_screen();
	show_frame_bcust(userid,name);			
}
void check_user_usage()
{	
	char text[30] = {"Checking Please Wait"};
	display_wait_msg(text);
	sleep(2);
	hide_check_screen();
	show_done_payment();			
	return;
}

void display_wait_msg(char * text)
{
	check_screen = pw_create_frame(toplevel,0, 135 , 240, 120,
                        PW_BGCOLOR, DRKGREY ,
                        PW_CONFIG_END);

	check_label = pw_create_label(check_screen, 20, 30, 120, 60,
			PW_TEXT, text ,
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, DRKGREY,
			PW_CONFIG_END);
	pw_show(check_screen);

	return;
}

/*Bool chng_order_page()
{
	char name[20];
	strcpy(name,pw_get_text(id_entry));
	if(atoi(name)==1)
	{
		hide_order_screen();
		show_frame_order();
	}
	else
	{
		pw_set_text(id_label,"please try again");
	}		 
	return False;
}
*/
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
	
	if(idqty<9 && idqty >0 && idpd<9 && idpd >0)
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


//////////only the graphics part ahead


void show_frame_order(char * ext_userid, char * ext_name)
{

////////copying the incoming stuff
	printf("reached show_frame_order\n");
	if(ext_name!=NULL)
	{
	strcpy(userid,ext_userid);
	strcpy(name,ext_name);
	}
	
	order_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, VIOLET ,
                        PW_CONFIG_END);
	order_label = pw_create_label(order_screen, 10, 10, 100, 20,
			PW_TEXT,"Order Please",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);

	order_label_pd = pw_create_label(order_screen, 15, 40, 70, 20,
			PW_TEXT,"Product: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	combo_pd= pw_create_combobox(order_screen, 140, 40, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_insert(combo_pd,"w1",True);
	pw_combobox_insert(combo_pd,"r2",True);
	pw_combobox_selection_intimate(combo_pd,combo_change_code);

	order_label_code = pw_create_label(order_screen, 15, 70, 70, 20,
			PW_TEXT,"Code: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);	
	order_labelo_code = pw_create_label(order_screen, 140, 70, 70, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	
	order_label_rate = pw_create_label(order_screen, 15, 100, 70, 20,
			PW_TEXT,"Rate: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	order_labelo_rate = pw_create_label(order_screen, 140, 100, 70, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);
	
	order_label_quantity = pw_create_label(order_screen, 15, 130, 70, 20,
			PW_TEXT,"Quantity: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	combo_qty= pw_create_combobox(order_screen, 140, 130, 70, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR, "white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_insert(combo_qty,"1 kg",False);
	pw_combobox_insert(combo_qty,"2 kg",False);
	pw_combobox_insert(combo_qty,"3 kg",False);
	pw_combobox_insert(combo_qty,"4 kg",False);
	pw_combobox_insert(combo_qty,"5 kg",False);
	pw_combobox_insert(combo_qty,"6 kg",False);
	pw_combobox_insert(combo_qty,"7 kg",False);
	pw_combobox_insert(combo_qty,"8 kg",False);
	pw_combobox_selection_intimate(combo_qty,combo_change_price);

	order_label_price = pw_create_label(order_screen, 15, 160, 70, 20,
			PW_TEXT,"Price: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	order_labelo_price = pw_create_label(order_screen, 140, 160, 70, 20,
			PW_TEXT,"",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_CONFIG_END);

 
	order_submit_button = pw_create_button(order_screen,110, 190, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/submit.png",
                        PW_NORMAL_IMAGE, "/root/PDS/submit.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, check_user_usage
,
                        PW_CONFIG_END);
	order_back_button = pw_create_button(order_screen,10, 190, 80,20,
                        PW_ACTIVE_IMAGE, "/root/PDS/back.png",
                        PW_NORMAL_IMAGE, "/root/PDS/back.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, back_user_usage
,
                        PW_CONFIG_END);
	
	//pw_eventloop();
	return;
}	
