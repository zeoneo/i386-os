void vgaGraphicsModeInit();
unsigned char *getFrameBufferSegment();
unsigned char getColorIndex(unsigned char r, unsigned char g, unsigned char b);
void fillRectangle(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b);