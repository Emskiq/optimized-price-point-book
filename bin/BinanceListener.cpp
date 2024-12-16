#include "../src/BinanceWebSocketListener.h"

int main()
{
	try {
		net::io_context ioc;
		net::ssl::context ctx{net::ssl::context::tlsv12_client};

		ctx.set_default_verify_paths();
		const std::string symbol = "bnbbtc";

		auto listener = std::make_shared<BinanceWebSocketListener>(ioc, ctx, symbol);
		listener->run();

		ioc.run();
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}
