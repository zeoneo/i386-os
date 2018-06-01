#include "screen.h"
#include "ports.h"

/* Declaration of private functions */
int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

unsigned short *video_memory = (unsigned short *)0xB8000;
// Stores the cursor position.
unsigned char cursor_x = 0;
unsigned char cursor_y = 0;
unsigned char text_color=0;

/**********************************************************
 * Public Kernel API functions                            *
 **********************************************************/

//Scrolls the text on the screen up by one line.
static void scroll()
{
  // Get a space character with the default colour attributes.
//   unsigned char attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
//   unsigned short blank = 0x20 /* space */ | (attributeByte << 8);
    unsigned char blank = ' ';

  unsigned short attribute = text_color << 8;
  // Row 25 is the end, this means we need to scroll up
  if(cursor_y >= 25)
  {
    // Move the current text chunk that makes up the screen
    // back in the buffer by a line
    int i;
    for (i = 0*80; i < 24*80; i++)
      video_memory[i] = video_memory[i+80];

    // The last line should now be blank. Do this by writing
    // 80 spaces to it.
    for (i = 24*80; i < 25*80; i++)
      video_memory[i] = blank | attribute;

    // The cursor should now be on the last line.
    cursor_y = 24;
  }
}

static void move_cursor()
{
  // The screen is 80 characters wide...
  unsigned short cursorLocation = cursor_y * 80 + cursor_x;
  port_byte_out(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
  port_byte_out(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
  port_byte_out(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
  port_byte_out(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Writes a single character out to the screen.
void monitor_put(char c)
{
  // The background colour is black (0), the foreground is white (15).
  unsigned char backColour = 0;
  unsigned char foreColour = 15;

  // The attribute byte is made up of two nibbles - the lower being the 
  // foreground colour, and the upper the background colour.
  unsigned char attributeByte = (backColour << 4) | (foreColour & 0x0F);
  // The attribute byte is the top 8 bits of the word we have to send to the
  // VGA board.
  unsigned short attribute = text_color << 8;

  // Handle a backspace, by moving the cursor back one space
  if (c == 0x08 && cursor_x)
    cursor_x--;

  // Handle a tab by increasing the cursor's X, but only to a point
  // where it is divisible by 8.
  else if (c == 0x09)
    cursor_x = (cursor_x+8) & ~(8-1);

  // Handle carriage return
  else if (c == '\r')
    cursor_x = 0;

  // Handle newline by moving cursor back to left and increasing the row
  else if (c == '\n')
  {
    cursor_x = 0;
    cursor_y++;
  }

  // Handle any other printable character.
  else if(c >= ' ')
  {
      video_memory[cursor_y*80 + cursor_x] = c | attribute;
      cursor_x++;
  }

  // Check if we need to insert a new line because we have reached the end
  // of the screen.
  if (cursor_x >= 80)
  {
      cursor_x = 0;
      cursor_y ++;
  }

  // Scroll the screen if needed.
  scroll();
  // Move the hardware cursor.
  move_cursor();
}

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear()
{
  // Make an attribute byte for the default colours
  unsigned char attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
  unsigned short blank = 0x20 /* space */ | (attributeByte << 8);

  int i;
  for (i = 0; i < 80*25; i++)
    video_memory[i] = blank;

  // Move the hardware cursor back to the start.
  cursor_x = 0;
  cursor_y = 0;
  move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c)
{
    while (*c)
      monitor_put(*c++);
}

//! Sets new font color
void screen_set_color (const unsigned char c) {
    text_color = c;
}

//! Sets new position
void goto_xy (unsigned char x, unsigned char y) {

	if (cursor_x <= 80)
	    cursor_x = x;

	if (cursor_y <= 25)
	    cursor_y = y;

	//! update hardware cursor to new position
	// DebugUpdateCur (cursor_x, cursor_y);
}

//! Clear screen
void clear_screen_with_color (const unsigned char c) {
    int i = 0;
	//! clear video memory by writing space characters to it
	for (i = 0; i < 80*25; i++)
        video_memory[i] = ' ' | (c << 8);

    //! move position back to start
    goto_xy (0,0);
}

/**
 * Print a message on the specified location
 * If col, row, are negative, we will use the current offset
 */
void kprint_at(char *message, int col, int row) {
    /* Set cursor if col/row are negative */
    int offset;
    if (col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else {
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }

    /* Loop through message and print it */
    int i = 0;
    while (message[i] != 0) {
        offset = print_char(message[i++], col, row, WHITE_ON_BLACK);
        /* Compute row/col for next iteration */
        row = get_offset_row(offset);
        col = get_offset_col(offset);
    }
}

void kprint(char *message) {
    kprint_at(message, -1, -1);
}


/**********************************************************
 * Private kernel functions                               *
 **********************************************************/


/**
 * Innermost print function for our kernel, directly accesses the video memory 
 *
 * If 'col' and 'row' are negative, we will print at current cursor location
 * If 'attr' is zero it will use 'white on black' as default
 * Returns the offset of the next character
 * Sets the video cursor to the returned offset
 */
int print_char(char c, int col, int row, char attr) {
    unsigned char *vidmem = (unsigned char*) VIDEO_ADDRESS;
    if (!attr) attr = WHITE_ON_BLACK;

    /* Error control: print a red 'E' if the coords aren't right */
    if (col >= MAX_COLS || row >= MAX_ROWS) {
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-2] = 'E';
        vidmem[2*(MAX_COLS)*(MAX_ROWS)-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
    if (col >= 0 && row >= 0) offset = get_offset(col, row);
    else offset = get_cursor_offset();

    if (c == '\n') {
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else {
        vidmem[offset] = c;
        vidmem[offset+1] = attr;
        offset += 2;
    }
    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset() {
    /* Use the VGA ports to get the current cursor position
     * 1. Ask for high byte of the cursor offset (data 14)
     * 2. Ask for low byte (data 15)
     */
    port_byte_out(REG_SCREEN_CTRL, 14);
    int offset = port_byte_in(REG_SCREEN_DATA) << 8; /* High byte: << 8 */
    port_byte_out(REG_SCREEN_CTRL, 15);
    offset += port_byte_in(REG_SCREEN_DATA);
    return offset * 2; /* Position * size of character cell */
}

void set_cursor_offset(int offset) {
    /* Similar to get_cursor_offset, but instead of reading we write data */
    offset /= 2;
    port_byte_out(REG_SCREEN_CTRL, 14);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    port_byte_out(REG_SCREEN_CTRL, 15);
    port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen() {
    int screen_size = MAX_COLS * MAX_ROWS;
    int i;
    char *screen = VIDEO_ADDRESS;

    for (i = 0; i < screen_size; i++) {
        screen[i*2] = ' ';
        screen[i*2+1] = WHITE_ON_BLACK;
    }
    set_cursor_offset(get_offset(0, 0));
}


int get_offset(int col, int row) { return 2 * (row * MAX_COLS + col); }
int get_offset_row(int offset) { return offset / (2 * MAX_COLS); }
int get_offset_col(int offset) { return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; }
