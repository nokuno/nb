
#ifndef _INC_DEBUG_
#define _INC_DEBUG_

#ifdef _DEBUG

#include <crtdbg.h>

void* operator new(size_t nSize, const char *file, int line);
void operator delete(void* p);

void* operator new[](size_t nSize, const char *file, int line);
void operator delete[](void* p);

void operator delete(void* p,const char *file, int line);
void operator delete[](void* p,const char *file, int line);

#define new new(__FILE__, __LINE__)

#endif //_DEBUG

#endif //_INC_DEBUG_