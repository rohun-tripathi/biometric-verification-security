#include<stdio.h>
#include<stdlib.h>
#include<pwrap2.h>

#include "features.h"
#ifdef __arm__
#include <biometric.h>
#endif

pw_widget toplevel, splash_screen ,Butt_splash, rb_benificary_splash;

int scan_template(char *fname)
{
	int fps;
	unlink(fname); // what does this do??
	int ret;

#ifdef __AMIDA__
	fps = bio_init(FP_SENSOR); //define FP_SENSOR
	//show_progress_frame("PLACE_UR_FINGER");
	ret = bio_scan_template(fps, fname, 5, 0);
	//hide_progress_frame();
	bio_uninit(fps);
#endif
	return(ret);
}



int keeper_enroll()
{
	char fname[256];
	int retval;
	int fin =1;   //this variable will later hold the no. of shopkpr that we are enrolling 

#ifdef __AMIDA__
	sprintf(fname, "/root/PDS/%s_%d.bir","shopkeeper",fin);
	retval = scan_template(fname);
#else
	retval = 0;
#endif

	return(retval);
}


