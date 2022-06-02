/* Part of the Xemu project, please visit: https://github.com/lgblgblgb/xemu
   **DO NOT USE THIS** Xemu has a built-in GTK3 GUI plugin!
   This is only used to test already-sort-of-working internal code of Xemu as the form of a plugin.
   Beware, ugly code ahead ...
   Copyright (C)2016-2022 LGB (Gábor Lénárt) <lgblgblgb@gmail.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */


// Things must be done _BEFORE_ including xemu_plugingui.h!
// Note: xemu_plugingui.h itself includes stdio.h and some other headers as well, including SDL.h

// Give a _short_ name for the plugin
#define	PLUGIN_NAME "FLTK"
// Define this (optionally) to have DEBUGGUI(...) then (redefined in xemu_plugingui.h) to have in-development DEBUG messages
// If you don't need this (production), simple do not define this at all
#define DEBUGGUI

// At this point, first, we want to include this:
#include "xemu_plugingui.h"

// Then your own stuff needed for the plugin
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/fl_ask.H>
#include <FL/fl_draw.H>


#include <climits>

// Needed by Xemu. If the value is not the same as Xemu thinks, Xemu will refuse to use your plugin.
const int XemuPluginGuiAPI_compatibility_const = 1;

static struct xemuplugingui_info_st *xemu_info = NULL;



// We don't have iteration stuff:
//int XemuPluginGuiAPI_iteration ( void );


int XemuPluginGuiAPI_init ( struct xemuplugingui_info_st *info )
{
	debug_fp = info->debug_fp;	// We need this to use DEBUG(...) and similar stuff!
	xemu_info = info;		// remember the info struct ptr, so we can use xemu_info from other callbacks as well from Xemu
	if (info->length != sizeof(struct xemuplugingui_info_st))
		DEBUGPRINT("UH-OH info struct size mismatch!");
	// Allow for double buffered windows?
	Fl::visual(FL_DOUBLE|FL_INDEX);
	// ideally there should be a check for "info->flags & PLUGINGUI_INITFLAG_X11" and trying to sync if SDL is X11 forcing FLTK to be X11 as well
	// (problem when SDL is wayland but FLTK is not or vice-versa) TODO
	DEBUGPRINT("initialized.");
	return 0;
}


void XemuPluginGuiAPI_shutdown ( void )
{
	// TODO?
	DEBUGGUI("end");
}


#if 0
int XemuPluginGuiAPI_iteration ( void )
{
	// Ugly hack! Since I cannot tell FLTK after closing a window to really hide from screen, without this
	// iteration part, the window remains as "zombie" on the screen. FLTK seems to be really complicated :-/
	// How I can wait for processing all pending events without waiting any longer though? And not here, but after closing a window.
	// There should be a way to do that, and moving that part after each window closing, and not using iteration here ever!
	// This solution if fairly unoptimal, as it takes cycles from the emulation about 50-60 times per second ...
	//Fl::wait(0);
	return 0;
}
#endif


static void calc_win_pos ( int sizex, int sizey, int &posx, int &posy )
{
	xemu_info->info_updater();
	posx = xemu_info->winx + xemu_info->sizex / 2 - sizex / 2;
	if (posx + sizex >= xemu_info->screenx)
		posx = xemu_info->screenx - sizex;
	if (posx < 0)
		posx = 0;
	posy = xemu_info->winy + xemu_info->sizey / 2 - sizey / 2;
	if (posy + sizey >= xemu_info->screeny)
		posy = xemu_info->screeny - sizey;
	if (posy < 0)
		posy = 0;
}


int XemuPluginGuiAPI_SDL_ShowSimpleMessageBox ( Uint32 flags, const char *title, const char *message, SDL_Window *window )
{
	fl_message_title(title);
	switch (flags) {
		//case SDL_MESSAGEBOX_ERROR:
		default:
			fl_alert("%s", message);
			break;
		case SDL_MESSAGEBOX_WARNING:
			fl_alert("%s", message);
			break;
		case SDL_MESSAGEBOX_INFORMATION:
			fl_message("%s", message);
			break;
	}
	Fl::wait(0);
	return 0;
}


static int button_clicked;

//static void cb_fl_ButtonClick ( Fl_Button *button, void *something )
static void cb_fl_ButtonClick ( Fl_Widget *button, void *something )
{
	button_clicked = (int)(intptr_t)something;
	DEBUGGUI("Clicky-click! %p %d", button, button_clicked);
}

// We're out of luck ... FLTK has some nice function for "choose dialog" - fl_choice() - but unfortunately
// only for 2 or 3 items :(
// we need for any number of items (even one, and more than three ...), so we need to do it as their own :(
// Also FLTK's fl_choice() seems to be odd how to tell which is the default button, you need to re-order (?)
// buttons then and other strange things ...

int XemuPluginGuiAPI_SDL_ShowMessageBox ( const SDL_MessageBoxData *box, int *buttonid )
{
	if (box->numbuttons < 1 || box->numbuttons > 10) {
		DEBUGPRINT("INVALID SDL_ShowMessageBox: wrong number of buttons = %d", box->numbuttons);
		return -1;
	}
	int xsize = 280;
	const int y = 115;
	fl_font(FL_HELVETICA, 10);
	int winxpos, winypos;
	calc_win_pos(xsize, 150, winxpos, winypos);
	auto win_fl = new Fl_Double_Window(winxpos, winypos, xsize, 150, box->title);
	win_fl->color((Fl_Color)48);
	auto box_fl = new Fl_Box(10, 10, 260, 95, "Box1");
	box_fl->box(FL_THIN_UP_BOX);
	auto msg_fl = new Fl_Box(20, 15, 240, 85, box->message);
	msg_fl->box(FL_FLAT_BOX);
	msg_fl->align(Fl_Align(FL_ALIGN_WRAP|FL_ALIGN_INSIDE|FL_ALIGN_LEFT));
	// Figure out the total width of the buttons first,
	// also extract default "enter" and "escape" assignments.
	// I really can't understand how FLTK works, I cannot find nice tutorial, it seems
	// I should count pixels, and no "containers" for widgets to do this at there own? :-(
	// the sampe applies to the text, how I can do auto-sized window to be enough for the text?
	// etc. I wish FLTK would have better documentation - or I should be smarter to find it ;)
	int total_width = 0;
	int shortcut_enter_index = -1;
	int shortcut_escape_index = -1;
	for (int i = 0; i < box->numbuttons; i++) {
		const int width = fl_width(box->buttons[i].text) + 30;
		total_width += width + 10;
		if ((box->buttons[i].flags & SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT))
			shortcut_enter_index = i;
		if ((box->buttons[i].flags & SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT))
			shortcut_escape_index = i;
	}
	if (shortcut_enter_index < 0) {	// no default "ENTER"? let's assume the first is the one!
		shortcut_enter_index = 0;
		DEBUGPRINT("Warning, no default ENTER item, using index %d", shortcut_enter_index);
	}
	if (shortcut_escape_index < 0) {
		// no default ESCAPE? try to assume the next after "ENTER", if that's over the num of buttons, warp around ;)
		// ugly. But caller really should make defaults as their own, anyway!
		shortcut_escape_index = (shortcut_enter_index + 1) % box->numbuttons;
		DEBUGPRINT("Warning, no default ESCAPE item, using index %d", shortcut_escape_index);
	}
	int x = xsize - total_width - 20;
	if (x < 0)
		x = 0;
	// Now the buttons ...
	for (int i = 0; i < box->numbuttons; i++) {
		const int width = fl_width(box->buttons[i].text) + 30;
		//DEBUGGUI("Adding button: [%s] size = %d", button_texts[i], width);
		auto button_fl = new Fl_Button(x, y, width, 25, box->buttons[i].text);
		x += width + 10;
		// don't want to have enter+escape for the same button as "shortcut", as probably there can be one.
		// so I prioritize enter, as escape anyway cancels selection but enter would not work otherwise!
		// I can still substitute the "default escape" if there was no selection (aborted by escape)
		if (i == shortcut_enter_index)
			button_fl->shortcut(FL_Enter);
		else if (i == shortcut_escape_index)
			button_fl->shortcut(FL_Escape);
		button_fl->selection_color(FL_LIGHT1);
		button_fl->callback(cb_fl_ButtonClick, (void*)(intptr_t)box->buttons[i].buttonid);
	}
	button_clicked = INT_MAX;
	win_fl->set_modal();
	win_fl->end();
	win_fl->show();
	while (Fl::wait())
		if (button_clicked != INT_MAX)
			break;
	*buttonid = button_clicked == INT_MAX ? box->buttons[shortcut_escape_index].buttonid : button_clicked;
	win_fl->hide();
	delete win_fl;
	Fl::wait(0);
	return 0;
}
