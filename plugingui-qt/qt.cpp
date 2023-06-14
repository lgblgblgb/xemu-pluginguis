/* Part of the Xemu project, please visit: https://github.com/lgblgblgb/xemu
   Experimental (WIP!) QT GUI plugin for Xemu. Tested only with QT5 building on Ubuntu Linux.
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

// https://doc.qt.io/qt-5/qmessagebox.html
// Pre-req on Ubuntu: apt-get install qtbase5-dev

// Things must be done _BEFORE_ including xemu_plugingui.h!
// Note: xemu_plugingui.h itself includes stdio.h and some other headers as well, including SDL.h

// Give a _short_ name for the plugin
#define	PLUGIN_NAME "QT"
// Define this (optionally) to have DEBUGGUI(...) then (redefined in xemu_plugingui.h) to have in-development DEBUG messages
// If you don't need this (production), simple do not define this at all
#define DEBUGGUI

// At this point, first, we want to include this:
#include "xemu_plugingui.h"

// Then your own stuff needed for this plugin:
#include <QApplication>
#include <QMessageBox>

// Needed by Xemu. If the value is not the same as Xemu thinks, Xemu will refuse to use your plugin.
const int XemuPluginGuiAPI_compatibility_const = 1;


static char  my_argv0[] = { 'X', 'e', 'm', 'u', '\0' };
static char *my_argv[] = { my_argv0 };
static int   my_argc = 1;


static QApplication *myqapp = nullptr;




int XemuPluginGuiAPI_init ( struct xemuplugingui_info_st *info )
{
	debug_fp = info->debug_fp;	// We need this to use DEBUG(...) and similar stuff!
	if (info->length != sizeof(struct xemuplugingui_info_st))
		DEBUGPRINT("UH-OH info struct size mismatch!");
#ifndef IS_WINDOWS
	if ((info->flags & PLUGINGUI_INFOFLAG_X11)) {
		// Workaround: on Wayland, it's possible that SDL uses x11, but the GUI (QT now) would use Wayland, mixing x11 and wayland within the same app, isn't a good idea
		// thus we try to force x11 for QT via an environment variable set here, if we detect SDL uses x11
		static const char qt_backend_var_name[]  = "QT_QPA_PLATFORM";
		static const char qt_backend_var_value[] = "xcb";	// it seems QT needs "xcb" here, not "x11" (unlike GDK)
		DEBUGPRINT("setting environment variable %s=%s to avoid possible QT backend mismatch with SDL", qt_backend_var_name, qt_backend_var_value);
		setenv(qt_backend_var_name, qt_backend_var_value, 1);
	}
#endif
	if (myqapp)
		return 1;
	DEBUGPRINT("Initializing ...");
	fflush(stdout);
	try {
		DEBUGPRINT("BEFORE"); fflush(stdout);
		//myqapp = new QApplication(myargc, myargv, 0);
		myqapp = new QApplication(my_argc, my_argv);
		DEBUGPRINT("AFTER"); fflush(stdout);
	} catch (...) {
		DEBUGPRINT("CARCH"); fflush(stdout);
		if (myqapp != nullptr) {
			DEBUGPRINT("Deleting QApplication QT object because of exception"); fflush(stdout);
			delete myqapp;
		} else
			DEBUGPRINT("QApplication instance creation resulted in nullptr");
		myqapp = nullptr;
	}
	if (myqapp == nullptr) {
		DEBUGPRINT("Could not initialize QT!");
		return 1;
	}
	DEBUGPRINT("QT initialized with faked argv \"%s\"", my_argv[0]);
	return 0;
}


void XemuPluginGuiAPI_shutdown ( void )
{
	if (myqapp) {
		delete myqapp;
		myqapp = nullptr;
		DEBUGGUI("QT end");
	}
}


int XemuPluginGuiAPI_SDL_ShowSimpleMessageBox ( Uint32 flags, const char *title, const char *message, SDL_Window *window )
{
	if (!myqapp)
		return 1;
	QMessageBox::Icon icon;
	switch (flags) {
		case SDL_MESSAGEBOX_INFORMATION:
			icon = QMessageBox::Information;
			break;
		case SDL_MESSAGEBOX_WARNING:
			icon = QMessageBox::Warning;
			break;
		case SDL_MESSAGEBOX_ERROR:
			icon = QMessageBox::Critical;
			break;
		default:
			icon = QMessageBox::NoIcon;
			break;
	}
	(new QMessageBox(icon, (const QString&)title, (const QString&)message))->exec();
	return 0;
}


#if 0
int XemuPluginGuiAPI_SDL_ShowMessageBox ( const SDL_MessageBoxData *box, int *buttonid )
{
	if (!myqapp)
		return 1;
	return 1;
}
#endif
