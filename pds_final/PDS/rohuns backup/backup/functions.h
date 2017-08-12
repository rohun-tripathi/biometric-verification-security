#define _FUNCTIONS_H

#include "global.h"

#include <usb.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <jpeglib.h>
#include <string.h>
#ifdef __AMIDA__
#include <sqlite3.h>
#include <cdma.h>
#include <gprs.h>
#endif



int  Gps_Test(double *lat, double *lon);
