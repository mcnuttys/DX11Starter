#pragma once

#include "DXCore.h"
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include "Vertex.h"

class Mesh
{
public:
	Mesh(
		Vertex _vertices[],
		int _vertexCount,
		unsigned int _indices[],
		int _indexCount,
		Microsoft::WRL::ComPtr<ID3D11Device> _device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext
	);
	Mesh(
		const char* objFile,
		Microsoft::WRL::ComPtr<ID3D11Device> _device,
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> _deviceContext);

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer();
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer();
	int GetIndexCount();
	void Draw();

private:
	void CreateBuffers(Vertex* vertices, int numVerts, unsigned int* indices, int numIndices, Microsoft::WRL::ComPtr<ID3D11Device> device);

	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> context;
	int indexCount;
};

