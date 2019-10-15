//
// Created by eliwas on 14.10.2019.
//

#ifndef WEB_SERV_TEST_UDP_RECIVER_H
#define WEB_SERV_TEST_UDP_RECIVER_H

#include <utility>

#include "shared_header.h"

class udp_reciver: public Poco::Runnable {
public:
    explicit udp_reciver(std::shared_ptr<f1_storage>  stor, int port = 20777, const std::string& addr = "0.0.0.0"): sa(addr, port), telem(std::move(stor)){}
    void run() override;
    std::shared_ptr<f1_storage> get_storage(){ return telem;}
    bool kill = false;
private:
    Poco::Net::SocketAddress sa;
    Poco::Net::DatagramSocket ds;
    std::shared_ptr<f1_storage> telem;
};


#endif //WEB_SERV_TEST_UDP_RECIVER_H
