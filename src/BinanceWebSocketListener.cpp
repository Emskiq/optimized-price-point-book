#include "BinanceWebSocketListener.h"

#include <chrono>
#include <iomanip>

#include <boost/json/src.hpp>

BinanceWebSocketListener::BinanceWebSocketListener(net::io_context& ioc, net::ssl::context& ctx, const std::string& symbol, const int64_t& updateId, const OrderBookUpdateCallback& updateCallback)
	: resolver(net::make_strand(ioc))
	, ws(net::make_strand(ioc), ctx)
	, host("stream.binance.com")
	, target("/ws/" + symbol + "@depth@100ms")
	, symbol(symbol)
	, snapshotUID(updateId)
	, updateCallback(updateCallback)
{ }

void BinanceWebSocketListener::run()
{
	resolver.async_resolve(host, "443", [self = shared_from_this()] (beast::error_code ec, tcp::resolver::results_type results) {
		self->onResolve(ec, results);
	});
}

void BinanceWebSocketListener::onResolve(beast::error_code ec, tcp::resolver::results_type results)
{
	if (ec) {
		std::cerr << "Resolve Error: " << ec.message() << std::endl;
		return;
	}

	beast::get_lowest_layer(ws).async_connect(results, [self = shared_from_this()] (beast::error_code ec, const tcp::endpoint&) {
		self->onConnect(ec);
	});
}

void BinanceWebSocketListener::onConnect(beast::error_code ec)
{
	if (ec) {
		std::cerr << "Connect Error: " << ec.message() << std::endl;
		return;
	}

	ws.next_layer().async_handshake(net::ssl::stream_base::client, [self = shared_from_this()] (beast::error_code ec) {
		self->onHandshake(ec);
	});
}

void BinanceWebSocketListener::onHandshake(beast::error_code ec)
{
	if (ec) {
		std::cerr << "SSL Handshake Error: " << ec.message() << std::endl;
		return;
	}

	ws.async_handshake(host, target, [self = shared_from_this()] (beast::error_code ec) {
		if (!ec) {
			self->onRead(ec, 0);
		}
	});
}

void BinanceWebSocketListener::onRead(beast::error_code ec, std::size_t bytes_transferred)
{
	boost::ignore_unused(bytes_transferred);

	if (ec) {
		std::cerr << "Read Error: " << ec.message() << std::endl;
		return;
	}

	std::string message = beast::buffers_to_string(buffer.data());
	buffer.consume(buffer.size());

	processMessage(message);

	ws.async_read(buffer, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
		self->onRead(ec, bytes_transferred);
	});
}

void BinanceWebSocketListener::processMessage(const std::string& message)
{
	try {
		auto json = boost::json::parse(message);
		auto updateId = json.at("u").as_int64();

		// std::cout << "Update ID received: " << updateId << std::endl;

		if (updateId >= snapshotUID) {
			// By the Official Specs from Binance:
			// https://developers.binance.com/docs/derivatives/usds-margined-futures/websocket-market-streams/How-to-manage-a-local-order-book-correctly
			auto bidsJsonElement = json.at("b").as_array();
			auto asksJsonElement = json.at("a").as_array();
			Orders bids{}, asks{};

			for (auto& bid : bidsJsonElement) {
				bids.emplace_back(std::stold(bid.at(0).as_string().c_str()),
						std::stold(bid.at(1).as_string().c_str()));
			}

			for (auto& ask : asksJsonElement) {
				asks.emplace_back(std::stold(ask.at(0).as_string().c_str()),
						std::stold(ask.at(1).as_string().c_str()));
			}

			updateCallback(symbol, bids, asks);
		}
	} catch (std::exception& e) {
		std::cerr << "JSON Parsing error: " << e.what() << "\n";
	}
}
