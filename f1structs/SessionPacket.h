//
// Created by elias on 01/07/2019.
//

#ifndef F12018_PARSER_SESSIONPACKET_H
#define F12018_PARSER_SESSIONPACKET_H

#include <cstdint>
#include "PacketHeader.h"

struct zoneFlag{
    static const int UNKNOWN = -1,
    NONE = 0,
    GREEN = 1,
    BLUE = 2,
    YELLOW = 3,
    RED = 4;
};

struct weather{
    static const int CLEAR = 0,
    LIGHT_CLOUDY = 1,
    OVERCAST = 2,
    LIGHT_RAIN = 3,
    HEAVY_RAIN = 4,
    STORM = 5;
};

struct sessionType{
    static const int UNKNOWN = 0,
    P1 = 1,
    P2 = 2,
    P3 = 3,
    SHORT_P = 4,
    Q1 = 5,
    Q2 = 6,
    Q3 = 7,
    SHORT_Q = 8,
    OSQ = 9,
    R = 10,
    R2 = 11,
    TIME_TRIAL = 12;
};

struct TrackIDs{
    static const std::string array(int index){
        if(index < 0 || index > 24){
            return "index out of range";
        }
        static const std::string track_names[25] = {"Melbourne",
                                                   "Paul Ricard",
                                                   "Shanghai",
                                                   "Sakhir (Bahrain)",
                                                   "Catalunya",
                                                   "Monaco",
                                                   "Montreal",
                                                   "Silverstone",
                                                   "Hockenheim",
                                                   "Hungaroring",
                                                   "Spa",
                                                   "Monza",
                                                   "Singapore",
                                                   "Suzuka",
                                                   "Abu Dhabi",
                                                   "Texas",
                                                   "Brazil",
                                                   "Austria",
                                                   "Sochi",
                                                   "Mexico",
                                                   "Baku (Azerbaijan)",
                                                   "Sakhir Short",
                                                   "Silverstone Short",
                                                   "Texas Short",
                                                   "Suzuka Short",};
        return track_names[index];
    }

    std::string operator[](int index){
        return array(index);

    }

};


struct era{
    static const int MODERN = 0,
    CLASSIC = 1;
};

struct sli_pro_support{
    static const int INACTIVE = 0,
    ACTIVE = 1;
};

struct safetyCar{
    static const int NONE = 0,
    FULL_SC = 1,
    VIRTUAL_SC = 2;
};

struct networkGame{
    static const int OFFLINE = 0,
    ONLINE = 1;
};

struct __attribute__ ((packed)) MarshalZone
{
    float  m_zoneStart;   // Fraction (0..1) of way through the lap the marshal zone starts
    int8_t   m_zoneFlag;    // -1 = invalid/unknown, 0 = none, 1 = green, 2 = blue, 3 = yellow, 4 = red
};

struct __attribute__ ((packed)) PacketSessionData
{
    PacketHeader    m_header;               	// Header

    uint8_t           m_weather;              	// Weather - 0 = clear, 1 = light cloud, 2 = overcast
    // 3 = light rain, 4 = heavy rain, 5 = storm
    int8_t	    m_trackTemperature;    	// Track temp. in degrees celsius
    int8_t	    m_airTemperature;      	// Air temp. in degrees celsius
    uint8_t           m_totalLaps;           	// Total number of laps in this race
    uint16_t          m_trackLength;           	// Track length in metres
    uint8_t           m_sessionType;         	// 0 = unknown, 1 = P1, 2 = P2, 3 = P3, 4 = Short P
    // 5 = Q1, 6 = Q2, 7 = Q3, 8 = Short Q, 9 = OSQ
    // 10 = R, 11 = R2, 12 = Time Trial
    int8_t            m_trackId;         		// -1 for unknown, 0-21 for tracks, see appendix
    uint8_t           m_era;                  	// Era, 0 = modern, 1 = classic
    uint16_t          m_sessionTimeLeft;    	// Time left in session in seconds
    uint16_t          m_sessionDuration;     	// Session duration in seconds
    uint8_t           m_pitSpeedLimit;      	// Pit speed limit in kilometres per hour
    uint8_t           m_gamePaused;               // Whether the game is paused
    uint8_t           m_isSpectating;        	// Whether the player is spectating
    uint8_t           m_spectatorCarIndex;  	// Index of the car being spectated
    uint8_t           m_sliProNativeSupport;	// SLI Pro support, 0 = inactive, 1 = active
    uint8_t           m_numMarshalZones;         	// Number of marshal zones to follow
    MarshalZone     m_marshalZones[21];         // List of marshal zones – max 21
    uint8_t           m_safetyCarStatus;          // 0 = no safety car, 1 = full safety car
    // 2 = virtual safety car
    uint8_t          m_networkGame;              // 0 = offline, 1 = online
};


#endif //F12018_PARSER_SESSIONPACKET_H
