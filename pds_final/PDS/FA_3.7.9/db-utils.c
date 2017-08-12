#include <sqlite3.h>
#include <pwrap2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void close_sqlite(sqlite3 *dbhandle)
{
	sqlite3_close(dbhandle);
}

int exec_query_load_into_combo(pw_widget combo, char *query, char *database)
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
		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return (-1);
	}
	for(i=0; i<nRows; i++){
		pw_combobox_insert(combo, qResults[i+1], False);
	}
	pw_update(combo, 1);
	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);
	return 0;
}

/*int exec_query_load_into_lbox_single(pw_widget lbox, char *query, char *database)
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
		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return (-1);
	}
	for(i=0; i<nRows; i++)
	{
		pw_tlistbox_insertend(lbox, qResults[i+1], 0);
		//printf("\n  qResults[i+1] is %s\n", qResults[i+1]);
		//pw_tlistbox_insertend(lbox, qResults[i+1], 1);
	}
	pw_update(lbox, 1);
	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);
	return 0;
}

*/
int exec_query_load_into_lbox_single(pw_widget lbox, char *query, char *database)

{

	printf("ctrl in exec_query_load_into_lbox_single query = %s \n", query);

	system("ls /var/pkgs/pwrap2*.info");

	sqlite3 *dbhandle=NULL;

	int rval=0, nRows=0, nCols=0, i;

	char *szErrMsg=NULL;

	char **qResults=NULL;


	i = sqlite3_open(database, &dbhandle);

	if (dbhandle == NULL)

		printf("Unable to open db\n");

	printf("query is %s\n", query);

	printf("TIME: before executing the query --->  ");

	system("date");

	rval = sqlite3_get_table(dbhandle, query, &qResults, &nRows, &nCols, &szErrMsg);

	printf("items found = %d \n TIME: after executing the query --->  ", nRows);

	system("date");



	if (rval != SQLITE_OK) {

		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);

		sqlite3_free(szErrMsg);

		return (-1);

	}

	for(i=0; i<nRows; i++)

	{

		pw_tlistbox_insertend(lbox, qResults[i+1], 0);

	}

	pw_update(lbox, 1);

	sqlite3_free_table(qResults);

	sqlite3_close(dbhandle);

	printf("TIME: after updating listbox --->  ");

	system("date");

	return 0;

}

int exec_query_load_into_lbox_double(pw_widget lbox, char *query, char *database)
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
	printf("after query loding into lbox double       row = %d col = %d \n", nRows, nCols);
	if (rval != SQLITE_OK) {
		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return (-1);
	}
	
	for(i=0; i<nRows; i++)
	{
				pw_tlistbox_insertend(lbox, qResults[(i+(i+2))], 0);
				pw_tlistbox_insertend(lbox, qResults[(i+(i+3))], 1);
				
	}
	pw_update(lbox, 1);
	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);
	return 0;
}


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
		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return NULL;
	}

	printf("row = %d col = %d 0 = %s val = %s \n", nRows, nCols, qResults[0], qResults[1]);
	//Memory Should be freed by the Calling function	
	data = malloc(strlen(qResults[1])+1);
	strcpy(data, qResults[1]);

	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);
	return data;
}
int exec_query_load_into_lbox_trriple(pw_widget lbox, char *query, char *database)
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
	printf("after query  row = %d col = %d \n", nRows, nCols);
	if (rval != SQLITE_OK) {
		fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);
		sqlite3_free(szErrMsg);
		return (-1);
	}
	
	for(i=0; i<nRows; i++)
	{
				pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+3)))], 0);
				pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+4)))], 1);
				pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+5)))], 2);
	}
	pw_update(lbox, 1);
	sqlite3_free_table(qResults);
	sqlite3_close(dbhandle);
	return 0;
}

int exec_query_load_into_lbox_qudraple(pw_widget lbox, char *query, char *database)
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
        printf("after query  row = %d col = %d \n", nRows, nCols);
        if (rval != SQLITE_OK) {
                fprintf(stderr, "Sqlite Error=%s\n", szErrMsg);
                sqlite3_free(szErrMsg);
                return (-1);
        }
        for(i=0; i<nRows; i++)
        {
	   pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+(i+4))))], 0);
                pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+(i+5))))], 1);
                pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+(i+6))))], 2);
                pw_tlistbox_insertend(lbox, qResults[(i+(i+(i+(i+7))))], 3);
        }
  pw_update(lbox, 1);
  sqlite3_free_table(qResults);
        sqlite3_close(dbhandle);
        return 0;
}

