//
// Created by elias on 01/07/2019.
//

#ifndef F12018_PARSER_LAPDATAPACKET_H
#define F12018_PARSER_LAPDATAPACKET_H

#include "PacketHeader.h"

struct pitStatus{
    static const  int NONE = 0,
    PITTING = 1,
    IN_PIT = 2;
};

struct sector{
    static const int SECTOR1 = 0,
    SECTOR2 = 1,
    SECTOR3 = 2;
};

struct lapValid{
    static const int VALID = 0,
    INVALID = 1;
};

struct driverStatus{
    static const int GARAGE = 0,
    FLYING_LAP = 1,
    IN_LAP = 2,
    OUT_LAP = 3,
    ON_TRACK = 4;
};

struct resultStatus{
    static const int INVALID = 0,
    INACTIVE = 1,
    ACTIVE = 2,
    FINISHED = 3,
    DISQUALIFIED = 4,
    NOT_CLASSIFIED = 5,
    RETIRED = 6;
};

struct __attribute__ ((packed)) LapData
{
    float       m_lastLapTime;           // Last lap time in seconds
    float       m_currentLapTime;        // Current time around the lap in seconds
    float       m_bestLapTime;           // Best lap time of the session in seconds
    float       m_sector1Time;           // Sector 1 time in seconds
    float       m_sector2Time;           // Sector 2 time in seconds
    float       m_lapDistance;           // Distance vehicle is around current lap in metres – could
    // be negative if line hasn’t been crossed yet
    float       m_totalDistance;         // Total distance travelled in session in metres – could
    // be negative if line hasn’t been crossed yet
    float       m_safetyCarDelta;        // Delta in seconds for safety car
    uint8_t       m_carPosition;           // Car race position
    uint8_t       m_currentLapNum;         // Current lap number
    uint8_t       m_pitStatus;             // 0 = none, 1 = pitting, 2 = in pit area
    uint8_t       m_sector;                // 0 = sector1, 1 = sector2, 2 = sector3
    uint8_t       m_currentLapInvalid;     // Current lap invalid - 0 = valid, 1 = invalid
    uint8_t       m_penalties;             // Accumulated time penalties in seconds to be added
    uint8_t       m_gridPosition;          // Grid position the vehicle started the race in
    uint8_t       m_driverStatus;          // Status of driver - 0 = in garage, 1 = flying lap
    // 2 = in lap, 3 = out lap, 4 = on track
    uint8_t       m_resultStatus;          // Result status - 0 = invalid, 1 = inactive, 2 = active
    // 3 = finished, 4 = disqualified, 5 = not classified
    // 6 = retired
};

struct __attribute__ ((packed)) PacketLapData
{
    PacketHeader    m_header;              // Header

    LapData         m_lapData[20];         // Lap data for all cars on track
};

#endif //F12018_PARSER_LAPDATAPACKET_H
