#include <stdio.h>
#include <stdlib.h>
#include <anet.h>
#include <unistd.h>
#include "types.h"

#define UF_COMMAND_LEN 13
#define SG_CMD_LEN 12
#define MAX_DEVICES 5
void show_progress_frame();
void hide_progress_frame();
void show_frame_optionspage();
int gprs();
//static char *device_name;
//static int i, ret,device = 0, test[MAX_DEVICES] = {0},res;
int  res,ret;
//int choice,ret,loop=1;
int network_connect()
{
	ret = anet_status();
	printf("\n anet_status() is %d\n",ret);

	if(ret < 0)
	{
		show_progress_frame("CHECKING NETWORK...");
		int ret1=anet_modem_on();
		printf("ret1 is %d\n",ret1);
		ret = anet_connect(); //connecting to gprs network
		//		printf("ret anet_connect() is %d\n",ret);

		hide_progress_frame();
		if(ret > 0)
		{
			printf("$$$$$$$$$$$ GPRS Connected. $$$$$$$$$\n");
			return 0;
		}
		else if(ret==-7)
		{
			show_message_frame("SIM NOT FOUND");

			//	show_frame_optionspage();
			return -1;
		}
		else if(ret==-4)
		{
			show_message_frame("NETWORK ERROR");
			printf("$$$$$$$$$$$ Sim not Found $$$$$$$$$$$\n");

			//      show_frame_optionspage();
			return -1;
		}
		else if(ret==-5)               /** Modified by 508316 */
		{
			show_message_frame("MODEM NOT RESPONDING");

			//    show_frame_optionspage();
			return -1;
		}

		else if(ret==-11)
		{
			show_message_frame("SIM NOT REGISTERED");

			//  show_frame_optionspage();
			return -1;
		}
		else if(ret==-16)
		{
			show_message_frame("LOW SIGNAL");

			// show_frame_optionspage();
			return -1;
		}

		/*else
		  {
		  show_message_frame("NETWORK NOT FOUND");        
		//	show_frame_optionspage();
		return -1;
		}
		 */
	} 
	else
	{
		printf("ret on GPRS status = %d \n", ret);

	}
	return 0;
}

int gp_test()
{
	int ret;

	ret = network_connect();

	printf(" network_connect() ret is %d\n",ret);
	if(ret ==-1 )
	{
		return -1;

	}

	return 0 ;
}
