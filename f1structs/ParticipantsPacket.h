//
// Created by elias on 01/07/2019.
//

#ifndef F12018_PARSER_PARTICIPANTSPACKET_H
#define F12018_PARSER_PARTICIPANTSPACKET_H

#include "PacketHeader.h"
#include <string>

struct aiControlled{
    static const int HUMAN = 0,
    AI = 1;
};


struct TeamIDs{
    static const std::string array(int index){
        if(index < 0 || index > 40){
            return "index out of range";
        }
        if(index >22 && index <34){
            return "undefined";
        }
        if(index >= 34) {
            index = index-11;
        }
        static const std::string team_names[30] = {"Mercedes",
                                                   "Ferrari",
                                                   "Red Bull",
                                                   "Williams",
                                                   "Force India",
                                                   "Renault",
                                                   "Toro Rosso",
                                                   "Haas",
                                                   "McLaren",
                                                   "Sauber",
                                                   "McLaren 1988",
                                                   "McLaren 1991",
                                                   "Williams 1992",
                                                   "Ferrari 1995",
                                                   "Williams 1996",
                                                   "McLaren 1998",
                                                   "Ferrari 2002",
                                                   "Ferrari 2004",
                                                   "Renault 2006",
                                                   "Ferrari 2007",
                                                   "McLaren 2008",
                                                   "Red Bull 2010",
                                                   "Ferrari 1976",
                                                   "McLaren 1976",
                                                   "Lotus 1972",
                                                   "Ferrari 1979",
                                                   "McLaren 1982",
                                                   "Williams 2003",
                                                   "Brawn 2009",
                                                   "Lotus 1978"};
        return team_names[index];
    }

    std::string operator[](int index){
        return array(index);

    }

};



//TODO: add driverID

//TODO: add nationality

struct __attribute__ ((packed)) ParticipantData
{
    uint8_t      m_aiControlled;           // Whether the vehicle is AI (1) or Human (0) controlled
    uint8_t      m_driverId;               // Driver id - see appendix
    uint8_t      m_teamId;                 // Team id - see appendix
    uint8_t      m_raceNumber;             // Race number of the car
    uint8_t      m_nationality;            // Nationality of the driver
    char       m_name[48];               // Name of participant in UTF-8 format – null terminated
    // Will be truncated with … (U+2026) if too long
};
struct __attribute__ ((packed)) PacketParticipantsData
{
    PacketHeader    m_header;            // Header

    uint8_t           m_numCars;           // Number of cars in the data
    ParticipantData m_participants[20];
};


#endif //F12018_PARSER_PARTICIPANTSPACKET_H
