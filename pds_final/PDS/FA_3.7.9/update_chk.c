#include "types.h"

char *exec_query_get_single_value(char *data, char *query, char *database);
int  Gps_Test(double *lat, double *lon);
double lat, lon;
char *sel_workid_chk();
char *sel_verifygrp_chk();
char *selected_jcno();
char *selected_name();
char *value();

int get_update_verify_details()
{
	char query_create[1024],query_insert[1024],*p_id=NULL,query[125];

	sqlite3 *dbhandle=NULL;
	sqlite3_open(DB_PACK, &dbhandle);
	if (dbhandle == NULL)
	{
		printf("Unable to open db\n");
	}

	sprintf(query,"select substr(jc_code,1,2)||substr(jc_code,6,4) from jc_info");

	p_id = exec_query_get_single_value(p_id, query, DB_PACK);

	Gps_Test(&lat, &lon);

	printf("\n pid=%s\n",p_id);
	printf("sel_workid=%s\n",sel_workid_chk());
	printf("sel_verifygrp=%s\n",sel_verifygrp_chk());



	printf("\n selected_jcno(),selected_name() is %s %s\n",selected_jcno(),selected_name());

	sprintf(query_create,"CREATE TABLE MVO_Details(pid varchar(10)  primary key not null,jobcardno char(20),name varchar(50),group_name varchar2(300),work_code char(20),checked_Status char(2),status char(2),remarks varchar(100),Gpslongitude double,Gpslatitude double,created_date default current_date)");

	sqlite3_exec(dbhandle, query_create, NULL, NULL, NULL);

	printf("query_create is %s\n", query_create);

	sprintf(query_insert,"insert into MVO_Details(pid,jobcardno,name,group_name,work_code,checked_Status,status,remarks,Gpslongitude,Gpslatitude) values('%s','%s%s','%s','%s','%s','%s','%s','%s','%lf','%lf')",p_id,value(),selected_jcno(),selected_name(),sel_verifygrp_chk(),sel_workid_chk(),"N","N","remarks",lon,lat);

	sqlite3_exec(dbhandle, query_insert, NULL, NULL, NULL);

	printf("query_insert is %s\n", query_insert);
	show_message_frame("UPDATED");
	free(p_id);

	sqlite3_close(dbhandle);

	return 0;
}
