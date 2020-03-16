#ifndef DEFINEDEBUGGERHEADERGUARD
#define DEFINEDEBUGGERHEADERGUARD

#include <iostream>
#define SHOWDEBUG
#define SHOWINFO
#define SHOWWARNING
//#define SHOWLOGSOURCE

#define SHADERWARNINGS

#define INDEXBUFFERWARNING
#define VBLAYOUTWARNING

#define CAMERAWARNINGS


#define SHADERDEBUG

#define SHOWGLVERSION
#define SHOWMSAA
#define SHOWINITINFO


#ifdef SHOWDEBUG
#define LOG(stream, type, format, ...){\
	fprintf(stream, "%-10s ", type);\
	fprintf(stream, format, ##__VA_ARGS__);\
}
#else
#define LOG(stream, type, format, ...)
#endif

#define pERROR(format, ...){\
	fprintf(stderr, "%-10s ", "[ERROR]:");\
	fprintf(stderr, format, ##__VA_ARGS__);\
	fprintf(stderr, "\n|- %-7s file: \"%s\" ; function: \"%s\" : line: %d\n", "in:", __FILE__, __FUNCTION__, __LINE__);\
}

#ifdef SHOWWARNING
	#ifdef SHOWLOGSOURCE
		#define pWARNING(format, ...){\
			LOG(stderr, "[WARNING]:", format, ##__VA_ARGS__);\
			fprintf(stderr, "\n|- %-7s file: \"%s\" ; function: \"%s\" : line: %d\n", "in:", __FILE__, __FUNCTION__, __LINE__);\
		}
		#else
		#define pWARNING(format, ...){\
			LOG(stderr, "[WARNING]:", format, ##__VA_ARGS__);\
			fprintf(stderr, "\n");\
		}
		#endif
	#else
	#define pWARNING(format, ...)
#endif

#ifdef SHOWINFO
#define pINFO(format, ...){\
	LOG(stdout, "[INFO]:", format, ##__VA_ARGS__);\
	printf("\n");\
}
#else
#define pINFO(format, ...)
#endif

#endif