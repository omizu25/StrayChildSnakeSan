//**************************************************
// 
// application.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "application.h"
#include "renderer.h"
#include "input.h"
#include "sound.h"
#include "object.h"
#include "texture.h"
#include "mode.h"
#include "fade.h"
#include "instancing.h"

//==================================================
// 定義
//==================================================
namespace
{
const bool FULL_SCREEN = false;	// フルスクリーンにするかどうか
const CMode::EMode START_MODE = CMode::MODE_TITLE;	// 最初のモード
}

//==================================================
// 静的メンバ変数
//==================================================
CApplication* CApplication::m_pApplication = nullptr;

//--------------------------------------------------
// インスタンスの取得
//--------------------------------------------------
CApplication* CApplication::GetInstance()
{
	if (m_pApplication == nullptr)
	{// nullチェック
		m_pApplication = new CApplication;
	}

	return m_pApplication;
}

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CApplication::CApplication() :
	m_pRenderer(nullptr),
	m_pSound(nullptr),
	m_pTexture(nullptr),
	m_pInstancing(nullptr),
	m_pMode(nullptr),
	m_pFade(nullptr)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CApplication::~CApplication()
{
	assert(m_pFade == nullptr);
	assert(m_pMode == nullptr);
	assert(m_pInstancing == nullptr);
	assert(m_pTexture == nullptr);
	assert(m_pSound == nullptr);
	assert(m_pRenderer == nullptr);
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
HRESULT CApplication::Init(HINSTANCE hInstance, HWND hWnd)
{
	{// レンダラー
		m_pRenderer = new CRenderer;

		// nullチェック
		assert(m_pRenderer != nullptr);

		if (FAILED(m_pRenderer->Init(hWnd, !FULL_SCREEN)))
		{// 初期化
			return S_FALSE;
		}
	}

	{// 入力
		CInput::Create();

		if (FAILED(CInput::GetKey()->Init(hInstance, hWnd)))
		{// 初期化
			return E_FAIL;
		}
	}

	{// サウンド
		m_pSound = new CSound;

		// nullチェック
		assert(m_pSound != nullptr);

		if (FAILED(m_pSound->Init(hWnd)))
		{// 初期化
			return S_FALSE;
		}
	}

	{// テクスチャ
		m_pTexture = new CTexture;

		// nullチェック
		assert(m_pTexture != nullptr);
	}

	{// インスタンシング
		m_pInstancing = new CInstancing;

		// nullチェック
		assert(m_pInstancing != nullptr);

		// 初期化
		m_pInstancing->Init();
	}

	{// フェード
		m_pFade = CFade::Create();

		// nullチェック
		assert(m_pFade != nullptr);

		// 初期化
		m_pFade->Init();
	}

	m_mode = START_MODE;

	// モードの設定
	m_pFade->ChangeMode(m_mode);

	// ランダムの初期化
	srand((unsigned int)time(NULL));

	return S_OK;
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CApplication::Uninit()
{
	// 全ての解放
	CObject::ReleaseAll();

	m_pMode = nullptr;

	if (m_pFade != nullptr)
	{// nullチェック
		m_pFade->Uninit();
		m_pFade = nullptr;
	}

	if (m_pInstancing != nullptr)
	{// nullチェック
		m_pInstancing->Uninit();
		delete m_pInstancing;
		m_pInstancing = nullptr;
	}

	if (m_pTexture != nullptr)
	{// nullチェック
		m_pTexture->ReleaseAll();
		delete m_pTexture;
		m_pTexture = nullptr;
	}

	if (m_pSound != nullptr)
	{// nullチェック
		m_pSound->Uninit();
		delete m_pSound;
		m_pSound = nullptr;
	}

	// 入力処理の終了
	CInput::GetKey()->Uninit();

	if (m_pRenderer != nullptr)
	{// nullチェック
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = nullptr;
	}
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CApplication::Update()
{
	// 入力処理の更新
	CInput::GetKey()->Update();

	if (m_pRenderer != nullptr)
	{// nullチェック
		// 更新
		m_pRenderer->Update();
	}
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CApplication::Draw()
{
	if (m_pRenderer != nullptr)
	{// nullチェック
		// 描画処理
		m_pRenderer->Draw();
	}
}

//--------------------------------------------------
// モードの設定
//--------------------------------------------------
CMode::EMode CApplication::GetModeEnum()
{
	return m_mode;
}
//--------------------------------------------------
// モードの設定
//--------------------------------------------------
void CApplication::SetMode(CMode::EMode mode)
{
	m_mode = mode;

	// 全ての解放
	CObject::ReleaseAll();

	// モードの生成
	m_pMode = CMode::Create(mode);
}

//--------------------------------------------------
// デバイスの取得
//--------------------------------------------------
LPDIRECT3DDEVICE9 CApplication::GetDevice()
{
	return m_pRenderer->GetDevice();
}

//--------------------------------------------------
// レンダラーの取得
//--------------------------------------------------
CRenderer* CApplication::GetRenderer()
{
	return m_pRenderer;
}

//--------------------------------------------------
// サウンドの取得
//--------------------------------------------------
CSound* CApplication::GetSound()
{
	return m_pSound;
}

//--------------------------------------------------
// テクスチャの取得
//--------------------------------------------------
CTexture* CApplication::GetTexture()
{
	return m_pTexture;
}

//--------------------------------------------------
// インスタンシングの取得
//--------------------------------------------------
CInstancing* CApplication::GetInstancing()
{
	return m_pInstancing;
}

//--------------------------------------------------
// モードの取得
//--------------------------------------------------
CMode* CApplication::GetMode()
{
	return m_pMode;
}

//--------------------------------------------------
// フェードの取得
//--------------------------------------------------
CFade* CApplication::GetFade()
{
	return m_pFade;
}
