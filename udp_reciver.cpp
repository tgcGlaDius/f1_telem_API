//
// Created by eliwas on 14.10.2019.
//

#include "udp_reciver.h"
void udp_reciver::run() {
    ds.bind(sa);
    logger.information("started UDP thread");

    init_postpacket();

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

                this->on_new_packet();
            } else{
                logger.warning("invalid packet format: " + to_string(header->m_packetFormat));
            }

        } else if(size != 0){
            logger.warning("got invalid packet of size " + to_string(size));
            char a;
            ds.receiveBytes(&a, 1);
        }
    }
}

inline void udp_reciver::init_postpacket() {
    post_packet[PacketType::LAP_DATA_PACKET].push_back(make_unique<timing_handler>(telem));
}

void udp_reciver::on_new_packet() {
    telem->Header_mut.lock();
    auto packid = telem->Header.m_packetId;
    telem->Header_mut.unlock();

    telem->Session_mut.lock();
    auto session_dat = telem->SessionData;
    telem->Session_mut.unlock();
    if(current_session_UID != session_dat.m_header.m_sessionUID){
        tp.joinAll();
        for(auto &i: post_packet){
            for(auto &a: i){
                a->on_new_session(session_dat);
            }
        }
        current_session_UID = session_dat.m_header.m_sessionUID;
    }

    for(unsigned long a = 0; a<post_packet[packid].size(); a++){
        const auto &i = post_packet[packid][a];
        try{
            if(!i->running){
                tp.start(*i);
            }
        }
        catch(const Poco::NoThreadAvailableException& e){
            while(tp.available()<=0){}
            a--;
        }
        catch(const Poco::Exception& e){
            logger.fatal(e.what());
        }
    }
}