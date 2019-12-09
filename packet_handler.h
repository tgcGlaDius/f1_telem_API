//
// Created by eliwas on 09.12.2019.
//

#ifndef F1_TELEM_API_PACKET_HANDLER_H
#define F1_TELEM_API_PACKET_HANDLER_H

struct __attribute__ ((packed)) checkpoint {
    float time = 0, dist = 0;
};

class packet_handler : public Poco::Runnable{
protected:
    std::shared_ptr<f1_storage> telem;
public:
    bool running = false;
    packet_handler() = delete;
    explicit packet_handler(std::shared_ptr<f1_storage>& stor):telem(stor){}
    virtual void on_new_session(PacketSessionData& p) = 0;
};

class timing_handler : public packet_handler{
    std::vector<checkpoint> breadcrumbs;
    std::vector<std::vector<float>> marshal_times;
    PacketSessionData sessionData = {0,};
    std::vector<float> diff;

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

        if(sessionData.m_sessionType == sessionType::R || sessionData.m_sessionType == sessionType::R2){
            //begin new marshal zone system
            //TODO: implement marshal zone system
            //end new marshal zone system

            //old breadcrumb based timing system. optimal for when short distance between drivers
            for(unsigned long i = 0; i<driver_in_pos.size(); i++){
                auto driver_index = driver_in_pos[i];
                auto dist = lapData.m_lapData[driver_index].m_totalDistance;
                auto time = lapData.m_header.m_sessionTime;

                if(breadcrumbs[driver_index].time == -1){
                    breadcrumbs[driver_index].time = time;
                    breadcrumbs[driver_index].dist = dist;
                }
                if(i!= 0){
                    auto driver_over = driver_in_pos[i-1];
                    if(dist>= breadcrumbs[driver_over].dist){
                        diff[driver_index] = time-breadcrumbs[driver_over].time;

                        breadcrumbs[driver_over].time = -1;
                    }
                }
            }
            //end old breadcrumb system
        } else{
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

        running = false;
    }
    void on_new_session(PacketSessionData& p) override {
        breadcrumbs.clear();
        marshal_times.clear();
        diff.clear();
        diff.resize(20);

        if(p.m_sessionType == sessionType::R || p.m_sessionType == sessionType::R2){
            breadcrumbs.resize(20);
            for(auto &i: breadcrumbs){
                i.time = -1;
            }
            marshal_times.resize(p.m_totalLaps);
            for(auto& i: marshal_times){
                i.resize(p.m_numMarshalZones);
            }
        }

        sessionData = p;
    }
};

#endif //F1_TELEM_API_PACKET_HANDLER_H
