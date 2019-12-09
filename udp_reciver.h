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
    explicit udp_reciver(std::shared_ptr<f1_storage> stor, int port = 20777, const std::string& addr = "0.0.0.0"): sa(addr, port), telem(std::move(stor)), logger(Poco::Logger::get("UDP_thread")){
        Poco::AutoPtr<Poco::ConsoleChannel> pCons(new Poco::ConsoleChannel);
        logger.setChannel(pCons);
    }

    udp_reciver(std::shared_ptr<f1_storage> stor, Poco::Logger& _logger ,int port = 20777, const std::string& addr = "0.0.0.0"): sa(addr, port), telem(std::move(stor)), logger(_logger){}
    void run() override;
    std::shared_ptr<f1_storage> get_storage(){ return telem;}
    bool kill = false;
private:

    void on_new_packet();
    void init_postpacket();

    uint64_t current_session_UID = 0;
    Poco::Net::SocketAddress sa;
    Poco::Net::DatagramSocket ds;
    Poco::Logger& logger;
    std::shared_ptr<f1_storage> telem;
    std::array<std::vector<std::unique_ptr<packet_handler>>, 8> post_packet;
    Poco::ThreadPool tp;
};


#endif //WEB_SERV_TEST_UDP_RECIVER_H
