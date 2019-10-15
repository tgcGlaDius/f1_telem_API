//
// Created by elias on 01/07/2019.
//

#ifndef F12018_PARSER_CARTELEMETRYPACKET_H
#define F12018_PARSER_CARTELEMETRYPACKET_H

#include "PacketHeader.h"


struct DRSStatus{
    static const int OFF = 0,
    ON = 1;
};

struct __attribute__ ((packed)) CarTelemetryData
{
    uint16_t    m_speed;                      // Speed of car in kilometres per hour
    uint8_t     m_throttle;                   // Amount of throttle applied (0 to 100)
    int8_t      m_steer;                      // Steering (-100 (full lock left) to 100 (full lock right))
    uint8_t     m_brake;                      // Amount of brake applied (0 to 100)
    uint8_t     m_clutch;                     // Amount of clutch applied (0 to 100)
    int8_t      m_gear;                       // Gear selected (1-8, N=0, R=-1)
    uint16_t    m_engineRPM;                  // Engine RPM
    uint8_t     m_drs;                        // 0 = off, 1 = on
    uint8_t     m_revLightsPercent;           // Rev lights indicator (percentage)
    uint16_t    m_brakesTemperature[4];       // Brakes temperature (celsius)
    uint16_t    m_tyresSurfaceTemperature[4]; // Tyres surface temperature (celsius)
    uint16_t    m_tyresInnerTemperature[4];   // Tyres inner temperature (celsius)
    uint16_t    m_engineTemperature;          // Engine temperature (celsius)
    float     m_tyresPressure[4];           // Tyres pressure (PSI)
};


struct __attribute__ ((packed)) PacketCarTelemetryData
{
    PacketHeader        m_header;                // Header

    CarTelemetryData    m_carTelemetryData[20];

    uint32_t              m_buttonStatus;         // Bit flags specifying which buttons are being
    // pressed currently - see appendices
};

struct ButtonFlag{
    static const uint32_t A = 0x0001,
            Y = 0x0002,
            B = 0x0004,
            X = 0x0008,
            D_pad_Left = 0x0010,
            D_pad_Right = 0x0020,
            D_pad_Up = 0x0040,
            D_pad_Down = 0x0080,
            Menu_Button = 0x0100,
            LB = 0x0200,
            RB = 0x0400,
            LT = 0x0800,
            RT = 0x1000,
            Left_Stick_Click = 0x2000,
            Right_Stick_Click = 0x4000;
    static bool isClicked(const uint32_t &button, const PacketCarTelemetryData &telemetryData){
        return (telemetryData.m_buttonStatus & button) == button;
    }

};

#endif //F12018_PARSER_CARTELEMETRYPACKET_H
