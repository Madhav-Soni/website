#include "./AppComponent.hpp"
#include "./controller/ApiController.hpp"

#include "oatpp/network/Server.hpp"

#include <iostream>

void run() {
  
  AppComponent components;

  OATPP_COMPONENT(std::shared_ptr<oatpp::web::server::HttpRouter>, router);

  auto apiController = ApiController::createShared();
  router->addController(apiController);

  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ConnectionHandler>, connectionHandler);
  OATPP_COMPONENT(std::shared_ptr<oatpp::network::ServerConnectionProvider>, connectionProvider);

  oatpp::network::Server server(connectionProvider, connectionHandler);

  OATPP_LOGI("Oat++ Showcase", "======================================");
  OATPP_LOGI("Oat++ Showcase", "Server running on port %s", connectionProvider->getProperty("port").getData());
  OATPP_LOGI("Oat++ Showcase", "======================================");
  OATPP_LOGI("Oat++ Showcase", "Endpoints available:");
  OATPP_LOGI("Oat++ Showcase", "  GET  /api/v1/hello");
  OATPP_LOGI("Oat++ Showcase", "  GET  /api/v1/stats");
  OATPP_LOGI("Oat++ Showcase", "  GET  /api/v1/examples");
  OATPP_LOGI("Oat++ Showcase", "  GET  /api/v1/benchmarks");
  OATPP_LOGI("Oat++ Showcase", "  GET  /api/v1/features");
  OATPP_LOGI("Oat++ Showcase", "  POST /api/v1/subscribe");
  OATPP_LOGI("Oat++ Showcase", "  POST /api/v1/contact");
  OATPP_LOGI("Oat++ Showcase", "  GET  /api/health");
  OATPP_LOGI("Oat++ Showcase", "======================================");

  server.run();
}

int main() {
  
  oatpp::base::Environment::init();

  run();

  oatpp::base::Environment::destroy();
  
  return 0;
}