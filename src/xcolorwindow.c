/*
 * Licensed under Public Domain or CC0.
 */

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>

#include "vroot.h"


int main(int argc, char *argv[]) {
	int color = 0x000000;
	int height = 600;
	char *title = "xcolorwindow";
	int width = 800;
	int x = 32;
	int y = 32;

	int c;
	while((c = getopt(argc, argv, "c:h:t:w:x:y:")) != -1) {
		switch (c) {
			case 'c':
				color = atoi(optarg);
				break;

			case 'h':
				height = atoi(optarg);
				break;

			case 't':
				title = strdup(optarg);
				break;

			case 'w':
				width = atoi(optarg);
				break;

			case 'x':
				x = atoi(optarg);
				break;

			case 'y':
				y = atoi(optarg);
				break;

		}
	}

	Display *display = XOpenDisplay(getenv("DISPLAY"));
	int screen = DefaultScreen(display);

	Window window = XCreateSimpleWindow(
	                    display,
	                    RootWindow(display, screen),
	                    x, y,
	                    width, height,
	                    1, BlackPixel(display, screen), WhitePixel(display, screen));
	XStoreName(display, window, title);
	XMapWindow(display, window);

	// Fetch the initial set of window attributes.
	XWindowAttributes attributes;
	XGetWindowAttributes(display, window, &attributes);

	GC graphics = XCreateGC(display, window, 0, NULL);

	// Register for the WM_DELETE_WINDOW message.
	Atom WM_DELETE_WINDOW = XInternAtom(display, "WM_DELETE_WINDOW", 0);
	XSetWMProtocols(display, window, &WM_DELETE_WINDOW, 1);

	// Register for the Exposure event
	XSelectInput(display, window, ExposureMask);

	XEvent event;

	// Initial drawing.
	XSetBackground(display, graphics, color);
	XSetForeground(display, graphics, color);
	XFillRectangle(
	    display, window, graphics,
	    0, 0,
	    attributes.width, attributes.height);
	XFlush(display);

	int running = 1;
	while(running) {
		XNextEvent(display, &event);
		switch (event.type) {
			case ClientMessage:
				// Safely ignore anything here.
				// We're only subscribed for WM_DELETE_WINDOW
				// so we should only receive that message.
			case DestroyNotify:
				running = 0;
				break;

			case Expose:
			case GraphicsExpose:
			case NoExpose:
				XGetWindowAttributes(display, window, &attributes);
				XSetBackground(display, graphics, color);
				XSetForeground(display, graphics, color);
				XFillRectangle(
				    display, window, graphics,
				    0, 0,
				    attributes.width, attributes.height);
				XFlush(display);
				break;
		}
	}

	XDestroyWindow(display, window);
	XCloseDisplay(display);

	return 0;
}

