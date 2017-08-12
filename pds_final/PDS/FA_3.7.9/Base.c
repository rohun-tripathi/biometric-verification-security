#include "Base.h"

void encode_block( unsigned char i[3], unsigned char o[4], int len)
{
	o[0] = cB64[ i[0] >> 2];
	o[1] = cB64[ ((i[0] & 0x03) << 4) | ((i[1] & 0xf0) >> 4)];
	o[2] = (unsigned char) (len > 1 ? cB64[ ((i[1] & 0x0f) << 2) | ((i[2] & 0xc0) >> 6)] : '=');
	o[3] = (unsigned char) (len > 2 ? cB64[ i[2] & 0x3f ] : '=');
}

int encode_buffer( unsigned char *inBuff, unsigned char *outBuff, int in_size )
{
	unsigned char in[3], out[4];
	int i=0, j=0;

	while (i < in_size) {
		if ((i+3) <= in_size) {
			in[0] = inBuff[i++];
			in[1] = inBuff[i++];
			in[2] = inBuff[i++];
			encode_block(in, out, 3);
		} else if ((i+2) == in_size) {
			in[0] = inBuff[i++];
			in[1] = inBuff[i++];
			in[2] = '\0';
			encode_block(in, out, 2);
		} else {
			in[0] = inBuff[i++];
			in[1] = '\0';
			in[2] = '\0';
			encode_block(in, out, 1);
		}
		outBuff[j++] = out[0];
		outBuff[j++] = out[1];
		outBuff[j++] = out[2];
		outBuff[j++] = out[3];
	}
	return(j);
}

void decode_block( unsigned char i[4], unsigned char o[3] )
{
	o[0] = (unsigned char) (i[0] << 2 | i[1] >> 4);
	o[1] = (unsigned char) (i[1] << 4 | i[2] >> 2);
	o[2] = (unsigned char) (((i[2] << 6) & 0xc0) | i[3]);
}

int decode_buffer( unsigned char *inBuff, unsigned char *outBuff, int in_size )
{
	unsigned char in[4], out[3], v;
	int i=0, j=0, k=0, len;

	while (i < in_size) {
		for (len=0; (len < 4) && (i < in_size); i++) {
			v = (unsigned char) inBuff[i];
			v = (unsigned char) ((v < 43 || v > 122) ? 0 : cD64[v-43]);
			if (v)
				v = (unsigned char) ((v == '$') ? 0 : v-61);
			if (v == 0) {
				continue;
			}
			if (v)
				in[len++] = (unsigned char) (v-1);
			else
				in[len++] = 0;
		}
		if (len) {
			decode_block(in, out);
			for (k=0; k < (len-1); k++)
				outBuff[j++] = out[k];
		}
	}
	return(j);
}

void save_decoded_bio_data( unsigned char *bioD, int len, char *id )
{
	char fname[256];
	unsigned char *template=NULL;
	int tlen=0;

	printf("Decoding bioData of len=%d, id=%s\n", len, id);
	template = (unsigned char *)malloc(sizeof(char)*len);
	tlen = decode_buffer(bioD, template, len);
	if (tlen) {
		int fd=0;

		sprintf(fname, "%s", id);
		fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		if (fd > 0) {
			write(fd, template, tlen);
			close(fd); fd = 0;
		}
	}
	if (template != NULL)
		free(template);
}

void save_decoded_member_image( unsigned char *mbrImg, int len, char *id )
{
	char fname[256];
	unsigned char *rawImg=NULL;
	int tlen=0;

	printf("Decoding raw image of len=%d, id=%s\n", len, id);
	rawImg = (unsigned char *)malloc(sizeof(char)*len);
	tlen = decode_buffer(mbrImg, rawImg, len);
	if (tlen) {
		int fd=0;

		sprintf(fname, "db/%s.raw.dat", id);
		fd = open(fname, O_WRONLY|O_CREAT|O_TRUNC, 0666);
		if (fd > 0) {
			write(fd, rawImg, tlen);
			close(fd); fd = 0;
		}
	}
	if (rawImg != NULL)
		free(rawImg);
}
