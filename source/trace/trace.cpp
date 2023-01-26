#define TRACE_BUILD
#include "trace/trace.hpp"
#include <queue>
#include <optional>
#include <chrono>
#include <cstdarg>
#include <cstdio>

#define DEFAULT_BUF_SIZE 1024
#define TRACER_FMT_MSG     "  %s `%s`\n"
#define TRACER_FMT_MSGLESS "  %s\n"

namespace trace {

using clock = std::chrono::steady_clock;
using time = clock::time_point;

struct Tracer {
  const char *tag;
  std::optional<const char *> msg;
  time timestamp;

  void free() {
    if(msg)
      delete[] *msg;
  }

  void print() {
    if(msg)
      fprintf(stderr, TRACER_FMT_MSG, tag, *msg);
    else
      fprintf(stderr, TRACER_FMT_MSGLESS, tag);
  }
};

std::queue<Tracer> gQ;

size_t gBufSize = DEFAULT_BUF_SIZE;

TRACE_API void clear() {
  while(!gQ.empty()) {
    auto &tracer = gQ.front();
    tracer.free();
    gQ.pop();
  }
}

TRACE_API void push(const char *tag, const char *fmt, ...) {
  auto msgBuf = new char[gBufSize];

  va_list ap;
  va_start(ap, fmt);
  vsnprintf(msgBuf, gBufSize, fmt, ap);
  va_end(ap);

  gQ.push({tag, msgBuf, clock::now()});
}

TRACE_API void push(const char *tag) {
  gQ.push({tag, std::nullopt, clock::now()});
}

TRACE_API void pop() {
  gQ.pop();
}

TRACE_API void inspect() {
  const auto& ref = gQ;
  auto copy = std::queue(ref);
  while(!copy.empty()) {
    auto &tracer = copy.front();
    tracer.print();
    copy.pop();
  }
}

TRACE_API void dump() {
  fprintf(stderr, "Trace:\n");
  while(!gQ.empty()) {
    auto &tracer = gQ.front();
    tracer.print();
    tracer.free();
    gQ.pop();
  }
}

}
