//
// Created by elias on 01/07/2019.
//



#ifndef F12018_PARSER_PACKETHEADER_H
#define F12018_PARSER_PACKETHEADER_H

#include <cstdint>
#include <string>

struct PacketType{
    static const uint8_t MOTION_PACKET = 0,
    SESSION_PACKET = 1,
    LAP_DATA_PACKET = 2,
    EVENT_PACKET = 3,
    PARTICIPANTS_PACKET = 4,
    CAR_SETUPS_PACKET = 5,
    CAR_TELEMETRY_PACKET = 6,
    CAR_STATUS = 7;
};

struct __attribute__ ((packed)) PacketHeader
{
    uint16_t    m_packetFormat;         // 2018
    uint8_t     m_packetVersion;        // Version of this packet type, all start from 1
    uint8_t    m_packetId;             // Identifier for the packet type, see below
    uint64_t    m_sessionUID;           // Unique identifier for the session
    float     m_sessionTime;          // Session timestamp
    uint   m_frameIdentifier;      // Identifier for the frame the data was retrieved on
    uint8_t     m_playerCarIndex;       // Index of player's car in the array
};


#endif //F12018_PARSER_PACKETHEADER_H
