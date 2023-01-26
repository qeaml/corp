# corp

Various libraries I always end up rewriting multiple times.

Everything here is licensed under the [BSD 3-Clause License](LICENSE).

Most of these are prepared to be compiled as DLLs and are tested as such. I do
not know how to compile shared objects on other platforms, so be wary.

Compile using [`bip`][bip]. Remember to change the `cc` field in the
[recipe][recipe] if you're not using MSVC.

## trace

Essentially allows you to use custom tracebacks. Especially useful as part of
an abort handler for assertions. May be replacable by C++23's
[`<stacktrace>`][stacktrace] but I'm unsure of how that deals with signals
and whatnot. Trace is completely separate and must be manually implemented in
problematic areas, hence it does not suffer from any drawbacks of using the
stack trace directly.

Basic usage:

```cpp
#include "trace/trace.hpp"
// ...

int main(int, char**) {
  signal(SIGABRT, [](int){
    trace::dump(); // this will put the entire trace at the point where SIGABRT
                   // was caught and CLEAR it -- the trace will be completely
                   // emptied and all allocated memory released

    std::exit(100);
  });

  // this will essentially deepen the trace by a level with the given tag and
  // message combo. The tag pointer (the "main" in this case) will be stored
  // as-is, so use string literals here unless you want to leak memory. The
  // message supports formatting and is heap-allocated because of that. Every
  // tracer's message will be freed when it is popped of the trace, which
  // includes trace::dump()
  trace::push("main", "Long and error-prone operation #%d", opID);

  // ...

  trace::inspect(); // this will put the entire trace at this exact point onto
                    // stderr WITHOUT clearing it

  // ...

  assert(condition); // this will cause the SIGABRT handler to pick up failure
                     // and dump the trace

  // ...

  trace::pop(); // last tracer is popped, hence the trace goes up a level here
}
```

[stacktrace]: https://en.cppreference.com/w/cpp/header/stacktrace
[bip]: https://github.com/qeaml/bip
[recipe]: recipe.toml
