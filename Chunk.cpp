#include "Chunk.h"
#include "PerlinNoise.h"

#include <vector>

using namespace DirectX;

// Should be noted that I pulled the Perlin Noise script from the tutorial: 
// https://solarianprogrammer.com/2012/07/18/perlin-noise-cpp-11/
// Which has a link to the github repo at the top

Chunk::Chunk(
	XMFLOAT3 _position, 
	XMINT3 _size, 
	Microsoft::WRL::ComPtr<ID3D11Device> _device, 
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext, 
	std::shared_ptr<Material> material
)
{
	device = _device;
	context = _deviceContext;
	position = _position;
	size = _size;

	GenerateMesh();
	chunkEntity = new GameEntity(chunkMesh, material);
}

Chunk::~Chunk()
{
	delete chunkMesh;
	delete chunkEntity;
}

GameEntity* Chunk::GetGameEntity()
{
	return chunkEntity;
}

void Chunk::GenerateMesh()
{
	PerlinNoise pn;

	// Setup the basic objects (I think)
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;

	for (int i = 0; i < size.x; i++) {
		for (int k = 0; k < size.z; k++) {
			float x = position.x + i;
			x -= (float)(size.x - 1) / 2;

			float z = position.z + k;
			z -= (float)(size.z - 1) / 2;

			float y = position.y;
			y += pn.noise(x * 0.1, z * 0.1, 0) * 10;

			Vertex v;
			v.Position = XMFLOAT3(x, y, z);
			v.UV = XMFLOAT2((float)i / size.x, (float)k / size.z);
			v.Tangent = XMFLOAT3(1, 0, 0);
			v.Normal = XMFLOAT3(0, 0, 0);

			vertices.push_back(v);

			if (i > 0 && k > 0) {
				// We can add indices
				int vCount = vertices.size() - 1;

				// First Triangle
				indices.push_back(vCount);
				indices.push_back(vCount - 1 - size.x);
				indices.push_back(vCount - size.x);

				// For each triangle do the cross product to find the normal
				XMFLOAT3 normal;

				XMVECTOR v0 = XMLoadFloat3(&vertices.at(vCount).Position);
				XMVECTOR v1 = XMLoadFloat3(&vertices.at(vCount - 1 - size.x).Position);
				XMVECTOR v2 = XMLoadFloat3(&vertices.at(vCount - size.x).Position);
				XMVECTOR normalVector = XMVector3Cross(v1 - v0, v2 - v0);

				XMStoreFloat3(&vertices.at(vCount).Normal, XMVector3Normalize(XMLoadFloat3(&vertices.at(vCount).Normal) + normalVector));
				XMStoreFloat3(&vertices.at(vCount - 1 - size.x).Normal, XMVector3Normalize(XMLoadFloat3(&vertices.at(vCount - 1 - size.x).Normal) + normalVector));
				XMStoreFloat3(&vertices.at(vCount - size.x).Normal, XMVector3Normalize(XMLoadFloat3(&vertices.at(vCount - size.x).Normal) + normalVector));

				// Second Triangle
				indices.push_back(vCount);
				indices.push_back(vCount - 1);
				indices.push_back(vCount - 1 - size.x);

				v0 = XMLoadFloat3(&vertices.at(vCount).Position);
				v1 = XMLoadFloat3(&vertices.at(vCount - 1).Position);
				v2 = XMLoadFloat3(&vertices.at(vCount - 1 - size.x).Position);
				normalVector = XMVector3Cross(v1 - v0, v2 - v0);

				XMStoreFloat3(&vertices.at(vCount).Normal, XMVector3Normalize(XMLoadFloat3(&vertices.at(vCount).Normal) + normalVector));
				XMStoreFloat3(&vertices.at(vCount - 1).Normal, XMVector3Normalize(XMLoadFloat3(&vertices.at(vCount - 1).Normal) + normalVector));
				XMStoreFloat3(&vertices.at(vCount - 1 - size.x).Normal, XMVector3Normalize(XMLoadFloat3(&vertices.at(vCount - 1 - size.x).Normal) + normalVector));
			}
		}
	}

	chunkMesh = new Mesh(vertices.data(), vertices.size(), indices.data(), indices.size(), device, context);
}
