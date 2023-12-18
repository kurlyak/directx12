//#include "Header.h"
#include "camera.h"

void CFirstPersonCamera::InitCamera(int nWidth, int nHeight)
{
	
	//matView = DirectX::XMMatrixIdentity();

	ShowCursor(FALSE);

	vRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	vUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	vLook = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	vCamPos = DirectX::XMVectorSet(25.0f, 5.0f, -6000.0f, 1.0f);

	/*
	DirectX::XMFLOAT3 vR = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
	vRight = DirectX::XMLoadFloat3(&vR);

	DirectX::XMFLOAT3 vU = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	vUp = DirectX::XMLoadFloat3(&(vU));

	DirectX::XMFLOAT3 vL = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
	vLook = DirectX::XMLoadFloat3(&(vL));

	DirectX::XMFLOAT3 vP = DirectX::XMFLOAT3(0.0f, 10.0f, -250.0f);
	vPos = DirectX::XMLoadFloat3(&(vP));
	*/
/*
	vRight = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)));
	vUp = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f)));
	vLook  = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f)));
	vPos  = DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3( 0.0f, 10.0f, -250.0f)));
	*/

	
	matView = DirectX::XMMatrixLookAtLH(vCamPos, vLook, vUp );
	
	nScreenWidth = nWidth;
	nScreenHeight = nHeight;
	/*
    m_vMouseDelta = DirectX::XMFLOAT2(0,0);
	DirectX::XMFLOAT3 m_vVelocityT = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
		m_vVelocity = DirectX::XMLoadFloat3(&m_vVelocityT);
	m_fDragTimer    = 0.0f;
	m_fTotalDragTimeToZero = 0.25;
	DirectX::XMFLOAT3 m_vVelocityDragT = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_vVelocityDrag = DirectX::XMLoadFloat3(&m_vVelocityDragT);
	*/
}

DirectX::XMMATRIX CFirstPersonCamera::FrameMove(float fTime)
{
	
	//mouse move
	POINT mousePos;
	GetCursorPos(&mousePos);

	//if( (mousePos.x == nScreenWidth/2) && (mousePos.y == nScreenHeight/2) ) return matView;
	SetCursorPos(nScreenWidth/2, nScreenHeight/2);

	int DeltaX = nScreenWidth / 2 - mousePos.x;
	int DeltaY = nScreenHeight / 2 - mousePos.y;

	//что бы перемещать камеру вертикально
	//поставте значение m_RotationScalerY
	//больше нуля
	//float m_RotationScalerY = 0.1f;
	float m_RotationScalerY = 0.0f;
	float m_RotationScalerX = 7.0f;
	
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
	//DirectX::XMVECTOR vAccel = DirectX::XMLoadFloat3(&(const DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f)));
	DirectX::XMVECTOR vAccel = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	if(GetAsyncKeyState('W')& 0xFF00) 
	{
		vAccel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(vLook), 0.0f, DirectX::XMVectorGetZ(vLook)))), ratioMove * fTime);
		//vAccel = FXMVECTOR(vLook.x, 0.0f, vLook.z) * ratioMove * fTime;
	}
	if(GetAsyncKeyState('S')& 0xFF00) 
	{
		vAccel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(vLook), 0.0f, DirectX::XMVectorGetZ(vLook)))), -ratioMove * fTime);
		//vAccel = FXMVECTOR(vLook.x, 0.0f, vLook.z) * -ratioMove * fTime;
	}
	if(GetAsyncKeyState('D')& 0xFF00) 
	{
		vAccel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(vRight), 0.0f, DirectX::XMVectorGetZ(vRight)))), ratioMove * fTime);
		//vAccel = FXMVECTOR(vRight.x, 0.0f, vRight.z) * ratioMove * fTime;
	}
	if(GetAsyncKeyState('A')& 0xFF00) 
	{
		vAccel = DirectX::XMVectorScale(DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(DirectX::XMVectorGetX(vRight), 0.0f, DirectX::XMVectorGetZ(vRight)))), -ratioMove * fTime);
		//vAccel = FXMVECTOR(vRight.x, 0.0f, vRight.z) * -ratioMove * fTime;
	}
	/*
	DirectX::XMVECTOR a = DirectX::XMVector3LengthSq( vAccel );
	if(DirectX::XMVectorGetX(a)  > 0 )
        {
            m_vVelocity = vAccel;
            m_fDragTimer = m_fTotalDragTimeToZero;
            m_vVelocityDrag = vAccel / m_fDragTimer;
        }
        else 
        {
            if( m_fDragTimer > 0 )
            {
                // Drag until timer is <= 0
                m_vVelocity -= m_vVelocityDrag * fTime;
                m_fDragTimer -= fTime;
				
            }
            else
            {
                // Zero velocity
                //m_vVelocity = DirectX::XMLoadFloat3(&(const DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f)));
				m_vVelocity = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
            }
        }
    
	vPos = DirectX::XMVectorAdd(vPos, m_vVelocity);
	*/
	//vPos+=vAccel;

	vCamPos = DirectX::XMVectorAdd(vCamPos, vAccel);
	
	
	//сторим видовую матрицу

	vLook = DirectX::XMVector3Normalize(vLook);
	vUp = DirectX::XMVector3Cross(vLook,vRight);
	vUp = DirectX::XMVector3Normalize(vUp);
	vRight = DirectX::XMVector3Cross(vUp,vLook);
	vRight = DirectX::XMVector3Normalize(vRight);
	
	DirectX::XMFLOAT4X4 matViewTemp;

	matViewTemp._11 = DirectX::XMVectorGetX(vRight);
	matViewTemp._21 = DirectX::XMVectorGetY(vRight);
	matViewTemp._31 = DirectX::XMVectorGetZ(vRight);

	matViewTemp._12 = DirectX::XMVectorGetX(vUp);
	matViewTemp._22 = DirectX::XMVectorGetY(vUp);
	matViewTemp._32 = DirectX::XMVectorGetZ(vUp);

	matViewTemp._13 = DirectX::XMVectorGetX(vLook);
	matViewTemp._23 = DirectX::XMVectorGetY(vLook);
	matViewTemp._33 = DirectX::XMVectorGetZ(vLook);

	matViewTemp._14 = 0.0f;
	matViewTemp._24 = 0.0f;
	matViewTemp._34 = 0.0f;
	matViewTemp._44 = 1.0f;

	matViewTemp._41 =-DirectX::XMVectorGetX(DirectX::XMVector3Dot(vCamPos, vRight ));
    matViewTemp._42 =-DirectX::XMVectorGetY(DirectX::XMVector3Dot(vCamPos, vUp    ));
	matViewTemp._43 =-DirectX::XMVectorGetZ(DirectX::XMVector3Dot(vCamPos, vLook  ));

	matView = DirectX::XMMatrixIdentity();

	matView = DirectX::XMLoadFloat4x4(&matViewTemp);
	

	/*
	float x = DirectX::XMVectorGetX(vPos);
	float y = DirectX::XMVectorGetY(vPos);
	float z = DirectX::XMVectorGetZ(vPos);

	vPos = DirectX::XMVectorSet(x, y, z, 1.0f);

	matView = DirectX::XMMatrixLookAtLH(vPos, vLook, vUp);
	*/

	return matView;
}


