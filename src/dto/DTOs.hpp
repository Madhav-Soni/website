#ifndef DTOs_hpp
#define DTOs_hpp

#include "oatpp/core/macro/codegen.hpp"
#include "oatpp/core/Types.hpp"

#include OATPP_CODEGEN_BEGIN(DTO)

class MessageDto : public oatpp::DTO {
  DTO_INIT(MessageDto, DTO)
  DTO_FIELD(String, message);
  DTO_FIELD(String, framework);
  DTO_FIELD(String, version);
};

class StatsDto : public oatpp::DTO {
  DTO_INIT(StatsDto, DTO)
  DTO_FIELD(Int64, totalRequests);
  DTO_FIELD(Int64, uptime);
  DTO_FIELD(String, version);
  DTO_FIELD(Float32, memoryUsageMB);
  DTO_FIELD(Int32, activeConnections);
};

class ExampleDto : public oatpp::DTO {
  DTO_INIT(ExampleDto, DTO)
  DTO_FIELD(String, id);
  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(String, code);
  DTO_FIELD(String, language);
  DTO_FIELD(String, category);
};

class SubscribeDto : public oatpp::DTO {
  DTO_INIT(SubscribeDto, DTO)
  DTO_FIELD(String, email);
  DTO_FIELD(String, name);
};

class ContactDto : public oatpp::DTO {
  DTO_INIT(ContactDto, DTO)
  DTO_FIELD(String, name);
  DTO_FIELD(String, email);
  DTO_FIELD(String, message);
};

class ResponseDto : public oatpp::DTO {
  DTO_INIT(ResponseDto, DTO)
  DTO_FIELD(Boolean, success);
  DTO_FIELD(String, message);
  DTO_FIELD(String, data);  // Changed from Object<oatpp::Any> to String
};

class BenchmarkDto : public oatpp::DTO {
  DTO_INIT(BenchmarkDto, DTO)
  DTO_FIELD(String, framework);
  DTO_FIELD(Int64, requestsPerSecond);
  DTO_FIELD(Float32, latencyMs);
  DTO_FIELD(Int32, memoryMB);
  DTO_FIELD(String, category);
};

class FeatureDto : public oatpp::DTO {
  DTO_INIT(FeatureDto, DTO)
  DTO_FIELD(String, id);
  DTO_FIELD(String, title);
  DTO_FIELD(String, description);
  DTO_FIELD(String, icon);
  DTO_FIELD(List<String>, benefits);
};

class DocsSectionDto : public oatpp::DTO {
  DTO_INIT(DocsSectionDto, DTO)
  DTO_FIELD(String, id);
  DTO_FIELD(String, title);
  DTO_FIELD(String, content);
  DTO_FIELD(String, category);
  DTO_FIELD(List<String>, tags);
};

#include OATPP_CODEGEN_END(DTO)

#endif