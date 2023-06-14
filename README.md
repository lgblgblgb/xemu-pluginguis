# Plugin-GUI for Xemu

This project tries to document, demonstrate and collect Plugin-GUIs for the Xemu project. The intent here is to write GUIs for Xemu which is not inside the source tree of Xemu, so there is no limitation in used programming languages, more "exotic" systems and so on. Still, the built-in GUIs in Xemu will remain and considered as the "mainstream" ones. The other intent here is to try to rationalite how Xemu's GUI work, since a different approach to use external "plugins" may reveal some problems I can fix then.

Currently, this repository does not contain too much really _useful_ GUI pluigns, hopefully it can change in the future.

**Xemu Plugin-GUI API documentation**: https://github.com/lgblgblgb/xemu-pluginguis/wiki/PluginGuiAPI

**About my Xemu project**: https://github.com/lgblgblgb/xemu

### How to use?

Create or compile one of the plugins in the `plugingui-*` directories (ie `cd ...` into the directory and say `make`), then copy the `plugingui.so` file into the "preferences directory" of your Xemu installation. Then, you need to use the command line parameter `-gui plugin` to use the plugin-GUI. If you want to make this choice permanent, you can put a `gui = plugin` line into your Xemu configuration file, so you don't need to use a command line parameter every time.

You need some decent SDL2 development library installed, though it's for its headers, the plugin itself won't be linked against the SDL2 library (surely if you want call an SDL function from your plugin, it's a different story, but it's not so much recommended at all). Surely, depending on the plugin in question you need some other libraries installed, like the FLTK development stuff if we're speaking about the FLTK plugin-GUI, and so on.

### Create your own!

Find some `plugingui-*` implementation which seems to be similar to your project (ie if C++ you may want some C++ one). Copy the whole directory to some other `plugingui-...` name. Customize the `Makefile` and the source file (or write your own), etc etc ... Surely you may need to refer the API documentation mentioned above. Test with Xemu. Makefile has the `test` target (ie: `make test`) so if you set `XEMU_BIN` to the path of your Xemu emulator, and `TEST_PATH` for the path of `plugingui.so` in your Xemu preferences directory, then a simple `make test` will copy your plugin and also start Xemu's emulator. Meanwhile the output if `grep`-ed to see only messages containing hopefully only the most relevant info for these kind of testings.

## Existing plugins:

### Directory: plugingui-gtk

Though Xemu has a built-in GTK GUI, this is very quick hack to try the GTK GUI as form of a plugin. The purpose is more to test if this plugin-GUI API works at with some code known to be working built-in already. Please **do not use** this, it's only for testing and really bad idea for every day use! Written in C. For compilation you need the development libraries and headers for GTK+3.

### Directory: plugingui-gtk4

Experimental GTK GUI version using GTK4 instead of GTK3 (which is the built-in one in Xemu, also here as "plugingui-gtk"). I'm planning to integrate this into Xemu at some point, so both of GTK version 3 or 4 can be used. Eventually the world will migrate to GTK3 anyway, so it's better to be ready ...

### Directory: plugingui-fltk

FLTK based plugin-GUI for Xemu. Work in progress. Written in C++. For compilation you need the development libraries and headers for FLTK (1.3). A kind of demonstration plugin only, this is my FIRST time I've ever try to use FLTK also C++. Work in progress, missing menus, file chooser :-/

### Directory: plugingui-qt

QT5 based plugin-GUI for Xemu. Work in progress. Written in C++.
