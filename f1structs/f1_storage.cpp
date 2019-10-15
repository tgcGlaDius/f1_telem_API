//
// Created by eliwas on 14.10.2019.
//

#include "f1_storage.h"

Poco::JSON::Object toJSON(PacketHeader pack){
    Poco::JSON::Object base;
    base.set("Format", to_string(pack.m_packetFormat));
    base.set("Version", to_string(pack.m_packetVersion));
    base.set("packetId", to_string(pack.m_packetId));
    base.set("sessionUID", to_string(pack.m_sessionUID));
    base.set("sessionTime", to_string(pack.m_sessionTime));
    base.set("frameIdentifier", to_string(pack.m_frameIdentifier));
    base.set("playerCarIndex", to_string(pack.m_playerCarIndex));
    return base;
}

Poco::JSON::Object toJSON(PacketCarSetupData pack){
    Poco::JSON::Object base;
    base.set("header", toJSON(pack.m_header));
    Poco::JSON::Array car_setups;
    for(auto & car : pack.m_carSetups){
        Poco::JSON::Object CarSetup;

        CarSetup.set("frontWing", car.m_frontWing);
        CarSetup.set("rearWing", car.m_rearWing);
        CarSetup.set("onThrottle", car.m_onThrottle);
        CarSetup.set("offThrottle", car.m_offThrottle);
        CarSetup.set("frontCamber", car.m_frontCamber);
        CarSetup.set("rearCamber", car.m_rearCamber);
        CarSetup.set("frontToe", car.m_frontToe);
        CarSetup.set("rearToe", car.m_rearToe);
        CarSetup.set("frontSuspension", car.m_frontSuspension);
        CarSetup.set("rearSuspension", car.m_rearSuspension);
        CarSetup.set("frontAntiRollBar", car.m_frontAntiRollBar);
        CarSetup.set("rearAntiRollBar", car.m_rearAntiRollBar);
        CarSetup.set("frontSuspensionHeight", car.m_frontSuspensionHeight);
        CarSetup.set("rearSuspensionHeight", car.m_rearSuspensionHeight);
        CarSetup.set("brakePressure", car.m_brakePressure);
        CarSetup.set("brakeBias", car.m_brakeBias);
        CarSetup.set("frontTyrePressure", car.m_frontTyrePressure);
        CarSetup.set("rearTyrePressure", car.m_rearTyrePressure);
        CarSetup.set("ballast", car.m_ballast);
        CarSetup.set("fuelLoad", car.m_fuelLoad);

        car_setups.add( CarSetup);
    }
    base.set("setups", car_setups);

    return base;
}

Poco::JSON::Object toJSON(PacketCarStatusData pack){
    Poco::JSON::Object base;
    base.set("header", toJSON(pack.m_header));

    Poco::JSON::Array arr;
    for(auto & dat : pack.m_carStatusData){
        Poco::JSON::Object car;

        car.set("tractionControl", dat.m_tractionControl);
        car.set("antiLockBrakes", dat.m_antiLockBrakes);
        car.set("fuelMix", dat.m_fuelMix);
        car.set("frontBrakeBias", dat.m_frontBrakeBias);
        car.set("pitLimiterStatus", dat.m_pitLimiterStatus);
        car.set("fuelInTank", dat.m_fuelInTank);
        car.set("fuelCapacity", dat.m_fuelCapacity);
        car.set("maxRPM", dat.m_maxRPM);
        car.set("idleRPM", dat.m_idleRPM);
        car.set("maxGears", dat.m_maxGears);
        car.set("drsAllowed", dat.m_drsAllowed);
        Poco::JSON::Array tyreWear;
        for(unsigned char a : dat.m_tyresWear){
            tyreWear.add( a);
        }
        car.set("tyresWear", tyreWear);
        car.set("tyreCompound", dat.m_tyreCompound);
        Poco::JSON::Array tyreDamage;
        for(unsigned char a : dat.m_tyresDamage){
            tyreDamage.add( a);
        }
        car.set("tyresDamage", tyreDamage);
        car.set("frontLeftWingDamage", dat.m_frontLeftWingDamage);
        car.set("frontRightWingDamage", dat.m_frontRightWingDamage);
        car.set("rearWingDamage", dat.m_rearWingDamage);
        car.set("engineDamage", dat.m_engineDamage);
        car.set("gearBoxDamage", dat.m_gearBoxDamage);
        car.set("exhaustDamage", dat.m_exhaustDamage);
        car.set("vehicleFiaFlags", dat.m_vehicleFiaFlags);
        car.set("ersStoreEnergy", dat.m_ersStoreEnergy);
        car.set("ersDeployMode", dat.m_ersDeployMode);
        car.set("ersHarvestedThisLapMGUK", dat.m_ersHarvestedThisLapMGUK);
        car.set("ersHarvestedThisLapMGUH", dat.m_ersHarvestedThisLapMGUH);
        car.set("ersDeployedThisLap", dat.m_ersDeployedThisLap);


        arr.add( car);
    }
    base.set("carStatusData", arr);
    return base;
}

Poco::JSON::Object toJSON(PacketCarTelemetryData pack){
    Poco::JSON::Object base;
    base.set("header", toJSON(pack.m_header));

    Poco::JSON::Array arr;
    for(auto & dat : pack.m_carTelemetryData){
        Poco::JSON::Object car;

        car.set("speed", dat.m_speed);
        car.set("throttle", dat.m_throttle);
        car.set("steer", dat.m_steer);
        car.set("brake", dat.m_brake);
        car.set("clutch", dat.m_clutch);
        car.set("gear", dat.m_gear);
        car.set("engineRPM", dat.m_engineRPM);
        car.set("drs", dat.m_drs);
        car.set("revLightsPercent", dat.m_revLightsPercent);

        Poco::JSON::Array braketemp;
        for(int j = 0; j<4; j++){
            auto a = dat.m_brakesTemperature[j];
            braketemp.add(a);
        }
        car.set("brakesTemperature", braketemp);
        Poco::JSON::Array tyresSurfaceTemp;
        for(int j = 0; j<4; j++){
            auto a = dat.m_tyresSurfaceTemperature[j];
            tyresSurfaceTemp.add(a);
        }
        car.set("tyresSurfaceTemperature", tyresSurfaceTemp);
        Poco::JSON::Array tyresInnerTemp;
        for(int j = 0; j<4; j++){
            auto a = dat.m_tyresInnerTemperature[j];
            tyresInnerTemp.add(a);
        }
        car.set("tyresInnerTemperature", tyresInnerTemp);
        car.set("engineTemperature", dat.m_engineTemperature);
        Poco::JSON::Array tyresPressure;
        for(int j = 0; j<4; j++){
            auto a = dat.m_tyresPressure[j];
            tyresPressure.add(a);
        }
        car.set("tyresPressure", tyresPressure);

        arr.add( car);
    }

    base.set("carTelemetryData", arr);
    base.set("buttonStatus", pack.m_buttonStatus);

    return base;
}

Poco::JSON::Object toJSON(PacketEventData pack){
    Poco::JSON::Object base;
    base.set("header", toJSON(pack.m_header));
    base.set("eventStringCode", std::string(pack.m_eventStringCode));
    return base;
}

Poco::JSON::Object toJSON(PacketLapData pack){
    Poco::JSON::Object base;
    base.set("header", toJSON(pack.m_header));

    Poco::JSON::Array arr;
    for(LapData& i:pack.m_lapData){
        Poco::JSON::Object car;

        car.set("lastLapTime", i.m_lastLapTime);
        car.set("currentLapTime", i.m_currentLapTime);
        car.set("bestLapTime", i.m_bestLapTime);
        car.set("sector1Time", i.m_sector1Time);
        car.set("sector2Time", i.m_sector2Time);
        car.set("lapDistance", i.m_lapDistance);
        car.set("totalDistance", i.m_totalDistance);
        car.set("safetyCarDelta", i.m_safetyCarDelta);
        car.set("carPosition", i.m_carPosition);
        car.set("currentLapNum", i.m_currentLapNum);
        car.set("pitStatus", i.m_pitStatus);
        car.set("sector", i.m_sector);
        car.set("currentLapInvalid", i.m_currentLapInvalid);
        car.set("penalties", i.m_penalties);
        car.set("gridPosition", i.m_gridPosition);
        car.set("driverStatus", i.m_driverStatus);
        car.set("resultStatus", i.m_resultStatus);

        arr.add(car);
    }


    base.set("lapData", arr);
    return base;
}

Poco::JSON::Object toJSON(PacketMotionData pack){
    Poco::JSON::Object base;
    base.set("header", toJSON(pack.m_header));

    Poco::JSON::Array car_arr;
    for(CarMotionData i: pack.m_carMotionData){
        Poco::JSON::Object car;

        car.set("worldPositionX", i.m_worldPositionX);
        car.set("worldPositionY", i.m_worldPositionY);
        car.set("worldPositionZ", i.m_worldPositionZ);
        car.set("worldVelocityX", i.m_worldVelocityX);
        car.set("worldVelocityY", i.m_worldVelocityY);
        car.set("worldVelocityZ", i.m_worldVelocityZ);
        car.set("worldForwardDirX", i.m_worldForwardDirX);
        car.set("worldForwardDirY", i.m_worldForwardDirY);
        car.set("worldForwardDirZ", i.m_worldForwardDirZ);
        car.set("worldRightDirX", i.m_worldRightDirX);
        car.set("worldRightDirY", i.m_worldRightDirY);
        car.set("worldRightDirZ", i.m_worldRightDirZ);
        car.set("gForceLateral", i.m_gForceLateral);
        car.set("gForceLongitudinal", i.m_gForceLongitudinal);
        car.set("gForceVertical", i.m_gForceVertical);
        car.set("yaw", i.m_yaw);
        car.set("pitch", i.m_pitch);
        car.set("roll", i.m_roll);

        car_arr.add(car);
    }
    base.set("carMotionData", car_arr);

    Poco::JSON::Array suspos;
    for(int j = 0; j<4; j++){suspos.add(pack.m_suspensionPosition[j]);}
    base.set("suspensionPosition", suspos);
    Poco::JSON::Array susvel;
    for(int j = 0; j<4; j++){susvel.add(pack.m_suspensionVelocity[j]);}
    base.set("suspensionVelocity", susvel);
    Poco::JSON::Array susacc;
    for(int j = 0; j<4; j++){susacc.add(pack.m_suspensionAcceleration[j]);}
    base.set("suspensionAcceleration", susacc);
    Poco::JSON::Array wheelspeed;
    for(int j = 0; j<4; j++){wheelspeed.add(pack.m_wheelSpeed[j]);}
    base.set("wheelSpeed", wheelspeed);
    Poco::JSON::Array wheelslip;
    for(int j = 0; j<4; j++){wheelslip.add(pack.m_wheelSlip[j]);}
    base.set("wheelSlip", wheelslip);

    base.set("localVelocityX", pack.m_localVelocityX);
    base.set("localVelocityY", pack.m_localVelocityY);
    base.set("localVelocityZ", pack.m_localVelocityZ);

    base.set("angularVelocityX", pack.m_angularVelocityX);
    base.set("angularVelocityY", pack.m_angularVelocityY);
    base.set("angularVelocityZ", pack.m_angularVelocityZ);

    base.set("angularAccelerationX", pack.m_angularAccelerationX);
    base.set("angularAccelerationY", pack.m_angularAccelerationY);
    base.set("angularAccelerationZ", pack.m_angularAccelerationZ);

    base.set("frontWheelsAngle", pack.m_frontWheelsAngle);

    return base;
}

Poco::JSON::Object toJSON(PacketParticipantsData pack){
    Poco::JSON::Object base;
    base.set("header", toJSON(pack.m_header));
    base.set("numCars", pack.m_numCars);

    Poco::JSON::Array arr;
    for(ParticipantData i: pack.m_participants){
        Poco::JSON::Object car;
        car.set("aiControlled", i.m_aiControlled);
        car.set("driverId", i.m_driverId);
        car.set("teamID", i.m_teamId);
        car.set("raceNumber", i.m_raceNumber);
        car.set("nationality", i.m_nationality);
        car.set("name", std::string(i.m_name));

        arr.add(car);
    }

    base.set("participants", arr);
    return base;
}

Poco::JSON::Object toJSON(PacketSessionData pack){
    Poco::JSON::Object base;
    base.set("header", toJSON(pack.m_header));
    base.set("weather", pack.m_weather);
    base.set("trackTemperature", pack.m_trackTemperature);
    base.set("airTemperature", pack.m_airTemperature);
    base.set("totalLaps", pack.m_totalLaps);
    base.set("trackLength", pack.m_trackLength);
    base.set("sessionType", pack.m_sessionType);
    base.set("trackId", pack.m_trackId);
    base.set("era", pack.m_era);
    base.set("sessionTimeLeft", pack.m_sessionTimeLeft);
    base.set("sessionDuration", pack.m_sessionDuration);
    base.set("pitSpeedLimit", pack.m_pitSpeedLimit);
    base.set("gamePaused", pack.m_gamePaused);
    base.set("isSpectating", pack.m_isSpectating);
    base.set("spectatorCarIndex", pack.m_spectatorCarIndex);
    base.set("sliProNativeSupport", pack.m_sliProNativeSupport);
    base.set("numMarshalZones", pack.m_numMarshalZones);
    Poco::JSON::Array arr;
    for(MarshalZone i: pack.m_marshalZones){
        Poco::JSON::Object zone;
        zone.set("zoneStart", i.m_zoneStart);
        zone.set("zoneFlag", i.m_zoneFlag);
        arr.add(zone);
    }
    base.set("marshalZones", arr);
    base.set("safetyCarStatus", pack.m_safetyCarStatus);
    base.set("networkGame", pack.m_networkGame);

    return base;
}