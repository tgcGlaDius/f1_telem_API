//
// Created by elias on 01/07/2019.
//

#ifndef F12018_PARSER_EVENTPACKET_H
#define F12018_PARSER_EVENTPACKET_H

#include "PacketHeader.h"
#include <cstring>


struct __attribute__ ((packed)) PacketEventData
{
    PacketHeader    m_header;               // Header

    char           m_eventStringCode[4];   // Event string code, see above
};

#endif //F12018_PARSER_EVENTPACKET_H
