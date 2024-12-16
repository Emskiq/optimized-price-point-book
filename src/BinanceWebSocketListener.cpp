#include "BinanceWebSocketListener.h"

BinanceWebSocketListener::BinanceWebSocketListener(net::io_context& ioc, net::ssl::context& ctx, const std::string& symbol)
	: resolver_(net::make_strand(ioc)), ws_(net::make_strand(ioc), ctx), host_("stream.binance.com"), target_("/ws/" + symbol + "@depth@100ms")
{ }

void BinanceWebSocketListener::run()
{
	resolver_.async_resolve(host_, "443", [self = shared_from_this()] (beast::error_code ec, tcp::resolver::results_type results) {
		self->onResolve(ec, results);
	});
}

void BinanceWebSocketListener::onResolve(beast::error_code ec, tcp::resolver::results_type results)
{
	if (ec) {
		std::cerr << "Resolve Error: " << ec.message() << std::endl;
		return;
	}

	beast::get_lowest_layer(ws_).async_connect(results, [self = shared_from_this()] (beast::error_code ec, const tcp::endpoint&) {
		self->onConnect(ec);
	});
}

void BinanceWebSocketListener::onConnect(beast::error_code ec)
{
	if (ec) {
		std::cerr << "Connect Error: " << ec.message() << std::endl;
		return;
	}

	ws_.next_layer().async_handshake(net::ssl::stream_base::client, [self = shared_from_this()] (beast::error_code ec) {
		self->onHandshake(ec);
	});
}

void BinanceWebSocketListener::onHandshake(beast::error_code ec)
{
	if (ec) {
		std::cerr << "SSL Handshake Error: " << ec.message() << std::endl;
		return;
	}

	ws_.async_handshake(host_, target_, [self = shared_from_this()] (beast::error_code ec) {
		if (!ec) {
			self->onRead(ec, 0);
		}
	});
}

void BinanceWebSocketListener::onRead(beast::error_code ec, std::size_t bytes_transferred)
{
	if (ec) {
		std::cerr << "Read Error: " << ec.message() << std::endl;
		return;
	}

	std::string message = beast::buffers_to_string(buffer_.data());
	buffer_.consume(buffer_.size());

	processMessage(message);

	ws_.async_read(buffer_, [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
		self->onRead(ec, bytes_transferred);
	});
}

void BinanceWebSocketListener::processMessage(const std::string& message)
{
	// faster output
	std::cerr << "Received Update: " << message << std::endl;
}

