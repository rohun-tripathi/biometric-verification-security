#include <pwrap2.h>
#include <stdlib.h>
#include <string.h>
#include<sqlite3.h>
#include<stdio.h>
#include <unistd.h>

#ifdef __AMIDA__
#include <biometric.h>
#endif

#include "functions.h"

pw_widget toplevel,progress_frame,progress_label,frame_optionspage;

Bool cb_splash(pw_widget w, pw_event e);
void hide_frame_splash();
extern Bool entry_num(pw_widget w, pw_event e);
extern Bool cb_entry_num(pw_widget w, pw_event e);
extern Bool cb_entry_alpha(pw_widget w, pw_event e);
int ret;


//Defines for widget size
#define PG_WTH 240
#define PG_HT 270

#define LBL_SMALL_HT 20
#define LBL_NORMAL_HT 25
#define LBL_LARGE_HT 50
#define LBL_LARGE_WTH 234

#define BTN_NORMAL_HT 25
#define BTN_LARGE_HT 50
#define BTN_NORMAL_WTH 50
#define BTN_LARGE_WTH 150

#define TAG_ENROLL "1"
#define TAG_VERIFY "2"

//#ifdef __AMIDA__
#define BUTT_SPLASH "/usr/local/apps/MGNREGS/imgs/splash.png"
#define BUTT_NORMAL "/usr/local/apps/MGNREGS/imgs/B_a.png"
#define BUTT_ACTIVE "/usr/local/apps/MGNREGS/imgs/B_d.png"
#define BUTT_DISABLE "/usr/local/apps/MGNREGS/imgs/disable.png"
#define BUTT_WORK "/usr/local/apps/MGNREGS/imgs/WRK-PRGS.png"
#define BUTT_SYNC "/usr/local/apps/MGNREGS/imgs/DT-SYNC.png"
#define BUTT_POS "/usr/local/apps/MGNREGS/imgs/POS-MGMT.png"
#define BUTT_MIS "/usr/local/apps/MGNREGS/imgs/MIS.png"
#define BUTT_MCC "/usr/local/apps/MGNREGS/imgs/MCC-MOD.png"
#define BUTT_BACK  "/usr/local/apps/MGNREGS/imgs/BACK.png"
#define BUTT_TA "/usr/local/apps/MGNREGS/imgs/TA.png"
#define BUTT_OK "/usr/local/apps/MGNREGS/imgs/OK.png"
#define BUTT_SUBMIT "/usr/local/apps/MGNREGS/imgs/SUBMIT.png"
#define BUTT_DWN_LD "/usr/local/apps/MGNREGS/imgs/DWN_LOAD.png"
#define BUTT_UPLOAD "/usr/local/apps/MGNREGS/imgs/UPLOAD.png"
#define BUTT_DWNLOAD "/usr/local/apps/MGNREGS/imgs/DWNLOAD.png"
#define BUTT_SEARCH "/usr/local/apps/MGNREGS/imgs/SEARCH.png"
#define BUTT_GPA "/usr/local/apps/MGNREGS/imgs/GP-ABSTRACT.png"
#define BUTT_DEL "/usr/local/apps/MGNREGS/imgs/DEL.png"
#define BUTT_ADD "/usr/local/apps/MGNREGS/imgs/ADD.png"
#define DB_PACK "/usr/local/apps/MGNREGS/TA.dat"
#define FP_TEMP_PATH "/usr/local/apps/MGNREGS/template/"
#define PHOTO_PATH "/usr/local/apps/MGNREGS/photo/"
#define FP_DEVICE BIO_SUPREMA_IN_ISO_FORMAT
#define FP_SENSOR BIO_SUPREMA_IN_ISO_FORMAT
#define TEMPLATE_SIZE 384
#define FP_IMAGE_JPG "/tmp/fpimage.jpg"
#define FP_IMAGE_RAW "/tmp/fpimage.raw"
#define FP_IMAGE_PGM "/tmp/fpimage.pgm"



#define BLUE "#7c90ac"
#define LINEN "linen"
#define GREY "lightgrey"
#define GREEN "#5F9EA0"
//#define VIOLET "#805922"

#define VIOLET "#6B2A75"
#define COLOR3 "#48525f"
#define COLOR4 "#bfbfbf"
#define COLOR5 "#687378"

#define GO "Go"
#define OK "Ok"
#define NEXT "Next"
#define ENROLL "Enroll"
#define BACK "Back"
#define CANCEL "Cancel"
#define IVRS "IVRS"
#define PRINT "Print"
#define RETRY "Retry"
#define START "Start"
#define CAPTURE "Capture"
#define ALERT "Alert"
#define CLEAR "Clear"
#define PLACR_UR_FINGER "Place your finger"
#define PLACE_YOUR_FINGER "Place your finger"


