#include "types.h"
#include<stdio.h>
#include<string.h>
#include <gcamera.h>
#include <sys/time.h>
#include <stdlib.h>
pw_widget frame_webcam,label_jc_photo[4];
void show_progress_frame();
void hide_progress_frame();
void hide_frame_webcam();
void show_frame_webcam_creation();
pw_timer timer;
char *get_muster_id();
char *get_work_code();
char *get_crow_bar_status();
char *get_from_date();
char *get_to_date();
char *get_sel_groupname();
char *get_sel_photojob_id();
char *get_grp_name();
char *get_group_id();
char *exec_query_get_single_value(char *data, char *query, char *database);
int Gps_Test();
void show_musterid_frame();
pw_widget frame_identify_must_photo;
double lat, lon;
char query[1024];
char total_count1[20],date_id1[20];
static char video_name[25],output_file[512];
static pw_timer stream_timeout;
#define TIMEOUT 30
static int stream_tick;
pw_widget button_frame,start_btn,capture_btn,save_btn,back_btn,gcam_frame;
void gcamera_hide_widget();

int get_count_date_photo_callback(void *parg, int argc, char **argv, char **colnames)
{
	strcpy(total_count1, argv[0]);
	strcpy(date_id1, argv[1]);
	return 0;
}
char *count_date_id2=NULL,*count_date_id3=NULL;
int rc;
char jc_photo_id[25],name[50],muster_grp_name[100];
int insert_into_db(char *jobcardno,char *name)
{
	Gps_Test(&lat, &lon);

	sqlite3 *dbhandle=NULL;

	char *count1=NULL;
	char *count=NULL;

	sprintf(query, "select ifnull(count(*),'0') from muster_attendance where substr(created_date,1,10)=current_date and jobcardno='%s'",jobcardno);

	count = exec_query_get_single_value(count, query, DB_PACK);

	if(atoi(count)>0)
	{

		show_message_frame("PHOTO MUSTER TAKEN");
		return -1;
	}
	sqlite3_open(DB_PACK, &dbhandle);

	if (dbhandle == NULL)

		printf("Unable to open db\n");

	sprintf(query, "select ifnull(count(*),'0') from muster_attendance where group_name='%s' and work_code='%s' and current_date>='%s' and current_date<='%s'", muster_grp_name, get_work_code(), get_from_date(), get_to_date());

	count1 = exec_query_get_single_value(count1, query, DB_PACK);
	sprintf(query,"select ifnull(count(*),'0'),ifnull(date_id,'0') from muster_dates where work_code='%s' and from_date='%s' and to_date='%s'",get_work_code(),get_from_date(), get_to_date());

	sqlite3_exec(dbhandle, query, get_count_date_photo_callback, NULL, NULL);

	if(atoi(total_count1)==0)
	{

		sprintf(query,"insert into muster_dates(work_code,from_date, to_date) values('%s','%s','%s')",get_work_code(),get_from_date(), get_to_date());

		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"select ifnull(date_id,'0') from muster_dates where work_code='%s' and from_date='%s' and to_date='%s'",get_work_code(),get_from_date(), get_to_date());

		count_date_id2=exec_query_get_single_value(count_date_id2, query, DB_PACK);

		sprintf(date_id1,"%s",count_date_id2);
	}
	else
	{
		sprintf(query,"select ifnull(date_id,'0') from muster_dates where work_code='%s' and from_date='%s' and to_date='%s'",get_work_code(),get_from_date(), get_to_date());

		count_date_id3=exec_query_get_single_value(count_date_id3, query, DB_PACK);

		sprintf(date_id1,"%s",count_date_id3);
	}
	char *cnt_muster_mid_c=NULL,*error;
	sprintf(query, "select ifnull(count(*),'0') from muster_id where work_code='%s' and  group_id='%s'and from_date='%s' and to_date='%s'",get_work_code(),get_group_id(),get_from_date(),get_to_date());

	cnt_muster_mid_c = exec_query_get_single_value(cnt_muster_mid_c, query, DB_PACK);
	if(atoi(cnt_muster_mid_c)==0)
	{
		sprintf(query, "insert into muster_id (work_code,group_id,muster_id,from_date,to_date)values('%s','%s','%s','%s','%s')",get_work_code(),get_group_id(),get_muster_id(),get_from_date(),get_to_date());
		rc =sqlite3_exec(dbhandle, query, NULL, NULL, &error);
		printf("rc value is %d\n",rc);
		printf("query is %s\n", query);

		printf("queryerror is %s\n", error);
		if(rc!=0)

		{
			printf("inside rcd");
			pw_hide(frame_identify_must_photo);
			show_musterid_frame();
			show_message_frame("DATA MISSING in Insert ");
			return -1;

		}
	}

	sprintf(query, "insert into muster_attendance(jobcardno, member_name, group_name, muster_id, status, distance, work_code, crowbar, group_id, worker_type, Mate, Gpslongitude, Gpslatitude, skilledtype, from_date, to_date,Attendance_type,progress_code) values ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%lf', '%lf', '%s', '%s', '%s','%s','%s')", jobcardno,name,muster_grp_name,get_muster_id(), "N", "N", get_work_code(), "N",get_group_id(), (strlen(jobcardno)>16)?"U":"S", "N", lon, lat, "U", get_from_date(), get_to_date(),"P",date_id1);

	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("query is %s\n",query);

	pw_tlistbox_insertend(lbox_identify_must_membs,name,0);
	pw_update(lbox_identify_must_membs,1);

	char *wrked_days=NULL,*count_payorder=NULL;
	sprintf(query, "select ifnull(count(*),'0') from Wage_Payorder_Info where jobcardno='%s'",jobcardno);
	count_payorder = exec_query_get_single_value(count_payorder, query, DB_PACK);

	if(atoi(count_payorder)==0)
	{
		sprintf(query, "insert into Wage_Payorder_Info(jobcardno,Worked_days) values ('%s', '%s')",jobcardno,"1");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		return 0;
	}
	else
	{
		sprintf(query, "select Worked_days from Wage_Payorder_Info where jobcardno='%s'",jobcardno);
		wrked_days = exec_query_get_single_value(wrked_days, query, DB_PACK);
		sprintf(query, "update Wage_Payorder_Info set Worked_days='%s'+1 where jobcardno='%s'",wrked_days,jobcardno);
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		return 0;
	}
	free(cnt_muster_mid_c);
	free(wrked_days);
	free(count);
	free(count1);
	free(count_payorder);
	sqlite3_close(dbhandle);
	return 0;
}

char date[15],file_path[100],jc_photo_no[50],photo_delete[100];
void save_photo()
{
	printf("\n date is %s\n",date);
	sprintf(jc_photo_no,"%s_%s.jpg",jc_photo_id,date);
	//	mogrify -resize 15% 18232350901001060201_07-06-2012.jpg 
	system("mogrify -resize 15% /tmp/test_img.jpg");
	sprintf(file_path,"cp /tmp/test_img.jpg  /home/Photos/%s",jc_photo_no);
	printf("\n file path is %s \n",file_path);
	system(file_path);
	sprintf(photo_delete,"rm /tmp/test_img.jpg");
	printf("\n file path is %s \n",photo_delete);
	system(photo_delete);
	get_sel_photojob_id()[0]='\0';
	pw_tlistbox_remove_selection_all(lbox_identify_must_photo_membs);
	return ;
}
static Bool back_cback(pw_widget widget, pw_event event)
{
	printf("in close\n");
	if(stream_timeout)
	{
		pw_destroy_timer(stream_timeout);
		stream_timeout = NULL;
		stream_tick = 0;
	}
	gcamera_hide_widget();
	printf("in close1\n");
	gcamera_stop_streaming();
	gcamera_uninit();
	pw_hide(gcam_frame);
	pw_show(frame_identify_must_photo);
	pw_tlistbox_remove_selection_all(lbox_identify_must_photo_membs);
	get_sel_photojob_id()[0]='\0';
	pw_stop_eventloop();

	return 0;
}


static Bool save_camera(pw_widget widget, pw_event event)
{
	printf("in close\n");

	char *count_payorder_ph=NULL,*ph_wrked_days=NULL,*no_days_serv_cam=NULL,*cam_cnt_exists=NULL;

	char tot_days[5],msg[50],cam_hh_code[20];

	memset(tot_days,'\0',sizeof(tot_days));	

	memset(cam_hh_code,'\0',sizeof(cam_hh_code));

	memset(msg,'\0',sizeof(msg));

	strncpy(cam_hh_code,jc_photo_id,18);

	sprintf(query,"select ifnull(count(*),'0') from wage_info where jobcard_no='%s'",cam_hh_code);

	cam_cnt_exists=exec_query_get_single_value(cam_cnt_exists, query, DB_PACK);


	if(atoi(cam_cnt_exists)==0)
	{	
		insert_into_db(jc_photo_id,name);

	}	
	else
	{
		sprintf(query,"select distinct no_days from wage_info where jobcard_no='%s'",cam_hh_code);
		no_days_serv_cam=exec_query_get_single_value(no_days_serv_cam, query, DB_PACK);

		sprintf(query, "select ifnull(count(*),'0') from Wage_Payorder_Info where jobcardno='%s'",cam_hh_code);
		count_payorder_ph = exec_query_get_single_value(count_payorder_ph, query, DB_PACK);

		if(atoi(count_payorder_ph)==0)
		{                                   
			ret=insert_into_db(jc_photo_id,name);

			if(ret==-1)
			{
				return -1;
			}

			sqlite3 *dbhandle=NULL;
			sqlite3_open(DB_PACK, &dbhandle);
			if (dbhandle == NULL)
				printf("Unable to open db\n");

			int total=atoi(no_days_serv_cam)+1;

			sprintf(tot_days,"%d",total);	

			sprintf(query, "insert into Wage_Payorder_Info(jobcardno,Worked_days) values ('%s', '%s')",cam_hh_code,tot_days);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

			sprintf(query,"select distinct Worked_days from wage_payorder_info where jobcardno='%s'",cam_hh_code);
			ph_wrked_days=exec_query_get_single_value(ph_wrked_days, query, DB_PACK);

			msg[0]='\0';

			sprintf(msg,"%s Days Completed\n%s",ph_wrked_days,cam_hh_code);
			printf("\n msg is %s\n",msg);

			//show_progress_frame(msg);
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS",msg);
			sqlite3_close(dbhandle);
			free(ph_wrked_days);
		}
		else
		{
			ret=insert_into_db(jc_photo_id,name);
			if(ret==-1)
			{
				return -1;
			}

			sqlite3 *dbhandle=NULL;
			sqlite3_open(DB_PACK, &dbhandle);
			if (dbhandle == NULL)
				printf("Unable to open db\n");
			sprintf(query, "update Wage_Payorder_Info set Worked_days=Worked_days+1 where jobcardno='%s'",cam_hh_code);
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
			printf("\n update querry is %s\n",query);

			sprintf(query,"select distinct Worked_days from wage_payorder_info where jobcardno='%s'",cam_hh_code);
			ph_wrked_days=exec_query_get_single_value(ph_wrked_days, query, DB_PACK);
			printf("\n photo wrked_days is %s\n",ph_wrked_days);
			msg[0]='\0';
			sprintf(msg,"%s Days Completed\n%s",ph_wrked_days,cam_hh_code);
			//show_progress_frame(msg);
			pw_messagebox(toplevel, PW_MSGBOX_BUTTON_OK,PW_MSGBOX_ICON_NOICON, "SUCCESS",msg);
			sqlite3_close(dbhandle);
			free(ph_wrked_days);
		}
	}
	if(stream_timeout)
	{
		pw_destroy_timer(stream_timeout);
		stream_timeout = NULL;
		stream_tick = 0;
	}
	gcamera_hide_widget();
	printf("in close1\n");
	gcamera_stop_streaming();
	gcamera_uninit();
	pw_hide(gcam_frame);
	pw_show(frame_identify_must_photo);
	pw_tlistbox_remove_selection_all(lbox_identify_must_photo_membs);
	//hide_progress_frame();
	get_sel_photojob_id()[0]='\0';
	free(count_payorder_ph);
	//free(ph_wrked_days);
	free(no_days_serv_cam);
	pw_stop_eventloop();
	return 0;
}
static Bool view_button_callback(pw_widget widget, pw_event event)
{
	if(stream_timeout)
		pw_destroy_timer(stream_timeout);
	stream_timeout = NULL;
	stream_tick = 0;
	gcamera_stop_streaming();
	gcamera_uninit();
	pw_set_text(start_btn,"Start");
	pw_disable(capture_btn);
	pw_show(gcam_frame);
	return 0;
}
static Bool capture_photo(pw_widget widget, pw_event event)
{
	struct RawImage *rimg=NULL, *zrimg=NULL;

	pw_disable_many(capture_btn,start_btn, save_btn, NULL);
	pw_update(capture_btn,1);
	pw_update(start_btn,1);
	pw_update(gcam_frame,1);
	pw_update(save_btn,1);
	pw_draw_text(gcam_frame,30,50,"Capturing image...","Sans 10","RED");
	pw_set_drawcolor("WHITE");
	pw_draw_rectangle(frame_webcam,40,55,161,120);
	printf("Image capture started ... \n");
	gcamera_capture(output_file,0);
	pimg_raw_load(output_file, &rimg, 0);
	pimg_raw_zoom(&rimg, &zrimg, 0.25, 0.25, 0);
	pimg_raw_save("/tmp/.camera.jpg", &zrimg, FORMAT_JPEG, 80);
	pimg_raw_free(&zrimg);
	pimg_raw_free(&rimg);
	printf("Image capture done ... \n");
	pw_set_bgimage_file(gcam_frame,"/tmp/.camera.jpg");
	pw_update(gcam_frame,1);
	pw_enable_many(capture_btn,start_btn, save_btn, NULL);
	stream_tick = 0;
	view_button_callback(widget,event);
	save_photo();
	return 0;
}

static Bool stream_timeout_callback()
{
	//      printf("tick %d",stream_tick);
	if((stream_tick++) == TIMEOUT)
	{
		gcamera_stop_streaming();
		pw_set_bgcolor(gcam_frame,"#ffffff");
		pw_update(gcam_frame,1);
		gcamera_uninit();
		pw_draw_text(gcam_frame,5,50,"Camera stream timeout...","Sans 10","#000000");
		pw_disable(capture_btn);
		pw_set_text(start_btn,"Start");
	}
	else if((stream_tick)>(TIMEOUT+2))
	{
		pw_destroy_timer(stream_timeout);
		stream_timeout = NULL;
		pw_show(gcam_frame);
		pw_update(gcam_frame,1);
		pw_draw_text(gcam_frame,10,50,"Please start camera...","Sans 10","#000000");
		stream_tick = 0;
	}
	return 0;
}
static int get_video_name(char *video)
{
	char v4l_name[10];
	FILE *fl;

	fl=popen("ls /sys/class/video4linux/","r");
	if(fl)
	{
		fscanf(fl,"%s",v4l_name);
		if(v4l_name[0]!='v')
		{
			pclose(fl);
			return -1;
		}
		sprintf(video,"/dev/%s",v4l_name);
		return 0;
	}
	else
		return -1;
}
static Bool start_stream (pw_widget widget, pw_event event)
{

	sprintf(photo_delete,"rm /tmp/test_img.jpg");
	printf("\n file path is %s \n",photo_delete);
	int fd;
	if(!strcmp(pw_get_text(widget),"Start"))
	{
		pw_update(gcam_frame,1);
		fd = get_video_name(video_name);
		if(fd<0)
		{
			pw_draw_text(gcam_frame,5,50,"Unable to open camera...","Sans 10","#000000");
			return True;
		}
		fd = gcamera_init(video_name);
		if(fd<0)
		{
			pw_draw_text(gcam_frame,5,50,"Unable to open camera...","Sans 10","#000000");
			return True;
		}
		gcamera_start_streaming(gcam_frame);
		pw_draw_text(gcam_frame,45,50,"Please wait...","Sans 10","#000000");
		if(!stream_timeout)
		{
			stream_timeout=pw_create_timer(1000000,True,stream_timeout_callback,NULL,NULL);
			stream_tick = 0;
		}
		pw_enable(capture_btn);
		pw_disable(save_btn);
		pw_set_text(widget,"Stop");
	}
	else
	{
		gcamera_stop_streaming();
		pw_destroy_timer(stream_timeout);
		stream_timeout = NULL;
		stream_tick = 0;
		gcamera_uninit();
		pw_set_bgcolor(gcam_frame,"#ffffff");
		pw_hide(gcam_frame);
		pw_update(gcam_frame,1);
		pw_show(gcam_frame);
		pw_draw_text(gcam_frame,10,50,"Please start camera...","Sans 10","#000000");
		pw_disable(capture_btn);
		pw_set_text(widget,"Start");
	}
	return 0;
}

void create_frame_webcam(void)
{

	frame_webcam =pw_create_frame(toplevel, 0, 0, 240, 270,
			//PW_TITLE, "Work Site Photo",
			PW_BGCOLOR,VIOLET ,
			PW_CONFIG_END);
	pw_hide(frame_webcam);	
	label_jc_photo[0] =pw_create_label(frame_webcam,02, 00, 40, 20,
			PW_TEXT, "JCNO:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_jc_photo[1] =pw_create_label(frame_webcam,40, 00, 200, 20,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);
	label_jc_photo[2] =pw_create_label(frame_webcam,02, 25, 45, 25,
			PW_TEXT, "Name:",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);

	label_jc_photo[3] =pw_create_label(frame_webcam,45, 25, 234, 25,
			PW_TEXT, "",
			PW_WRAP, PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR,VIOLET,
			PW_FGCOLOR, "white",
			PW_CONFIG_END);


	gcam_frame = pw_create_frame(frame_webcam,40,55,160,120,
			PW_BGCOLOR,"#ffffff",
			PW_NA);

	button_frame =pw_create_frame(frame_webcam,10,185,220,80,
			PW_BGCOLOR, VIOLET,
			PW_CONFIG_END);

	start_btn = pw_create_button(button_frame, 40, 0, 60, 25,
			PW_TEXT, "Start",
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, start_stream,
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CONFIG_END);

	capture_btn = pw_create_button(button_frame, 140, 0, 60, 25,
			PW_TEXT, "Capture",
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, capture_photo,
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CONFIG_END);

	save_btn = pw_create_button(button_frame, 140, 40, 60, 25,
			PW_TEXT, "Save",
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, save_camera,
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CONFIG_END);

	back_btn = pw_create_button(button_frame, 20, 45, 34, 27,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_BACK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, back_cback,
			PW_BORDER, PW_RECTANGLE_BORDER,
			PW_CONFIG_END);
	return;

}
void custom_gcamera_widget(pw_widget toplevel,char *out_file,int val)
{
	if(!frame_webcam)
		create_frame_webcam();
	pw_show(frame_webcam);
	gcam_frame = pw_create_frame(frame_webcam,40,55,160,120,
			PW_BGCOLOR,"#ffffff",
			PW_NA);
	pw_set_drawcolor("WHITE");
	pw_draw_rectangle(frame_webcam,40,55,161,120);
	pw_draw_text(gcam_frame,10,50,"Please start camera...","Sans 10","#000000");
	pw_disable_many(capture_btn,save_btn,NULL);
	strcpy(output_file,out_file);
	pw_set_text(start_btn,"Start");
	pw_set_text(label_jc_photo[1],jc_photo_id);
	pw_set_text(label_jc_photo[3],name);
	pw_eventloop();

	return;
}
void gcamera_hide_widget()
{
	if(stream_timeout)
		pw_destroy_timer(stream_timeout);
	stream_timeout = NULL;
	stream_tick = 0;
	if(!strcmp(pw_get_text(start_btn),"Stop"))
	{
		gcamera_stop_streaming();
		gcamera_uninit();
	}
	pw_hide(frame_webcam);
	pw_hide(gcam_frame);
}
void show_frame_webcam_creation()
{
	char *ph_name=NULL;
	jc_photo_id[0]='\0';
	muster_grp_name[0]='\0';

	sprintf(jc_photo_id,"%s",get_sel_photojob_id());
	sprintf(muster_grp_name,"%s",get_grp_name());

	sprintf(query,"select Name from household_details where is_active='Y' and household_code||worker_code='%s' and  group_name='%s' union select Member_Name as Name from tmp_group_members where group_name='%s' and Is_Active='Y' and JobCardNo='%s'",jc_photo_id,muster_grp_name,muster_grp_name,jc_photo_id);

	ph_name=exec_query_get_single_value(ph_name, query, DB_PACK);
	name[0]='\0';
	sprintf(name,"%s",ph_name);
	free(ph_name);
	custom_gcamera_widget(toplevel, "/tmp/test_img.jpg",0);
	return;
}
void hide_frame_webcam(void)
{
	pw_hide(frame_webcam);
}
