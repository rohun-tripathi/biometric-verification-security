#include "types.h"
// #include <autils.h>

#define BUTT_OK "" 
pw_widget Butt_splash,splash_screen;

void hide_splash_screen();
void show_progress_frame();
void hide_progress_frame();
void show_frame_optionspage();
void hide_message_frame();
void show_frame_memory_info();

extern char date[15];
char get_date[15];
int cb_meminfo_upload();
char *exec_query_get_single_value(char *data, char *query, char *database);
int gp_test();
int hour, min, sec, day, month, year;
void hide_frame_memory_info();
char vers_no[10] = "3.7.9";
pw_widget btn_frame,btn_label,btn_relese,text_frame, text_label;

int version_check_upload()
{
	char *version_check=NULL;
	char query[516];
	sqlite3 *dbhandle=NULL;

	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	sprintf(query,"select ifnull(max(apps_version),0) from version_info");
	version_check =exec_query_get_single_value(version_check, query, DB_PACK);

	if(!strcmp(version_check,vers_no)==0)
	{
		printf("inside version upload");
		ret= gp_test();
		show_frame_memory_info();
		int mem_up = cb_meminfo_upload();	
		printf("mem_up is %d\n",mem_up);
		hide_frame_memory_info();
		if(mem_up==0)
		{
			sprintf(query,"update version_info set apps_version='%s',vers_upstatus='%s',created_date=current_timestamp",vers_no,"Y");
			sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
			printf("query1 = %s \n", query);
		}
	}
	free(version_check);
	sqlite3_close(dbhandle);
	return 0;
}
int check_posdate()
{
	char *max_date=NULL;
	char query[516];
	sprintf(query,"select ifnull(max(substr(created_date,1,10)),'0') <=current_date from muster_attendance");
	max_date = exec_query_get_single_value(max_date, query, DB_PACK);
	if(atoi(max_date)==0)
	{
		show_progress_frame(" WRONG DATE IN POS,SET DATE ");
		sleep(4);
		hide_progress_frame();
		system("killall -9 FA");
		return 0 ;
	}
	free(max_date);
	return 0;
}

int create_db()
{
	char query[1024];
	printf("inside create1");
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}

	sprintf(query,"CREATE TABLE weekly_download(next_download_date  varchar(15),downloaded_date default current_date)");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

	sprintf(query,"alter TABLE temp_demand_grpdetails add column demand_type varchar(2)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE Batch_info(Batch_type varchar(15) not null,version_no varchar(10) not null,created_date varchar(25))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE Demand_Group_details (group_id varchar(15) not null,group_name varchar(150) not null,Householdcode varchar(20) not null,Demanddays varchar(10) not null,recepit_id not null,fromdate varchar(15) not null,todate varchar(15) not null,demanddate default current_timestamp,Gpslongitude double,Gpslatitude double,status varchar(2), allocated_status varchar(2), demand_type varchar(2), group_type varchar(25), application_date varchar(30))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"delete from Demand_Group_details where substr(application_date,1,10)<'2013-03-30'");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE Wage_Info(jobcard_no  varchar(20), no_days integer)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	//sprintf(query,"delete from  Wage_Info");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE Wage_Payorder_Info(jobcardno varchar(20),Worked_days varchar(10))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	//sprintf(query,"delete from Wage_Payorder_Info ");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE enrollment_master(jobcard_no varchar(20) not null,name varchar(100),enroll_status char(2) not null,authorized_person TEXT not null,created_date  default current_timestamp)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"DROP TABLE backup_info");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"DROP TABLE gp_abstract");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"DROP TABLE Batch_Allocation");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE tempmuster_attendance(jobcardno char(20) not null,member_name char(60) not null,created_date default current_timestamp,group_name varchar2(300) not null,muster_id char(13) not null,status char(2) not null,distance varchar(4),work_code char(20) not null,crowbar varchar(2),group_id varchar(20) not null,worker_type varchar(10),Mate varchar(4),Gpslongitude double,Gpslatitude double,skilledtype varchar(10),from_date varchar(20) not null,to_date varchar(20) not null, Attendance_type char(4),progress_code varchar(2))");

	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("temp musterquery %s\n",query);

	sprintf(query,"insert into tempmuster_attendance select * from muster_attendance where substr(created_date,1,10)>'2013-04-01' and status='N'");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("temp insertmusterquery %s\n",query);
	sprintf(query,"DROP TABLE muster_attendance");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"ALTER TABLE tempmuster_attendance RENAME TO muster_attendance");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	printf("temp renamemusterquery %s\n",query);

	//sprintf(query,"update muster_attendance set status='Y' where status='N' and substr(created_date,1,10)<'2013-05-07'");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from muster_attendance where substr(created_date,1,10)<'2013-03-30' and status!='N'");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE mcc_demand_group_details(demand_id integer not null,group_id varchar(5) not null,household_code varchar(18) not null,worker_code varchar(2) not null,registered_date varchar(25) not null,demand_fromdate varchar(15) not null,demand_todate varchar(15) not null,demanded_days integer not null,allocated_days integer not null,is_allocated char(1) not null,receipt_no varchar(14) not null,demand_type char(1) not null)");

	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	//sprintf(query,"delete from mcc_demand_group_details ");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE mcc_work_allocation_details(demand_id integer not null,work_code varchar(20) not null,group_id varchar(15) not null,household_code  varchar(18) not null,worker_code varchar(2) not null,allocation_type char(1),demand_dt varchar(12),allocation_date  varchar(12),demand_frm_dt varchar(12),demand_to_dt varchar(12),alloc_from_date varchar(12),alloc_to_date varchar(12),alloc_days varchar(10),group_name varchar(300) not null,muster_id varchar(12) not null)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	//sprintf(query,"delete from mcc_work_allocation_details");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE no_work_demand(group_id varchar(15) not null,group_name varchar(150) not null,reason TEXT default 'NWD',status char(1) default 'N',transcation_date default current_timestamp, applicationdate varchar(15))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from no_work_demand where substr(transcation_date,1,10)<'2013-03-30'");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE remaining_mandays_details(work_code varchar(20),tot_mandays varchar(10),spent_mandays varchar(10),balance_mandays varchar(10))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	//sprintf(query,"delete from remaining_mandays_details");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	//sprintf(query,"drop table work_allocation_details");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE work_allocation_details(work_code varchar(20),group_name varchar(100),group_id varchar(15),jobcardno varchar(20),demanded_days varchar(10),alloc_days varchar(10),alloc_from_date varchar(12),alloc_to_date varchar(12),demand_dt varchar(12),demand_frm_dt varchar(12),demand_to_dt varchar(12),recepit_no varchar(9),gps_long double,gps_lat double,allocation_date varchar(25) not null,status char(2))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from work_allocation_details where substr(allocation_date,1,10)<'2013-03-30'");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE temp_demand_grpdetails(jc_info varchar(80) not null , demand_type varchar(2))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from  temp_demand_grpdetails");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE download_status(download_status char(1),download_date default current_date)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	//sprintf(query,"delete from download_status");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"DROP TABLE pos_info");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE MVO_Details(pid varchar(10) not null,jobcardno char(20),name varchar(50),group_name varchar2(300),group_id varchar(15),work_code char(20),status char(2),Gpslongitude double,Gpslatitude double,crowbar char(2),created_date default current_date,mvo_status char(2))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE MVO_remarks(remarks varchar(50),created_date default current_date,status char(2))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE Master_remarks(fa_remarks varchar(100))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE Work_Registration(work_code varchar(18) not null,work_name varchar(200) not null,description varchar2(30),habitation_code varchar2(5) not null,work_type_code varchar(10),work_type_name varchar(200),created_date varchar(15),work_strategy char(2),project_id varchar(5),estimated_days varchar(10))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"alter table Work_Registration add column estimated_days varchar(10)");
	sqlite3_exec(dbhandle,query,NULL,NULL,NULL);
	sprintf(query,"CREATE TABLE household_details(household_code varchar2(18) not null,worker_code char(2),name varchar2(50),caste varchar2(10),sex char(1),age number not null,is_mate char(1) not null,is_active char(1) not null,disabled char(1) not null,group_id varchar2(14) not null,group_name varchar2(300),group_type varchar2(10) not null)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE jc_info (jc_code varchar(15) primary key not null,district varchar(30),assembly varchar(30),mandal varchar(30),panchayat varchar(30))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE load_groups_details(group_id varchar(16) not null,load_date default current_timestamp)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"DROP TABLE muster_dates");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE muster_dates(work_code varchar(20),from_date varchar(12),to_date varchar(12),date_id integer primary key autoincrement)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE muster_duplication(jobcardno char(20),member_name char(50),created_date default current_date,group_name varchar2(300),muster_id char(6),status char(2),distance varchar(4),work_code char(20),crowbar varchar(2),group_id varchar(20),worker_type varchar(10),Mate varchar(4),Gpslongitude double,Gpslatitude double,skilledtype varchar(10),from_date varchar(20),to_date varchar(20), Attendance_type char(4),progress_code varchar(2))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from  muster_duplication");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"DROP TABLE muster_id");
        sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE muster_id(work_code varchar(20),group_id char(15) not null,muster_id varchar(12),from_date varchar(20),to_date varchar(20),created_date default current_date,primary key (muster_id))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);	
	//sprintf(query,"delete from muster_id");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE splitted_jobcards(new_jobcard varchar(20),new_worker_code varchar(2),old_jobcard varchar(20),old_worker_code varchar(20),download_date varchar(15),status char(2))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	//sprintf(query,"delete from splitted_jobcards");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE tagged_groups(group_name varchar(100) not null,work_code varchar(30) not null,group_id varchar2(14) not null,tagged_date default current_date)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from tagged_groups");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE temp_groups_allocation(group_id varchar(16) not null,group_name varchar(150) not null,work_code varchar(20),created_date default current_timestamp)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from temp_groups_allocation");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE temp_nodemand_grpdetails(grpid varchar(15) not null,grp_nam varchar(200) not null)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	//sprintf(query,"delete from temp_nodemand_grpdetails");
	//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE temp_tag_groups(work_code varchar(20) not null,group_id varchar(20) not null,group_name varchar(150)not null,status varchar(2),tagged_date default current_timestamp)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from temp_tag_groups");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE tmp_group_master(group_name varchar2(300),group_id varchar(10),status varchar(5),created_date default current_date)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from tmp_group_master");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE tmp_group_members(jobcardno varchar(20),is_mate varchar(4),member_name varchar(50) not null,group_name varchar2(300),labour_type varchar(10),is_active varchar(2))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"delete from tmp_group_members");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE tmp_habitation_master(habitation_code varchar(10),habitation_name varchar(30))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE wg_gpname_bio_info (sid integer,nrega_id varchar(21), name TEXT, grname TEXT)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE FP_Sensor_info(next_del_date varchar(15),delete_date default current_date)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE version_info(apps_version varchar(8) not null,vers_upstatus varchar(2),schema_change varchar(2) not null,temp_c1 varchar(2),temp_c2 varchar(2), create_table char(2),created_date default current_timestamp)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	printf("TABLE version_info is %s\n",query);
	sprintf(query,"CREATE TABLE tmp_demand_allocation(group_name varchar2(300) not null,work_code varchar(20) not null,alloc_from_date varchar(12) not null,alloc_to_date varchar(12) not null,alloc_days varchar(10) not null)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"drop table allocation_rejection_details");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"drop table Demand_rejection_details");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"CREATE TABLE allocation_rejection_details(work_code varchar(20),group_id varchar(15),Householdcode varchar(20) not null,worker_code varchar(3) not null,demand_dt varchar(12),allocation_date varchar(12),demand_frm_dt varchar(12),demand_to_dt varchar(12),alloc_from_date varchar(12),alloc_to_date varchar(12),alloc_days varchar(10),error_code varchar(10) not null)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE Demand_rejection_details(group_id varchar(15) not null,jobcard_no varchar(20) not null,fromdate varchar(25) not null,todate varchar(25),demanddate varchar(25) not null,receipt_id varchar(20),error_code varchar(10) not null)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"CREATE TABLE enroll_login (user varchar(5) not null,password varchar(10) not null)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"insert into enroll_login values ('MPDO','123456')");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"insert into enroll_login values ('APO','123456')");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"insert into enroll_login values ('EC','123456')");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"Insert into version_info(apps_version,vers_upstatus,schema_change,temp_c1,temp_c2,create_table,create_table1,finacal_yrdel)values('%s','N','N','N','N','C','S','N')",vers_no);
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	printf("inser version %s\n",query);
	sprintf(query,"CREATE TABLE tmp_demand_group_allocate(jobcardno varchar(20),demand_frmdt varchar(15),demand_todt varchar(15),allocated_days integer)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"create table groups_distinct(group_name varchar(50),group_id varchar(20))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"insert into groups_distinct select distinct group_name,group_id from household_details");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"delete from groups_distinct where rowid not in(select max(rowid) from groups_distinct group by group_id)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"ALTER TABLE Demand_Group_details ADD COLUMN allocated_status varchar(2)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

	sprintf(query,"ALTER TABLE Demand_Group_details ADD COLUMN demand_type varchar(2)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"ALTER TABLE Demand_Group_details ADD COLUMN group_type varchar(25)");
	printf("query of alter is %s\n",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"ALTER TABLE Demand_Group_details ADD COLUMN application_date varchar(12)");
	printf("query of alter is %s\n",query);
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	int fps;
	sprintf(query,"delete from wg_gpname_bio_info");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"delete from load_groups_details");
	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	printf("query is %s\n",query);
	//Deleting the old data.
	fps = init_fps_in_iso();
	remove_all_templates_from_fps(fps);
	bio_uninit(fps);
	sqlite3_close(dbhandle);
	return 0;
}
Bool cb_splash_screen()
{
	hide_splash_screen();
	show_frame_optionspage();
	return False;
}
void create_splash_screen()
{
	splash_screen = pw_create_frame(toplevel, 0, 0, 240, 270, 
			PW_BGCOLOR, VIOLET , 
			PW_CONFIG_END);
	pw_hide(splash_screen);
	Butt_splash = pw_create_button(splash_screen, 0, 0, 240, 270, 
			PW_ACTIVE_IMAGE, BUTT_SPLASH, 
			PW_NORMAL_IMAGE, BUTT_SPLASH, 
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_splash_screen, 
			PW_CONFIG_END);
	return;
}
void show_splash_screen()
{
	if(!splash_screen)
		create_splash_screen();

	pw_show(splash_screen);
	return;
}

void hide_splash_screen()
{
	pw_hide(splash_screen);
}

int main(int ac, char **av)
{
	pw_init(PW_SUPPORT_ALCHEMY);

	toplevel = pw_create_window(240, 270, 
			PW_TITLE, "MGNREGS - V3.7.9",
			PW_BGCOLOR, VIOLET , 
			PW_CONFIG_END);
	pw_alchemy_command(toplevel,PW_WM_SET_STATUS_MSG,"powered by APOnline",20);	

	au_get_time(&hour, &min, &sec, &day, &month, &year);
	sprintf(date, "%02d-%02d-%04d", day, month, year);
	sprintf(get_date,"%04d-%02d-%02d",year,month,day);
	printf("Date = %s \n ", date);
	printf("Date = %s \n ", get_date);

	sprintf(date_format,"%s","%d-%m-%Y");
	check_posdate();
	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}
	system("mkdir /usr/local/audio/");
	char *creat_table=NULL,query[512];
	sprintf(query,"delete from  mcc_work_allocation_details where  substr(alloc_to_date,1,10)<current_date");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"delete from mcc_work_allocation_details where substr(muster_id,1,6)='201213'");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"delete from mcc_work_allocation_details where rowid not in(select max(rowid) from mcc_work_allocation_details group by demand_id,group_id,household_code,worker_code,substr(alloc_from_date,1,10),substr(alloc_to_date,1,10))");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"delete from version_info where created_date!=(select max(created_date) from version_info)");
	sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
	sprintf(query,"select ifnull(count(*),'0') from version_info where create_table='C'");
	creat_table = exec_query_get_single_value(creat_table, query, DB_PACK);
	if(atoi(creat_table)<=0)
	{
		printf("delete data from prvs years%s\n",query);
		/*sprintf(query,"update batch_info set batch_type='B1' where batch_type='1'");
		  sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		  sprintf(query,"update batch_info set batch_type='B2' where batch_type='2'");
		  sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		 */sprintf(query,"delete from mcc_demand_group_details  where substr(registered_date,1,10)<'2013-03-30'");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"delete from Demand_Group_details where substr(application_date,1,10)<'2013-03-27'");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"delete from allocation_rejection_details where substr(allocation_date,1,10)<'2013-03-30'");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		sprintf(query,"delete from Demand_rejection_details where substr(demanddate,1,10)<'2013-03-30'");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"alter table version_info add column create_table1 char(2)");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"alter table version_info add column finacal_yrdel char(2)");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"update household_details set is_active='N' where group_type='SSS-T' and household_code||worker_code IN(select household_code||worker_code from household_details where group_type='GENERAL' and is_active='Y')");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		sprintf(query,"update household_details set is_active='N' where group_type='GENERAL' and household_code||worker_code IN(select household_code||worker_code from household_details where group_type='DISABLED' and is_active='Y')");

		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);

		sprintf(query,"update household_details set is_active='N' where group_type='SSS-T' and household_code||worker_code IN(select household_code||worker_code from household_details where group_type='DISABLED' and is_active='Y')");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
		sprintf(query,"delete from version_info");
		sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
//		sprintf(query,"Insert into version_info(apps_version,vers_upstatus,schema_change,temp_c1,temp_c2,create_table,create_table1,finacal_yrdel)values('%s','N','N','N','N','C','S','N')",vers_no);
	//	sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	//	printf("inser version %s\n",query);
		sprintf(query,"drop table mcc_demand_group_details");
		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

		sprintf(query,"CREATE TABLE mcc_demand_group_details(demand_id integer not null,group_id varchar(5) not null,household_code varchar(18) not null,worker_code varchar(2) not null,registered_date varchar(25) not null,demand_fromdate varchar(15) not null,demand_todate varchar(15) not null,demanded_days integer not null,allocated_days integer not null,is_allocated char(1) not null,receipt_no varchar(14) not null,demand_type char(1) not null)");
		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);

		//		system("rm -r /root/*");
		//		system("rm /home/Photos/*");
	}
	sprintf(query,"select ifnull(count(*),'0') from version_info where create_table1='S'");
	creat_table = exec_query_get_single_value(creat_table, query, DB_PACK);
	sprintf(query,"update no_work_demand set status='N' where  substr(transcation_date,1,10)  between '2013-05-09' and '2013-05-14'");
		sqlite3_exec(dbhandle, query, NULL, NULL,NULL);
	if(atoi(creat_table)<=0)
	{
		printf("inside  creat_tablecreate\n");
		create_db();
	}
	free(creat_table);
	sqlite3_close(dbhandle);
	version_check_upload();
	show_splash_screen();   
	printf("version no is %s\n",vers_no);
	pw_eventloop();
	pw_exit();
	return (0);
}

void create_progress_frame()   
{
	progress_frame = pw_create_frame(toplevel, 0, 125, 230, 60, 
			PW_BORDER, 1, 
			PW_WRAP, PW_CHAR_WRAP, 
			PW_BGCOLOR, "lightgrey", 
			PW_NA);

	progress_label = pw_create_label(progress_frame, 05, 5, 230, 60, 
			PW_FONT, PW_BOLD_FONT, 
			PW_WRAP, PW_CHAR_WRAP, 
			PW_BGCOLOR,"lightgrey", 
			PW_FGCOLOR,"Red",  
			PW_JUSTIFY, PW_JUSTIFY_CENTER, 
			PW_NA);
}

void show_progress_frame(char *msg)
{
	if(!progress_frame)
		create_progress_frame();

	pw_show(progress_frame);
	pw_raise(progress_frame);

	pw_set_text(progress_label, msg);
	pw_update(progress_label, 1);
	pw_grab(progress_frame);
}

void hide_progress_frame()
{
	pw_ungrab();
	if(progress_frame)
		pw_hide(progress_frame);
}

void set_progress_frame_msg(char *msg)
{
	if(!progress_frame)
		create_progress_frame();

	pw_set_text(progress_label, msg);
	pw_update(progress_label, 1);
	pw_show(progress_frame);
	pw_raise(progress_frame);
}
pw_widget message_frame,message_label,message_btn;
void cb_msg_ok()
{
	hide_message_frame();
}
void create_message_frame()
{
	message_frame = pw_create_frame(toplevel, 10, 105, 215, 100,
			PW_BORDER, 1,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR, "lightgrey",
			PW_NA);

	message_label = pw_create_label(message_frame, 00, 05, 230, 55,
			PW_FONT, PW_BOLD_FONT,
			PW_WRAP, PW_CHAR_WRAP,
			PW_BGCOLOR,"lightgrey",
			PW_FGCOLOR,"Red",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_NA);

	message_btn = pw_create_button(message_frame, 80, 65,60, 30,
			PW_TEXT, "OK",
			PW_FONT, PW_BOLD_FONT,
			PW_FGCOLOR, "#3b3b3b",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_msg_ok ,
			PW_CONFIG_END);
}
void show_message_frame(char *msg)
{
	if(!message_frame)
		create_message_frame();

	pw_show(message_frame);
	pw_raise(message_frame);

	pw_set_text(message_label, msg);
	pw_update(message_label, 1);
	pw_grab(message_frame);
}

void hide_message_frame()
{
	pw_ungrab();
	if(message_frame)
		pw_hide(message_frame);
}
void set_message_frame_msg(char *msg)
{
	if(!message_frame)
		create_message_frame();

	pw_set_text(message_label, msg);
	pw_update(message_label, 1);
	pw_show(message_frame);
	pw_raise(message_frame);
}
void cb_btn_relese(pw_widget wdgt, int idx)
{
	hide_btn_frame();
}
void create_btn_frame(void)
{
	btn_frame = pw_create_frame(toplevel,05,70,235,120,
			PW_BORDER, 1,
			PW_WRAP,PW_CHAR_WRAP,
			PW_BGCOLOR, "lightgrey",
			PW_NA);

	btn_label = pw_create_label(btn_frame,02,5,235,80,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, "lightgrey",
			PW_FGCOLOR, "RED",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_NA);
	btn_relese = pw_create_button(btn_frame,120, 90, 34, 24,
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_FONT, PW_BOLD_FONT,
			PW_NORMAL_IMAGE,BUTT_OK,
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_btn_relese,
			PW_CONFIG_END);

}
void show_btn_frame(char *msg)
{
	if(!btn_frame)
		create_btn_frame();

	pw_show(btn_frame);
	pw_raise(btn_frame);

	pw_set_text(btn_label, msg);
	pw_update(btn_label, 1);
}

void hide_btn_frame(void)
{
	if(btn_frame)
		pw_hide(btn_frame);
}
void set_btn_frame_msg(char *msg)
{
	if(!btn_frame)
		create_btn_frame();

	pw_set_text(btn_label, msg);
	pw_update(btn_label, 1);
	pw_show(btn_frame);
	pw_raise(btn_frame);
}
void create_text_frame(void)
{
	text_frame = pw_create_frame(toplevel,05,10,230,90,
			PW_BORDER, 1,
			PW_WRAP,PW_CHAR_WRAP,
			PW_BGCOLOR, "lightgrey",
			PW_NA);

	text_label = pw_create_label(text_frame,05,5,230,80,
			PW_WRAP,PW_CHAR_WRAP,
			PW_FONT, PW_BOLD_FONT,
			PW_BGCOLOR, "lightgrey",
			PW_FGCOLOR, "RED",
			PW_JUSTIFY, PW_JUSTIFY_CENTER,
			PW_NA);
}
void show_text_frame(char *msg)
{
	if(!text_frame)
		create_text_frame();
	pw_show(text_frame);
	pw_raise(text_frame);

	pw_set_text(text_label, msg);
	pw_update(text_label, 1);
}

void hide_text_frame(void)
{
	if(text_frame)
		pw_hide(text_frame);
}
void set_text_frame_msg(char *msg)
{
	if(!text_frame)
		create_text_frame();
	pw_set_text(text_label, msg);
	pw_update(text_label, 1);
	pw_show(text_frame);
	pw_raise(text_frame);
}
