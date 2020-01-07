//
// Created by eliwas on 14.10.2019.
//

#ifndef WEB_SERV_TEST_UDP_RECIVER_H
#define WEB_SERV_TEST_UDP_RECIVER_H

#include <utility>

#include "shared_header.h"
#include "packet_handler.h"


class udp_reciver: public Poco::Runnable {
public:
    explicit udp_reciver(std::shared_ptr<f1_storage> stor, Poco::Util::LayeredConfiguration& _conf, int port = 20777, const std::string& addr = "0.0.0.0"):conf(_conf), sa(addr, port), telem(std::move(stor)), logger(Poco::Logger::get("UDP_thread")){
        Poco::AutoPtr<Poco::ConsoleChannel> pCons(new Poco::ConsoleChannel);
        logger.setChannel(pCons);
    }

    udp_reciver(std::shared_ptr<f1_storage> stor, Poco::Util::LayeredConfiguration& _conf, Poco::Logger& _logger ,int port = 20777, const std::string& addr = "0.0.0.0"):conf(_conf), sa(addr, port), telem(std::move(stor)), logger(_logger){}
    void run() override;
    std::shared_ptr<f1_storage> get_storage(){ return telem;}
    bool kill = false;
private:

    void on_new_packet();
    void init_postpacket();

    static constexpr int packet_type_count = 8;

    uint64_t current_session_UID = 0;
    Poco::Net::SocketAddress sa;
    Poco::Net::DatagramSocket ds;
    Poco::Logger& logger;
    std::shared_ptr<f1_storage> telem;
    std::array<std::vector<std::unique_ptr<packet_handler>>, packet_type_count> post_packet;
    Poco::ThreadPool tp;

    Poco::Util::LayeredConfiguration& conf;
};


#endif //WEB_SERV_TEST_UDP_RECIVER_H
