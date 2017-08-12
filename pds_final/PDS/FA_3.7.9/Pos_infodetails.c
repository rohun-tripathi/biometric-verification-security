#include "types.h"

pw_widget frame_pos_infodetails,lbl_gpinfo[15],btn_gpinfo_back;
char *exec_query_get_single_value(char *data, char *query, char *database);
void show_fa_module();
void hide_frame_pos_infodetails();
int no_worker_found;
char dt_name[10][10],assembly_name[50][50],mandal_name[50][50],panchayat_name[50][50];

int get_posdetails(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(dt_name[no_worker_found], argv[1]);
	strcpy(assembly_name[no_worker_found],argv[2]);
	strcpy(mandal_name[no_worker_found], argv[3]);
	strcpy(panchayat_name[no_worker_found], argv[4]);
	no_worker_found++;
	return(0);
}
char *pos_user=NULL,pos_no[2];
char dist_name[50],assb_name[50],mand_name[50],pancha_name[50];
int pos_details()
{
	sqlite3 *dbhandle=NULL;
	char query[512];
	int i;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	sprintf(query,"select ifnull(count(*),0),ifnull(district,0),ifnull(assembly,0),ifnull(mandal,0),ifnull(panchayat,0) from jc_info ");
	no_worker_found=0;
	sqlite3_exec(dbhandle, query, get_posdetails, NULL, NULL);

	for(i=0;i<no_worker_found;i++)
	{
		sprintf(dist_name,"%s",dt_name[i] );
		sprintf(assb_name,"%s",assembly_name[i]);
		sprintf(mand_name,"%s",mandal_name[i]);
		sprintf(pancha_name,"%s",panchayat_name[i]);
	}

	sprintf(query,"select substr(jc_code,10,1) from jc_info");

	pos_user = exec_query_get_single_value(pos_user, query, DB_PACK);
	sprintf(pos_no,"%s",pos_user);
	printf("pos no is %s",pos_no);
	sqlite3_close(dbhandle);
	return 0;
}
void cb_pos_back(void)
{
	pw_hide(frame_pos_infodetails);
	show_fa_module();
}
void create_frame_pos_infodetails(void)
{
	frame_pos_infodetails = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	pw_hide(frame_pos_infodetails);
	lbl_gpinfo[0] =  pw_create_label(frame_pos_infodetails, 30, 0, 210, 20,
			PW_TEXT, "GP SYSTEM INFORMATION",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_gpinfo[1]  =  pw_create_label(frame_pos_infodetails, 10, 35, 80, 20,
			PW_TEXT, "District:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_gpinfo[2]  =  pw_create_label(frame_pos_infodetails, 110, 35, 140, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_gpinfo[3]  =  pw_create_label(frame_pos_infodetails, 10, 70, 80, 20,
			PW_TEXT, "Assembly:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_gpinfo[4]  =  pw_create_label(frame_pos_infodetails, 110, 70, 140, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	lbl_gpinfo[5]  =  pw_create_label(frame_pos_infodetails, 10, 105, 80, 20,
			PW_TEXT, "Mandal:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_gpinfo[6]  =  pw_create_label(frame_pos_infodetails, 110, 105, 140, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_gpinfo[7]  =  pw_create_label(frame_pos_infodetails, 10, 140, 80, 20,
			PW_TEXT, "Panchyat:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_gpinfo[9]  =  pw_create_label(frame_pos_infodetails, 110, 140, 140, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	lbl_gpinfo[12]  =  pw_create_label(frame_pos_infodetails, 10, 175, 130, 20,
			PW_TEXT, "Pos User:",
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "green",
			PW_CONFIG_END);
	lbl_gpinfo[13]  =  pw_create_label(frame_pos_infodetails, 135, 175, 25, 20,
			PW_TEXT, "",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "green",
			PW_CONFIG_END);

	btn_gpinfo_back = pw_create_button(frame_pos_infodetails, 60, 230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pos_back,
			PW_CONFIG_END);

	return;
}
//Creation of Frame for displaying Pos Details,version etc..
void show_frame_pos_infodetails()
{
	if(! frame_pos_infodetails)
	{
		create_frame_pos_infodetails();
		pw_show(frame_pos_infodetails);
	}
	else
		pw_show(frame_pos_infodetails);
	pos_details();

	pw_set_text(lbl_gpinfo[2],dist_name);
	pw_set_text(lbl_gpinfo[4],assb_name);
	pw_set_text(lbl_gpinfo[6],mand_name);
	pw_set_text(lbl_gpinfo[9],pancha_name);
	pw_set_text(lbl_gpinfo[13],pos_no);
	return;
}
void hide_frame_pos_infodetails(void)
{
	pw_hide(frame_pos_infodetails);
}
