#include "types.h"


pw_widget entry_normalid[5],frame_normallogin_page,lbl_normal_login[4],btn_normal_ok,btn_normalback;
void show_frame_get_normal();
void hide_frame_normallogin_page();
void show_frame_normallogin_page();
void show_normal_module();
void show_frame_get_muster();
void show_frame_optionspage();
void entry_normal_back()
{
	hide_frame_normallogin_page();
	show_frame_get_muster();
}
int entry_normal_event(pw_widget widget, pw_event event)
{
	int  ret, ret1;
	char username[10], password[10];
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "qwerty");
	strcpy(username, pw_get_text(entry_normalid[1]));
	strcpy(password, pw_get_text(entry_normalid[2]));

	printf("username = %s\n", username);
	printf("password = %s\n", password);

	ret=strcmp(username, "nrm123");
	printf("ret is %d\n", ret);
	ret1=strcmp(password, "nrm123");
	printf("ret1 is %d\n", ret1);

	if(ret==0 && ret1==0)
	{
		hide_frame_normallogin_page();
		show_frame_get_normal();
	}
	else
	{
		show_message_frame("WRONG LOGIN TRY AGAIN");
		pw_entry_clear(entry_normalid[2]);
		return -1;
	}

	return 0;
}
void create_frame_normallogin_page(void)
{
	frame_normallogin_page = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, BLUE,
			PW_CONFIG_END);
	pw_hide(frame_normallogin_page);
	lbl_normal_login[1] = pw_create_label(frame_normallogin_page, 5, 20, 70, 30,
			PW_TEXT, "UserId:",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_normalid[1] = pw_create_entry(frame_normallogin_page, 100, 20, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha
			, PW_NA);


	pw_focus(entry_normalid[1]);

	pw_entry_clear(entry_normalid[1]);


	lbl_normal_login[2] = pw_create_label(frame_normallogin_page, 5, 70, 70, 30,
			PW_TEXT, "Password:",
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_normalid[2] = pw_create_entry(frame_normallogin_page, 100, 70, 100, 30,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_alpha
			, PW_NA);

	pw_entry_passwdchar( entry_normalid[2], '*' );
	pw_entry_clear(entry_normalid[2]);

	btn_normalback = pw_create_button(frame_normallogin_page, 40, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_normal_back,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);

	btn_normal_ok = pw_create_button(frame_normallogin_page, 140, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_OK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_normal_event,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);
}
void show_frame_normallogin_page(void)
{
	if(!frame_normallogin_page)
	{
		create_frame_normallogin_page();
		pw_show(frame_normallogin_page);
	}
	else
		pw_show(frame_normallogin_page);
	pw_entry_clear(entry_normalid[1]);
	pw_entry_clear(entry_normalid[2]);
	pw_focus(entry_normalid[1]);
	return;
}

void hide_frame_normallogin_page(void)
{
	pw_hide(frame_normallogin_page);
}
