#ifndef _CAMERA_
#define _CAMERA_

#include <windows.h>
#include <DirectXMath.h>

class CFirstPersonCamera
{
public:
	
	DirectX::XMMATRIX MatView;

	DirectX::XMVECTOR vCamPos;
		
	void InitCamera(int Width, int Height);
	DirectX::XMMATRIX FrameMove(float fTime);

private:
	DirectX::XMVECTOR vRight, vUp, vLook;
	int nScreenWidth, nScreenHeight;
};


#endif