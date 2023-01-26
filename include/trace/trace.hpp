#pragma once

/*
trace.hpp
----------
Trace System Public API

This file is licensed under the BSD 3-Clause Lincense. See the LICENSE file for
details.
*/

#if defined(_WIN32) && defined(TRACE_BUILD)
  #define TRACE_API __declspec(dllexport)
#elif defined(_WIN32)
  #define TRACE_API __declspec(dllimport)
#else
  #define TRACE_API
#endif

#ifndef _MSC_BUILD
  #define TRACE_AFMT(a, b) __attribute__((__format__(printf, a, b)))
#else
  #define TRACE_AFMT(a, b)
#endif

/*
The Trace API is built to help debug apps.
*/
namespace trace {

// set the size of the buffer used to format messages. default is 1024
TRACE_API void useBufSize(size_t);

// clear the trace
TRACE_API void clear(void);
// push onto the trace with a formatted message
TRACE_API void TRACE_AFMT(2, 3) push(const char *tag, const char *msg, ...);
// push onto the trace without any message
TRACE_API void push(const char *tag);
// pop from the trace
TRACE_API void pop(void);

// output trace in a human-readable format to stderr
TRACE_API void inspect(void);
// output trace in a human-readable format to stderr and clear it
TRACE_API void dump(void);

}
