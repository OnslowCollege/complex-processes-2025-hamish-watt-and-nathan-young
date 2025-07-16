#pragma once
#include "./screen.h"

int handlevwndmessages(struct VScreen *vscreen);
void sendvwndevent(struct VScreen *vscreen, VWNDIDX vwndidx, enum VWndMsg msg, long param);
void sendglobalevent(struct VScreen *vscreen, enum VWndMsg msg, long param);
void removeevent(struct VScreen *vscreen, enum VWndMsg msg);
