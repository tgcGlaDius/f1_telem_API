//
// Created by eliwas on 14.10.2019.
//

#include "udp_reciver.h"
void udp_reciver::run() {
    ds.bind(sa);
    std::cout << "startet UDP thread" << std::endl;
    int prev_size = 0;
    while (!kill){
        int size = ds.available();
        if( size >= sizeof(PacketHeader)){
            std::unique_ptr<unsigned char[]> buffer = make_unique<unsigned char[]>(size+1);
            ds.receiveBytes(buffer.get(), size);
            auto header = (PacketHeader*)buffer.get();
            if(header->m_packetFormat == 2018){
                telem->Header_mut.lock();
                memcpy(&telem->Header, buffer.get(), sizeof(PacketHeader));
                telem->Header_mut.unlock();
                if(header->m_packetId == PacketType::CAR_SETUPS_PACKET){
                    telem->CarSetups_mut.lock();
                    memcpy(&telem->CarSetupData, buffer.get(), sizeof(PacketCarSetupData));
                    telem->CarSetups_mut.unlock();
                }
                else if(header->m_packetId == PacketType::CAR_STATUS){
                    telem->CarStatus_mut.lock();
                    memcpy(&telem->CarStatusData, buffer.get(), sizeof(PacketCarStatusData));
                    telem->CarStatus_mut.unlock();
                }
                else if(header->m_packetId == PacketType::CAR_TELEMETRY_PACKET){
                    telem->CarTelemetry_mut.lock();
                    memcpy(&telem->CarTelemetryData, buffer.get(), sizeof(PacketCarTelemetryData));
                    telem->CarTelemetry_mut.unlock();
                }
                else if(header->m_packetId == PacketType::EVENT_PACKET){
                    telem->Event_mut.lock();
                    memcpy(&telem->EventData, buffer.get(), sizeof(PacketEventData));
                    telem->Event_mut.unlock();
                }
                else if(header->m_packetId == PacketType::LAP_DATA_PACKET){
                    telem->LapData_mut.lock();
                    memcpy(&telem->LapData, buffer.get(), sizeof(PacketLapData));
                    telem->LapData_mut.unlock();
                }
                else if(header->m_packetId == PacketType::MOTION_PACKET){
                    telem->Motion_mut.lock();
                    memcpy(&telem->MotionData, buffer.get(), sizeof(PacketMotionData));
                    telem->Motion_mut.unlock();
                }
                else if(header->m_packetId == PacketType::PARTICIPANTS_PACKET){
                    telem->Participants_mut.lock();
                    memcpy(&telem->ParticipantsData, buffer.get(), sizeof(PacketParticipantsData));
                    telem->Participants_mut.unlock();
                }
                else if(header->m_packetId == PacketType::SESSION_PACKET){
                    telem->Session_mut.lock();
                    memcpy(&telem->SessionData, buffer.get(), sizeof(PacketSessionData));
                    telem->Session_mut.unlock();
                }
            } else{
                std::cout << "invalid packet format: " << header->m_packetFormat << std::endl;
            }



        } else if(size != 0){
            char a;
            ds.receiveBytes(&a, 1);
        }
    }
}