#include "types.h"
pw_widget entry_mccid[2],frame_mcclogin_page,lbl_mcc_login[2],btn_mcc_ok,btn_mccback;

void hide_frame_mcclogin_page();
void show_frame_mcclogin_page();
void show_mcc_module();
void show_frame_optionspage();
void entry_mcc_back()
{
	hide_frame_mcclogin_page();
	show_frame_optionspage();
}
int entry_mcc_event(pw_widget widget, pw_event event)
{
	int  ret, ret1;
	char username[10], password[10];
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "qwerty");
	strcpy(username, pw_get_text(entry_mccid[1]));
	strcpy(password, pw_get_text(entry_mccid[2]));

	printf("username = %s\n", username);
	printf("password = %s\n", password);

	ret=strcmp(username, "mcclogin");
	printf("ret is %d\n", ret);
	ret1=strcmp(password, "mcclogin");
	printf("ret1 is %d\n", ret1);

	if(ret==0 && ret1==0)
	{
		hide_frame_mcclogin_page();
		show_mcc_module();
	}
	else
	{
		show_message_frame("WRONG LOGIN TRY AGAIN");
		pw_entry_clear(entry_mccid[2]);
		return -1;
	}

	return 0;
}
void create_frame_mcclogin_page(void)
{
	frame_mcclogin_page = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE,
			PW_CONFIG_END);
	pw_hide(frame_mcclogin_page);
	lbl_mcc_login[1] = pw_create_label(frame_mcclogin_page, 5, 20, 70, 30,
			PW_TEXT, "UserId:",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_mccid[1] = pw_create_entry(frame_mcclogin_page, 100, 20, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha
			, PW_NA);
	pw_focus(entry_mccid[1]);

	pw_entry_clear(entry_mccid[1]);


	lbl_mcc_login[2] = pw_create_label(frame_mcclogin_page, 5, 70, 70, 30,
			PW_TEXT, "Password:",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_mccid[2] = pw_create_entry(frame_mcclogin_page, 100, 70, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha
			, PW_NA);

	pw_entry_passwdchar( entry_mccid[2], '*' );
	pw_entry_clear(entry_mccid[2]);

	btn_mccback = pw_create_button(frame_mcclogin_page, 40, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_mcc_back,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);

	btn_mcc_ok = pw_create_button(frame_mcclogin_page, 140, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_OK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_mcc_event,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);
}
void show_frame_mcclogin_page(void)
{
	if(!frame_mcclogin_page)
	{
		create_frame_mcclogin_page();
		pw_show(frame_mcclogin_page);
	}
	else
		pw_show(frame_mcclogin_page);
	pw_entry_clear(entry_mccid[1]);
	pw_entry_clear(entry_mccid[2]);
	pw_focus(entry_mccid[1]);
}

void hide_frame_mcclogin_page(void)
{
	pw_hide(frame_mcclogin_page);
}
