//
// メモリリーク位置検出用
// deleteされなかったnewの位置をデバッガに出力する
//

#include "stdapp.h"

#include <stdlib.h>
#include "debug.h"

#ifdef _DEBUG

#undef new

void* operator new(size_t nSize, const char *file, int line)
{
	return _malloc_dbg(nSize, _NORMAL_BLOCK, file, line);
}

void operator delete(void* p)
{
    _free_dbg(p, _NORMAL_BLOCK);
}

void* operator new[](size_t nSize, const char *file, int line)
{
	return _malloc_dbg(nSize, _NORMAL_BLOCK, file, line);
}

void operator delete[](void* p)
{
    _free_dbg(p, _NORMAL_BLOCK);
}

void operator delete(void* p,const char *file, int line)
{
    _free_dbg(p, _NORMAL_BLOCK);
}
void operator delete[](void* p,const char *file, int line)
{
    _free_dbg(p, _NORMAL_BLOCK);
}


#endif //_DEBUG

