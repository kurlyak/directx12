#ifndef _CAMERA_
#define _CAMERA_

#include <windows.h>
#include <DirectXMath.h>

class CFirstPersonCamera
{
public:
	
	/*
	DirectX::XMFLOAT2 m_vMouseDelta;
	DirectX::XMVECTOR           m_vVelocity;
	FLOAT                 m_fDragTimer;
	FLOAT                 m_fTotalDragTimeToZero;
	DirectX::XMVECTOR           m_vVelocityDrag;
	*/
	DirectX::XMMATRIX matView;
	DirectX::XMVECTOR vRight,vUp,vLook, vCamPos;
	int nScreenWidth, nScreenHeight;
	
	void InitCamera(int nWidth, int nHeight);
	DirectX::XMMATRIX FrameMove(float fTime);
};


#endif