//===================================================
//
// プレイヤーヘッダー
// Author:Teruto Sato
//
//===================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_	//二重インクルード防止

//--------------------------------
// インクルード
//--------------------------------
#include "object3D.h"

//================================
// プレイヤークラスの定義
//================================
class CPlayer : public CObject3D
{
public:
	CPlayer();				//コンストラクタ
	~CPlayer() override;	//デストラクタ

	//--------------------
	// メンバ関数
	//--------------------
	void Init() override;	// 初期化
	void Uninit() override;	// 終了
	void Update() override;	// 更新
	void Draw() override;	// 描画
	void SetKill();	// キルの設定

	//--------------------
	// 静的メンバ変数
	//--------------------
	static CPlayer* Create();	//生成
	static bool GetKill();	// キルされたかどうか
	static void SetLife();
	static int GetLife();

private:
	//--------------------
	// プライベート関数
	//--------------------
	D3DXVECTOR3 Move(int nUp, int nDown, int nLeft, int nRight);	//移動

private:
	//--------------------
	// 静的メンバ変数
	//--------------------
	static bool m_kill;	// キルされたか
	static int m_life;

	//--------------------
	// メンバ変数
	//--------------------
	D3DXVECTOR3 m_pos;	//位置
	D3DXVECTOR3 m_move;	//移動量
	D3DXVECTOR3 m_size;	//大きさ
	int m_time;
	bool m_interval;
};

#endif