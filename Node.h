#pragma once
#include <DirectXMath.h>

class Node
{
public:
	Node(DirectX::XMFLOAT3 _position, float _weight);
	~Node();

	DirectX::XMFLOAT3 position;
	float weight;
};

