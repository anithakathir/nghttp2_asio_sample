#include <nghttp2/asio_http2_server.h>
#include <thread>
#include <chrono>

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::server;

void req_callback(const response &res) {
    res.on_close([&res](uint32_t ec) {
        std::cout << "Closing the stream, ec=" << ec << std::endl;
    });
    std::thread th ([&res]() {
        std::this_thread::sleep_for(std::chrono::seconds(80)); //Random wait assuming the server is processing something
        std::cout << "Server completed processing. Sending response to client..." << std::endl;
        auto & ios = res.io_service();
        ios.post([&res]() {
            res.write_head(200);
            res.end("Dummy response to client from server");
        });
    });
    th.detach();
}

int main(int argc, char *argv[]) {
    boost::system::error_code ec;
    size_t i=1;
    http2 server;
    server.handle("/", [](const request &req, const response &res) {
        std::cout << "Server received request" << std::endl;
        req_callback(res);
    });

    std::string ip = "localhost";
    std::string port = "3000";
    std::cout << "Server is running on " << ip << ":" << port <<std::endl;
    ec = server.listen_and_serve(ec, ip, port);
    if(ec != 0)
        std::cerr << "Error: " << ec.value() << ":::" <<ec.message() << std::endl;
}

