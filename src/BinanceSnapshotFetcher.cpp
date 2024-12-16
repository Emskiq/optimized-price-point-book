#include "BinanceSnapshotFetcher.h"

BinanceSnapshotFetcher::BinanceSnapshotFetcher(const std::string& ticker)
	: ticker(ticker), lastUpdateId(0) 
{ }

bool BinanceSnapshotFetcher::fetchSnapshot() {
	try {
		// Setup Boost.Asio
		boost::asio::io_context ioc;
		ssl::context ctx{ssl::context::tlsv12_client};
		ctx.set_default_verify_paths();

		// Resolve Binance API
		tcp::resolver resolver(ioc);
		beast::ssl_stream<beast::tcp_stream> stream(ioc, ctx);

		// Set SNI
		if (!SSL_set_tlsext_host_name(stream.native_handle(), "fapi.binance.com")) {
			throw beast::system_error{beast::error_code{}, "Failed to set SNI"};
		}

		auto const results = resolver.resolve("fapi.binance.com", "443");
		beast::get_lowest_layer(stream).connect(results);
		stream.handshake(ssl::stream_base::client);

		// Make the HTTP request
		std::string target = "/fapi/v1/depth?symbol=" + ticker + "&limit=1000";
		http::request<http::string_body> req{http::verb::get, target, 11};
		req.set(http::field::host, "fapi.binance.com");
		req.set(http::field::user_agent, "Boost.Beast/1.87.0");

		http::write(stream, req);

		// Receive the response
		beast::flat_buffer buffer;
		http::response<http::string_body> res;
		http::read(stream, buffer, res);

		if (res.result() != http::status::ok) {
			std::cerr << "Error: " << res.result_int() << " " << res.reason() << "\n";
			return false;
		}

		parseSnapshot(res.body());
		return true;

	} catch (const std::exception& e) {
		std::cerr << "Snapshot Fetch Error: " << e.what() << "\n";
		return false;
	}
}

void BinanceSnapshotFetcher::parseSnapshot(const std::string& response) {
	// Parse JSON response
	auto json = boost::json::parse(response);

	lastUpdateId = json.at("lastUpdateId").as_int64();

	auto bidsJsonElement = json.at("bids").as_array();
	auto asksJsonElement = json.at("asks").as_array();

	for (auto& bid : bidsJsonElement) {
		bids.emplace_back(std::stold(bid.at(0).as_string().c_str()),
		                  std::stold(bid.at(1).as_string().c_str()));
	}

	for (auto& ask : asksJsonElement) {
		asks.emplace_back(std::stold(ask.at(0).as_string().c_str()),
		                  std::stold(ask.at(1).as_string().c_str()));
	}
}

int64_t BinanceSnapshotFetcher::getLastUpdateId() const {
	return lastUpdateId;
}

const Orders& BinanceSnapshotFetcher::getBids() const {
	return bids;
}

const Orders& BinanceSnapshotFetcher::getAsks() const {
	return asks;
}
