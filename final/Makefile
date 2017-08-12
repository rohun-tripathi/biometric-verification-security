CC = gcc -Wall -fno-stack-protector
LIBS = -lpwrap2 -lsqlite3 -lcurl
CFLAGS = -g
SRCS = Main.o funct2_option.o funct2_userid.o funct3_authcust.o funct5.o funct4.o keybrd_funct.o bioenrlsec_funct.o funct1_authsale.o funct1_authsale_extra.o root.c root2_option.o root3_enroll.o root3_enroll_cust.o root3_enroll_sup.o root3_enroll_ins.o root4_register.c root4_register_cust.o root4_register_sup.o root4_register_ins.o sqlfunct.o sup1_auth.o sup2_tables.o sup3_update.o sup4_result.o sup5_auth.o sup6_finoutput.o mis1_auth.o  mis2_option.o mis3_stock.o mis4_stocklast.o mis5_today.o mis6_sinlast.o

FA: $(SRCS)
	$(CC) $^ -o $@ $(LIBS)

clean: 
	rm -f *.o FA
    
