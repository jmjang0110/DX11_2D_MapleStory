#include "pch.h"
#include "CDamageParticle.h"

#include "CTimeMgr.h"

#include "CTransform.h"
#include "CResMgr.h"

#include "CDamageParticleShader.h"


CDamageParticle::CDamageParticle()
	: CRenderComponent(COMPONENT_TYPE::DAMAGE_PARTICLESYSTEM)
	, m_iMaxCount(5)
	, m_bPosInherit(0)
	, m_iAliveCount(5)
	, m_fAccTime(0.f)
	, m_fMinLifeTime(2.f)
	, m_fMaxLifeTime(2.f)
	, m_fStartSpeed(100.f)
	, m_fEndSpeed(10.f)
	, m_vStartColor(Vec4(1.f, 0.2f, 0.7f, 1.f))
	, m_vEndColor(Vec4(1.f, 1.f, 1.f, 1.f))
	, m_vStartScale(Vec3(50.f, 50.f, 1.f))
	, m_vEndScale(Vec3(50.f, 50.f, 1.f))
	, m_fParticleCreateDistance(25.f)
	, m_fParticleCreateTerm(0.f)

	, m_fTimer(0.f)
	, m_fDeltaTime(0.f)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\DamageParticleRenderMtrl.mtrl"));

	m_CS = (CDamageParticleShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"DamageParticleUpdateShader").Get();

	// ���⼭ �� �ʿ䰡 ������??? 
	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, true, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);



}

CDamageParticle::CDamageParticle(const CDamageParticle& _origin)
	: CRenderComponent(_origin)
	, m_iMaxCount(_origin.m_iMaxCount)
	, m_bPosInherit(_origin.m_bPosInherit)
	, m_iAliveCount(_origin.m_iAliveCount)
	, m_fAccTime(_origin.m_fAccTime)
	, m_fMinLifeTime(_origin.m_fMinLifeTime)
	, m_fMaxLifeTime(_origin.m_fMaxLifeTime)
	, m_fStartSpeed(_origin.m_fStartSpeed)
	, m_fEndSpeed(_origin.m_fEndSpeed)
	, m_vStartColor(_origin.m_vStartColor)
	, m_vEndColor(_origin.m_vEndColor)
	, m_vStartScale(_origin.m_vStartScale)
	, m_vEndScale(_origin.m_vEndScale)
	, m_fParticleCreateDistance(_origin.m_fParticleCreateDistance)
	, m_fParticleCreateTerm(_origin.m_fParticleCreateTerm)

	,m_fTimer(_origin.m_fTimer)
	,m_fDeltaTime(_origin.m_fDeltaTime)
{
	SetMesh(CResMgr::GetInst()->FindRes<CMesh>(L"PointMesh"));
	SetSharedMaterial(CResMgr::GetInst()->FindRes<CMaterial>(L"material\\DamageParticleRenderMtrl.mtrl"));

	m_CS = (CDamageParticleShader*)CResMgr::GetInst()->FindRes<CComputeShader>(L"DamageParticleUpdateShader").Get();

	m_ParticleBuffer = new CStructuredBuffer();
	m_ParticleBuffer->Create(sizeof(tParticle), m_iMaxCount, SB_TYPE::READ_WRITE, true, nullptr);

	m_DataBuffer = new CStructuredBuffer;
	m_DataBuffer->Create(sizeof(tParticleData), 1, SB_TYPE::READ_WRITE, true, nullptr);
}

CDamageParticle::~CDamageParticle()
{
	SAFE_DELETE(m_ParticleBuffer);
	SAFE_DELETE(m_DataBuffer);
}

void CDamageParticle::SetMaxParticleCount(UINT _iMax)
{
	if (m_iMaxCount < _iMax)
	{
		m_ParticleBuffer->Create(sizeof(tParticle), _iMax, SB_TYPE::READ_WRITE, true, nullptr);
	}
	m_iMaxCount = _iMax;
}




void CDamageParticle::finalupdate()
{

	m_fAccTime += DT;
	if (m_fParticleCreateTerm < m_fAccTime)
	{
		m_fAccTime = 0.f;

		tParticleData data = {};
		data.iAliveCount = m_iAliveCount;
		m_DataBuffer->SetData(&data, 1);
	}

	m_CS->SetParticleCreateDistance(m_fParticleCreateDistance);
	m_CS->SetParticleBuffer(m_ParticleBuffer);
	m_CS->SetParticleDataBuffer(m_DataBuffer);

	m_CS->SetParticleMinMaxTime(m_fMinLifeTime, m_fMaxLifeTime);
	m_CS->SetStartEndSpeed(m_fStartSpeed, m_fEndSpeed);
	m_CS->SetStartEndColor(m_vStartColor, m_vEndColor);
	m_CS->SetStartEndScale(m_vStartScale, m_vEndScale);

	m_CS->SetObjectWorldPos(Transform()->GetWorldPos());

	m_CS->Excute();
}

void CDamageParticle::render()
{
	Transform()->UpdateData();

	m_ParticleBuffer->UpdateData(PIPELINE_STAGE::GS | PIPELINE_STAGE::PS, 16);

	//GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_0, &i);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_1, &m_bPosInherit);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_0, &m_vStartLT_UV);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_1, &m_vSlice_UV);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::FLOAT_0, &m_fDeltaTime);

	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_2, &m_hitEffect_StartUV);
	GetMaterial()->SetScalarParam(SCALAR_PARAM::VEC2_3, &m_hitEffect_SliceUV);

	m_fTimer += DT;
	if(m_fTimer >= 1.f)
		m_fDeltaTime += DT * 10.f;
	// �̰� ��ũ�� �ణ �ȸ¾Ƽ� ŹŹ ����� ������ ��ó�� ���̴°��� 
	if (m_fTimer >= m_fMaxLifeTime)
	{
		m_fDeltaTime = 0.f;
		m_fTimer = 0.f;

	}

	GetMaterial()->SetScalarParam(SCALAR_PARAM::INT_2, &m_DamageNum);

	GetMaterial()->UpdateData();
	GetMesh()->render_particle(m_iMaxCount);



	m_ParticleBuffer->Clear();
}


void CDamageParticle::SaveToScene(FILE* _pFile)
{
	CRenderComponent::SaveToScene(_pFile);

	wstring strKey;
	if (nullptr != m_CS)
		strKey = m_CS->GetKey();
	SaveWStringToFile(strKey, _pFile);

	fwrite(&m_iMaxCount, sizeof(UINT), 1, _pFile);
	fwrite(&m_bPosInherit, sizeof(int), 1, _pFile);
	fwrite(&m_iAliveCount, sizeof(int), 1, _pFile);
	fwrite(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fwrite(&m_fStartSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_fEndSpeed, sizeof(float), 1, _pFile);
	fwrite(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fwrite(&m_vStartScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vEndScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_fParticleCreateDistance, sizeof(float), 1, _pFile);
	fwrite(&m_fParticleCreateTerm, sizeof(float), 1, _pFile);
}

void CDamageParticle::LoadFromScene(FILE* _pFile)
{
	CRenderComponent::LoadFromScene(_pFile);

	wstring strKey;
	LoadWStringFromFile(strKey, _pFile);
	m_CS = (CDamageParticleShader*)CResMgr::GetInst()->FindRes<CComputeShader>(strKey).Get();

	UINT iMaxCount = 0;
	fread(&iMaxCount, sizeof(UINT), 1, _pFile);
	SetMaxParticleCount(iMaxCount);

	fread(&m_bPosInherit, sizeof(int), 1, _pFile);
	fread(&m_iAliveCount, sizeof(int), 1, _pFile);
	fread(&m_fMinLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fMaxLifeTime, sizeof(float), 1, _pFile);
	fread(&m_fStartSpeed, sizeof(float), 1, _pFile);
	fread(&m_fEndSpeed, sizeof(float), 1, _pFile);
	fread(&m_vStartColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vEndColor, sizeof(Vec4), 1, _pFile);
	fread(&m_vStartScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vEndScale, sizeof(Vec3), 1, _pFile);
	fread(&m_fParticleCreateDistance, sizeof(float), 1, _pFile);
	fread(&m_fParticleCreateTerm, sizeof(float), 1, _pFile);
}
