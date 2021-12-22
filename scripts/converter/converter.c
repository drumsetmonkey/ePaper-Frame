#include <stdio.h>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

uint32_t palette[8] = {
	0x060606, // Black
	0xf0f0f0, // White
	0x4c6e55, // Green
    0x39476c, // Blue
    0xa6534f, // Red
    0xddcd5e, // Yellow
    0xc16653, // Orange
    0xd2a691, // Tan/Clean
};

uint8_t depalette(uint8_t * color)
{
    uint8_t bestc = 1;
	for (unsigned p = 0; p < sizeof(palette)/sizeof(palette[0]); p++ ) {
        uint32_t readColor = 0xFFFFFF &
                           ( ((color[0] & 0xFF) << 16)
                           | ((color[1] & 0xFF) << 8)
                           | ((color[2] & 0xFF) << 0) );

		if (readColor == palette[p]) {
			bestc = p;
            break;
        }
	}

	return bestc;
}


int main(int argc, char ** argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: converter [image file] [out file]\n");
		return -1;
	}

	int x,y,n;
	unsigned char *data = stbi_load(argv[1], &x, &y, &n, 0);
	if (!data) {
		fprintf(stderr, "Error: Can't open image.\n");
		return -6;
	}
	if (x != 600) {
		fprintf(stderr, "Error: image dimensions must be 600 x ??.\n");
		return -2;
	}
	if (y > 448) {
		y = 448;
	}

	FILE * fout = fopen(argv[2], "wb");
    uint16_t signature = 0x565c;
    fwrite(&signature, 2, 1, fout);

	int margin = 448 - y;
	uint8_t line[600/2];
	if (y < 448) {
        printf("Image is less than 448 pixels tall...\n");
		int k;
		int ke = margin / 2;
		//memset( line, 0x66, 600/2 );
		memset(line, 0x11, 600/2);
		for (k = 0; k < ke; k++) {
			fwrite( line, 600/2, 1, fout );
		}
		margin -= ke;
	}

	if (y > 448)
        y = 448;

	for (int j = 0; j < y; j++) {
		for (int i = 0; i < x/2; i++) {
			uint8_t c1 = depalette(data + n*(i*2 + x*j));
			uint8_t c2 = depalette(data + n*(i*2 + x*j + 1));
			line[i] = c2 | (c1<<4);
		}
		fwrite(line, 600/2, 1, fout);
	}
	//memset( line, 0x66, 600/2 );
	memset(line, 0x11, 600/2);
	for (int k = 0; k < margin; k++) {
		fwrite(line, 600/2, 1, fout);
	}
	stbi_image_free(data);
}



