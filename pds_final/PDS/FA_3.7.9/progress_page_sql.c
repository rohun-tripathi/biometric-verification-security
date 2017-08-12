//LiB Includes
#include <sqlite3.h>
#include <stdlib.h>
#include <string.h>

//Project Includes
#include "progress_page_sql.h"
#include "types.h"
//char jc_value;

void progress_page_sql_exec_query(local_query_callback queryCallback, TProgressPageQuery queryType)
{
	char* query = NULL;
	switch(queryType)
	{
		case EGetWorkidNames:
			{
				
			}
			break;
			
		case EGetTasknames:
			{
				query = "select task_name from Tasks_master";
				//query="select task_name from tasks_master tm, work_tasks wt where work_code='%s' and wt.task_code=tm.task_code", jc_value;
					
			}
			break;
			
		default:
			break;
	}
	
	if(query)
	{
		sqlite3 *dbhandle = NULL;
		char *error;
		sqlite3_open(DB_PACK, &dbhandle);
		if (dbhandle == NULL)
		{
			printf("Unable to open db\n");	
			return;
		}
		sqlite3_exec(dbhandle, query, queryCallback, NULL, &error);
		sqlite3_close(dbhandle);
	}
}
