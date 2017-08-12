#include "types.h"
pw_widget entry_id[2],frame_loginmvo,lbl_login[3],btn_ok,btn_back;

void hide_frame_loginmvo();
void show_frame_loginmvo();
void show_frame_optionsmvo();
void  show_frame_pos_details();
void show_mcc_module();
void show_frame_muster_verify();
void  show_frame_optionspage();
void entry_event_mvo_back()
{
	hide_frame_loginmvo();
	show_frame_optionspage();
}
int entry_event_mvo(pw_widget widget, pw_event event)
{
	char username[50], password[50];
	int  ret, ret1;
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
	strcpy(username, pw_get_text(entry_id[1]));
	strcpy(password, pw_get_text(entry_id[2]));

	ret=strcmp(username, "mvonzb");
	ret1=strcmp(password, "mvonzb");

	if(ret==0 && ret1==0)
	{
		hide_frame_loginmvo();
		//show_frame_pos_details();
		show_frame_muster_verify();

	}
	else
	{
		show_message_frame("WRONG LOGIN TRY AGAIN");
		pw_entry_clear(entry_id[2]);		
		return -1;
	}

	return 0;
}

void create_frame_loginmvo(void)
{
	frame_loginmvo = pw_create_frame(toplevel, 0, 0, 240, 270, 
			PW_BGCOLOR, BLUE, 
			PW_CONFIG_END);
	pw_hide(frame_loginmvo);
	lbl_login[1] = pw_create_label(frame_loginmvo, 5, 20, 70, 30, 
			PW_TEXT, "UserId:", 
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_id[1] = pw_create_entry(frame_loginmvo, 100, 20, 100, 30, 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_alpha
			, PW_NA);
	pw_focus(entry_id[1]);

	pw_entry_clear(entry_id[1]);


	lbl_login[2] = pw_create_label(frame_loginmvo, 5, 70, 70, 30, 
			PW_TEXT, "Password:", 
			PW_BGCOLOR, BLUE, PW_CONFIG_END);
	entry_id[2] = pw_create_entry(frame_loginmvo, 100, 70, 100, 30, 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_entry_alpha
			, PW_NA);

	pw_entry_passwdchar( entry_id[2], '*' );

	pw_entry_clear(entry_id[2]);

	btn_back = pw_create_button(frame_loginmvo, 40, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK, 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_event_mvo_back, 
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);

	btn_ok = pw_create_button(frame_loginmvo, 140, 150, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_OK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, entry_event_mvo,
			PW_FONT, PW_BOLD_FONT, PW_CONFIG_END);
}
void show_frame_loginmvo(void)
{
	if(!frame_loginmvo)
	{	
		create_frame_loginmvo();
		pw_show(frame_loginmvo);
	}
	else
		pw_show(frame_loginmvo);

	pw_entry_clear(entry_id[1]);
	pw_entry_clear(entry_id[2]);
	pw_focus(entry_id[1]);
}

void hide_frame_loginmvo(void)
{
	pw_hide(frame_loginmvo);
}

