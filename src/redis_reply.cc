#include "redis_reply.h"

namespace Redis {

void Reply(evbuffer *output, const std::string &data) {
  evbuffer_add(output, data.c_str(), data.length());
}

std::string SimpleString(std::string data) { return "+" + data + CRLF; }

std::string Error(std::string err) { return "-" + err + CRLF; }

std::string Integer(int64_t data) { return ":" + std::to_string(data) + CRLF; }

std::string BulkString(std::string data) {
  if (!data.empty()) {
    return "$" + std::to_string(data.length()) + CRLF + data + CRLF;
  }
  return NilString();
}

std::string NilString() {
  return "$-1\r\n";
}

std::string MultiLen(int64_t len) {
  return "*"+std::to_string(len)+"\r\n";
}

std::string MultiBulkString(std::vector<std::string> list) {
  for (size_t i = 0; i < list.size(); i++) {
    if (list[i].empty()) {
      list[i] = NilString();
    }  else {
      list[i] = BulkString(list[i]);
    }
  }
  return Array(list);
}

std::string Array(std::vector<std::string> list) {
  auto buf = "*" + std::to_string(list.size()) + CRLF;
  for (const auto &s : list) {
    buf += s;
  }
  return buf;
}

} // namespace Redis