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

using Price = long double;
using Quantity = long double;

using Order = std::pair<Price, Quantity>;
using Orders = std::vector<Order>;

using OrderBookUpdateCallback = std::function<void(const std::string& symbol, const Orders& bids, const Orders& asks)>;

class BinanceWebSocketListener final : public std::enable_shared_from_this<BinanceWebSocketListener> {
public:
	BinanceWebSocketListener(net::io_context& ioc,
	                         net::ssl::context& ctx,
	                         const std::string& symbol,
	                         const int64_t& updateId,
	                         const OrderBookUpdateCallback& updateCallback);

	void run(); // Start the connection

private:
	void onResolve(beast::error_code ec, tcp::resolver::results_type results);
	void onConnect(beast::error_code ec);
	void onHandshake(beast::error_code ec);
	void onRead(beast::error_code ec, std::size_t bytes_transferred);
	void processMessage(const std::string& message);

	tcp::resolver resolver;
	beast::websocket::stream<net::ssl::stream<beast::tcp_stream>> ws;
	beast::flat_buffer buffer;
	std::string host;
	std::string target;
	std::string symbol;

	int64_t snapshotUID{};

	OrderBookUpdateCallback updateCallback;
};
