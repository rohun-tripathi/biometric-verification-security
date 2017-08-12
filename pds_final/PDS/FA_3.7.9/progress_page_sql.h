#ifndef __PROGRESS_PAGE_SQL_H__
#define __PROGRESS_PAGE_SQL_H__

enum TProgressPageQuery
{
	EGetTasknames,
	EGetNameGroup,
	EGetWorkidNames,
	EOther
};

typedef enum TProgressPageQuery TProgressPageQuery;

typedef void (*local_query_callback)(void *parg, int argc, char **argv, char **colnames);

void progress_page_sql_exec_query(local_query_callback queryCallback,TProgressPageQuery queryType);
#endif 
