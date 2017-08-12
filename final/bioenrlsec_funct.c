#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pwrap2.h>

#include "features_live.h"
#ifdef __arm__
#include <biometric.h>
#endif

pw_widget toplevel, splash_screen ,Butt_splash, rb_benificary_splash;
void display_wait_msg(char * text);
void hide_check_screen();

int scan_template(char *fname)
{
	int fps;
	unlink(fname); // what does this do??
	int ret;

#ifdef __AMIDA__
	fps = bio_init(FP_SENSOR); //define FP_SENSOR
	
	char text[30] = {"Place Your finger"};
	display_wait_msg(text);

	ret = bio_scan_template(fps, fname, 5, 0);	//functionality while displaying

	hide_check_screen();
	bio_uninit(fps);
#endif
	return(ret);
}

int keeper_enroll_root3_extra(char* userid,char * name)
{
	char fname[256];
	int retval;
	   //fin will later hold the no. of shopkpr that we are enrolling 

#ifdef __AMIDA__
	sprintf(fname, "/root/PDS/%s_%s.bir",name,userid);
	retval = scan_template(fname);
#else
	retval = 0;
#endif

	return(retval);
}
