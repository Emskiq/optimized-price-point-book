#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/json.hpp>

#include <string>
#include <vector>
#include <iostream>

using tcp = boost::asio::ip::tcp;
namespace beast = boost::beast;
namespace http = beast::http;
namespace ssl = boost::asio::ssl;

using Price = long double;
using Quantity = long double;

using Order = std::pair<Price, Quantity>;
using Orders = std::vector<Order>;

class BinanceSnapshotFetcher final {
public:
	BinanceSnapshotFetcher(const std::string& ticker);

	bool fetchSnapshot();
	int64_t getLastUpdateId() const;
	const Orders& getBids() const;
	const Orders& getAsks() const;

private:
	std::string ticker;
	int64_t lastUpdateId;
	Orders bids;
	Orders asks;

	void parseSnapshot(const std::string& response);
};
