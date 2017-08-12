#include "types.h"

void show_progress_frame();
void hide_progress_frame();
void show_frame_pos_infodetails();
int gp_test();
void show_frame_pos_appsupdates();
void show_frame_memory_info();

int apps_download()
{
	chdir("/usr/local/apps/nrega_AP/");
	ret=system("curl http://emulate.aponline.gov.in/WSS.zip -o WSS.zip");
	printf("\n file download ret is :%d:\n",ret);
	if(ret==0)
	{
		system("unzip WSS.zip");
		system("mv /usr/local/apps/nrega_AP/WSS/faapps/FA   /usr/local/apps/nrega_AP/" );
		chdir("/usr/local/audio/");
		system("mv /usr/local/apps/nrega_AP/WSS/faapps/S2.mp3   /usr/local/audio/" );
		system("rm -r WSS*");
		system("chmod 777 /usr/local/apps/nrega_AP/FA");
		show_message_frame("SUCCESS");
		system("killall -9 FA");	

		return 0;
	}
	else
		show_progress_frame(" Try Again.... ");
	hide_progress_frame();
	return 0;
}
void show_apps_dwnld_frame()
{
	ret= gp_test();
	printf("\n ret is %d\n",ret);
	ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
			PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
	if(ret==PW_MSGBOX_RETURN_OK)
	{
		system("cp /usr/local/apps/nrega_AP/FA  /home/");
		show_progress_frame(" Downloading Please Wait ");
		apps_download();
		hide_progress_frame();
	}
	else
	{
		show_frame_pos_appsupdates();
		return ;
	}
	return;
}
