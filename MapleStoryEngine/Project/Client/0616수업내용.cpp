/*
	0616 -  �����е� �Ȱ����ͼ� ����ٰ� ���ڽ��ϴ�...


	
	1. CGameObject.cpp ���� - COmpoennt Load �߰��ϴ��� 
		- CGameObject - ������ ���� 
		- 7��14�ʿ� �� 

	2. Component - Save/ Load Scene ���� 
		- Transform ���� ( 10:37 ) 
			* arrRElativeDir, arrworldDir, m_matWorld -> ������ finmalUpdate���� ���ǹǷ� ������ �ʿ� ���� 

		- CCamera ���� ( 12:47 )
			* iLayerMask ���° ���̾� �׸����� �˷��ش�. - �׳� �̰� ���� ��Ծ�� ����aa
			* ( 14��10��) �ٽú��� 
		- CCollider2D ���� (15:57)
			* Mesh , Mtrl ��� �� ���ΰ� - Collider Ÿ�Կ� ���� �ε��ϱ� ������ �������
			 
		- CAnimator2D ���� - 22:58 Animation2D - (24:50)-�̸��� �����ϴ� ����
			*   �ִϸ��̼� ���� ������ �ϳ� ���� ��ü�� �� �ִϸ��̼��� �޾ƿ� ���ְ� �ص� �ȴٰ� �ϳ�? �ٵ�
			*	�̷��Ը� �ص� ũ�� ������� 
			*	Animation2D - Component �ƴ� ���ε��� ��ü
			*	CAnimation2D Save/Load ���� 
			*	CAnimation2D ������ ���� 
			*	CAnimation2D - LoadFromSCene - resize 
			*	� ���ҽ��� ���� �ϰ� �־���. - ������ �ٽ� �ε��ϰ� �����ؾ��Ѵ� - SvaeResroucePtr() �Լ� ������� - (37:22)
			
			*	Engine - func.h - SaveREsPtr(Ptr<ReS> _ptr, FILE* _pFile), LoadResPtr() ���� 
			*	pAtlasTex �̷��� nullptr �̾��ٸ� ���� bNullPtr ������ ������ ���̴�. 
			*	LoadResPtr �� ������ �޾ƿð��̶� & _Ptr �� �޴´�. 
			*	-> ��ǻ� Save / Load REsPtr  �Ǵ°� Texture �ۿ� ����. �ٸ��� ������ ���� 
			*	global.h ���� 
			*	��� ���� �ڻ쳲 - singleton.h -> fun.h ������ �ٲ����  
			

// 2���� 
		 ------ RenderComponent ------

		- CMeshRender ���� 
			* Save / Load scene ���� 
			* ���� Mayerail �� ���� RUntime �ÿ� ���������. - �̸� ������� �ʿ䰡 ���� �빮�� 
			* �Ű澵 �ʿ䰡 ����. ( File ������) 
		- CTileMap
			*	Save/Load Scene ���� 
			*  structuredBuffer  - ��Ȯ���ϴ� ������ �� �ʿ� ����? ( 12:00 ���� �� ) 
		- CParticleSystem ���� 
			*  Save / Load Scene ���� 
			* ParticleUpdateSHader - > �׳� SaveResPtr �� �� �� ��Ȯ�ϰ� �ؾ��� - CCOmputeShader �� �������� 
			* MaxCount �ٲ�� StructuredBuffer �� �ٽ� Create �ϰ� �Ѵ�. - 20�к��� �ٽ� ���  
			* 
			* UI ���� MaxCount �ٲٰ� �ѹ� Create �� �ٽ� ȣ���ϰ� �غ��� 
			* CPArticleUpdateShader �� CParticleUpdateShader ���� ComputeShader �� �����߰ڴ�? 25 �� �ٽõ�� 
			* -> ComputeShader ������ ���� �� ������ �� ���� 
			* MaxCount �ٲ�� ũ�Ⱑ (�����ڶ� ����)�������� Ŀ���� Create �� �ٽ� ���ش�. - StructuredBuffer ũ�⸦ �ø���. ( 27��) 
			* CParticleSystem.h -> setMaxParticleCount(UINT _iMax) �߰� 
			* 
			* ComputeShader ������ ��� �����..???? CResMGr::Load ���� ������ 
			* CComputeShader ���� �⺻������ �߰� 
		- RenderComponent
		- 











*/