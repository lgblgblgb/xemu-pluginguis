/* Part of the Xemu project, please visit: https://github.com/lgblgblgb/xemu
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

#ifndef XEMU_COMMON_EMUTOOLS_GUI_H_INCLUDED
#define XEMU_COMMON_EMUTOOLS_GUI_H_INCLUDED

#define NL "\n"


#define	DEBUGPRINT	printf

//#define DEBUGGUI	DEBUGPRINT
#define DEBUGGUI	printf
//#define DEBUGGUI(...)

#define XEMUGUI_FSEL_DIRECTORY		0
#define XEMUGUI_FSEL_OPEN		1
#define XEMUGUI_FSEL_SAVE		2
#define XEMUGUI_FSEL_FLAG_STORE_DIR	0x100

#define XEMUGUI_MENUID_CALLABLE		0
#define XEMUGUI_MENUID_SUBMENU		1
#define XEMUGUI_MENUID_TITLE		2

#define XEMUGUI_MENUFLAG_INACTIVE	0x00100
#define XEMUGUI_MENUFLAG_BEGIN_RADIO	0x00200
#define XEMUGUI_MENUFLAG_END_RADIO	0x00400
#define XEMUGUI_MENUFLAG_ACTIVE_RADIO	0x00800
#define XEMUGUI_MENUFLAG_SEPARATOR	0x01000
#define XEMUGUI_MENUFLAG_CHECKED	0x02000
#define XEMUGUI_MENUFLAG_QUERYBACK	0x04000
#define XEMUGUI_MENUFLAG_UNCHECKED	0x08000
#define XEMUGUI_MENUFLAG_HIDDEN		0x10000

#ifndef XEMUGUI_MAX_SUBMENUS
#define XEMUGUI_MAX_SUBMENUS		100
#endif
#ifndef XEMUGUI_MAX_ITEMS
#define XEMUGUI_MAX_ITEMS		900
#endif

#ifndef XEMUGUI_MAINMENU_NAME
#define XEMUGUI_MAINMENU_NAME "Main Menu"
#endif

#define XEMUGUI_RETURN_CHECKED_ON_QUERY(query,status) \
	do { if (query) { \
		*query |= (status) ? XEMUGUI_MENUFLAG_CHECKED : XEMUGUI_MENUFLAG_UNCHECKED; \
		return; \
	} } while (0)

struct menu_st;

typedef void (*xemugui_callback_t)(const struct menu_st *desc, int *query);

struct menu_st {
	const char *name;
	int type;
	const xemugui_callback_t handler;
	const void *user_data;
};

#define	PLUGINGUI_INITFLAG_X11		1
#define	PLUGINGUI_INITFLAG_WAYLAND	2

extern const int XemuPluginGuiAPI_compatibility_const;
int XemuPluginGuiAPI_init ( const int flags );
void XemuPluginGuiAPI_shutdown ( void );
int  XemuPluginGuiAPI_SDL_ShowSimpleMessageBox(Uint32 flags, const char *title, const char *message, SDL_Window *window);
int  XemuPluginGuiAPI_SDL_ShowMessageBox(const SDL_MessageBoxData *messageboxdata, int *buttonid);
int  XemuPluginGuiAPI_iteration ( void );
int  XemuPluginGuiAPI_file_selector ( int dialog_mode, const char *dialog_title, char *default_dir, char *selected, int path_max_size );
int  XemuPluginGuiAPI_popup ( const struct menu_st desc[] );


#if defined(_WIN32) || defined(_WIN64)
#	define	IS_WINDOWS
#	define	DIRSEP_CHR	'\\'
#else
#	define	DIRSEP_CHR	'/'
#endif


static inline void store_dir_from_file_selection ( char *store_dir, const char *filename, int dialog_mode )
{
	if (store_dir && (dialog_mode & XEMUGUI_FSEL_FLAG_STORE_DIR)) {
		if ((dialog_mode & 0xFF) == XEMUGUI_FSEL_DIRECTORY)
			strcpy(store_dir, filename);
		else {
			char *p = strrchr(filename, DIRSEP_CHR);
			if (p) {
				memcpy(store_dir, filename, p - filename + 1);
				store_dir[p - filename + 1] = '\0';
			}
		}
	}
}


#endif
