#include "nx_trace_impl.h"

using namespace std;
using namespace nxos;

NxTraceImpl::~NxTraceImpl()
{
}

void
NxTraceImpl::syslog (NxTrace::Priority prio,
                     const char *fmt, ...)
{
}

void
NxTraceImpl::event (const char *fmt, ...)
{
}

void
NxTraceImpl::error (const char *fmt, ...)
{
}
