#include "Node.h"

Node::Node(DirectX::XMFLOAT3 _position, float _weight)
{
	position = _position;
	weight = _weight;
}

Node::~Node() { }
