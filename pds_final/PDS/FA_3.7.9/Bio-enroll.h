#include <pwrap2.h>
#include "types.h"
#include <sys/stat.h>
#include <time.h>
#include <string.h>

pw_widget label_customer_det[10], rb_fin[10], frame_bio_enroll;
pw_widget button_FP;
int enroll_flag[8];

extern Bool cb_entry_num(pw_widget w, pw_event e);
extern Bool cb_entry_alpha(pw_widget w, pw_event e);

#define ENROLLED 1
#define NOT_ENROLLED -1


