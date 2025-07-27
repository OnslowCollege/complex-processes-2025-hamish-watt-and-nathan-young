#pragma once
#include "./screen.h"

struct MsgFlags
{
    long mousemoved;
    long scaled;
    long moved;
    long windowmoved;
};

/* These values cannot overlap in binary so that they can be combined with a  bitwise OR */
enum VWndMsg
{
    SCALED = 0b00000001,
    MOUSEMOVED = 0b00000010,
    MOVED = 0b00000100,
    WINDOWMOVED = 0b00001000,
};

int handlevwndmessages(struct VScreen *vscreen);
void sendvwndevent(struct VScreen *vscreen, VWNDIDX vwndidx, enum VWndMsg msg, long param);
void sendglobalevent(struct VScreen *vscreen, enum VWndMsg msg, long param);
void removeevent(struct VScreen *vscreen, enum VWndMsg msg);
void initgea();
