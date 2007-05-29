FLAGS=-Wall -g
GTK_FLAGS=`pkg-config --cflags --libs gtk+-2.0`
CARBON_FLAGS=-framework Carbon

all: test-menu

test-menu: test-menu.c sync-menu.c
	gcc $(FLAGS) $(GTK_FLAGS) $(CARBON_FLAGS) test-menu.c sync-menu.c -o $@

