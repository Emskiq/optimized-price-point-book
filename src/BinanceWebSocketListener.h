#pragma once

#include <boost/asio/strand.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/beast/websocket/ssl.hpp>
#include <boost/asio/ssl/context.hpp>
#include <iostream>
#include <string>

namespace beast = boost::beast;
namespace net = boost::asio;
using tcp = net::ip::tcp;

class BinanceWebSocketListener : public std::enable_shared_from_this<BinanceWebSocketListener> {
public:
	BinanceWebSocketListener(net::io_context& ioc, net::ssl::context& ctx, const std::string& symbol);

	void run(); // Start the connection

private:
	void onResolve(beast::error_code ec, tcp::resolver::results_type results);
	void onConnect(beast::error_code ec);
	void onHandshake(beast::error_code ec);
	void onRead(beast::error_code ec, std::size_t bytes_transferred);
	void processMessage(const std::string& message);

	tcp::resolver resolver_;
	beast::websocket::stream<net::ssl::stream<beast::tcp_stream>> ws_;
	beast::flat_buffer buffer_;
	std::string host_;
	std::string target_;
};
