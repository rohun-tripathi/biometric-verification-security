#include "types.h"

void show_progress_frame();
void hide_progress_frame();
void show_frame_pos_infodetails();
int gp_test();
void show_frame_pos_appsupdates();
char *exec_query_get_single_value(char *data, char *query, char *database);

int birs_download()
{
	char *pid_value=NULL,birs_data[512],query[256],zip_data[10],cp_birs[50];
	chdir("/home/");
	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");
	pid_value = exec_query_get_single_value(pid_value, query, DB_PACK);
	sprintf(zip_data,"%s.zip",pid_value);
	printf("zip data is %s\n",zip_data);
	sprintf(birs_data,"curl http://emulate.aponline.gov.in/%s -o %s",zip_data,zip_data);
	printf("birs_data is %s\n",birs_data);
	ret =system(birs_data);
	printf("\n file download ret is :%d:\n",ret);
	if(ret==0)
	{
		system(" unzip *.zip");
		sprintf(cp_birs,"cp /home/%s/* /home/Backup/",pid_value);
		//system("mv /home/zip_data/*   /home/Backup/" );
		system(cp_birs);
		system("rm -r 18*");
		show_frame_pos_appsupdates();
		show_message_frame("SUCCESS");
		return 0;
	}
	else
	{
		show_progress_frame(" Try Again.... ");
		hide_progress_frame();
		show_frame_pos_appsupdates();
		return 0;
	}

	return 0;
}
void show_birs_dwnld_frame()
{
	ret= gp_test();
	printf("\n ret is %d\n",ret);
	ret=pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OKCANCEL,
			PW_MSGBOX_ICON_QUESTION, "Alert", "Do You Want To Continue ");
	if(ret==PW_MSGBOX_RETURN_OK)
	{
		show_progress_frame(" Downloading Please Wait ");
		birs_download();
		hide_progress_frame();
		return;
	}
	else
	{
		show_frame_pos_appsupdates();
		return ;
	}
	return;
}
