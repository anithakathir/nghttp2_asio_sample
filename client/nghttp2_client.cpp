#include <iostream>
#include <nghttp2/asio_http2_client.h>

using boost::asio::ip::tcp;

using namespace nghttp2::asio_http2;
using namespace nghttp2::asio_http2::client;

int main(int argc, char *argv[]) {
    boost::system::error_code ec;
    boost::asio::io_service io_service;

    // connect to localhost:3000
    session sess(io_service, "localhost", "3000");
    sess.on_connect([&sess](tcp::resolver::iterator endpoint_it) {
        boost::system::error_code ec;

        auto req = sess.submit(ec, "GET", "http://localhost:3000/");

        req->on_response([](const response &res) {
            // print status code and response header fields.
            std::cerr << "HTTP/2 " << res.status_code() << std::endl;
            for (auto &kv : res.header()) {
                std::cerr << kv.first << ": " << kv.second.value << "\n";
            }
            std::cerr << std::endl;

            res.on_data([](const uint8_t *data, std::size_t len) {
                std::cout << "Receiving data from server..." << std::endl;
                std::cerr.write(reinterpret_cast<const char *>(data), len);
                std::cerr << std::endl;
                getchar();
            });
        });

        req->on_close([&sess](uint32_t error_code) {
            std::cout << "Shutting down client session..." << std::endl;
            // shutdown session after first request was done.
            sess.shutdown();
        });
    });

    sess.on_error([](const boost::system::error_code &ec) {
        std::cerr << "error: " << ec.message() << std::endl;
    });

    io_service.run();
}
