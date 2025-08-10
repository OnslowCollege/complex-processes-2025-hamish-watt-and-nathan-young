#pragma once
#include "./screen.h"

#define VWNDMSG enum VWndMsg

typedef struct
{
    long mousemoved;
    long scaled;
    long moved;
    long windowmoved;
    long mouseclicked;
} MsgFlags;

/* These values cannot overlap in binary so that they can be combined with a bitwise OR */
enum VWndMsg
{
    SCALED = 0b00000001,
    MOUSEMOVED = 0b00000010,
    MOVED = 0b00000100,
    WINDOWMOVED = 0b00001000,
    MOUSECLICKED = 0b00010000,
    DOUBLECLICKED = 0b00100000,
};

int handlevwndmessages(VScreen *vscreen);
void sendvwndevent(VScreen *vscreen, VWNDIDX vwndidx, VWNDMSG msg, long param);
void sendglobalevent(VScreen *vscreen, VWNDMSG msg, long param);
void removeevent(VScreen *vscreen, VWNDMSG msg);
void initgea();
