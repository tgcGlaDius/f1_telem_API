//
// Created by eliwas on 14.10.2019.
//

#ifndef WEB_SERV_TEST_SHARED_HEADER_H
#define WEB_SERV_TEST_SHARED_HEADER_H

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>

#include <Poco/Mutex.h>
#include <Poco/Thread.h>
#include <Poco/Runnable.h>
#include <Poco/URI.h>

#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Array.h>

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <memory>
#include <algorithm>

#include <Poco/Net/DNS.h>
#include <Poco/Net/StreamSocket.h>
#include <Poco/Net/SocketStream.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/StreamCopier.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HostEntry.h>
#include <Poco/Net/NetworkInterface.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/DatagramSocket.h>
#include "Poco/Exception.h"

#include <Poco/Message.h>
#include <Poco/Logger.h>
#include <Poco/ConsoleChannel.h>
#include <Poco/AsyncChannel.h>
#include <Poco/AutoPtr.h>
#include <Poco/SplitterChannel.h>
#include <Poco/FileChannel.h>
#include <Poco/FormattingChannel.h>
#include <Poco/PatternFormatter.h>


#include "f1structs/f1_storage.h"

#endif //WEB_SERV_TEST_SHARED_HEADER_H
