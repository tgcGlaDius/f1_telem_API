#include "shared_header.h"
#include "udp_reciver.h"

using namespace Poco::Net;

class req_404 : public HTTPRequestHandler
{
public:
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) override {
        resp.setStatus(HTTPResponse::HTTP_BAD_REQUEST);
        resp.setContentType("text/html");
        std::ostream& out = resp.send();
        out << "<h1>404 page not found</h1>";
        out.flush();
    }
};

class f12018_telem_handler: public HTTPRequestHandler{
public:
    void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp) override {
        if(_logger == nullptr){
            _logger = &Poco::Logger::root();
        }
        Poco::Logger& logger = *_logger;

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
            logger.error("no telemetry storage object attached");
        }

        std::ostream& out = resp.send();
        base_obj->stringify(out);
        out.flush();
    }

    static void set_storage(const std::shared_ptr<f1_storage>& stor){_telem = stor;}
    static void clear_storage(){_telem.reset();}
    static void set_logger(Poco::Logger* logger){_logger = logger;}
private:
    static std::weak_ptr<f1_storage> _telem;
    static Poco::Logger* _logger;
};
std::weak_ptr<f1_storage> f12018_telem_handler::_telem;
Poco::Logger* f12018_telem_handler::_logger = nullptr;


class request_handler: public HTTPRequestHandlerFactory{
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &req) override {
        Poco::URI uri(req.getURI());
        std::string fmt = req.get("fmt", "2018");
        if(uri.getPath() == "/f1telem"){
            if(fmt=="2018"){return new f12018_telem_handler;}

        }
        return new req_404;
    }
};

using namespace Poco;

class webserv: public Poco::Util::ServerApplication{
public:

protected:

    int main(const std::vector<std::string> &) override {
        AutoPtr<ConsoleChannel> pCons(new ConsoleChannel);
        AutoPtr<FileChannel> pFile(new FileChannel);
        AutoPtr<PatternFormatter> pPF(new PatternFormatter);

        pPF->setProperty("pattern", "[%p][%d-%m-%Y][UTC:%H:%M:%S][PID:%P][TID:%I][ %t ]");

        AutoPtr<FormattingChannel> pFC(new FormattingChannel(pPF, pFile));

        pFile->setProperty("purgeAge", "4 weeks");
        pFile->setProperty("purgeCount", "5");
        pFile->setProperty("path", "/var/log/f1_telem/f1_telem.log");
        pFile->setProperty("rotation", "5 M");
        pFile->setProperty("archive", "timestamp");

        AutoPtr<FormattingChannel> pFCons(new FormattingChannel(pPF, pCons));


        AutoPtr<AsyncChannel> pAsync(new AsyncChannel(pFC));
        AutoPtr<AsyncChannel> pAsynccon(new AsyncChannel(pFCons));

        Logger::root().setChannel(pAsync);
        Logger::root().setLevel(Message::PRIO_INFORMATION);
        Logger& logger = Logger::get("MainLogger");
        logger.setLevel(Message::PRIO_DEBUG);
        logger.setChannel(pAsynccon);
        setLogger(logger);

        std::shared_ptr<f1_storage> stor = make_shared<f1_storage>();
        f12018_telem_handler::set_storage(stor);

        udp_reciver _UDP(stor, logger);
        Poco::Thread UDP_thread;
        UDP_thread.start(_UDP);



        HTTPServer s(new request_handler, ServerSocket(8080), new HTTPServerParams);
        s.start();
        this->logger().information("starting webserver");
        waitForTerminationRequest();
        s.stop();

        _UDP.kill = true;
        UDP_thread.join();
        return Application::EXIT_OK;
    }

private:
};

POCO_SERVER_MAIN(webserv);