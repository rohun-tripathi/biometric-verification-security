#include "types.h"
pw_widget frame_pos_details,lbl_gpinfo[7],btn_gpinfo_back,frame_pos_mngmnt; 
void cb_pos_back()
{
	pw_hide(frame_pos_details);
	pw_show(frame_optionspage);
}
void create_frame_pos_details(void)
{
	frame_pos_details = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	pw_hide(frame_pos_details);

	lbl_gpinfo[1] =  pw_create_label(frame_pos_details, 30, 20, 210, 30,
			PW_TEXT, "GP SYSTEM INFORMATION",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_gpinfo[2]  =  pw_create_label(frame_pos_details, 10, 80, 80, 20,
			PW_TEXT, "DISTRICT:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_gpinfo[3]  =  pw_create_label(frame_pos_details, 110, 80, 140, 20,
			PW_TEXT, "NIZAMABAD",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_gpinfo[4]  =  pw_create_label(frame_pos_details, 10, 130, 80, 20,
			PW_TEXT, "MANDAL:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_gpinfo[5]  =  pw_create_label(frame_pos_details, 110, 130, 140, 20,
			PW_TEXT, "BHIKNOOR",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_gpinfo[6]  =  pw_create_label(frame_pos_details, 10, 180, 80, 20,
			PW_TEXT, "PANCHAYAT:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_gpinfo[7]  =  pw_create_label(frame_pos_details, 110, 180, 140, 20,
			PW_TEXT, "Tippapur",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	btn_gpinfo_back = pw_create_button(frame_pos_details, 90, 230, 60, 30,
			PW_TEXT, "Back",
			PW_FGCOLOR, "#3b3b3b",
			PW_FONT, PW_BOLD_FONT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pos_back,
			PW_CONFIG_END);
	return;
}

void show_frame_pos_details()
{
	if(!frame_pos_details)
	{
		create_frame_pos_details();
		pw_show(frame_pos_details);
	}
	else
		pw_show(frame_pos_details);
}

void hide_frame_pos_details(void)
{
	pw_hide(frame_pos_details);
}

