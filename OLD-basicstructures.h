typedef struct charfreq{
	unsigned char key;
	long long int frequency;
}CharFreq;

typedef struct keycode{
	unsigned int bitsSize;
	unsigned char bitsCode[64];
}Keycode;

typedef struct mybyte{
	unsigned char byte;
	unsigned char counter;
}myByte;
