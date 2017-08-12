#include "types.h"
#include "functions.h"
#ifdef __arm__
#include <biometric.h>
#endif

void show_progress_frame();
int insert_template_into_db();
int encode_buffer();
void hide_progress_frame();
//unsigned char** templ;
//int template_len[8];
pw_widget lbl_msg;

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

unsigned long  identify_finger_print()
{
#ifdef __arm__
	int  rval;
	unsigned long uid;

	rval = bio_identify_by_scan(fps, &uid, 10);

	printf("User id is ------------%lu \n", uid);

	printf("\n rval is %d\n",rval);
	if(rval == 0)
	{
		printf("User id = %lu \n", uid);
		system("madplay /usr/local/audio/S2.mp3");
	} 
	else if (rval==-4)
	{
		pw_set_text(lbl_msg, "SENSOR TIME OUT...");
		//system("madplay /usr/local/audio/T1.mp3");
		pw_update(lbl_msg, 1);
		sleep(2);
		//		show_message_frame("SENSOR TIME OUT");    
		return 0;
	}
	else 
	{
		pw_set_text(lbl_msg, "VERIFICATION FAILED...");
		//system("madplay /usr/local/audio/T1.mp3");
		pw_update(lbl_msg, 1);
		sleep(2);
		printf("Verification Failed pp \n");
		//show_message_frame("VERIFICATION FAILED");
		return 0;
	}

	return(uid);
#else
	return(0);
#endif
}

int insert_template_into_db(char *jobcard_no)
{
	int i, j;
	char *template, templ_op[10][600],fname[256],file_path[256];
	int imgFD=0, fsz=0;
	struct stat fst;

	system("mkdir birs");
	for(i=0; i<8; i++)
	{
		//sprintf(file_path, "mv /tmp/%s_%d.bir /home/Backup/%s_%d.bir ",jobcard_no, i,jobcard_no,i+1);
		//	sprintf(file_path, "cp /tmp/%s_%d.bir  /home/birs/%s_%d.bir ",jobcard_no, i,jobcard_no,i+1);

		//	system(file_path);
		sprintf(file_path, "mv /tmp/%s_%d.bir /home/Backup/%s_%d.bir ",jobcard_no, i,jobcard_no,i+1);
		system(file_path);
		printf("moving birs from temp is %s\n",file_path);
		sprintf(fname, "/home/Backup/%s_%d.bir", jobcard_no, i);
		printf("fname birs is %s\n",fname);
		imgFD = open(fname, O_RDONLY);
		fstat(imgFD, &fst);
		fsz = fst.st_size;
		printf("the size fsz = %d \n ", fsz);
		template = (char *)malloc(sizeof(char)*(fsz+1));
		read(imgFD, template, fsz);
		close(imgFD);
		j = encode_buffer(template, templ_op[i], fsz);
		templ_op[i][j] = '\0';
		//printf("\ni=%d, size = %d len = %d data = %s \n", i, j, strlen(templ_op[i]), templ_op[i]);
		free(template);
		template = NULL;
	}
	return(0);
}
int get_file_size(char *fname)     //Returns file size
{
	struct stat fst;
	int imgFD= -1, fsz=0;

	imgFD = open(fname, O_RDONLY);
	if(imgFD > -1){
		fstat(imgFD, &fst);
		fsz = fst.st_size;
		close(imgFD);
		return(fsz);
	} else
		return(-1);
}

char *read_data_from_file(char *data, char *path)
{
	int imgFD=0, fsz=0;
	struct stat fst;

	imgFD = open(path, O_RDONLY);
	fstat(imgFD, &fst);
	fsz = fst.st_size;
	printf("the size fsz = %d \n ", fsz);
	if (fsz <= 0)
		return (0);
	data = (char *)malloc(sizeof(char)*(fsz+1));
	read(imgFD, data, fsz);
	close(imgFD);
	return (data);
}
int scan_template(char *fname)
{
	int fps;
	unlink(fname);
#ifdef __AMIDA__
	fps = bio_init(FP_SENSOR);
	show_progress_frame("PLACE_UR_FINGER");
	ret = bio_scan_template(fps, fname, 5, 0);
	hide_progress_frame();
	bio_uninit(fps);
#endif
	return(ret);
}

Bool cb_entry_num(pw_widget w, pw_event e)
{
	pw_alchemy_command(toplevel, PW_WM_SET_KEYBOARD_LAYOUT, "123");
	pw_alchemy_command(toplevel, PW_WM_SHOW_KEYBOARD, "123");
	pw_alchemy_command(toplevel, PW_WM_SHOW_INPUT, "123");
	return False;
}

Bool cb_entry_alpha(pw_widget w, pw_event e)
{
	pw_alchemy_command(toplevel, PW_WM_SET_KEYBOARD_LAYOUT, "qwerty");
	pw_alchemy_command(toplevel, PW_WM_SHOW_KEYBOARD, "qwerty");
	pw_alchemy_command(toplevel, PW_WM_SHOW_INPUT, "");
	return False;
}

void hide_keyboard(void)
{
	pw_alchemy_command(toplevel, PW_WM_HIDE_KEYBOARD, "");
	pw_alchemy_command(toplevel, PW_WM_HIDE_INPUT, "");
}

void show_keyboard(void)
{
	pw_alchemy_command(toplevel, PW_WM_SHOW_KEYBOARD, "");
	pw_alchemy_command(toplevel, PW_WM_SHOW_INPUT, "");
}
