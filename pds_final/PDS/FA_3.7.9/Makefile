CC= gcc -Wall
LIBS= -lpwrap2 -lsqlite3 -lcurl
CFLAGS= -g
SRCS=Main_page.o Functions.o Options_Page.o Base.o Bio_enroll.o Biometric_DetailsPage.o Group_CreationPage.o MusterRoll_Page.o  Connect_Disconnect.o Gprs_Page.o  Gprs_function.o  gps.o string_list.o  gps.o db-utils.o Identify_grp.o Gprs_down.o Reports.o Camera.o  PosMangement.o Pos_infodetails.o Login_Page.o Mcc_login.o Response_all.o update_chk.o Mvo_report.o  Mvo_login.o  no_wrk_up.oMemory_info_upload.o  Wage_info_down.o Application_download.o FP_download.o Demand_options.o demand_creation_page.o demand_report.o demand_allocation_page.o work_allocation_report.o mcc_work_alloc_down.o mcc_demand_groups_down.o no_work_demand_up.o noworkdemand.o mcc_work_allocation_report.o notenroll_photocapture.o rejection_details.o dwn_work_alloc_rejection.o dwn_demand_rejection.o batch_info_dwn.o normal_login.o  normal_page.o

FA: $(SRCS)
	$(CC) $^ -o $@ $(LIBS)

clean:
	rm -f *.o FA

