//SDK Includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Project includes 
#include "string_list.h"

string_item* g_first_item = NULL;
int g_total_items = 0;

// Append char* item to list
void str_append(char* item)
{
	if(!item)
		return;

	g_total_items++;	
	printf("No of Grps=%d\n", g_total_items);
	string_item* obj = (string_item*)malloc(sizeof(string_item));
	obj->iName = (char*)calloc(strlen(item)+1, sizeof(char));
	strcpy(obj->iName, item);
	obj->iNext = NULL;
	if(g_first_item == NULL)
		g_first_item = obj;
	else
	{
		obj->iNext = g_first_item;
		g_first_item = obj;
	}
	return;
}

void str_get_all_names(void (*names_callback)(char* name))
{
	if(!g_first_item)
		return;
	string_item* obj = g_first_item;
	//	string_item* next = NULL;

	while(obj != NULL)		
	{


		names_callback(obj->iName);
		printf("grpname= %s\n", obj->iName);
		obj = obj->iNext;
	}
}

//this is private function; dont call from outside of file
void pvt_free_node(string_item* node)
{
	if(!node)
		return;

	free(node->iName);
	free(node);		
}

//Clean up All the Memory  Allocated
void str_clean_up_memory()
{
	if(g_first_item == NULL)
		return;

	string_item* obj = g_first_item;
	while(obj != NULL)
	{
		string_item* cur = obj;
		obj = obj->iNext;
		pvt_free_node(cur);
	}
	g_first_item = NULL;
	g_total_items  = 0;
}



