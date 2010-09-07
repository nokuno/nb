
/********************************************************************************

	nb.h
	よく修正されるヘッダはここでインクルードする
	ソースファイルではstdapp.hとnb.hをインクルードすれば
	全てのヘッダがインクルードされる

********************************************************************************/
#ifndef _INC_NB_H_
#define _INC_NB_H_
//相互依存を実現するときに必要なクラス名宣言
class CMysImageEx;
class CMysSoundEx;
class CPlayer;
class CObj;
class CItem;
class CBlock;
class CFire;
class CErase;
class CBomb;
#include "CConfig\\syscfg.h"
#include "CMysImageEx.h"
#include "CMysSoundEx.h"
#include "CMysMidiEx.h"

#include "valuedefine.h"
#include "func.h"
#include "CPlayer.h"
#include "CObj.h"
#include "CItem.h"
#include "CBlock.h"
#include "CFire.h"
#include "CErase.h"
#include "CBomb.h"
#include "CMap.h"
#include "CTitle.h"
#include "CSelectPlayer.h"
#include "CSelectStage.h"
#include "CShowResult.h"
#include "CMainGame.h"

#include "global.h"
#include "AIRutin.h"

#endif