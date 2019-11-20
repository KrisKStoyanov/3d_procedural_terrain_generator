#include "WaterGen.h"

WaterGen::WaterGen()
{
}

WaterGen::~WaterGen()
{
}

void WaterGen::OnUpdate(float _deltaTime)
{
	for (int i = 0; i < m_VertexCollection.size(); ++i) {
		m_VertexCollection[i].Coords.y +=
			sin(m_VertexCollection[i].Coords.y * PI / 180) * _deltaTime;
	}
}

void WaterGen::Configure()
{
}

void WaterGen::Draw(Camera*& _camera)
{
}

void WaterGen::Clear()
{
}
