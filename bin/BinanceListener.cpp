#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <chrono>

#include "../src/BinanceWebSocketListener.h"
#include "../src/BinanceSnapshotFetcher.h"

#include "../src/OrderBooksCollection.h"


namespace net = boost::asio;

void printBestBidAsk(OrderBooksCollection& book, const std::string& ticker, net::io_context& ioc) {
	static net::steady_timer timer(ioc, std::chrono::seconds(3));

	timer.expires_after(std::chrono::seconds(3));
	timer.async_wait([&book, &ticker, &ioc](const boost::system::error_code& ec) {
		if (!ec) {
			auto bestBid = book.getBestBid(ticker);
			auto bestAsk = book.getBestAsk(ticker);

			if (bestBid) {
				std::cout << "Best Bid: Price = " << bestBid->first << ", Quantity = " << bestBid->second << "\n";
			} else {
				std::cout << "Best Bid: None\n";
			}

			if (bestAsk) {
				std::cout << "Best Ask: Price = " << bestAsk->first << ", Quantity = " << bestAsk->second << "\n";
			} else {
				std::cout << "Best Ask: None\n";
			}

			printBestBidAsk(book, ticker, ioc);
		}
	});
}

int main()
{
	const std::string ticker = "btcusdt";

	BinanceSnapshotFetcher snapshotFetcher(ticker);

	OrderBooksCollection book;

	book.addSymbol(ticker);

	if (snapshotFetcher.fetchSnapshot()) {
		std::cout << "Snapshot for " << ticker << " fetched successfully!\n";
		std::cout << "Last Update ID: " << snapshotFetcher.getLastUpdateId() << "\n";
	} else {
		std::cerr << "Failed to fetch snapshot.\n";
		return 1;
	}

	book.applyEvent(ticker, snapshotFetcher.getBids(), snapshotFetcher.getAsks());

	try {
		net::io_context ioc;
		net::ssl::context ctx{net::ssl::context::tlsv12_client};

		ctx.set_default_verify_paths();

		auto updateOrderBook = [&book] (const std::string& symbol, const Orders& bids, const Orders& asks) {
			book.applyEvent(symbol, bids, asks);
		};

		auto listener = std::make_shared<BinanceWebSocketListener>(ioc, ctx, ticker, snapshotFetcher.getLastUpdateId(), updateOrderBook);
		listener->run();

		printBestBidAsk(book, ticker, ioc);

		ioc.run();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
