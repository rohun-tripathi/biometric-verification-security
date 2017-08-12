#include <stdio.h>
#include <stdlib.h>
#ifdef __arm__
#include <libgps.h>
#endif

#include "types.h"

//extern struct gps_data temp;
int  Gps_Test(double *lat, double *lon)
{
#ifdef __arm__
	int fd;
	struct gps_data temp;
	int status = -1;
	int snr = 0;

	printf("init\n");
	fd = gps_init();
	if(fd < 0)
	{
		printf("Unable to init....\n");
	}
	if(fd != -1)
	{
		status = read_gps_data(fd, &temp, 5);
		//print gps data
		/*		printf("---------GPS data---------\n\n");
				printf("Time %ld\n", temp.time);
				printf("latitude %lf ", temp.latitude);
				printf("%c\n", temp.lat_dir);*/
		*lat=temp.latitude;

		//printf("Longitude %lf ", temp.longitude);
		printf("%c\n", temp.lon_dir);
		*lon=temp.longitude;
		//	printf("Satellites %d\n", temp.sno);
		//	printf("Altitude %lf ", temp.altitude);
		//	printf("%c\n", temp.alt_unit);
		status = read_gps_signalstrength(fd, &snr, 5);
		//	printf("Signal strength %d\n", snr);

		gps_uninit(fd);
		//	return;
	}
#endif
	return 0;
}
