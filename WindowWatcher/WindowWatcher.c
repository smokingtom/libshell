#include "WindowWatcher.h"
#include "EventQueue.h"
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xatom.h>
#include <string.h>
#include <X11/Xutil.h>
#include <unistd.h>
//#define DEBUG

static WindowRegistry reg = {0};
Display* d = NULL;
Window root = None;

Atom WM_STATE;
Atom NET_WM_STATE;
Atom NET_WM_STATE_SKIP_TASKBAR;

Atom NET_WM_WINDOW_TYPE;
Atom NET_WM_WINDOW_TYPE_DESKTOP;
Atom NET_WM_WINDOW_TYPE_DOCK;
Atom NET_WM_WINDOW_TYPE_POPUP_MENU;
Atom NET_WM_WINDOW_TYPE_MENU;
Atom NET_WM_WINDOW_TYPE_TOOLBAR;
Atom NET_WM_WINDOW_TYPE_SPLASH;
Atom NET_WM_WINDOW_TYPE_NOTIFICATION;

Atom WM_NAME;
Atom NET_WM_NAME;
Atom NET_CLIENT_LIST;
Atom NET_ACTIVE_WINDOW;

void sync()
{
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *data = NULL;

    if (XGetWindowProperty(
            d,
            root,
            NET_CLIENT_LIST,
            0,
            (~0L),
            False,
            XA_WINDOW,
            &actual_type,
            &actual_format,
            &nitems,
            &bytes_after,
            &data) != Success)
    {
        return;
    }

    if (!data)
        return;

    Window *clients = (Window *)data;
    WindowNode *cur = reg.head;

    while (cur)
    {
        WindowNode *next = cur->next;

        Bool exists = False;

        for (unsigned long i = 0; i < nitems; i++)
        {
            if (clients[i] == cur->xwindow)
            {
                exists = True;
                break;
            }
        }

        if (!exists)
        {
            uEvent ev = {
                .type = WINDOW_DESTROYED
            };

            ev.window.xwindow = cur->xwindow;

            send_event(ev);
            reg_remove(cur->xwindow);
        }

        cur = next;
    }
    for (unsigned long i = 0; i < nitems; i++)
    {
        Window w = clients[i];

        if (!relevant(w))
            continue;

        if (find(w))
            continue;

        add(w);

        uEvent ev = {
            .type = WINDOW_CREATED
        };

        ev.window = query(w);
        send_event(ev);
    }


    XFree(data);
}

int x_err(Display *display, XErrorEvent *e)
{
#ifdef DEBUG
    printf("[DEBUG] X error: code=%d request=%d minor=%d resource=%lu\n",
           e->error_code,
           e->request_code,
           e->minor_code,
           e->resourceid);


#endif
    return 0;
}

void x_events()
{
    XEvent ev;

    while (1)
    {
        XNextEvent(d, &ev);
        #ifdef DEBUG
            printf("EVENT TYPE: %d\n", ev.type);
        #endif
        switch(ev.type)
        {
        case GenericEvent:
            continue;
        case PropertyNotify:
        {
            Window w = ev.xproperty.window;
            Atom a = ev.xproperty.atom;

            if (w == root && a == NET_CLIENT_LIST)
            {
                sync();
                break;
            }

            if (find(w))
            {
                if (a == NET_WM_NAME ||
                    a == WM_NAME ||
                    a == NET_WM_STATE)
                {
                    uEvent out = {
                        .type = WINDOW_UPDATED
                    };

                    out.window.xwindow = w;
                    name(w, out.window.name);
                    send_event(out);
                }
            }

            break;
        }
            case DestroyNotify:
            case MapNotify:
            case UnmapNotify:
            {
                sync();
                break;
            }


            default:
                break;
        }
    }
}

void setup_atoms(void)
{
    NET_ACTIVE_WINDOW = XInternAtom(d, "_NET_ACTIVE_WINDOW", False);
    WM_STATE = XInternAtom(d, "WM_STATE", False);
    WM_NAME  = XInternAtom(d, "WM_NAME", False);

    NET_CLIENT_LIST = XInternAtom(d, "_NET_CLIENT_LIST", False);

    NET_WM_NAME = XInternAtom(d, "_NET_WM_NAME", False);
    NET_WM_STATE = XInternAtom(d, "_NET_WM_STATE", False);
    NET_WM_STATE_SKIP_TASKBAR =
        XInternAtom(d, "_NET_WM_STATE_SKIP_TASKBAR", False);

    NET_WM_WINDOW_TYPE =
        XInternAtom(d, "_NET_WM_WINDOW_TYPE", False);

    NET_WM_WINDOW_TYPE_DESKTOP =
        XInternAtom(d, "_NET_WM_WINDOW_TYPE_DESKTOP", False);

    NET_WM_WINDOW_TYPE_DOCK =
        XInternAtom(d, "_NET_WM_WINDOW_TYPE_DOCK", False);

    NET_WM_WINDOW_TYPE_POPUP_MENU =
        XInternAtom(d, "_NET_WM_WINDOW_TYPE_POPUP_MENU", False);

    NET_WM_WINDOW_TYPE_MENU =
        XInternAtom(d, "_NET_WM_WINDOW_TYPE_MENU", False);

    NET_WM_WINDOW_TYPE_TOOLBAR =
        XInternAtom(d, "_NET_WM_WINDOW_TYPE_TOOLBAR", False);

    NET_WM_WINDOW_TYPE_SPLASH =
        XInternAtom(d, "_NET_WM_WINDOW_TYPE_SPLASH", False);

    NET_WM_WINDOW_TYPE_NOTIFICATION =
        XInternAtom(d, "_NET_WM_WINDOW_TYPE_NOTIFICATION", False);
}

void setup_x11()
{
    XSetErrorHandler(x_err);
    d = XOpenDisplay(NULL);
    if(!d)
    {
        printf("Failed to open Display Connection with X11 Server\n");
        exit(-1);
    }
    setup_atoms();
    root = DefaultRootWindow(d);
    if(root == None)
    {
        printf("Failed to fetch root Window");
        exit(-1);
    }
    XSelectInput(d, root, SubstructureNotifyMask | PropertyChangeMask);
    XSync(d, False);
}

Bool relevant(Window w)
{
    if(w == root || w == None) return False;
    XWindowAttributes xwa;
    if (!XGetWindowAttributes(d, w, &xwa)) return False;
    if(xwa.override_redirect) return False;

    Window transient_for = None;
    if(XGetTransientForHint(d, w, &transient_for) && transient_for != None)
    {
        return False;
    }
    Atom type;
    int format; unsigned long nit, aft;
    unsigned char *data = NULL;        
    /*
        if (XGetWindowProperty(
                d,
                w,
                wms,
                0,
                1,
                False,
                AnyPropertyType,
                &type,
                &format,
                &nit,
                &aft,
                &data) != Success)
        {
            return False;
        }

        if (!data)
            return False;

        XFree(data);
    */
    if(NET_WM_STATE != None && NET_WM_STATE_SKIP_TASKBAR != None)
    {
        if(XGetWindowProperty(d, w, NET_WM_STATE, 0, (~0L), False, XA_ATOM, &type, &format, &nit, &aft, &data) == Success && data)
        {
            Atom *states = (Atom*) data;
            for(unsigned long i = 0; i < nit; ++i)
            {
                if(states[i] == NET_WM_STATE_SKIP_TASKBAR)
                {
                    XFree(data);
                    return False;
                }
            }
            XFree(data);
        }
    }

    if(NET_WM_WINDOW_TYPE != None && XGetWindowProperty(d, w, NET_WM_WINDOW_TYPE, 0, 1, False, XA_ATOM, 
        &type, &format, &nit, &aft, &data) == Success && data)
    {
        Atom *window_type = (Atom*) data;
        if(*window_type == NET_WM_WINDOW_TYPE_DESKTOP || *window_type == NET_WM_WINDOW_TYPE_DOCK || *window_type == NET_WM_WINDOW_TYPE_POPUP_MENU 
            || *window_type == NET_WM_WINDOW_TYPE_MENU || *window_type == NET_WM_WINDOW_TYPE_TOOLBAR 
            || *window_type == NET_WM_WINDOW_TYPE_SPLASH || *window_type == NET_WM_WINDOW_TYPE_NOTIFICATION)
        {
            XFree(data);
            return False;
        }
        XFree(data);
    }

    return True;
}

void create_reg(void)
{
    reg.head = NULL;
}

void name(Window w, char *dest) {
    Atom utf8_string = XInternAtom(d, "UTF8_STRING", False);
    Atom net_wm_name = XInternAtom(d, "_NET_WM_NAME", False);
    Atom wm_name = XInternAtom(d, "WM_NAME", False);
    Atom actual_type;
    int actual_format;
    unsigned long nitems, bytes_after;
    unsigned char *prop = NULL;
    
    Atom atoms[] = {net_wm_name, wm_name};
    
    for (int i = 0; i < 2; i++) {
        if (XGetWindowProperty(d, w, atoms[i], 0, 256, False, utf8_string,
                               &actual_type, &actual_format, &nitems, &bytes_after, &prop) == Success) {
            if (prop && nitems > 0) {
                snprintf(dest, 256, "%s", (char *)prop);
                XFree(prop);
                return;
            }
            if (prop) XFree(prop);
        }
    }
    
    dest[0] = '\0';
}


uWindow query(Window w)
{
    uWindow uw = {0};
    uw.xwindow = w;
    XWindowAttributes xwa;

    if (!XGetWindowAttributes(d, w, &xwa))
    {

        return uw;
    }   
    uw.x = xwa.x;
    uw.y = xwa.y;
    uw.w = xwa.width;
    uw.h = xwa.height;
    name(w, uw.name);
    uw.is_mapped = (xwa.map_state == IsViewable);
    return uw;

}

void destroy_reg()
{
    WindowNode* cur = reg.head;
    while(cur) {
        WindowNode* next = cur->next;
        free(cur);
        cur = next;
    }
    reg.head = NULL;
}

Bool find(Window w)
{
    WindowNode *cur = reg.head;

    while(cur)
    {
        if(cur->xwindow == w)
            return True;

        cur = cur->next;
    }

    return False;
}

void add(Window w)
{
    if (find(w))
        return;

    WindowNode *node = malloc(sizeof *node);

    if (!node)
        return;

    node->xwindow = w;

    node->next = reg.head;
    reg.head = node;

    XSelectInput(d, w, PropertyChangeMask|StructureNotifyMask);
}

void reg_remove(Window id)
{
    WindowNode* prev = NULL;
    WindowNode* cur = reg.head;

    while (cur) {
        if (cur->xwindow == id) {
            if (prev) prev->next = cur->next;
            else reg.head = cur->next;

            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}



void x_cleanup()
{
    destroy_reg();
    XCloseDisplay(d);
};

void run_ww()
{
    init_queue("/windowwatcher", 1);
    create_reg();
    setup_x11();
    sync();
    x_events();
    x_cleanup();
}

int main()
{
    mq_unlink("/windowwatcher");
    run_ww();
    mq_unlink("/windowwatcher");
    return 0;
}