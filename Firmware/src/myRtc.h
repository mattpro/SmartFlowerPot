#ifndef _MYRTC_H
#define _MYRTC_H

#include <stdint.h>

void resetRtcCounter( uint32_t time );
void lfclk_config( void );
void rtcInit( void );

#endif
