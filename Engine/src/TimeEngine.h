#pragma once
#include "Singleton.h"

class TimeEngine : public Singleton<TimeEngine>
{
public:
	void SetDeltaTime(float deltaTime) { m_DeltaTime = deltaTime; }
private:
	float m_DeltaTime{ 0.0f };
};