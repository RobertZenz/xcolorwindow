#include <stdlib.h>
#include <X11/Xlib.h>

#include "vroot.h"


int main(int argc, char *argv[]) {
	int color = 0x00FF00;

	Display *display = XOpenDisplay(getenv("DISPLAY"));
	int screen = DefaultScreen(display);
	
	Window window = XCreateSimpleWindow(
		display, 
		RootWindow(display, screen),
		24,	48,
		800, 640,
		1, BlackPixel(display, screen), WhitePixel(display, screen));
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
				XSetBackground(display, graphics, color);
				XSetForeground(display, graphics, color);
				XFillRectangle(
					display, window, graphics,
					0, 0,
					attributes.width, attributes.height);
				XFlush(display);
				break;

			case ResizeRequest:
				XGetWindowAttributes(display, window, &attributes);
				break;				
		}
	}
	
	XDestroyWindow(display, window);
	XCloseDisplay(display);	
	
	return 0;
}

