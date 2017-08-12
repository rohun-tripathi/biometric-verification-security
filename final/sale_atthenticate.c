// the one where the sale takes place
// come here using the sale button on the Main page
#include <fcntl.h>
#include<sqlite3.h>
#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>
#include"features_live.h"
#include <sys/stat.h>
#ifdef __arm__
#include <biometric.h>
#endif
int fps=0;
pw_widget toplevel, splash_screen ,Butt_splash_try,Butt_splash_home;
pw_widget rb_benificary_splash[3], sale_label[4];

//////////declaring functions already in header is not necessary

///// to move to next frame
void show_frame_home();
void hide_splash_screen()
{
	pw_hide(splash_screen);
}
	
///// for biometric scan enrollment
int scan_template(char *fname);
int keeper_enroll(int fin);
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
	printf("the rval in authentication is %d;;;;;;;;;;;\n",rval);

	printf("\n rval is %d\n",rval);
	if(rval == 0)
	{
		printf("User id = %lu \n", uid);
		//system("madplay /usr/local/audio/S2.mp3");
	} 
	else if (rval==-4)
	{
//		pw_set_text(lbl_msg, "SENSOR TIME OUT...");
		//system("madplay /usr/local/audio/T1.mp3");
//		pw_update(lbl_msg, 1);		
		sleep(2);
		//		show_message_frame("SENSOR TIME OUT");    
		return 0;
	}
	else 
	{
//		pw_set_text(lbl_msg, "VERIFICATION FAILED...");
		//system("madplay /usr/local/audio/T1.mp3");
//		pw_update(lbl_msg, 1);
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

void keeper_authenticate_splash()
{
	//int fps=0;
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

		sprintf(fname, "/root/PDS/%s_%s.bir", "shopkeeper", "1");

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
//	printf("Number of finger prints enrolled into sensor for nrega_id:%s is %d\n", nrega_id, count);
long int id1=0;
	//uninit_fps(fps);
//fps = init_fps_in_iso();
id1=identify_finger_print();
print("::::::::::::::::::the numbers returned while authenticating %d;;;;;;;;;",id1);
uninit_fps(fps);
}

///////////////////////////putting authentication

////timebeing
void show_frame_idnumberpage();
Bool cb_splash_screen()
{
	hide_splash_screen();
	show_frame_home();
	return False;
}

Bool cb_try_again_kpr()
{
	pw_set_text(sale_label[0],"please try again");
	int i = 0;
	for (i=0;i<3;i++)
	{
		pw_radiobutton_deselect(rb_benificary_splash[i]);
		pw_set_bgcolor(rb_benificary_splash[i], GREY);
	}
	return False;
		
}

Bool cb_home_screen()
{
	hide_splash_screen();
	show_frame_home();
	return False;
}
void show_frame_shop()
{
//page graphics begin


	splash_screen = pw_create_frame(toplevel,0, 40, 240, 230,
                        PW_BGCOLOR, GREY ,
                        PW_CONFIG_END);

	sale_label[0] = pw_create_label(splash_screen, 15, 10, 240, 20,
			PW_TEXT,"Shopkeeper Authentication",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);
	
	sale_label[1] = pw_create_label(splash_screen, 20, 40, 120, 20,
			PW_TEXT,"Shopkeeper 1: ",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);

	rb_benificary_splash[0] = pw_create_radiobutton(splash_screen,160, 40, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_authenticate_splash,
			PW_NA);

	sale_label[2] = pw_create_label(splash_screen, 20, 80, 100, 20,
			PW_TEXT,"Shopkeeper 2",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);

	rb_benificary_splash[1] = pw_create_radiobutton(splash_screen,160, 80, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_authenticate_splash,
			PW_NA);

	sale_label[3] = pw_create_label(splash_screen, 20, 120, 100, 20,
			PW_TEXT,"Shopkeeper 3",
			PW_FONT, PW_BOLD_FONT,
			//PW_BORDER, PW_RECTANGLE_BORDER,
			//PW_CALLBACK, PW_BUTTON_RELEASE_EVENT,cb_alocfrm_date,
			PW_FGCOLOR, "BLACK",
			PW_BGCOLOR, GREY,
			PW_CONFIG_END);

	rb_benificary_splash[2] = pw_create_radiobutton(splash_screen,160, 120, 20, 20, NULL, 
			PW_BGCOLOR, GREY, 
			PW_FONT, PW_BOLD_FONT, 
			PW_FGCOLOR, "white", 
			PW_TEXT, "",
			PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, keeper_authenticate_splash,
			PW_NA);

	Butt_splash_try = pw_create_button(splash_screen, 90, 180, 80,20,
                        PW_ACTIVE_IMAGE, "tryag.png",
                        PW_NORMAL_IMAGE, "tryag.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_try_again_kpr,
                        PW_CONFIG_END);







///////////////////////////////////////////we need to change the functionality of the button under here later. ryt now it goes to home!!
	Butt_splash_home = pw_create_button(splash_screen, 15, 180, 60,20,
                        PW_ACTIVE_IMAGE, "home.png",
                        PW_NORMAL_IMAGE, "home.png",
                        PW_CALLBACK, PW_BUTTON_RELEASE_EVENT, cb_splash_screen,
                        PW_CONFIG_END);

	return ;
}
