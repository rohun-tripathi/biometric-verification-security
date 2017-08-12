#include "types.h"
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
pw_widget combo_benef, label_beneficary[6] , label_beneficary_back,combo_benef_hab ,button_next,label_get_detail[4],txt_entry_jcno,Butt_jcno_search,button_reenroll;
void hide_frame_beneficary_details();
void show_frame_bioenrollpage();
int  verify_customer_from_enrollment_db();
void show_progress_frame();
void hide_progress_frame(); 
void show_frame_optionspage();
int exec_query_load_into_combo(pw_widget combo, char *query, char *database);
char *exec_query_get_single_value(char *data, char *query, char *database);
void show_frame_wageseeker_details();
char wrkcode[20], name[100],  gender[02], skill_wrkcode[20], skill_name[50], skill_gender[02],dist_mand_pan_id[20],count_jc[5],sel_jc[20];
char query[1024];
char *enroll_name()
{
	return name;
}
int get_jobcard(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(count_jc,argv[0]);
	strcpy(wrkcode, argv[1]);
	strcpy(name, argv[2]);
	strcpy(gender, argv[3]);
	return(0);
}
int get_skill_jobcard(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(skill_wrkcode, argv[0]);
	strcpy(skill_name, argv[1]);
	strcpy(skill_gender, argv[3]);
	return(0);
}
char comp_value[21],sel_habname[50],jc_value[21],sel_jobcardno[21];
void cb_beneficary_details_reenroll(pw_widget w, pw_event e)
{
     //   if(strcmp(w->tag, TAG_ENROLL) == 0)
       // {
                hide_frame_beneficary_details();
                show_frame_bioenrollpage(jc_value);
        //}
        return;
}

void cb_beneficary_details_next(pw_widget w, pw_event e)
{
	if(strcmp(w->tag, TAG_ENROLL) == 0)
	{
		hide_frame_beneficary_details();
		show_frame_bioenrollpage(jc_value);
	}
	return;
}
int check_enrollment()
{
	char *tot_jc_cnt=NULL;	
	sprintf(query,"select  ifnull(count(*),'0') from enrollment_master  where jobcard_no='%s'",jc_value);
	tot_jc_cnt = exec_query_get_single_value(tot_jc_cnt, query, DB_PACK);
	if(atoi(tot_jc_cnt)>0)
	{
		pw_disable(button_next);
		pw_disable(button_reenroll);   
	}
	free(tot_jc_cnt);
	return 0;
}
int cb_beneficary_details()
{
	sqlite3 *dbhandle=NULL;
	char *error ;
	int imgFD=0, fsz=0,i,count_fsz=0,ret1;
	char fname[256];
	struct stat fst;

	jc_value[0]='\0';

	sprintf(jc_value,"%s%s",comp_value,sel_jobcardno);

	ret1=strcmp(sel_habname,"");
	if(ret1==0)
	{
		show_message_frame("SELECT HABITATION");
		return 0;
	}

	ret= strcmp(sel_jobcardno, "");
	if(ret==0)
	{
		show_message_frame("SELECT JOBCARDNO");
		return 0;
	}

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");

	if(strlen(sel_jobcardno)==6)
	{
		sprintf(query, "select ifnull(count(*),0), ifnull(worker_code,0), ifnull(name,0), ifnull(sex,0) from household_details where household_code||worker_code='%s' and is_active='Y' ",jc_value);

			printf("query is %s\n", query);

		sqlite3_exec(dbhandle, query, get_jobcard, NULL, NULL);

		if(atoi(count_jc)==0)
		{
			show_message_frame("SELECT JOBCARDNO");
			return 0;
		}
		for(i=0; i<=8; i++)
		{
			sprintf(fname, "/home/Backup/%s_%d.bir",jc_value, i);
			printf("\n fname is %s\n",fname);
			imgFD = open(fname,O_RDONLY);
			printf("\n imfd is %d\n",imgFD);
			fstat(imgFD, &fst);
			fsz = fst.st_size;
			printf("the size fsz = %d \n ", fsz);
			//if(fsz <= 2)
			if(imgFD==-1)
			{
				count_fsz=count_fsz+1;
				printf("\n  count_fsz is %d\n", count_fsz);
			}
		}
		if(count_fsz>=6)
		{
			pw_set_text(label_get_detail[3], "NOT ENROLLED");
			pw_enable(button_next);
			pw_redraw(button_next);
			pw_disable(button_reenroll);
		}
		else
		{
			pw_set_text(label_get_detail[3], "ENROLLED");
			pw_disable(button_next);
			pw_enable(button_reenroll);
			check_enrollment();
					}
		sqlite3_close(dbhandle);
		pw_set_text(label_get_detail[0], name);
		pw_set_text(label_get_detail[1], gender );
		pw_set_text(label_get_detail[2], wrkcode );
	}
	else
	{
		sprintf(query, "select SkilledWorker_Code, name, age, sex from skilledworker_info where SkilledWorker_Code='%s'", jc_value);
		printf("query2 is %s\n", query);
		sqlite3_exec(dbhandle, query, get_skill_jobcard, NULL, &error);

		for(i=0; i<=9; i++)
		{
			sprintf(fname, "/home/Backup/%s_%d.bir",jc_value, i);
			//	printf("\n fname is %s\n",fname);
			imgFD = open(fname,O_RDONLY);
			//	printf("\n imfd is %d\n",imgFD);
			fstat(imgFD, &fst);
			fsz = fst.st_size;
			//			printf("the size fsz = %d \n ", fsz);
			if(fsz <= 0)
			{
				count_fsz=count_fsz+1;
				//				printf("\n  count_fsz is %d\n", count_fsz);
			}
		}
		if(count_fsz>5)
		{
			pw_set_text(label_get_detail[3], "NOT ENROLLED");
			pw_enable(button_next);
			pw_redraw(button_next);
		}
		else
		{
			pw_set_text(label_get_detail[3], "ENROLLED");
			pw_disable(button_next);
			check_enrollment();	
		}
		sqlite3_close(dbhandle);
		pw_set_text(label_get_detail[0], skill_name);
		pw_set_text(label_get_detail[1], skill_gender );
		pw_set_text(label_get_detail[2], skill_wrkcode );
		//		pw_set_text(label_get_detail[3], enroll_status);
	}
	return 0;
}
int get_jc_searchno(pw_widget widget, int index)
{
	 pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "123");
        char *hh_cnt=NULL;

        strcpy(sel_jc,pw_get_text(txt_entry_jcno));

        if(strcmp(sel_habname,"")==0)

        {
                show_message_frame("SELECT HABITATION");
                return 0;
        }
         pw_combobox_clear(combo_benef);
        sprintf(query,"select count(*) from household_details where is_active='Y' and Household_code ||worker_code  like '%%%s%s%%'",comp_value,sel_jc);

        hh_cnt = exec_query_get_single_value(hh_cnt, query, DB_PACK);

        if(atoi(hh_cnt)>0)
	{
	                sprintf(query,"select substr(Household_code,15,4)||worker_code from household_details where is_active='Y' and  Household_code ||worker_code like '%%%s%s%%' order by substr(Household_code,15,4)||worker_code",comp_value,sel_jc);

                pw_combobox_clear(combo_benef);
                exec_query_load_into_combo(combo_benef, query, DB_PACK);
        }
        else
        {
                show_message_frame("NO DATA");
	pw_entry_clear(txt_entry_jcno);	
                return 0;
        }
	free(hh_cnt);
        return 0;
}
void combobox(pw_widget widget, int index)
{
	char *jobcardno;
	unsigned idx;
	sel_jobcardno[0]='\0';
		printf("Selected index: %d\n", index);
	pw_combobox_get_selection(combo_benef, &jobcardno, &idx);
		printf("\n job card is %s\n", jobcardno);
	strcpy(sel_jobcardno, jobcardno);
	free(jobcardno);
	//	printf("copied job card is %s\n", sel_jobcardno);
		printf("\n sel job is %s\n", sel_jobcardno);
	cb_beneficary_details();	
	return;
}
void combobox_benef_habchange(pw_widget widget, int index)
{
	pw_set_text(label_get_detail[0], " ");
	pw_set_text(label_get_detail[1], " ");
	pw_set_text(label_get_detail[2], " ");
	pw_set_text(label_get_detail[3], " ");

	char habit_code[50]="";
	char *habitationname;
	unsigned idx;
		printf("Selected index: %d\n", index);
	pw_combobox_get_selection(combo_benef_hab,&habitationname,&idx);
	strcpy(sel_habname,habitationname);
	free(habitationname);
		printf("\n sel_habname is %s\n",sel_habname);

	strncpy(habit_code,sel_habname,5);	
	sprintf(comp_value,"%s%s",dist_mand_pan_id,habit_code);
		printf("\n comp_value is %s\n",comp_value);
	sprintf(query,"select  distinct substr(Household_code,15,4)||worker_code from household_details where  is_active='Y' and substr(Household_code,1,14)='%s' order by substr(Household_code,15,4)||worker_code",comp_value);
	pw_combobox_clear(combo_benef);
	pw_set_text(combo_benef,"--Select--");
	exec_query_load_into_combo(combo_benef, query, DB_PACK);
	return ;
}
void cb_beneficary_details_back()
{
	hide_frame_beneficary_details();
	sel_habname[0]='\0';	
	sel_jobcardno[0]='\0';
	show_mcc_module();
	return;
}
void create_frame_beneficary_details(void)
{
	frame_beneficary_details = pw_create_frame(toplevel, 0, 0, 240, 270, 
			PW_BGCOLOR, VIOLET, 
			PW_CONFIG_END);

	pw_hide(frame_beneficary_details);

	label_beneficary[0] =  pw_create_label(frame_beneficary_details, 0, 0, 240, 15, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER, 
			PW_FONT, PW_BOLD_FONT, 
			PW_TEXT, "Wage Seekers Details", 
			PW_FGCOLOR, VIOLET , 
			PW_CONFIG_END);
	label_beneficary[1] =  pw_create_label(frame_beneficary_details,01, 17, 85, 20,
			PW_TEXT, "Habitation",
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR,"white",
			PW_CONFIG_END);
	combo_benef_hab = pw_create_combobox(frame_beneficary_details, 85, 17, 149,20,
			PW_COMBOBOX_TYPE,PW_COMBOBOX_READONLY, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,
			PW_NA);

	pw_combobox_selection_intimate(combo_benef_hab,combobox_benef_habchange);

	 label_beneficary[2] =  pw_create_label(frame_beneficary_details, 01, 40, 80, 20, 
                        PW_TEXT, "JC No",     
                        PW_BGCOLOR, VIOLET,
                        PW_FONT, PW_BOLD_FONT,
                        PW_FGCOLOR, "white",
                        PW_CONFIG_END);
	 txt_entry_jcno =  pw_create_entry(frame_beneficary_details,80, 40, 58,20 ,
                        PW_FONT, PW_BOLD_FONT,
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_entry_num,
                        PW_CONFIG_END);

        pw_focus(txt_entry_jcno);

        Butt_jcno_search = pw_create_button(frame_beneficary_details, 160, 40, 76, 24,
                        PW_JUSTIFY, PW_JUSTIFY_CENTER,
                        PW_FONT, PW_BOLD_FONT,
                        PW_NORMAL_IMAGE,BUTT_SEARCH,
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,get_jc_searchno,
                        PW_CONFIG_END);


	label_beneficary[2] =  pw_create_label(frame_beneficary_details, 01, 72, 85, 20, 
			PW_TEXT, "JC No", 
			PW_BGCOLOR, VIOLET, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);
	combo_benef = pw_create_combobox(frame_beneficary_details, 90, 72, 100, 20,
			PW_COMBOBOX_TYPE, PW_COMBOBOX_READONLY,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_BGCOLOR,"white",
			PW_FONT, PW_BOLD_FONT,			
			PW_NA);
	pw_combobox_selection_intimate(combo_benef, combobox);

	label_beneficary[3] =  pw_create_label(frame_beneficary_details, 0, 105, 70, 25, 
			PW_TEXT, "NAME",
			//PW_TEXT, "   పేరు",
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET , 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	label_beneficary[4] =  pw_create_label(frame_beneficary_details, 0, 140, 100, 20, 
			PW_TEXT, "Gender", 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET, 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	label_beneficary[5] =  pw_create_label(frame_beneficary_details, 0, 170, 100, 20, 
			PW_TEXT, "Worker Code", 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET, 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	label_beneficary[6] =  pw_create_label(frame_beneficary_details, 0, 200, 100, 20, 
			PW_TEXT, "Status", 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET, 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	label_get_detail[0] =  pw_create_label(frame_beneficary_details, 70, 105, 169, 30, 
			PW_TEXT, "", 
			PW_FONT, PW_BOLD_FONT, 
			PW_WRAP, PW_CHAR_WRAP, 
			PW_BGCOLOR, VIOLET, 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	label_get_detail[1] =  pw_create_label(frame_beneficary_details, 100, 140, 180, 20, 
			PW_TEXT, "", 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET, 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	label_get_detail[2] =  pw_create_label(frame_beneficary_details, 110, 170, 180, 20, 
			PW_TEXT, "", 
			PW_WRAP, PW_CHAR_WRAP, 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET, 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	label_get_detail[3] =  pw_create_label(frame_beneficary_details, 100, 200, 180, 20, 
			PW_TEXT, "", 
			PW_FONT, PW_BOLD_FONT, 
			PW_BGCOLOR, VIOLET, 
			PW_FGCOLOR, "white", 
			PW_CONFIG_END);

	button_reenroll = pw_create_button(frame_beneficary_details, 80, 230, 70, 27,
			PW_TEXT, "Re-enroll",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			//  PW_NORMAL_IMAGE,BUTT_FRWD,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_beneficary_details_reenroll,
			PW_CONFIG_END);

	button_next = pw_create_button(frame_beneficary_details, 171, 230, 34, 27, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_FRWD, 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_beneficary_details_next, 
			PW_CONFIG_END);

	label_beneficary_back = pw_create_button(frame_beneficary_details, 20, 230, 34, 27, 
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_beneficary_details_back, 
			PW_CONFIG_END);
	return;
}

void show_frame_beneficary_details(char *btag, char *btext)
{
	if(! frame_beneficary_details)
	{
		create_frame_beneficary_details();
		pw_show(frame_beneficary_details);
	}
	else
		pw_show(frame_beneficary_details);

	char *dist_mand_pan_code=NULL;
	sprintf(query,"select substr(jc_code,1,9) from jc_info");

	dist_mand_pan_code = exec_query_get_single_value(dist_mand_pan_code, query, DB_PACK);
	sprintf(dist_mand_pan_id,"%s",dist_mand_pan_code);

	sprintf(query,"select distinct Habitation_Code||'-'||habitation_name from tmp_Habitation_Master ");

	//hab_name = exec_query_get_single_value(hab_name, query, DB_PACK);

	pw_combobox_clear(combo_benef_hab);
	pw_combobox_clear(combo_benef);
	pw_set_text(combo_benef_hab,"  --Select--");
	pw_set_text(combo_benef,"--Select--");
	exec_query_load_into_combo(combo_benef_hab, query, DB_PACK);
	//free(hab_name);
	pw_set_text(label_get_detail[0], " ");
	pw_set_text(label_get_detail[1], " ");
	pw_set_text(label_get_detail[2], " ");
	pw_set_text(label_get_detail[3], " ");
	pw_set_text(button_next, btext);
	pw_set_tag(button_next, btag);
	pw_disable(button_next);
	pw_disable(button_reenroll);
	pw_show(frame_beneficary_details);
	return;
}
void hide_frame_beneficary_details(void)
{
	pw_hide(frame_beneficary_details);
return;
}
