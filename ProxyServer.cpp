#include "ProxyServer.h"
#include "CacheManager.h"
#include "LoadBalancer.h"

#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

using namespace Poco::Net;
using namespace Poco;
using namespace std;

CacheManager cache;
LoadBalancer loadBalancer({ "example.com", "httpbin.org" });

class SimpleRequestHandler : public HTTPRequestHandler {
public:
    void handleRequest(HTTPServerRequest& request, HTTPServerResponse& response) override {
        std::string uri = request.getURI();

        if (cache.isCached(uri)) {
            response.setStatus(HTTPResponse::HTTP_OK);
            ostream& out = response.send();
            out << cache.get(uri);
            return;
        }

        std::string backend = loadBalancer.getNextServer();
        HTTPClientSession session(backend);
        HTTPRequest req(HTTPRequest::HTTP_GET, uri);
        session.sendRequest(req);
        HTTPResponse res;
        istream& rs = session.receiveResponse(res);
        std::ostringstream oss;
        StreamCopier::copyStream(rs, oss);
        std::string content = oss.str();
        cache.put(uri, content);
        response.setStatus(res.getStatus());
        ostream& out = response.send();
        out << content;
    }
};

class SimpleRequestHandlerFactory : public HTTPRequestHandlerFactory {
public:
    HTTPRequestHandler* createRequestHandler(const HTTPServerRequest& request) override {
        return new SimpleRequestHandler;
    }
};

void ProxyServer::start() {
    HTTPServerParams* params = new HTTPServerParams;
    ServerSocket socket(8080);
    HTTPServer server(new SimpleRequestHandlerFactory, socket, params);
    server.start();
    cout << "Proxy server running with cache and load balancer on http://localhost:8080" << endl;
    // getchar();
    while (true) {
        std::this_thread::sleep_for(std::chrono::hours(24));
    }
    server.stop();
}
