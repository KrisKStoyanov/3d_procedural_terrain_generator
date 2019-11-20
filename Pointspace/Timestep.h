#pragma once
class Timestep {
public:
	Timestep(float _time = 0.0f) : m_Time(_time)
	{
	}

	//Default return cast value in seconds
	operator float() const { return m_Time; }

	float GetSeconds() const { return m_Time; }
	float GetMilliseconds() const { return m_Time * 1000.0f; }
private:
	float m_Time;
};

