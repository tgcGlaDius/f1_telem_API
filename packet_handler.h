//
// Created by eliwas on 09.12.2019.
//

#ifndef F1_TELEM_API_PACKET_HANDLER_H
#define F1_TELEM_API_PACKET_HANDLER_H

class packet_handler : public Poco::Runnable{
protected:
    std::shared_ptr<f1_storage> telem;
public:
    bool running = false;
    packet_handler() = delete;
    explicit packet_handler(std::shared_ptr<f1_storage>& stor):telem(stor){}
    virtual void on_new_session(PacketSessionData& p) = 0;
};


struct __attribute__ ((packed)) checkpoint {
    float time = 0, dist = 0;
};

class timing_handler : public packet_handler{
    std::vector<checkpoint> breadcrumbs;
    std::vector<std::vector<std::vector<float>>> marshal_times;
    PacketSessionData sessionData = {0,};
    std::vector<float> diff;
    std::vector<bool> checkpoint_sys;
    const float system_change = 1.0f;

public:
    timing_handler() = delete;
    explicit timing_handler(std::shared_ptr<f1_storage>& stor): packet_handler(stor){}
    void run() override{
        running = true;
        telem->LapData_mut.lock();
        PacketLapData lapData = telem->LapData;
        telem->LapData_mut.unlock();
        std::vector<int> driver_in_pos(20);
        for(unsigned long a = 0; a<20; a++){
            auto i = lapData.m_lapData[a];
            driver_in_pos[i.m_carPosition-1] = a;
        }

        if(sessionData.m_sessionType == sessionType::R || sessionData.m_sessionType == sessionType::R2){ // if race
            //begin new marshal zone system

            for(unsigned long i = 0; i<driver_in_pos.size(); i++){
                auto driver_index = driver_in_pos[i];
                float fraq_through = (lapData.m_lapData[driver_index].m_lapDistance/float(sessionData.m_trackLength));
                auto lap = lapData.m_lapData[driver_index].m_currentLapNum;

                ulong current_marshal = -1;
                for(ulong a = 0; a<sessionData.m_numMarshalZones; a++){
                    if(fraq_through >= sessionData.m_marshalZones[a].m_zoneStart){
                        current_marshal = a;
                    }
                    else{
                        break;
                    }
                }

                if(marshal_times[driver_index][lap][current_marshal] == -1){
                    marshal_times[driver_index][lap][current_marshal] = lapData.m_header.m_sessionTime;
                }

                if(!checkpoint_sys[driver_index]){
                    if(i != 0){
                        auto driver_over = driver_in_pos[i-1];
                        auto timing_diff = marshal_times[driver_index][lap][current_marshal]-marshal_times[driver_over][lap][current_marshal];
                        diff[driver_index] = timing_diff;
                        if(timing_diff<system_change){
                            checkpoint_sys[driver_index] = true;
                        }
                    }else{
                        diff[driver_index] = lapData.m_lapData[driver_index].m_currentLapTime;
                    }
                }
            }

            //end new marshal zone system

            //old breadcrumb based timing system. optimal for when short distance between drivers
            //bad for when drivers get distance between eachother. use marshal zone system when distance is large enough
            for(unsigned long i = 0; i<driver_in_pos.size(); i++){
                auto driver_index = driver_in_pos[i];
                auto dist = lapData.m_lapData[driver_index].m_totalDistance;
                auto time = lapData.m_header.m_sessionTime;

                if(breadcrumbs[driver_index].time == -1){
                    breadcrumbs[driver_index].time = time;
                    breadcrumbs[driver_index].dist = dist;
                }
                if(checkpoint_sys[driver_index]){
                    if(i!= 0){
                        auto driver_over = driver_in_pos[i-1];
                        if(dist>= breadcrumbs[driver_over].dist){
                            diff[driver_index] = time-breadcrumbs[driver_over].time;

                            if(diff[driver_index] > system_change){
                                checkpoint_sys[driver_index] = false;
                            }

                            breadcrumbs[driver_over].time = -1;
                        }
                    } else{
                        diff[driver_index] = lapData.m_lapData[driver_index].m_currentLapTime;
                    }
                }
            }
            //end old breadcrumb system
        } else if(sessionData.m_sessionType != sessionType::UNKNOWN && sessionData.m_sessionType >= 12){ // if not a race
            for(unsigned long i = 0; i<driver_in_pos.size(); i++){
                auto driver_index = driver_in_pos[i];
                auto best_lap = lapData.m_lapData[driver_index].m_bestLapTime;
                if(i == 0){
                    diff[driver_index] = best_lap;
                }
                else if(best_lap > 0){
                    diff[driver_index] = best_lap-(lapData.m_lapData[driver_in_pos[i-1]].m_bestLapTime);
                }
            }
        }

        telem->timing_mut.lock();
        telem->driver_diffs = diff;
        telem->timing_mut.unlock();

        running = false;
    }
    void on_new_session(PacketSessionData& p) override {
        breadcrumbs.clear();
        marshal_times.clear();
        diff.clear();
        checkpoint_sys.clear();

        diff.resize(20);

        telem->timing_mut.lock();
        telem->driver_diffs = diff;
        telem->timing_mut.unlock();

        if(p.m_sessionType == sessionType::R || p.m_sessionType == sessionType::R2){
            breadcrumbs.resize(20);
            for(auto &i: breadcrumbs){
                i.time = -1;
            }
            checkpoint_sys.resize(20);
            marshal_times.resize(20);
            for(auto& i: marshal_times){
                i.resize(p.m_totalLaps);
                for(auto& a:i){
                    a.resize(p.m_numMarshalZones);
                    for(auto& j : a){
                        j = -1;
                    }
                }
            }
        }

        sessionData = p;
    }
};

#endif //F1_TELEM_API_PACKET_HANDLER_H
