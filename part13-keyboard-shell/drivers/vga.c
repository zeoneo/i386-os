
#include "vga.h"
#include "ports.h"
unsigned char g_320x200x256[] = {
    /* MISC */
    0x63,
    /* SEQ */
    0x03, 0x01, 0x0F, 0x00, 0x0E,
    /* CRTC */
    0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
    0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
    0xFF,
    /* GC */
    0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
    0xFF,
    /* AC */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x41, 0x00, 0x0F, 0x00, 0x00
    /* New */
};

void vgaGraphicsModeInit()
{
    writeRegisters(g_320x200x256);
}

void writeRegisters(unsigned char *registers)
{
    unsigned char i;
    //  misc
    port_byte_out(0x3c2, *(registers++));

    // sequencer
    for (i = 0; i < 5; i++)
    {
        port_byte_out(0x3c4, i);
        port_byte_out(0x3c5, *(registers++));
    }

    // cathode ray tube controller
    port_byte_out(0x3d4, 0x03);
    // crtcIndexPort.Write(0x03);
    port_byte_out(0x3d5, (unsigned char)port_byte_in(0x3d5) | 0x80);
    // crtcDataPort.Write(crtcDataPort.Read() | 0x80);
    port_byte_out(0x3d4, 0x11);
    // crtcIndexPort.Write(0x11);
    port_byte_out(0x3d5, port_byte_in(0x3d5) & ~0x80);
    // crtcDataPort.Write(crtcDataPort.Read() & ~0x80);

    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;

    for (i = 0; i < 25; i++)
    {
        port_byte_out(0x3d4, i);
        // crtcIndexPort.Write(i);
        port_byte_out(0x3d5, *(registers++));
        // crtcDataPort.Write(*(registers++));
    }

    // graphics controller
    for (i = 0; i < 9; i++)
    {
        port_byte_out(0x3ce, i);
        // graphicsControllerIndexPort.Write(i);
        port_byte_out(0x3cf, *(registers++));
        // graphicsControllerDataPort.Write();
    }

    // attribute controller
    for (i = 0; i < 21; i++)
    {
        port_byte_in(0x3da);
        port_byte_out(0x3c0, i);
        port_byte_out(0x3c0, *(registers++));
        // attributeControllerResetPort.Read();
        // attributeControllerIndexPort.Write(i);
        // attributeControllerWritePort.Write(*(registers++));
    }
    port_byte_in(0x3da);
    port_byte_out(0x3c0, 0x20);
    // attributeControllerResetPort.Read();
    // attributeControllerIndexPort.Write(0x20);
}

int isSupportedMode(unsigned int width, unsigned int height, unsigned int colordepth)
{
    return width == 320 && height == 200 && colordepth == 8;
}

void setMode(unsigned int width, unsigned int height, unsigned int colordepth)
{
    if (!isSupportedMode(width, height, colordepth))
        return;
    writeRegisters(g_320x200x256);
}

unsigned char *getFrameBufferSegment()
{
    // graphicsControllerIndexPort.Write(0x06);
    port_byte_out(0x3ce, 0x06);
    unsigned char segmentNumber = port_byte_in(0x3cf) & (3 << 2);
    switch (segmentNumber)
    {
    default:
    case 0 << 2:
        return (unsigned char *)0x00000;
    case 1 << 2:
        return (unsigned char *)0xA0000;
    case 2 << 2:
        return (unsigned char *)0xB0000;
    case 3 << 2:
        return (unsigned char *)0xB8000;
    }
}

void _putPixel(int x, int y, unsigned char colorIndex)
{
    if (x < 0 || 320 <= x || y < 0 || 200 <= y)
        return;

    unsigned char *pixelAddress = getFrameBufferSegment() + 320 * y + x;
    *pixelAddress = colorIndex;
}

unsigned char getColorIndex(unsigned char r, unsigned char g, unsigned char b)
{
    if (r == 0x00 && g == 0x00 && b == 0x00)
        return 0x00; // black
    if (r == 0x00 && g == 0x00 && b == 0xA8)
        return 0x01; // blue
    if (r == 0x00 && g == 0xA8 && b == 0x00)
        return 0x02; // green
    if (r == 0xA8 && g == 0x00 && b == 0x00)
        return 0x04; // red
    if (r == 0xFF && g == 0xFF && b == 0xFF)
        return 0x3F; // white
    return 0x00;
}

void putPixel(int x, int y, unsigned char r, unsigned char g, unsigned char b)
{
    _putPixel(x, y, getColorIndex(r, g, b));
}

void fillRectangle(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned int Y, X;
    for (Y = y; Y < y + h; Y++)
        for (X = x; X < x + w; X++)
            putPixel(X, Y, r, g, b);
}
