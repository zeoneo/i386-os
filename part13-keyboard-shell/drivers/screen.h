#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f
#define RED_ON_WHITE 0xf4

/* Screen i/o ports */
#define REG_SCREEN_CTRL 0x3d4
#define REG_SCREEN_DATA 0x3d5

// unsigned short *video_memory = (unsigned short *)0xB8000;
// // Stores the cursor position.
// unsigned char cursor_x = 0;
// unsigned char cursor_y = 0;

/* Public kernel API */
void clear_screen();
void kprint_at(char *message, int col, int row);
void kprint(char *message);
void monitor_put(char c);
void get_cursor_xy (unsigned char * x, unsigned char * y);
unsigned char get_screen_width();
unsigned char get_screen_height();
// Clear the screen to all black.
void monitor_clear();

// Output a null-terminated ASCII string to the monitor.
void monitor_write(char *c);
