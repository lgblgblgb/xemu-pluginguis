XEMU_SUBDIR = xemu-dev

all:
	awk '/__PLUGIN_EXTRACT_INFO_ST__/ { if(grab) exit; grab=1;next } grab == 1 { print }' < ../../$(XEMU_SUBDIR)/xemu/gui/gui_plugin.c > info_struct_definition.tmp
	mv info_struct_definition.tmp info_struct_definition.h

clean:
	rm -f info_struct_definition.tmp

.PHONY: all clean
