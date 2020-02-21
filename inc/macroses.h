#ifndef MACROSES_H
#define MACROSES_H

#include <sys/cdefs.h>
#include <sys/_types.h>
#include <sys/_types/_pid_t.h>
#include <sys/_types/_id_t.h>
#include <sys/signal.h>
#include <sys/resource.h>

#define MX_W_INT(w)       (*(int *)&(w))
#define MX_WST(x)     (x & 0177)
#define MX_WIFEXIT(x)    (MX_WST(x) == 0)
#define MX_WIFSIG(x)  (MX_WST(x) != _WSTOPPED && MX_WST(x) != 0)
#define MX_WTERMSIG(x)     (MX_WST(x))
#define MX_EXSTATUS(x)  ((MX_W_INT(x) >> 8) & 0x000000ff)

#endif
