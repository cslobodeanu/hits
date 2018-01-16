#ifndef __MORGANA_BASE_MEMORY_MACROS_H_INCLUDED__
#define __MORGANA_BASE_MEMORY_MACROS_H_INCLUDED__

#define SAFE_DEL(x) do {if (x) {delete x; x = NULL;}} while (0)
#define SAFE_DEL_ARRAY(x) do {if (x) {delete[] x; x = NULL;}} while (0)

#define NEW		new

#endif