#include "ChromaLogger.h"
#include <stdarg.h>


using namespace ChromaSDK;


void ChromaLogger::printf(const char* format, ...)
{
#if _DEBUG
	va_list args;
	va_start(args, format);
	::printf(format, args);
	va_end(args);
#endif
}

void ChromaLogger::fprintf(FILE* stream, const char* format, ...)
{
#if _DEBUG
	va_list args;
	va_start(args, format);
	::fprintf(stream, format, args);
	va_end(args);
#endif
}
