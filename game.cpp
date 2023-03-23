//**************************************************
// 
// game.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "game.h"
#include "sound.h"
#include "input.h"
#include "application.h"
#include "fade.h"
#include "object3D.h"
#include "effect.h"
#include "player.h"
#include "enemy_manager.h"
#include "circle.h"
#include "number_manager.h"

//==================================================
// 静的メンバ変数宣言
//==================================================
CPlayer* CGame::m_pPlayer = nullptr;	//プレイヤー
CCircle* CGame::m_pCircle = nullptr;	//プレイヤー

//==================================================
// 定義
//==================================================
namespace
{
}

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CGame::CGame()
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CGame::~CGame()
{
}

//--------------------------------------------------
// プレイヤーの取得
//--------------------------------------------------
CPlayer* CGame::GetPlayer()
{
	return m_pPlayer;
}

//--------------------------------------------------
// プレイヤーの取得
//--------------------------------------------------
CCircle* CGame::GetCircle()
{
	return m_pCircle;
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void CGame::Init()
{
	m_time = 0;

	{// 背景
		CObject3D* pObj = CObject3D::Create();

		// 位置の設定
		pObj->SetPos(D3DXVECTOR3(0.0f, 0.0f, 0.0f));

		// サイズの設定
		pObj->SetSize(D3DXVECTOR3((float)CApplication::SCREEN_WIDTH, (float)CApplication::SCREEN_HEIGHT, 0.0f));

		// 色の設定
		pObj->SetCol(D3DXCOLOR(0.25f, 0.25f, 0.25f, 1.0f));

		// テクスチャの設定
		pObj->SetTexture(CTexture::LABEL_NONE);
	}

	{// スコア
		D3DXVECTOR3 size = D3DXVECTOR3(50.0f, 100.0f, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3((float)CApplication::SCREEN_WIDTH - 30.0f, size.y * 0.65f, 0.0f);

		// 生成
		m_pTime = CNumberManager::Create(pos, size, 30);

		//スコア用UIの生成
		CObject3D* pScore = CObject3D::Create();

		// 位置の設定
		pScore->SetPos(D3DXVECTOR3(420.0f, 290.0f, 0.0f));

		// サイズの設定
		pScore->SetSize(D3DXVECTOR3(170.0f, 170.0f, 0.0f));

		// 色の設定
		pScore->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// テクスチャの設定
		pScore->SetTexture(CTexture::LABEL_UI_Stake);
	}

	{// タイム
		D3DXVECTOR3 size = D3DXVECTOR3(50.0f, 100.0f, 0.0f);
		D3DXVECTOR3 pos = D3DXVECTOR3((float)CApplication::SCREEN_WIDTH * 0.27f, size.y * 0.65f, 0.0f);

		// 生成
		m_pScore = CNumberManager::Create(pos, size, 0);

		//タイム用UIの生成
		CObject3D* pTime = CObject3D::Create();

		// 位置の設定
		pTime->SetPos(D3DXVECTOR3(-520.0f, 290.0f, 0.0f));

		// サイズの設定
		pTime->SetSize(D3DXVECTOR3(200.0f, 150.0f, 0.0f));

		// 色の設定
		pTime->SetCol(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

		// テクスチャの設定
		pTime->SetTexture(CTexture::LABEL_Time);
	}

	// サークル
	m_pCircle = CCircle::Create();

	//プレイヤーの生成
	m_pPlayer = CPlayer::Create();

	// 生成
	CEnemyManager::Create();

	CPlayer::SetKill(false);

	// 曲の再生
	CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_BGM_Game);
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CGame::Uninit()
{
	// 曲の停止
	CApplication::GetInstance()->GetSound()->Stop();

	// 解放
	Release();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CGame::Update()
{
	m_time++;

	if (m_time % 60 == 0)
	{
		m_pTime->Add(-1);

		if (m_pTime->Get() <= 0)
		{
			// モードの変更
			CApplication::GetInstance()->GetFade()->ChangeMode(EMode::MODE_RESULT);
		}
	}

#ifdef _DEBUG
	CInput* pInput = CInput::GetKey();

	if (pInput->Trigger(DIK_F1))
	{// キーが押された
		// モードの変更
		CApplication::GetInstance()->GetFade()->ChangeMode(EMode::MODE_RESULT);
	}
	else if (pInput->Trigger(DIK_F2))
	{// キーが押された
		CEffect::Explosion(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}
	else if (pInput->Trigger(DIK_F3))
	{// キーが押された
		CApplication::GetInstance()->GetSound()->SetRate(CSound::LABEL_BGM_Game, 1.2f);
	}
	else if (pInput->Trigger(DIK_F4))
	{// キーが押された
		CApplication::GetInstance()->GetSound()->SetRate(CSound::LABEL_BGM_Game, 0.8f);
	}
	else if (pInput->Trigger(DIK_F5))
	{// キーが押された
		CApplication::GetInstance()->GetSound()->SetVolume(CSound::LABEL_BGM_Game, 1.5f);
	}
	else if (pInput->Trigger(DIK_F6))
	{// キーが押された
		CApplication::GetInstance()->GetSound()->SetVolume(CSound::LABEL_BGM_Game, 0.5f);
	}
#endif // _DEBUG
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CGame::Draw()
{
}
