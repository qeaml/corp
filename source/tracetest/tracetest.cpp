#include "trace/trace.hpp"
#include <iostream>
#include <cassert>
#include <csignal>

int main(int, char**) {
  signal(SIGABRT, [](int){
    std::cerr << "Abort! Trace:" << std::endl;
    trace::dump();
    std::exit(100);
  });

  trace::push("main", "Hello world!");
  assert(false);
  trace::pop();
}
