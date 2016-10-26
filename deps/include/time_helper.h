#ifndef TIME_HELPER_H_8FVB3L0U
#define TIME_HELPER_H_8FVB3L0U

#ifndef DISABLE_TIMING
#include <ctime>
#include <cstdio>

#ifdef _MSC_VER
#define __TIC__() double __timing_start = clock()
#define __TOC__()                                                 \
	do {                                                      \
		double __timing_end = clock();            \
		fprintf(stdout, "TIME(ms): %lf\n",                \
			(__timing_end - __timing_start)   \
				/ CLOCKS_PER_SEC * 1000);         \
	} while (0)
#else
#include <unistd.h>
#include <sys/time.h>

#define __TIC__()                                    \
	struct timeval __timing_start, __timing_end; \
	gettimeofday(&__timing_start, NULL);

#define __TOC__()                                                        \
	do {                                                             \
		gettimeofday(&__timing_end, NULL);                       \
		double __timing_gap = (__timing_end.tv_sec -     \
					       __timing_start.tv_sec) *  \
					      1000.0 +                     \
				      (__timing_end.tv_usec -    \
					       __timing_start.tv_usec) / \
					      1000.0;                    \
		fprintf(stdout, "TIME(ms): %lf\n", __timing_gap);        \
		gettimeofday(&__timing_start, NULL);	\
	} while (0)

#endif

#else
#define __TIC__()
#define __TOC__()
#endif

#endif /* end of include guard: IMAGEHELPER_HPP_8FVB3L0U */
