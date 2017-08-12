
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


void cb_topbar_button()
{
//	pw_hide(splash_screen);
//	pw_hide(bcust_screen);
//	pw_hide(order_screen);
//	pw_hide(id_screen);
	show_frame_home();
}
