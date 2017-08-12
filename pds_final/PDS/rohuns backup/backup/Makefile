CC = gcc -Wall
LIBS = -lpwrap2 -lsqlite3 -lcurl
CFLAGS = -g
SRCS = Main.o funct2.o funct3.o funct4.o keybrd_funct.o bioenrlsec_funct.o pre_main.c root.c  register.c bioenroll.c

FA: $(SRCS)
	$(CC) $^ -o $@ $(LIBS)

clean: 
	rm -f *.o FA
    
