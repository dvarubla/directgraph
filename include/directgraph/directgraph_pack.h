#ifndef DIRECTGRAPH_PACK_H
#define DIRECTGRAPH_PACK_H

#ifdef _MSC_VER
#define DIRECTGRAPH_PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
#else
#define DIRECTGRAPH_PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
#endif

#endif