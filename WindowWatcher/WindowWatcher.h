/*
 * libshell - A shell library
 * Copyright (C) 2026 smokingtom
 *
 * Author: smokingtom
 * Project: https://github.com/smokingtom/libshell
 *
 * This file is part of libshell.
 *
 * libshell is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libshell is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libshell. If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef WINDOWWATCHER_H
#define WINDOWWATCHER_H

#include <X11/Xlib.h>
#include <stdint.h>

extern Atom WM_STATE;
extern Atom NET_WM_STATE;
extern Atom NET_WM_STATE_SKIP_TASKBAR;
extern Atom NET_WM_WINDOW_TYPE;
extern Atom NET_WM_WINDOW_TYPE_DESKTOP;
extern Atom NET_WM_WINDOW_TYPE_DOCK;
extern Atom NET_WM_WINDOW_TYPE_POPUP_MENU;
extern Atom NET_WM_WINDOW_TYPE_MENU;
extern Atom NET_WM_WINDOW_TYPE_TOOLBAR;
extern Atom NET_WM_WINDOW_TYPE_SPLASH;
extern Atom NET_WM_WINDOW_TYPE_NOTIFICATION;
extern Atom WM_NAME;
extern Atom NET_WM_NAME;
extern Atom NET_CLIENT_LIST;
extern Atom NET_ACTIVE_WINDOW;


typedef struct
{
    Window xwindow;

    char name[256];

    int x;
    int y;
    int w;
    int h;

    Bool is_mapped;
    Bool is_focused;

} uWindow;

typedef enum
{
    WINDOW_CREATED = 1,
    WINDOW_DESTROYED = 2,
    WINDOW_UPDATED = 3

} uEventType;

typedef struct
{
    uEventType type;

    uWindow window;

} uEvent;

typedef struct WindowNode
{
    Window xwindow;

    struct WindowNode *next;

} WindowNode;

typedef struct
{
    WindowNode *head;

} WindowRegistry;
/* registry */

void create_reg(void);

void destroy_reg(void);

Bool find(Window w);

void add(Window w);

void reg_remove(Window w);



/* window */

uWindow query(Window w);

void name(Window w, char *dest);

Bool relevant(Window w);



/* x11 */

void setup_x11(void);

void setup_atoms(void);

void x_events(void);

void x_cleanup(void);

int x_err(Display *display, XErrorEvent *error);



/* synchronization */

void sync(void);
#endif