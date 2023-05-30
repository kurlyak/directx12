//======================================================================================
//	Ed Kurlyak 2023 Timer Class
//======================================================================================

#include "Timer.h"

int CTimer::CalculateFPS()
{
	QueryPerformanceCounter((LARGE_INTEGER *)&m_CurrentTime);

	float TimeElapsed; 
	
	TimeElapsed = (m_CurrentTime - m_LastTime) * m_TimeScale;

	if(m_LimitFPS > 0.0f)
	{
	while ( TimeElapsed < (1.0f / m_LimitFPS))
        {
		    QueryPerformanceCounter((LARGE_INTEGER*)&m_CurrentTime);
		    // Вычисляем прошедшее время в секундах
	        TimeElapsed = (m_CurrentTime - m_LastTime) * m_TimeScale;
        }
	}
	m_LastTime = m_CurrentTime;

	m_FPSFrameCount++;
	m_FPSTimeElapsed += TimeElapsed;
	if ( m_FPSTimeElapsed > 1.0f) 
    {
	m_FrameRate			= m_FPSFrameCount;
	m_FPSFrameCount		= 0;
	m_FPSTimeElapsed	= 0.0f;
	}
   	
	return m_FrameRate;
}

void CTimer::TimerStart(float LimitFPS)
{
	m_LimitFPS = LimitFPS;

	QueryPerformanceFrequency((LARGE_INTEGER *)&m_PerfFreq);
	QueryPerformanceCounter((LARGE_INTEGER *) &m_LastTime); 
	m_StartTime= m_LastTime;
	m_AppStartTime= m_LastTime;
			
	m_TimeScale			= 1.0f / m_PerfFreq;

	m_FrameRate			= 0;
	m_FPSFrameCount		= 0;
	m_FPSTimeElapsed	= 0.0f;

}

float CTimer::GetAbsoluteTime()
{
	__int64 nowTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&nowTime);
	m_AbsoluteTime = nowTime * m_TimeScale;
    return (FLOAT) m_AbsoluteTime;
}

float CTimer::GetAppTime()
{
	__int64 nowTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&nowTime);
	m_AppTime = ((nowTime - m_AppStartTime)*m_TimeScale);
    return  m_AppTime;

}

float CTimer::GetElaspedTime()
{
	__int64 nowTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&nowTime);
	m_ElapsedTime = (nowTime - m_StartTime)*m_TimeScale;
	m_StartTime=nowTime;
	return m_ElapsedTime;

}