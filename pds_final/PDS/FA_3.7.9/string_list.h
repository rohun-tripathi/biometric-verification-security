#ifndef __STRING_LIST_H__
#define __STRING_LIST_H__

/**
 ** Linked List Implemention for char* variables
 */ 
 
struct string_item
{
	char* iName;
	struct string_item* iNext;
};

typedef struct string_item string_item;
 
//Append char* item to list
void str_append(char* item);

//Pass callback to get all names in the list one by one
void str_get_all_names(void (*names_callback)(char* name));

//Clean up all allocated memory
void str_clean_up_memory();


#endif	//__STRING_LIST_H__
