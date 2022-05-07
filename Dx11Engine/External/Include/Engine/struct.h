#pragma once


struct tEventInfo
{
	EVENT_TYPE	eType;
	DWORD_PTR	lParam;
	DWORD_PTR	wParam;
};


struct Vertex
{
	Vec3 vPos;		// ��ǥ	
	Vec4 vColor;	// ����

	Vec2 vUV;

};

typedef Vertex Vtx;

// =======
// TileMap
// =======
struct tTileData
{
	int		iImgIdx;
	Vec2	vLTUV;		// ���ϴ� Ÿ���� UV �»�� ��ǥ 
	int		iPadding;

	tTileData()
		: iImgIdx(-1)
		, iPadding(0)
	{}
};



// ============
// Animation2D
// ============
struct tAnim2DFrame
{
	Vec2	vLT;
	Vec2	vSlice;		
	Vec2	vOffset;
	float	fDuration;	// ���� ���� �ð�



};




// ========
// Particle 
// ========
struct tParticle
{
	Vec3	vPos;
	Vec3	vScale;
	Vec3	vDir;
	Vec4	vColor;

	int		Alive;
	Vec2	Padding;
};



// ===================
// ������� ���� ����ü
// ===================
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};

struct tScalarParam
{
	// ���� 16 ����Ʈ �ȸ��߸� err �� 
	int iArr[4];
	float fArr[4];
	Vec2 v2Arr[4];
	Vec4 v4Arr[4];
	Matrix matArr[4];

	int bTex[10];
	int iPadding[2];

};


struct tAnim2D
{

	Vec2	vLT;
	Vec2	vSlice;
	Vec2	vBackgroundSize;		//
	Vec2	vOffset;
	// 16 byte 

	int     useAnim2D;		// Animation ���� ��� ����
	float   Atlas_Width;
	float   Atlas_Height;   // �ػ� ���� 
	float	Anim2D_Padding;


};

struct tGlobal
{
	Vec2	vResolution;
	float	fDT;
	float	fAccTime;
};
