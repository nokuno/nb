
#ifndef _INC_CMYSLIB2_H_
#define _INC_CMYSLIB2_H_

#include "Mys\\CMysLib.h"

inline word MAKERGB16(byte r, byte g, byte b)
{
	return (word)(r&0xF8)<<11 | (word)(g&0xFC)<<5 | (word)(b&0xF8);
}

class CMysLib2 : public CMysLib
{
public:
	bool InitApp(HINSTANCE hInstance);
	void EndApp();
	bool ReadSysIni();
	bool WriteSysIni();
	
protected:

};

#endif //_INC_CMYSLIB2_H_