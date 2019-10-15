#include "shared_header.h"
#include "udp_reciver.h"

using namespace Poco::Net;

class req_404 : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) override {
        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType("text/html");
        std::ostream& out = resp.send();
        out << "<h1>404 page not found</h1>";
        out.flush();
    }
};

class rand_num_req:public HTTPRequestHandler{
public:
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) override {
        resp.set("Access-Control-Allow-Origin", "*");
        resp.set("Access-Control-Allow-Headers", "*");
        Poco::JSON::Object::Ptr test = new Poco::JSON::Object;
        test->set("num", rand());
        resp.setStatus(HTTPResponse::HTTP_OK);
        resp.setContentType("application/json");
        std::ostream& out = resp.send();
        test->stringify(out);
        out.flush();
    }

private:

};

class f12018_telem_handler: public HTTPRequestHandler{
public:
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) override {
        resp.set("Access-Control-Allow-Origin", "*");
        resp.set("Access-Control-Allow-Headers", "*");
        Poco::JSON::Object::Ptr base_obj = new Poco::JSON::Object;
        resp.setContentType("application/json");
        Poco::URI uri(req.getURI());
        auto params = uri.getQueryParameters();
        const std::string req_name = "req";
        std::vector<std::string> requests;
        for(const auto& i : params){
            if(i.first == req_name){
                requests.push_back(i.second);
            }
        }

        if(requests.empty()){
            base_obj->set("error", "did not request any packet");
            resp.setStatus(HTTPResponse::HTTPStatus::HTTP_BAD_REQUEST);
        }
        else if(auto telem = _telem.lock()){
            base_obj->set("error","OK");

            if(find(requests.begin(), requests.end(), "Header") != requests.end()){
                telem->Header_mut.lock();
                PacketHeader p = telem->Header;
                telem->Header_mut.unlock();
                base_obj->set("Header", toJSON(p));
            }
            if(find(requests.begin(), requests.end(), "CarSetups") != requests.end()){
                telem->CarSetups_mut.lock();
                auto p = telem->CarSetupData;
                telem->CarSetups_mut.unlock();
                base_obj->set("CarSetups", toJSON(p));
            }
            if(find(requests.begin(), requests.end(), "CarStatus") != requests.end()){
                telem->CarStatus_mut.lock();
                auto p = telem->CarStatusData;
                telem->CarStatus_mut.unlock();
                base_obj->set("CarStatus", toJSON(p));
            }
            if(find(requests.begin(), requests.end(), "CarTelemetry") != requests.end()){
                telem->CarTelemetry_mut.lock();
                auto p = telem->CarTelemetryData;
                telem->CarTelemetry_mut.unlock();
                base_obj->set("CarTelemetry", toJSON(p));
            }
            if(find(requests.begin(), requests.end(), "EventData") != requests.end()){
                telem->Event_mut.lock();
                auto p = telem->EventData;
                telem->Event_mut.unlock();
                base_obj->set("EventData", toJSON(p));
            }
            if(find(requests.begin(), requests.end(), "LapData") != requests.end()){
                telem->LapData_mut.lock();
                auto p = telem->LapData;
                telem->LapData_mut.unlock();
                base_obj->set("LapData", toJSON(p));
            }
            if(find(requests.begin(), requests.end(), "MotionData") != requests.end()){
                telem->Motion_mut.lock();
                auto p = telem->MotionData;
                telem->Motion_mut.unlock();
                base_obj->set("MotionData", toJSON(p));
            }
            if(find(requests.begin(), requests.end(), "Participants") != requests.end()){
                telem->Participants_mut.lock();
                auto p = telem->ParticipantsData;
                telem->Participants_mut.unlock();
                base_obj->set("Participants", toJSON(p));
            }
            if(find(requests.begin(), requests.end(), "SessionData") != requests.end()){
                telem->Session_mut.lock();
                auto p = telem->SessionData;
                telem->Session_mut.unlock();
                base_obj->set("SessionData", toJSON(p));
            }


            resp.setStatus(HTTPResponse::HTTP_OK);
        }
        else{
            base_obj->set("error", "no telemetry storage object attached");
            resp.setStatus(HTTPResponse::HTTPStatus::HTTP_INTERNAL_SERVER_ERROR);
        }

        std::ostream& out = resp.send();
        base_obj->stringify(out);
        out.flush();
    }

    static void set_storage(const std::shared_ptr<f1_storage>& stor){_telem = stor;}
    static void clear_storage(){_telem.reset();}
private:
    static std::weak_ptr<f1_storage> _telem;
};
std::weak_ptr<f1_storage> f12018_telem_handler::_telem;


class request_handler: public HTTPRequestHandlerFactory{
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &req) override {
        Poco::URI uri(req.getURI());
        std::string fmt = req.get("fmt", "2018");
        if(uri.getPath() == "/rand"){ return new rand_num_req;}

        if(uri.getPath() == "/f1telem"){
            if(fmt=="2018"){return new f12018_telem_handler;}

        }

        return new req_404;
    }
};

class webserv: public Poco::Util::ServerApplication{
protected:
    int main(const std::vector<std::string> &) override {
        HTTPServer s(new request_handler, ServerSocket(8080), new HTTPServerParams);
        s.start();
        std::cout << "starting webserver" << std::endl;
        waitForTerminationRequest();
        s.stop();
        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    std::shared_ptr<f1_storage> stor = make_shared<f1_storage>();
    f12018_telem_handler::set_storage(stor);

    udp_reciver _UDP(stor);
    Poco::Thread UDP_thread;
    UDP_thread.start(_UDP);

    //create udp server

    webserv app;
    int ret = app.run(argc, argv);
    _UDP.kill = true;
    UDP_thread.join();
    return ret;
}