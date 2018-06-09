#include "shell.h"
#include "../cpu/timer.h"
#include "../drivers/kybrd.h"
#include "../drivers/printk.h"
#include "../drivers/screen.h"
// sleeps a little bit. This uses the HALs get_tick_count() which in turn uses the PIT
void sleep (unsigned int ms) {
	unsigned int ticks = ms + get_timer_ticks();
	while (ticks > get_timer_ticks ());
}

//! wait for key stroke
enum KEYCODE	getch () {
	enum KEYCODE key = KEY_UNKNOWN;
	//! wait for a keypress
	while (key==KEY_UNKNOWN)
		key = kkybrd_get_last_key ();

	//! discard last keypress (we handled it) and return
	kkybrd_discard_last_key ();
	return key;
}

//! command prompt
void cmd () {

	printk ("\nCommand> ");
}

//! gets next command
void get_cmd (char* buf, int n) {

	cmd ();

	enum KEYCODE key = KEY_UNKNOWN;
	int	BufChar;

	//! get command string
	int i=0;
	while ( i < n ) {

		//! buffer the next char
		BufChar = 1;

		//! grab next char
		key = getch ();

		//! end of command if enter is pressed
		if (key==KEY_RETURN)
			break;

		//! backspace
		if (key==KEY_BACKSPACE) {

			//! dont buffer this char
			BufChar = 0;

			if (i > 0) {

				//! go back one char
				unsigned char y, x;
				get_cursor_xy(&x, &y);
				if (x>0)
					goto_xy (--x, y);
				else {
					//! x is already 0, so go back one line
					y--;
					x = get_screen_width();
				}

				//! erase the character from display
				printk ("%c", ' ');
				goto_xy (x, y);

				//! go back one char in cmd buf
				i--;
			}
		}

		//! only add the char if it is to be buffered
		if (BufChar) {

			//! convert key to an ascii char and put it in buffer
			char c = kkybrd_key_to_ascii (key);
			if (c != 0) { //insure its an ascii char

				printk("%c", c);
				buf [i++] = c;
			}
		}

		//! wait for next key. You may need to adjust this to suite your needs
		sleep (10);
	}

	//! null terminate the string
	buf [i] = '\0';
}

//! our simple command parser
int run_cmd (char* cmd_buf) {

	//! exit command
	if (strcmp (cmd_buf, "exit") == 0) {
		return 1;
	}

	//! clear screen
	else if (strcmp (cmd_buf, "cls") == 0) {
		clear_screen_with_color (0x17);
	}

	//! help
	else if (strcmp (cmd_buf, "help") == 0) {

		printk ("\n Zeus OS ");
		printk ("\nwith a basic Command Line Interface (CLI)\n\n");
		printk ("Supported commands:\n");
		printk (" - exit: quits and halts the system\n");
		printk (" - cls: clears the display\n");
		printk (" - help: displays this message\n");
	}

	//! invalid command
	else {
		printk ("\nUnkown command: %s", cmd_buf);
	}

	return 0;
}

void run () {

	//! command buffer
	static char	cmd_buf [100];

	while (1) {

		//! get command
		get_cmd (cmd_buf, 98);

		//! run command
		if (run_cmd (cmd_buf) == 1)
			break;
	}
}