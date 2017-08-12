//used by funct1_authsale
// used for auto filling of combo box

// the one where the sale starts/takes place ka extra
// come here using the sale button on the Main page
// goes to funct2
// components one combo box and one biometric scanner
//comes from main.c

#include<sqlite3.h>
#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

#include <fcntl.h>
#include <sys/stat.h>

#include"features_live.h"

#ifdef __arm__
#include <biometric.h>
#endif


///to load into combo///////////////specific only to shopkeeper verification
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

//authentication stuff below
unsigned long enroll_template_into_fps(int fps, unsigned char *template, int size)
{
#ifdef __arm__
	unsigned long id = 0;
	int rval;
	rval = bio_enroll_by_template(fps, &id, template, size);
	if(rval < 0)
		return(-1);
	return(id); //Return enrolled id on success
#endif
}

int remove_all_templates_from_fps(int fps)
{
#ifdef __arm__
	return(bio_delete_all_templates(fps));
#endif
}

int init_fps_in_iso()
{
#ifdef __arm__
	return(bio_init(BIO_SUPREMA_IN_ISO_FORMAT));
#endif
}

int uninit_fps(int fps)
{
#ifdef __arm__
	bio_uninit(fps);

#endif
	return 1;
}

unsigned long identify_finger_print(int fps)
{
#ifdef __arm__
	int  rval;
	unsigned long uid;

	rval = bio_identify_by_scan(fps, &uid, 10);
	printf("\nUser id is ------------%lu \n", uid);

	printf("rval is %d\n",rval);
	if(rval == 0)
	{
		printf("\nUser id = %lu \n", uid);
		return(1);		
		//system("madplay /usr/local/audio/S2.mp3");
	} 
	else if (rval==-4)
	{
		printf("sensor time out\n");
		return 2;
	}
	else 
	{
		printf("Verification Failed pp \n");
		return 3;
	}

#else
	return(1);		//if not on device, return 1 to move to next page
#endif
}

long int person_auth(char * name, char * univuserid,int fps)
{


	//int fps=0;
	printf("entered person authorization for '%s' with id '%s'\n",name,univuserid);
	fps = init_fps_in_iso();
	remove_all_templates_from_fps(fps);
	int i;
	unsigned char *template;
	char fname[256];
	unsigned long id;
	int imgFD=0, fsz=0, count=0;
	struct stat fst;
//	for(i=1; i<=10; i++)
//	{
		id = 0;
		fsz = 0;

		sprintf(fname, "/root/PDS/%s_%s.bir", name, univuserid);

		printf("fname is %s\n",fname);
		imgFD = open(fname, O_RDONLY); //Needs clarification
		if(imgFD > 0)
		{
			printf("imgFD = %d \n", imgFD);
			fstat(imgFD, &fst);
			fsz = fst.st_size;

			if (fsz <= 0)

			{
				printf("Invalid Data \n");
			}

			else

			{
				template = (unsigned char *)malloc(sizeof(unsigned char)*(fsz+1));
				read(imgFD, template, fsz);
				id = enroll_template_into_fps(fps, template, fsz);
				free(template);
			}
			close(imgFD);
			printf("\n fname is %s size %d id = %lu \n", fname, fsz, id);

			if(id > 0)

			{
				count++;
				//sprintf(query, "insert into wg_gpname_bio_info (sid, nrega_id, name, grname) values ('%lu', '%s', '%s', '%s')",
				//		id, nrega_id, wname, groupname);

				//sqlite3_exec(dbhandle, query, NULL, NULL, NULL);
				//printf("\n insert query into wg_gpname is %s\n",query);
			}
		//}
	}

	long int id1=0;
	id1=identify_finger_print(fps);
	
	return(id1);
	uninit_fps(fps);

}

///////////////////////////putting authentication done ^//////////////////////////////

