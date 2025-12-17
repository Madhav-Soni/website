#ifndef ApiController_hpp
#define ApiController_hpp

#include "dto/DTOs.hpp"
#include "oatpp/web/server/api/ApiController.hpp"
#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/macro/component.hpp"
#include <chrono>
#include <random>

#include OATPP_CODEGEN_BEGIN(ApiController)

class ApiController : public oatpp::web::server::api::ApiController {
private:
  std::chrono::steady_clock::time_point startTime;
  std::atomic<int64_t> requestCounter{0};
  std::atomic<int32_t> activeConnections{0};

  float getRandomMemoryUsage() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<> dis(10.0, 15.0);
    return static_cast<float>(dis(gen));
  }

  // Helper to add CORS headers to any response
  void addCorsHeaders(const std::shared_ptr<OutgoingResponse>& response) {
    response->putHeader("Access-Control-Allow-Origin", "*");
    response->putHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    response->putHeader("Access-Control-Allow-Headers", "Content-Type, Authorization");
  }

public:
  ApiController(OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper))
    : oatpp::web::server::api::ApiController(objectMapper)
    , startTime(std::chrono::steady_clock::now())
  {}

  static std::shared_ptr<ApiController> createShared(
    OATPP_COMPONENT(std::shared_ptr<ObjectMapper>, objectMapper)
  ){
    return std::make_shared<ApiController>(objectMapper);
  }

  ENDPOINT("OPTIONS", "*", cors) {
    auto response = createResponse(Status::CODE_204, "");
    addCorsHeaders(response);
    return response;
  }

  ENDPOINT("GET", "/", root) {
    auto response = createResponse(Status::CODE_200, 
      "Oat++ Showcase API v1.0 - Visit /api/v1/hello to get started");
    addCorsHeaders(response);
    return response;
  }

  ENDPOINT("GET", "/api/v1/hello", hello) {
    requestCounter++;
    auto dto = MessageDto::createShared();
    dto->message = "Hello from Oat++! 🚀";
    dto->framework = "Oat++";
    dto->version = "1.3.1";
    
    auto response = createDtoResponse(Status::CODE_200, dto);
    addCorsHeaders(response);
    return response;
  }

  ENDPOINT("GET", "/api/v1/stats", stats) {
    requestCounter++;
    auto dto = StatsDto::createShared();
    
    auto now = std::chrono::steady_clock::now();
    auto uptime = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
    
    dto->totalRequests = requestCounter.load();
    dto->uptime = uptime;
    dto->version = "1.3.1";
    dto->memoryUsageMB = getRandomMemoryUsage();
    dto->activeConnections = activeConnections.load();
    
    auto response = createDtoResponse(Status::CODE_200, dto);
    addCorsHeaders(response);
    return response;
  }

  ENDPOINT("GET", "/api/v1/examples", examples) {
    requestCounter++;
    
    auto list = oatpp::List<oatpp::Object<ExampleDto>>::createShared();
    
    // Example 1
    auto ex1 = ExampleDto::createShared();
    ex1->id = "hello-world";
    ex1->title = "Hello World Endpoint";
    ex1->description = "Create your first REST endpoint in Oat++";
    ex1->code = R"(ENDPOINT("GET", "/hello", hello) {
  auto dto = MessageDto::createShared();
  dto->message = "Hello World!";
  return createDtoResponse(Status::CODE_200, dto);
})";
    ex1->language = "cpp";
    ex1->category = "basics";
    list->push_back(ex1);
    
    // Example 2
    auto ex2 = ExampleDto::createShared();
    ex2->id = "post-endpoint";
    ex2->title = "POST Request Handler";
    ex2->description = "Handle POST requests with DTOs";
    ex2->code = R"(ENDPOINT("POST", "/users", createUser,
         BODY_DTO(Object<UserDto>, userDto)) {
  // Validate and process user data
  if(!userDto->email) {
    return createResponse(Status::CODE_400, "Email required");
  }
  return createDtoResponse(Status::CODE_201, userDto);
})";
    ex2->language = "cpp";
    ex2->category = "rest-api";
    list->push_back(ex2);
    
    // Example 3
    auto ex3 = ExampleDto::createShared();
    ex3->id = "path-params";
    ex3->title = "Path Parameters";
    ex3->description = "Extract parameters from URL paths";
    ex3->code = R"(ENDPOINT("GET", "/users/{userId}", getUserById,
         PATH(Int32, userId)) {
  auto dto = UserDto::createShared();
  dto->id = userId;
  dto->name = "User " + std::to_string(userId);
  return createDtoResponse(Status::CODE_200, dto);
})";
    ex3->language = "cpp";
    ex3->category = "rest-api";
    list->push_back(ex3);
    
    // Example 4
    auto ex4 = ExampleDto::createShared();
    ex4->id = "query-params";
    ex4->title = "Query Parameters";
    ex4->description = "Handle URL query parameters";
    ex4->code = R"(ENDPOINT("GET", "/search", search,
         QUERY(String, q),
         QUERY(Int32, limit, "limit")) {
  auto response = ResultDto::createShared();
  response->query = q;
  response->limit = limit ? limit : 10;
  return createDtoResponse(Status::CODE_200, response);
})";
    ex4->language = "cpp";
    ex4->category = "rest-api";
    list->push_back(ex4);
    
    // Example 5
    auto ex5 = ExampleDto::createShared();
    ex5->id = "async-handler";
    ex5->title = "Async Request Handler";
    ex5->description = "Non-blocking asynchronous endpoints";
    ex5->code = R"(ENDPOINT_ASYNC("GET", "/async", Async) {
  ENDPOINT_ASYNC_INIT(Async)
  
  Action act() override {
    auto dto = MessageDto::createShared();
    dto->message = "Async response";
    return _return(controller->createDtoResponse(
      Status::CODE_200, dto));
  }
};)";
    ex5->language = "cpp";
    ex5->category = "advanced";
    list->push_back(ex5);
    
    // Example 6
    auto ex6 = ExampleDto::createShared();
    ex6->id = "websocket";
    ex6->title = "WebSocket Connection";
    ex6->description = "Real-time bidirectional communication";
    ex6->code = R"(ENDPOINT("GET", "/ws", ws,
         REQUEST(std::shared_ptr<IncomingRequest>, request)) {
  return oatpp::websocket::Handshaker::serversideHandshake(
    request->getHeaders(),
    std::make_shared<WSListener>()
  );
})";
    ex6->language = "cpp";
    ex6->category = "websocket";
    list->push_back(ex6);
    
    auto response = createDtoResponse(Status::CODE_200, list);
    addCorsHeaders(response);
    return response;
  }

  ENDPOINT("POST", "/api/v1/subscribe", subscribe,
           BODY_DTO(Object<SubscribeDto>, dto)) {
    requestCounter++;
    
    auto response = ResponseDto::createShared();
    
    if(!dto->email || dto->email->empty()) {
      response->success = false;
      response->message = "Email is required";
      auto resp = createDtoResponse(Status::CODE_400, response);
      addCorsHeaders(resp);
      return resp;
    }
    
    // Email validation regex
    std::string email = dto->email->c_str();
    if(email.find('@') == std::string::npos) {
      response->success = false;
      response->message = "Invalid email format";
      auto resp = createDtoResponse(Status::CODE_400, response);
      addCorsHeaders(resp);
      return resp;
    }
    
    response->success = true;
    response->message = "Successfully subscribed to newsletter!";
    auto resp = createDtoResponse(Status::CODE_200, response);
    addCorsHeaders(resp);
    return resp;
  }

  ENDPOINT("POST", "/api/v1/contact", contact,
           BODY_DTO(Object<ContactDto>, dto)) {
    requestCounter++;
    
    auto response = ResponseDto::createShared();
    
    if(!dto->email || !dto->name || !dto->message) {
      response->success = false;
      response->message = "All fields are required";
      auto resp = createDtoResponse(Status::CODE_400, response);
      addCorsHeaders(resp);
      return resp;
    }
    
    response->success = true;
    response->message = "Message sent successfully! We'll get back to you soon.";
    auto resp = createDtoResponse(Status::CODE_200, response);
    addCorsHeaders(resp);
    return resp;
  }

  ENDPOINT("GET", "/api/v1/benchmarks", benchmarks) {
    requestCounter++;
    
    auto list = oatpp::List<oatpp::Object<BenchmarkDto>>::createShared();
    
    auto oatpp = BenchmarkDto::createShared();
    oatpp->framework = "Oat++";
    oatpp->requestsPerSecond = 500000;
    oatpp->latencyMs = 0.2f;
    oatpp->memoryMB = 12;
    oatpp->category = "cpp";
    list->push_back(oatpp);
    
    auto express = BenchmarkDto::createShared();
    express->framework = "Express.js";
    express->requestsPerSecond = 25000;
    express->latencyMs = 8.0f;
    express->memoryMB = 45;
    express->category = "javascript";
    list->push_back(express);
    
    auto fastapi = BenchmarkDto::createShared();
    fastapi->framework = "FastAPI";
    fastapi->requestsPerSecond = 18000;
    fastapi->latencyMs = 12.0f;
    fastapi->memoryMB = 38;
    fastapi->category = "python";
    list->push_back(fastapi);
    
    auto spring = BenchmarkDto::createShared();
    spring->framework = "Spring Boot";
    spring->requestsPerSecond = 22000;
    spring->latencyMs = 10.0f;
    spring->memoryMB = 150;
    spring->category = "java";
    list->push_back(spring);
    
    auto aspnet = BenchmarkDto::createShared();
    aspnet->framework = "ASP.NET Core";
    aspnet->requestsPerSecond = 85000;
    aspnet->latencyMs = 2.5f;
    aspnet->memoryMB = 35;
    aspnet->category = "csharp";
    list->push_back(aspnet);
    
    auto resp = createDtoResponse(Status::CODE_200, list);
    addCorsHeaders(resp);
    return resp;
  }

  ENDPOINT("GET", "/api/v1/features", features) {
    requestCounter++;
    
    auto list = oatpp::List<oatpp::Object<FeatureDto>>::createShared();
    
    auto f1 = FeatureDto::createShared();
    f1->id = "performance";
    f1->title = "Blazing Fast Performance";
    f1->description = "Handle millions of requests per second with minimal latency";
    f1->icon = "zap";
    f1->benefits = {"500K+ req/sec", "Sub-millisecond latency", "Low CPU usage"};
    list->push_back(f1);
    
    auto f2 = FeatureDto::createShared();
    f2->id = "lightweight";
    f2->title = "Zero Dependencies";
    f2->description = "Pure C++ framework with no external dependencies";
    f2->icon = "feather";
    f2->benefits = {"Small binary size", "Easy deployment", "No bloat"};
    list->push_back(f2);
    
    auto f3 = FeatureDto::createShared();
    f3->id = "typesafe";
    f3->title = "Type-Safe DTOs";
    f3->description = "Compile-time type checking for all API endpoints";
    f3->icon = "shield";
    f3->benefits = {"Catch errors early", "Auto serialization", "Clear API contracts"};
    list->push_back(f3);
    
    auto f4 = FeatureDto::createShared();
    f4->id = "async";
    f4->title = "Async/Await Support";
    f4->description = "Non-blocking I/O for maximum concurrency";
    f4->icon = "cpu";
    f4->benefits = {"Handle 10K+ connections", "Efficient resource use", "Scalable design"};
    list->push_back(f4);
    
    auto resp = createDtoResponse(Status::CODE_200, list);
    addCorsHeaders(resp);
    return resp;
  }

  ENDPOINT("GET", "/api/health", health) {
    requestCounter++;
    auto response = ResponseDto::createShared();
    response->success = true;
    response->message = "Server is healthy";
    auto resp = createDtoResponse(Status::CODE_200, response);
    addCorsHeaders(resp);
    return resp;
  }

  ENDPOINT("GET", "/api/v1/docs/{section}", getDocs,
           PATH(String, section)) {
    requestCounter++;
    
    auto dto = DocsSectionDto::createShared();
    dto->id = section;
    dto->title = "Documentation: " + section;
    dto->content = "Documentation content for " + section + " section";
    dto->category = "documentation";
    dto->tags = {"oatpp", "cpp", "web-framework"};
    
    auto resp = createDtoResponse(Status::CODE_200, dto);
    addCorsHeaders(resp);
    return resp;
  }
};

#include OATPP_CODEGEN_END(ApiController)

#endif