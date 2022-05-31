
#define PLUGINGUI_INFOFLAG_X11		1
#define PLUGINGUI_INFOFLAG_WAYLAND	2

// A pointer to this struct is passed the plugin's init function, which can be used by the plugin then
// Also can be updated by the plugin, by calling this very struct's function pointer: info_updater
// **** DO NOT MODIFY THIS STRUCT, IT WILL BREAK COMPATIBILITY WITH PLUGINS ****
struct xemuplugingui_info_st {
	int length;		// length of the structure (bytes) can be used to compare result from plugin's definition for this struct
	Uint64 flags;		// various flags, see: PLUGINGUI_INFOFLAG_*
	SDL_Window *sdl_window;	// SDL window of the emulator
	const char *xemu_version;	// Xemu's "version"
	struct xemuplugingui_info_st *(*info_updater)(void);	// call this from plugin to update the passed struct pointer at 'init'
	int mousex,mousey;	// mouse position in pixels
	int winx,winy;		// Xemu's window position in pixels
	int sizex,sizey;	// Xemu's window size in pixels
	FILE *debug_fp;		// DEBUG file's FILE* pointer, can be zero if not used!
	int screenx,screeny;	// screen size in pixels
};

