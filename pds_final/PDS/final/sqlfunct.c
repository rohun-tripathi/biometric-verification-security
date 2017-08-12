
#include<pwrap2.h>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>

#include"features_live.h"

char *exec_query_get_single_value(char *data, char *query, char *database)
{
	sqlite3 *dbhandle=NULL;
	
	int rval=0, nRows=0, nCols=0, i;
	char *szErrMsg=NULL;
	char **qResults=NULL;

	i = sqlite3_open(database, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	printf("query is %s\n", query);

	rval = sqlite3_get_table(dbhandle, query, &qResults, &nRows, &nCols, &szErrMsg);

	if (rval != SQLITE_OK) {
		printf("Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return NULL;
	}

	printf("row = %d col = %d 0 = %s val = %s \n", nRows, nCols, qResults[0], qResults[1]);
	printf("::::::::::::::::::::::1:::::::::::::::::::::::::::::\n");	
	if (qResults[1] == NULL)
		return NULL;	
	data = (char *)malloc(strlen(qResults[1])+1);
	printf("::::::::::::::::::::::2:::::::::::::::::::::::::::\n");	
	strcpy(data, qResults[1]);
	printf(";;;;;;;;;;;;;;;;;;;;;;the data to be returned is %s;;;;;;;;;;;;;;;;;;;;;;\n",data);

	//Memory Should be freed by the Calling function	
	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);
	return data;
}

int exec_query_check_valid_value(char *data, char *query, char *database)
{
	sqlite3 *dbhandle=NULL;
	
	int rval=0, nRows=0, nCols=0, i;
	char *szErrMsg=NULL;
	char **qResults=NULL;

	i = sqlite3_open(database, &dbhandle);
	if (dbhandle == NULL)
		printf("Unable to open db\n");
	printf("query is %s\n", query);

	rval = sqlite3_get_table(dbhandle, query, &qResults, &nRows, &nCols, &szErrMsg);

	if (rval != SQLITE_OK) {
		printf("Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return NULL;
	}
	printf("row = %d col = %d 0 = %s val = %s \n", nRows, nCols, qResults[0], qResults[1]);
	printf("::::::::::::::::::::::1:::::::::::::::::::::::::::::\n");	
	int retval = atoi(qResults[1]);

	//Memory Should be freed by the Calling function	
	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);

	return retval;
}
