CFLAGS = -Wall
INCLUDES = -I/usr/X11R6/include
LFLAGS = -lX11
MAIN = xcolorwindow


all: $(MAIN)

$(MAIN): src/$(MAIN).c
	$(CC) $< $(CFLAGS) $(INCLUDES) $(LFLAGS) -o $@

format:
	astyle --indent=tab --indent-switches src/$(MAIN).c

clean:
	$(RM) $(MAIN)

