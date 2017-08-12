#include "types.h"

char *exec_query_get_single_value(char *data, char *query, char *database);
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
void show_progress_frame();
void hide_progress_frame();
void show_frame_pos_details();
void hide_frame_pos_details();

pw_widget frame_pos_details,btn_back, label[6],combo_dist,btn_submit,combo_assmb,combo_mand,combo_panch,combo_hab,combo_posno;
char query[1024],sel_assmb_name[40],assmbly_id[5], district_id[3],sel_mandal_name[40],sel_pancht_name[40], mandal_id[5],panchayat_id[5],sel_hab_name[40],habit_code[10][10],habit_name[30][30];
char sel_dtname[50];
char village_code[10],habitation_code[10],panchayat_name[50];
int no_worker_found;
int get_habitation(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(habit_code[no_worker_found], argv[0]);
	strcpy(habit_name[no_worker_found],argv[1]);
	no_worker_found++;
	return(0);
}
char habitation_id[10];
int cb_pos_details()
{
	sqlite3 *dbhandle=NULL;
	int i;

	char query_insert[512],sel_pos_id[2],posmgmt_value[15];
	strcpy(sel_pos_id,pw_get_text(combo_posno));
	printf("selected pos id is %s\n",sel_pos_id);
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	if((strcmp(district_id,"")==0) || (strcmp(mandal_id,"")==0) || (strcmp(panchayat_id,"")==0) || (strcmp(assmbly_id,"")==0) || (strcmp(habitation_id,"")==0) || (strcmp(sel_pos_id,"")==0))
	{
		show_message_frame("SELECT DATA");
		return 0 ;	
	}
	sprintf(query,"delete from  tmp_habitation_master");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"select village_code||habitation_code,habitation_name from habit_master where district_code='%s' and mandal_code='%s' and panchayat_code='%s' ",district_id,mandal_id,panchayat_id);

	no_worker_found=0;
	sqlite3_exec(dbhandle, query, get_habitation, NULL, NULL);
	for(i=0; i<no_worker_found; i++)
	{
		sprintf(query_insert,"insert into tmp_habitation_master (habitation_code,habitation_name) values('%s','%s')",habit_code[i],habit_name[i]);

		sqlite3_exec(dbhandle, query_insert, NULL, NULL, NULL);
	}
	sprintf(posmgmt_value,"%s%s%s%s%s",district_id,assmbly_id,mandal_id,panchayat_id,pw_get_text(combo_posno));
	sprintf(query,"delete from  jc_info");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"insert into jc_info(jc_code,district ,assembly,mandal ,panchayat) values ('%s','%s','%s','%s','%s')",posmgmt_value,sel_dtname,sel_assmb_name,sel_mandal_name,sel_pancht_name);

	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sqlite3_close(dbhandle);

	pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,
			PW_MSGBOX_ICON_NOICON, "SUCCESS", "Successfully Submitted");
	pw_show(frame_pos_details);
	return 0;
}
void combo_change_hab(pw_widget widget, int index)
{
	char *habitationname;
	unsigned idx;
	pw_combobox_get_selection(combo_hab,&habitationname,&idx);
	strcpy(sel_hab_name,habitationname);
	free(habitationname);

	char *habitation_code=NULL;

	sprintf(query,"select village_code||habitation_code from habit_master where habitation_name='%s' and panchayat_code='%s' and Mandal_Code='%s' and District_Code='%s'",sel_hab_name,panchayat_id,mandal_id,district_id);
	habitation_code = exec_query_get_single_value(habitation_code,query, DB_PACK);
	sprintf(habitation_id,"%s",habitation_code);
	free(habitation_code);

	label[6] =  pw_create_label(frame_pos_details,05, 180, 80, 20,
			PW_TEXT, "Pos User:",
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_posno= pw_create_combobox(frame_pos_details, 80, 180, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	pw_combobox_insert(combo_posno,"1",False);
	pw_combobox_insert(combo_posno,"2",True);
}
void combo_change_panch(pw_widget widget, int index)
{
	char *panchayatname;
	unsigned idx;
	pw_combobox_get_selection(combo_panch,&panchayatname,&idx);
	strcpy(sel_pancht_name,panchayatname);
	free(panchayatname);

	char *panchid=NULL;
	sprintf(query,"select panchayat_code from panchayat_master where district_code='%s' and  mandal_code='%s' and panchayat_name='%s'",district_id,mandal_id,sel_pancht_name);

	panchid= exec_query_get_single_value(panchid, query, DB_PACK);
	sprintf(panchayat_id,"%s",panchid);
	free(panchid);

	label[5] =  pw_create_label(frame_pos_details,05, 150, 80, 20,
			PW_TEXT, "Habitation:",
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_hab= pw_create_combobox(frame_pos_details, 80, 150, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,	
			PW_NA);

	sprintf(query,"select distinct habitation_name from habit_master where panchayat_code='%s'and Mandal_Code='%s' and District_Code='%s'",panchayat_id,mandal_id,district_id);

	pw_combobox_clear(combo_hab);
	pw_set_text(combo_hab,"--Select--");
	exec_query_load_into_combo(combo_hab, query, DB_PACK);
	pw_combobox_selection_intimate(combo_hab,combo_change_hab);
}
void combo_change_mandal(pw_widget widget, int index)
{
	char *mandalname;
	unsigned idx;
	pw_combobox_get_selection(combo_mand,&mandalname,&idx);
	strcpy(sel_mandal_name,mandalname);
	free(mandalname);
	char *mandid=NULL;
	sprintf(query,"select Mandal_id from Mandal_master where Mandal_Description ='%s' and District_id='%s'",sel_mandal_name,district_id);
	mandid= exec_query_get_single_value(mandid, query, DB_PACK);	
	sprintf(mandal_id,"%s",mandid);
	free(mandid);

	label[4] =  pw_create_label(frame_pos_details,05, 120, 80, 20,
			PW_TEXT, "panchayat:",
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_panch = pw_create_combobox(frame_pos_details, 80, 120, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,	
			PW_NA);
	sprintf(query, "select distinct panchayat_name from  Panchayat_master where mandal_code='%s' and district_code='%s'",mandal_id,district_id);

	pw_combobox_clear(combo_panch);
	pw_set_text(combo_panch,"--Select--");
	exec_query_load_into_combo(combo_panch, query, DB_PACK);//exec_query_load_into_combo
	pw_combobox_selection_intimate(combo_panch,combo_change_panch);

}
void mandal_desc()
{
	sprintf(query,"select distinct Mandal_Description from Mandal_master mm,panchayat_master pm where mm.Mandal_id=pm.Mandal_code and mm.district_id='%s' and pm.assembly_code='%s'",district_id,assmbly_id);
	pw_combobox_clear(combo_mand);
	pw_set_text(combo_mand,"--Select--");
	exec_query_load_into_combo(combo_mand, query, DB_PACK);
	pw_combobox_selection_intimate(combo_mand,combo_change_mandal);
	return ;
}
void combo_change_assmb(pw_widget widget, int index)
{
	char *assmbname;
	unsigned idx;
	pw_combobox_get_selection(combo_assmb,&assmbname,&idx);
	strcpy(sel_assmb_name,assmbname);
	free(assmbname);
	char *assmblyid=NULL;
	sprintf(query, "select assembly_code from assembly_master where assembly_name='%s'",sel_assmb_name);
	assmblyid= exec_query_get_single_value(assmblyid, query, DB_PACK);
	sprintf(assmbly_id,"%s",assmblyid);
	free(assmblyid);

	label[3] =  pw_create_label(frame_pos_details,05, 90, 80, 20,  
			PW_TEXT, "Mandal:",
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	combo_mand= pw_create_combobox(frame_pos_details, 80, 90, 150,20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	mandal_desc();
}
void combo_change_dist(pw_widget widget, int index)
{
	char *dtname;
	unsigned idx;
	pw_combobox_get_selection(combo_dist,&dtname,&idx);
	strcpy(sel_dtname,dtname);
	free(dtname);
	char *distid=NULL;
	sprintf(query, "select district_id from district_master where district_description='%s'",sel_dtname);
	distid= exec_query_get_single_value(distid, query, DB_PACK);
	sprintf(district_id,"%s",distid);
	free(distid);

	label[2] =  pw_create_label(frame_pos_details,05, 60, 80, 20,  
			PW_TEXT, "Assembly:",
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	combo_assmb = pw_create_combobox(frame_pos_details, 80, 60, 150,20, 
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);
	sprintf(query,"select distinct assembly_name from assembly_master where district_code='%s'",district_id);

	pw_combobox_clear(combo_assmb);
	pw_set_text(combo_assmb,"--Select--");

	exec_query_load_into_combo(combo_assmb, query, DB_PACK);	
	pw_combobox_selection_intimate(combo_assmb,combo_change_assmb);
}
void cb_pos_details_back()
{
	hide_frame_pos_details();
	show_mcc_module();
}
void create_frame_pos_details(void)
{

	frame_pos_details = pw_create_frame(toplevel, 0, 0, 240, 270,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);
	pw_hide(frame_pos_details);

	label[0] =  pw_create_label(frame_pos_details, 00, 00, 240, 20,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_TEXT, "Pos Mangement Details",
			PW_BGCOLOR, "grey",PW_CONFIG_END);

	label[1] =  pw_create_label(frame_pos_details,05, 30, 80, 20,
			PW_TEXT, "District:",
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);

	combo_dist = pw_create_combobox(frame_pos_details, 80, 30, 150,20, 
			PW_COMBOBOX_TYPE, PW_COMBOBOX_EDITABLE,
			PW_FGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_dist,combo_change_dist);

	btn_submit = pw_create_button(frame_pos_details, 140,230, 72, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_SUBMIT,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pos_details,
			PW_CONFIG_END);
	btn_back = pw_create_button(frame_pos_details, 40,230, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_pos_details_back,
			PW_CONFIG_END);
}

void show_frame_pos_details(void)
{
	if(!frame_pos_details)
	{
		create_frame_pos_details();
		pw_show(frame_pos_details);
	}
	else
		pw_show(frame_pos_details);
	sprintf(query, "select distinct district_description from district_master");
	pw_combobox_clear(combo_dist);
	pw_set_text(combo_dist,"--Select--");
	exec_query_load_into_combo(combo_dist, query, DB_PACK);
}
void hide_frame_pos_details(void)
{
	pw_hide(frame_pos_details);
}
