//
// Created by eliwas on 14.10.2019.
//

#ifndef WEB_SERV_TEST_F1_STORAGE_H
#define WEB_SERV_TEST_F1_STORAGE_H

#include "../shared_header.h"

#include "PacketHeader.h"
#include "CarSetupsPacket.h"
#include "CarStatusPacket.h"
#include "CarTelemetryPacket.h"
#include "EventPacket.h"
#include "LapDataPacket.h"
#include "MotionPacket.h"
#include "ParticipantsPacket.h"
#include "SessionPacket.h"

using namespace std;

Poco::JSON::Object toJSON(PacketHeader pack);

Poco::JSON::Object toJSON(PacketCarSetupData pack);

Poco::JSON::Object toJSON(PacketCarStatusData pack);

Poco::JSON::Object toJSON(PacketCarTelemetryData pack);

Poco::JSON::Object toJSON(PacketEventData pack);

Poco::JSON::Object toJSON(PacketLapData pack);

Poco::JSON::Object toJSON(PacketMotionData pack);

Poco::JSON::Object toJSON(PacketParticipantsData pack);

Poco::JSON::Object toJSON(PacketSessionData pack);

typedef Poco::FastMutex _mutex;



struct f1_storage {
    _mutex Header_mut;
    PacketHeader Header = {0,};

    _mutex CarSetups_mut;
    PacketCarSetupData CarSetupData = {0,};

    _mutex CarStatus_mut;
    PacketCarStatusData CarStatusData = {0,};

    _mutex CarTelemetry_mut;
    PacketCarTelemetryData CarTelemetryData = {0,};

    _mutex Event_mut;
    PacketEventData EventData = {0,};

    _mutex LapData_mut;
    PacketLapData LapData = {0,};

    _mutex Motion_mut;
    PacketMotionData MotionData = {0,};

    _mutex Participants_mut;
    PacketParticipantsData ParticipantsData = {0,};

    _mutex Session_mut;
    PacketSessionData SessionData = {0,};

    _mutex timing_mut;
    std::vector<float> driver_diffs;

#if DEBUG

#endif

};

#endif //WEB_SERV_TEST_F1_STORAGE_H
