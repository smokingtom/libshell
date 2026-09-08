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
#include "EventQueue.h"
#include <unistd.h>

static void dbg(const char *prefix, const uEvent *ev)
{
    printf(
        "\n[%s]\n"
        "  xwindow    : 0x%lx\n"
        "  title      : \"%s\"\n"
        "  position   : (%d,%d)\n"
        "  size       : %dx%d\n"
        "  mapped     : %s\n"
        "  focused    : %s\n",
        prefix,
        (unsigned long)ev->window.xwindow,
        ev->window.name,
        ev->window.x,
        ev->window.y,
        ev->window.w,
        ev->window.h,
        ev->window.is_mapped ? "yes" : "no",
        ev->window.is_focused ? "yes" : "no"
    );
}



 /*
int main(void)
{
    init_queue("/windowwatcher", 0);
    while(1)
    {
        uEvent ev = receive_event();
        
        if(ev.type == 0)
        {
            continue;
        }
        switch (ev.type)
        {
            case WINDOW_CREATED:
                dbg("WINDOW_CREATED", &ev);
                break;

            case WINDOW_DESTROYED:
                dbg("WINDOW_DESTROYED", &ev);
                break;

            case WINDOW_UPDATED:
                dbg("WINDOW_UPDATED", &ev);
                break;

            default:
                printf("Unknown event type: %d\n", ev.type);
                break;
        }
    }
    return 0;
}
*/