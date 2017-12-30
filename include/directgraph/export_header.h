#ifndef DIRECTGRAPH_EXPORT_HEADER_H
#define DIRECTGRAPH_EXPORT_HEADER_H

#ifdef DIRECTGRAPH_STATIC_DEFINE
#  define DIRECTGRAPH_EXPORT
#  define DIRECTGRAPH_NO_EXPORT
#else
#  ifdef DIRECTGRAPH_EXPORTS
#    define DIRECTGRAPH_EXPORT __declspec(dllexport)
#    define DIRECTGRAPH_NOMAIN
#  else
#    define DIRECTGRAPH_EXPORT __declspec(dllimport)
#  endif
#endif

#endif //DIRECTGRAPH_EXPORT_HEADER_H
