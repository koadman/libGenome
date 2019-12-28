#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "libGenome/gnDefs.h"

// some compilers don't have abs() for 64 bit ints
#if (defined(__GNUG__) && ( __GNUC__ <= 2 )) || defined(__INTEL_COMPILER) || (defined _MSC_VER && defined __cplusplus)
int64 abs( int64 a ){
	return a < 0 ? -a : a;
}
#endif
