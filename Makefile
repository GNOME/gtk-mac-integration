FLAGS=-Wall -g
GTK_FLAGS=`pkg-config --cflags --libs gtk+-2.0`
CARBON_FLAGS=-framework Carbon

all: test-menu

test-menu: test-menu.c ige-mac-menu.c ige-mac-menu.h
	gcc $(FLAGS) $(GTK_FLAGS) $(CARBON_FLAGS) test-menu.c ige-mac-menu.c -o $@

clean:
	rm -f test-menu
