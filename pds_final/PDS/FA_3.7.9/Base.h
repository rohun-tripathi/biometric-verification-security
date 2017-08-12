// Loan Collection and Disbursements

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

static const char cB64[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char cD64[] =
"|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

void encode_block( unsigned char i[3], unsigned char o[4], int len);
int encode_buffer( unsigned char *inBuff, unsigned char *outBuff, int in_size );
void decode_block( unsigned char i[4], unsigned char o[3] );
int decode_buffer( unsigned char *inBuff, unsigned char *outBuff, int in_size );
void save_decoded_bio_data( unsigned char *bioD, int len, char *id );
void save_decoded_member_image( unsigned char *mbrImg, int len, char *id );
