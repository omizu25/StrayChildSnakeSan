//**************************************************
// 
// snake_head.cpp
// Author  : katsuki mizuki
// 
//**************************************************

//==================================================
// インクルード
//==================================================
#include "snake_head.h"
#include "snake_body.h"
#include "application.h"
#include "utility.h"
#include "game.h"
#include "player.h"
#include "circle.h"
#include "enemy_manager.h"
#include "effect.h"
#include "sound.h"

//==================================================
// 定義
//==================================================
namespace
{
const int MAX_BODY = 25;			// 体の最大数
const int IDX_PARENT = 0;			// 親の番号
const int STD_TIME = 30;			// 発生時間
const float STD_SIZE = 30.0f;		// サイズの標準値
const float OBJ_SIZE = 300.0f;		// サイズの標準値
const float STD_MOVE = 1.5f;		// 移動量の標準値
const float AMPLITUDE_WIDTH = 3.0f;	// 振幅の幅
const float AMPLITUDE_SPEED = 2.0f;	// 振幅の速度
}

//--------------------------------------------------
// 生成
//--------------------------------------------------
CSnakeHead* CSnakeHead::Create(const D3DXVECTOR3& pos)
{
	CSnakeBody** pBody = new CSnakeBody*[MAX_BODY];

	for (int i = 0; i < MAX_BODY; i++)
	{
		pBody[i] = nullptr;
	}

	for (int i = 0; i < MAX_BODY; i++)
	{
		// 生成
		pBody[i] = CSnakeBody::Create();
		pBody[i]->SetPos(pos);
		pBody[i]->SetPosDest(pos);
	}

	CSnakeHead* pSnake = new CSnakeHead;

	if (pSnake == nullptr)
	{// nullチェック
		assert(false);
		return nullptr;
	}

	// 初期化
	pSnake->Init();

	// 設定
	pSnake->Set(pos, pBody);

	return pSnake;
}

//--------------------------------------------------
// デフォルトコンストラクタ
//--------------------------------------------------
CSnakeHead::CSnakeHead() :
	m_time(0),
	m_move(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_target(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_posOld(D3DXVECTOR3(0.0f, 0.0f, 0.0f)),
	m_pBody(nullptr),
	m_pObj(nullptr),
	m_chase(false)
{
}

//--------------------------------------------------
// デストラクタ
//--------------------------------------------------
CSnakeHead::~CSnakeHead()
{
}

//--------------------------------------------------
// 初期化
//--------------------------------------------------
void CSnakeHead::Init()
{
	m_time = 0;
	m_chase = false;

	// 初期化
	CObject3D::Init();
}

//--------------------------------------------------
// 終了
//--------------------------------------------------
void CSnakeHead::Uninit()
{
	for (int i = 0; i < MAX_BODY; i++)
	{
		if (m_pBody[i] == nullptr)
		{// nullチェック
			continue;
		}

		// 解放
		m_pBody[i] = nullptr;
	}

	if (m_pBody != nullptr)
	{// nullチェック
		delete[] m_pBody;
		m_pBody = nullptr;
	}

	// 終了
	CObject3D::Uninit();
}

//--------------------------------------------------
// 更新
//--------------------------------------------------
void CSnakeHead::Update()
{
	if (CPlayer::GetKill())
	{
		CEnemyManager::SubPop();
		SetRelease();

		for (int i = 0; i < MAX_BODY; i++)
		{
			if (m_pBody[i] == nullptr)
			{// nullチェック
				continue;
			}

			// 解放
			m_pBody[i]->SetRelease();
		}

		m_pObj->SetRelease();

		return;
	}

	m_time++;

	if (m_time == STD_TIME)
	{
		// 色の取得
		D3DXCOLOR col = CObject3D::GetCol();

		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);

		// 色の設定
		CObject3D::SetCol(col);
	}

	if (m_time <= STD_TIME)
	{// 生成時間中

		// 色の取得
		D3DXCOLOR col = CObject3D::GetCol();

		col.a = SinCurve(m_time, 0.1f);

		// 色の設定
		CObject3D::SetCol(col);

		for (int i = 0; i < MAX_BODY; i++)
		{
			if (m_pBody[i] == nullptr)
			{// nullチェック
				continue;
			}

			// 色の取得
			D3DXCOLOR colBody = m_pBody[i]->GetCol();

			colBody.a = 0.0f;

			// 色の設定
			m_pBody[i]->SetCol(colBody);
		}

		return;
	}
	else
	{

		D3DXCOLOR col = CObject3D::GetCol();

		col.a = 1.0f;

		// 色の設定
		CObject3D::SetCol(col);

		for (int i = 0; i < MAX_BODY; i++)
		{
			if (m_pBody[i] == nullptr)
			{// nullチェック
				continue;
			}

			// 色の取得
			D3DXCOLOR colBody = m_pBody[i]->GetCol();

			colBody.a = 1.0f;

			// 色の設定
			m_pBody[i]->SetCol(colBody);
		}

		Target();

		// 移動量の設定
		SetMove();

		D3DXVECTOR3 pos = CObject3D::GetPos();

		// 前回の位置の保存
		m_posOld = pos;

		pos.x += m_move.x;
		pos.y += m_move.y;

		D3DXVECTOR3 range = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		range.x = ((float)CApplication::SCREEN_WIDTH * 0.5f) - (STD_SIZE * 0.5f);
		range.y = ((float)CApplication::SCREEN_HEIGHT * 0.5f) - (STD_SIZE * 0.5f);

		// 範囲内
		InRange(&pos, range);

		// 位置の設定
		CObject3D::SetPos(pos);
		m_pObj->SetPos(pos);

		if (m_pBody[IDX_PARENT] != nullptr)
		{// nullチェック
			// 目的の位置の設定
			m_pBody[IDX_PARENT]->SetPosDest(m_posOld);
		}

		for (int i = 0; i < MAX_BODY; i++)
		{
			if (m_pBody[i] == nullptr || i == IDX_PARENT)
			{// nullチェック
				continue;
			}

			int parent = i - 1;
			D3DXVECTOR3 posOld = m_pBody[parent]->GetPosOld();

			// 目的の位置の設定
			m_pBody[i]->SetPosDest(posOld);
		}

		// 当たり判定
		PlayerCollision();
		CircleCollision();
	}

	// 更新
	CObject3D::Update();
}

//--------------------------------------------------
// 描画
//--------------------------------------------------
void CSnakeHead::Draw()
{
	// 描画
	CObject3D::Draw();
}

//--------------------------------------------------
// 前回の位置の取得
//--------------------------------------------------
const D3DXVECTOR3& CSnakeHead::GetPosOld() const
{
	return m_posOld;
}

//--------------------------------------------------
// 設定
//--------------------------------------------------
void CSnakeHead::Set(const D3DXVECTOR3& pos, CSnakeBody** pBody)
{
	// 位置の設定
	CObject3D::SetPos(pos);

	m_posOld = pos;

	m_pBody = pBody;

	// サイズの設定
	CObject3D::SetSize(D3DXVECTOR3(STD_SIZE, STD_SIZE, 0.0f));

	// テクスチャの設定
	CObject3D::SetTexture(CTexture::LABEL_Shape_Circle);

	// 移動量の設定
	SetMove();

	{// 円
		m_pObj = CObject3D::Create();

		m_pObj->SetPos(pos);
		m_pObj->SetSize(D3DXVECTOR3(OBJ_SIZE, OBJ_SIZE, 0.0f));
		m_pObj->SetCol(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.25f));
		m_pObj->SetTexture(CTexture::LABEL_Enemy_ver3_outside);
	}
}

//--------------------------------------------------
// 移動量の設定
//--------------------------------------------------
void CSnakeHead::SetMove()
{
	float fRotMove, fRotDest, fRotDiff;

	// 現在の移動方向(角度)
	fRotMove = atan2f(m_move.x, m_move.y);

	// 目的の移動方向(角度)
	fRotDest = atan2f(m_target.x - GetPos().x, m_target.y - GetPos().y);

	fRotDiff = fRotDest - fRotMove;	// 目的の移動方向までの差分

	// 角度の正規化
	NormalizeAngle(&fRotDiff);

	fRotMove += fRotDiff;	// 移動方向(角度)の補正

	// 角度の正規化
	NormalizeAngle(&fRotMove);

	float sinCurve = sinf(D3DXToRadian(m_time * AMPLITUDE_SPEED)) * AMPLITUDE_WIDTH;

	m_move.x = (sinf(fRotMove) * (STD_MOVE + FloatRandom(STD_MOVE, 0.0f))) + (sinCurve * sinf(fRotMove + D3DX_PI * 0.5f));
	m_move.y = (cosf(fRotMove) * (STD_MOVE + FloatRandom(STD_MOVE, 0.0f))) + (sinCurve * cosf(fRotMove + D3DX_PI * 0.5f));

	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	move.x = (sinf(fRotMove) * STD_MOVE) + (sinCurve * sinf(fRotMove + D3DX_PI * 0.5f));
	move.y = (cosf(fRotMove) * STD_MOVE) + (sinCurve * cosf(fRotMove + D3DX_PI * 0.5f));

	float rot = atan2f(move.x, move.y);

	// 角度の正規化
	NormalizeAngle(&rot);

	// 向きの設定
	CObject3D::SetRot(D3DXVECTOR3(0.0f, 0.0f, rot));
}

//--------------------------------------------------
// 移動量の設定
//--------------------------------------------------
void CSnakeHead::Target()
{
	CGame* pGame = (CGame*)CApplication::GetInstance()->GetMode();
	CPlayer* pPlayer = pGame->GetPlayer();

	if (pPlayer == nullptr)
	{// nullチェック
		return;
	}

	D3DXVECTOR3 pos = CObject3D::GetPos();
	float size = OBJ_SIZE * 0.5f;
	D3DXVECTOR3 targetPos = pPlayer->GetPos();
	float targetSize = pPlayer->GetSize().x * 0.5f;

	D3DXCOLOR col = GetCol();

	if (CollisionCircle(pos, size, targetPos, targetSize))
	{// 当たり判定
		m_target = targetPos;
		m_chase = true;

		col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	}
	else if (CollisionCircle(pos, size, m_target, targetSize))
	{
		float width = (float)CApplication::SCREEN_WIDTH * 0.5f;
		float height = (float)CApplication::SCREEN_HEIGHT * 0.5f;

		m_target.x = FloatRandom(width, -width);
		m_target.y = FloatRandom(height, -height);

		m_chase = false;

		col = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	}

	SetCol(col);
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
void CSnakeHead::PlayerCollision()
{
	CGame* pGame = (CGame*)CApplication::GetInstance()->GetMode();
	CPlayer* pPlayer = pGame->GetPlayer();

	if (pPlayer == nullptr)
	{// nullチェック
		return;
	}

	D3DXVECTOR3 pos = CObject3D::GetPos();
	float size = STD_SIZE * 0.25f;
	D3DXVECTOR3 targetPos = pPlayer->GetPos();
	float targetSize = pPlayer->GetSize().x * 0.25f;

	if (CollisionCircle(pos, size, targetPos, targetSize))
	{// 当たり判定
		pPlayer->SetKill();
	}
}

//--------------------------------------------------
// 当たり判定
//--------------------------------------------------
void CSnakeHead::CircleCollision()
{
	if (!m_chase)
	{
		return;
	}

	CGame* pGame = (CGame*)CApplication::GetInstance()->GetMode();
	CCircle* pCircle = pGame->GetCircle();

	if (pCircle == nullptr)
	{// nullチェック
		return;
	}

	if (!pCircle->GetCollision())
	{
		return;
	}

	D3DXVECTOR3 pos = CObject3D::GetPos();
	float size = STD_SIZE * 0.35f;
	D3DXVECTOR3 targetPos = pCircle->GetPos();
	float targetSize = pCircle->GetSize().x * 0.35f;

	if (CollisionCircle(pos, size, targetPos, targetSize))
	{// 当たり判定
		CEnemyManager::SubPop();

		SetRelease();

		for (int i = 0; i < MAX_BODY; i++)
		{
			if (m_pBody[i] == nullptr)
			{// nullチェック
				continue;
			}

			// 解放
			m_pBody[i]->SetRelease();
		}

		m_pObj->SetRelease();

		pGame->AddScore(1);

		CEffect::Explosion(GetPos());
		CApplication::GetInstance()->GetSound()->Play(CSound::LABEL_SE_DethEnemy);
	}
}
