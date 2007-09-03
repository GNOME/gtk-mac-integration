FLAGS=-Wall -g
GTK_FLAGS=`pkg-config --cflags --libs gtk+-2.0`
CARBON_FLAGS=-framework Carbon

all: test-menu

test-menu: test-menu.c gtk-macmenu.c gtk-macmenu.h
	gcc $(FLAGS) $(GTK_FLAGS) $(CARBON_FLAGS) test-menu.c gtk-macmenu.c -o $@

