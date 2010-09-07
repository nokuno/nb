
#ifndef _INC_CPLAYER_H_
#define _INC_CPLAYER_H_

#include <vector>
using namespace std;
//方向キー入力タイプ
enum DIRECTION
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	NEUTRAL,
};

//AI用行動リスト
enum ACTION
{
	ACT_UP,
	ACT_DOWN,
	ACT_LEFT,
	ACT_RIGHT,
	ACT_NEUTRAL,
	ACT_BOMB,
	ACT_NULL,
};
//行動とその時の評価のセット
typedef struct
{
	ACTION	act;	//行動
	int		value;	//評価点
}ACTDATA,*LPACTDATA;
//プレイヤークラス
class CPlayer
{
public:
	CPlayer(){};
	~CPlayer(){};
	void StepFrame();	//フレーム処理
	void Render();		//描画処理
	void Damage(int damage,DIRECTION d,int bombIndex);		//ダメージ

	//ここからインラインメンバ関数
	void Initialize(int i){ReadMember(i);InitMember();};
	int GetFire(){return State.fire;};
	void DecBombNum(){m_bombNum--;};//デクリメント
	void IncBombNum(){m_bombNum++;};//インクリメント
	int GetBombNum(){return m_bombNum;};
	int GetX(){return m_x;};
	int GetY(){return m_y;};
	bool GetFlag(){return m_existFlag;};
	bool GetMuteki(){return m_mutekiCount!=0;};
	void IncSpeed(){if(State.speed<8)State.speed++;};
	void IncFire(){State.fire++;};
	void IncBomb(){State.bomb++;};
	void SetOnIceFlag(){m_iceFlag = 2;};
	void SetOffIceFlag(){if(m_iceFlag>0)m_iceFlag = 0;};
	//ここからstaticメンバ関数
	static void InitStatic();
private:
	bool m_existFlag;			//存在フラグ
	bool m_fCom;				//コンピュータであるかどうかのフラグ
	CMysImageEx		m_cimg;
	CMysImageEx		m_cimgDamage;//ダメージ画像
	int m_number;				//プレイヤー番号
	DIRECTION devdir;			//方向キー、後押し優先でひとつだけ認識
	bool devBtn[4];				//ボタン入力

	int m_x,m_y;				//座標
	int m_anmCount;				//アニメカウンタ：フラグが変わるまで数える：０からMAX_ANMCOUNT-1まで
	int m_anmFrame;				//アニメフレーム：画像の左から何番目のフレームか：０からMAX_ANMFRAME-1の間
	DIRECTION m_direct;			//向き

	int m_bombNum;				//現在爆弾数：０からState.bombの間
	int m_bombCount;			//爆弾カウンタの初期状態：通常は０
	int m_bombKind;				//爆弾の種類：通常は０

	bool m_damageFlag;			//吹っ飛び中フラグ（方向はm_directを参照のこと）
	int m_damageBombIndex;		//ダメージを与えた爆弾の識別番号
	int m_damageCount;			//ダメージを与えた爆弾の爆風が消える時間（当たり判定に必要）

	int m_iceFlag;				//氷で滑ってるフラグ
	int m_mutekiCount;			//無敵カウンタ：０からMAX_MUTEKICOUNTまで

	struct PLAYERSTATE			//プレイヤーのステータス状態
	{
		int hp;
		int bomb;
		int fire;
		int speed;
	}State,maxState,initState;	//現在、最大、初期のステータス

	//ここからヘルパー関数
	void Die();			//死亡
	void InitMember();				//メンバー初期化
	void ReadMember(int i);			//メンバー初期値読み込み(プレイヤー番号）

	DIRECTION GetDevDir();			//方向キー入力所得
	void OnB1();					//ノーマル爆弾のボタン
	void OnUpMap();					//CMapの上にいるとき
	//ここから移動用関数
	void Move();					//移動
	void Blow();					//吹き飛び
	void Slip();					//氷の上ですべる
		bool OnLeft(int);				//方向キー入力。返り値はアニメフラグ
		bool OnRight(int);
		bool OnUp(int);
		bool OnDown(int);
		void AnmRgst(bool anmFlag);		//アニメ登録。引数はアニメフラグ
		bool ObjColPlayer(int index);	//指定オブジェクトが通過可能か
	//ここからインライン
	void RoundCoordinats(){m_x=round((float)m_x/32)*32;m_y=round((float)m_y/32)*32;};//座標を四捨五入
	//ここからstaticメンバ
	static CMysSoundEx	s_sndDeath;		//死の悲鳴（笑
	static CMysImageEx	s_imgKage;		//影画像
	//ここから定数
	static const int MAX_ANMCOUNT;		//アニメカウンタの最大値
	static const int MAX_ANMFRAME;		//アニメフレームの数
	static const int MAX_MUTEKI_COUNT;	//無敵時間
	static const int MAX_CORRECT;		//最大移動補正量
	static const int MUTEKI_DAMAGE;
	static const int BLOW_SPEED;

	//ここからＡＩ関係
	ACTION m_act;		//現在のアクション
	CBomb* m_pFirstBomb;//最初の爆弾
	static int s_cpuLevel;		//ＣＰＵレベル
	void AIRutin();		//全てのＡＩ呼び出し元
	bool Serch(int,int,int,ACTION,ACTION,int,int,int,int);		//探索の再帰関数
	void Action();		//m_actに基づき行動
public:
	CBomb* GetBombPointer(){return m_pFirstBomb;};
	void InitBombPointer(){m_pFirstBomb = NULL;};
};

#endif

/*
	座標について
プレイヤーで使う座標は三種類。

１．スクリーン座標sx,sy(描画時のみ）
２．相対座標rx,ry（総合管理、m_x,m_yなど）
３．ブロック座標bx,by（INIファイルの記述など）

3->2
rx = bx * 32;
ry = by * 32;
2->1
sx = rx + MAP_X;
sy = ry + MAP_Y - 32;//縦に長いぶん上になる
2->3
bx = rx / 32;
by = ry / 32;

爆弾とかアイテムとか、座標がきっちりブロック座標のときはブロック座標を使うことにする。
ってか２次元配列にするからその添え字がブロック座標になる。
*/
