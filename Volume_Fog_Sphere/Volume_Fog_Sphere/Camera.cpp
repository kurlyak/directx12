//#include "Header.h"
#include "camera.h"

void CFirstPersonCamera::Init_Camera(int Width, int Height)
{
	
	//MatView = DirectX::XMMatrixIdentity();

	ShowCursor(FALSE);

	VecRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	VecUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	VecLook = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	VecCamPos = DirectX::XMVectorSet(25.0f, 5.0f, -6000.0f, 1.0f);
	
	MatView = DirectX::XMMatrixIdentity();
	
	ScreenWidth = Width;
	ScreenHeight = Height;
}

DirectX::XMMATRIX CFirstPersonCamera::Frame_Move(float fTime)
{
	
	//mouse move
	POINT mousePos;
	GetCursorPos(&mousePos);

	//if( (mousePos.x == ScreenWidth/2) && (mousePos.y == ScreenHeight/2) ) return MatView;
	SetCursorPos(ScreenWidth/2, ScreenHeight/2);

	int DeltaX = ScreenWidth / 2 - mousePos.x;
	int DeltaY = ScreenHeight / 2 - mousePos.y;

	//что бы перемещать камеру вертикально
	//поставте значение m_RotationScalerY
	//больше нуля
	//float m_RotationScalerY = 0.1f;
	float m_RotationScalerY = 0.0f;
	float m_RotationScalerX = 2.0f;
	
	//реакция на движение мышью

	//горизонтальное смещение камеры (вокруг оси Y)
	if (DeltaX < 0) m_RotationScalerX = m_RotationScalerX;
	else if (DeltaX > 0) m_RotationScalerX = -m_RotationScalerX;
	else if (DeltaX == 0) m_RotationScalerX = 0;
	
	DirectX::XMMATRIX matRotUp;
	//DirectX::XMVECTOR vUpTemp     = DirectX::XMLoadFloat3(&(const DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f)));
	DirectX::XMVECTOR vUpTemp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	matRotUp = DirectX::XMMatrixRotationAxis(vUpTemp,(float)DirectX::XMConvertToRadians(m_RotationScalerX));
		
	VecRight = XMVector3Transform(VecRight,matRotUp);
	VecUp = XMVector3Transform(VecUp,matRotUp);
	VecLook = XMVector3Transform(VecLook,matRotUp);
		

	//вертикальное смещение камеры (вокруг оси X)
	if (DeltaY < 0) m_RotationScalerY = m_RotationScalerY;
	else if (DeltaY > 0) m_RotationScalerY = -m_RotationScalerY;
	else if (DeltaY == 0) m_RotationScalerY = 0;

	DirectX::XMMATRIX matRotRight;
	matRotRight = DirectX::XMMatrixRotationAxis(VecRight, (float)DirectX::XMConvertToRadians(m_RotationScalerY));
	
	VecRight = DirectX::XMVector3Transform(VecRight,matRotRight);
	VecUp = DirectX::XMVector3Transform(VecUp,matRotRight);
	VecLook = DirectX::XMVector3Transform(VecLook,matRotRight);
	
	//keyboard move

	float ratioMove = 5000;
	DirectX::XMVECTOR vAccel = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	if(GetAsyncKeyState('W')& 0xFF00) 
	{
		DirectX::XMVECTOR vTemp = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(VecLook), 0.0f, DirectX::XMVectorGetZ(VecLook)))), ratioMove * fTime);
		vAccel = DirectX::XMVectorAdd(vAccel, vTemp);
	}
	if(GetAsyncKeyState('S')& 0xFF00) 
	{
		DirectX::XMVECTOR vTemp = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(VecLook), 0.0f, DirectX::XMVectorGetZ(VecLook)))), -ratioMove * fTime);
		vAccel = DirectX::XMVectorAdd(vAccel, vTemp);
	}
	if(GetAsyncKeyState('D')& 0xFF00) 
	{
		DirectX::XMVECTOR vTemp = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(VecRight), 0.0f, DirectX::XMVectorGetZ(VecRight)))), ratioMove * fTime);
		vAccel = DirectX::XMVectorAdd(vAccel, vTemp);
	}
	if(GetAsyncKeyState('A')& 0xFF00) 
	{
		DirectX::XMVECTOR vTemp = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(VecRight), 0.0f, DirectX::XMVectorGetZ(VecRight)))), -ratioMove * fTime);
		vAccel = DirectX::XMVectorAdd(vAccel, vTemp);
	}

	VecCamPos = DirectX::XMVectorAdd(VecCamPos, vAccel);
	
	//сторим видовую матрицу

	VecLook = DirectX::XMVector3Normalize(VecLook);
	VecUp = DirectX::XMVector3Cross(VecLook,VecRight);
	VecUp = DirectX::XMVector3Normalize(VecUp);
	VecRight = DirectX::XMVector3Cross(VecUp,VecLook);
	VecRight = DirectX::XMVector3Normalize(VecRight);
	
	DirectX::XMFLOAT4X4 MatViewTemp;

	MatViewTemp._11 = DirectX::XMVectorGetX(VecRight);
	MatViewTemp._21 = DirectX::XMVectorGetY(VecRight);
	MatViewTemp._31 = DirectX::XMVectorGetZ(VecRight);

	MatViewTemp._12 = DirectX::XMVectorGetX(VecUp);
	MatViewTemp._22 = DirectX::XMVectorGetY(VecUp);
	MatViewTemp._32 = DirectX::XMVectorGetZ(VecUp);

	MatViewTemp._13 = DirectX::XMVectorGetX(VecLook);
	MatViewTemp._23 = DirectX::XMVectorGetY(VecLook);
	MatViewTemp._33 = DirectX::XMVectorGetZ(VecLook);

	MatViewTemp._14 = 0.0f;
	MatViewTemp._24 = 0.0f;
	MatViewTemp._34 = 0.0f;
	MatViewTemp._44 = 1.0f;

	MatViewTemp._41 =-DirectX::XMVectorGetX(DirectX::XMVector3Dot(VecCamPos, VecRight ));
    MatViewTemp._42 =-DirectX::XMVectorGetY(DirectX::XMVector3Dot(VecCamPos, VecUp    ));
	MatViewTemp._43 =-DirectX::XMVectorGetZ(DirectX::XMVector3Dot(VecCamPos, VecLook  ));

	MatView = DirectX::XMMatrixIdentity();

	MatView = DirectX::XMLoadFloat4x4(&MatViewTemp);
	
	return MatView;
}


