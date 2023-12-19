//#include "Header.h"
#include "camera.h"

void CFirstPersonCamera::InitCamera(int Width, int Height)
{
	
	//MatView = DirectX::XMMatrixIdentity();

	ShowCursor(FALSE);

	vRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	vUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	vLook = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	vCamPos = DirectX::XMVectorSet(25.0f, 5.0f, -6000.0f, 1.0f);
	
	MatView = DirectX::XMMatrixIdentity();
	
	nScreenWidth = Width;
	nScreenHeight = Height;
}

DirectX::XMMATRIX CFirstPersonCamera::FrameMove(float fTime)
{
	
	//mouse move
	POINT mousePos;
	GetCursorPos(&mousePos);

	//if( (mousePos.x == nScreenWidth/2) && (mousePos.y == nScreenHeight/2) ) return MatView;
	SetCursorPos(nScreenWidth/2, nScreenHeight/2);

	int DeltaX = nScreenWidth / 2 - mousePos.x;
	int DeltaY = nScreenHeight / 2 - mousePos.y;

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
		
	vRight = XMVector3Transform(vRight,matRotUp);
	vUp = XMVector3Transform(vUp,matRotUp);
	vLook = XMVector3Transform(vLook,matRotUp);
		

	//вертикальное смещение камеры (вокруг оси X)
	if (DeltaY < 0) m_RotationScalerY = m_RotationScalerY;
	else if (DeltaY > 0) m_RotationScalerY = -m_RotationScalerY;
	else if (DeltaY == 0) m_RotationScalerY = 0;

	DirectX::XMMATRIX matRotRight;
	matRotRight = DirectX::XMMatrixRotationAxis(vRight, (float)DirectX::XMConvertToRadians(m_RotationScalerY));
	
	vRight = DirectX::XMVector3Transform(vRight,matRotRight);
	vUp = DirectX::XMVector3Transform(vUp,matRotRight);
	vLook = DirectX::XMVector3Transform(vLook,matRotRight);
	
	//keyboard move

	float ratioMove = 5000;
	DirectX::XMVECTOR vAccel = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	if(GetAsyncKeyState('W')& 0xFF00) 
	{
		vAccel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(vLook), 0.0f, DirectX::XMVectorGetZ(vLook)))), ratioMove * fTime);
	}
	if(GetAsyncKeyState('S')& 0xFF00) 
	{
		vAccel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(vLook), 0.0f, DirectX::XMVectorGetZ(vLook)))), -ratioMove * fTime);
	}
	if(GetAsyncKeyState('D')& 0xFF00) 
	{
		vAccel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(vRight), 0.0f, DirectX::XMVectorGetZ(vRight)))), ratioMove * fTime);
	}
	if(GetAsyncKeyState('A')& 0xFF00) 
	{
		vAccel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(vRight), 0.0f, DirectX::XMVectorGetZ(vRight)))), -ratioMove * fTime);
	}

	vCamPos = DirectX::XMVectorAdd(vCamPos, vAccel);
	
	//сторим видовую матрицу

	vLook = DirectX::XMVector3Normalize(vLook);
	vUp = DirectX::XMVector3Cross(vLook,vRight);
	vUp = DirectX::XMVector3Normalize(vUp);
	vRight = DirectX::XMVector3Cross(vUp,vLook);
	vRight = DirectX::XMVector3Normalize(vRight);
	
	DirectX::XMFLOAT4X4 MatViewTemp;

	MatViewTemp._11 = DirectX::XMVectorGetX(vRight);
	MatViewTemp._21 = DirectX::XMVectorGetY(vRight);
	MatViewTemp._31 = DirectX::XMVectorGetZ(vRight);

	MatViewTemp._12 = DirectX::XMVectorGetX(vUp);
	MatViewTemp._22 = DirectX::XMVectorGetY(vUp);
	MatViewTemp._32 = DirectX::XMVectorGetZ(vUp);

	MatViewTemp._13 = DirectX::XMVectorGetX(vLook);
	MatViewTemp._23 = DirectX::XMVectorGetY(vLook);
	MatViewTemp._33 = DirectX::XMVectorGetZ(vLook);

	MatViewTemp._14 = 0.0f;
	MatViewTemp._24 = 0.0f;
	MatViewTemp._34 = 0.0f;
	MatViewTemp._44 = 1.0f;

	MatViewTemp._41 =-DirectX::XMVectorGetX(DirectX::XMVector3Dot(vCamPos, vRight ));
    MatViewTemp._42 =-DirectX::XMVectorGetY(DirectX::XMVector3Dot(vCamPos, vUp    ));
	MatViewTemp._43 =-DirectX::XMVectorGetZ(DirectX::XMVector3Dot(vCamPos, vLook  ));

	MatView = DirectX::XMMatrixIdentity();

	MatView = DirectX::XMLoadFloat4x4(&MatViewTemp);
	
	return MatView;
}


