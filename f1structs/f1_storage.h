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

typedef Poco::FastMutex mutex;



struct f1_storage {
    mutex Header_mut;
    PacketHeader Header;

    mutex CarSetups_mut;
    PacketCarSetupData CarSetupData;

    mutex CarStatus_mut;
    PacketCarStatusData CarStatusData;

    mutex CarTelemetry_mut;
    PacketCarTelemetryData CarTelemetryData;

    mutex Event_mut;
    PacketEventData EventData;

    mutex LapData_mut;
    PacketLapData LapData;

    mutex Motion_mut;
    PacketMotionData MotionData;

    mutex Participants_mut;
    PacketParticipantsData ParticipantsData;

    mutex Session_mut;
    PacketSessionData SessionData;

};


#endif //WEB_SERV_TEST_F1_STORAGE_H
