#ifndef _CAMERA_
#define _CAMERA_

#include <windows.h>
#include <DirectXMath.h>

class CFirstPersonCamera
{
public:
	
	DirectX::XMMATRIX MatView;

	DirectX::XMVECTOR VecCamPos;
		
	void Init_Camera(int Width, int Height);
	DirectX::XMMATRIX Frame_Move(float fTime);

private:
	DirectX::XMVECTOR VecRight, VecUp, VecLook;
	int ScreenWidth, ScreenHeight;
};


#endif