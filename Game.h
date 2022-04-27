#pragma once
#include "Mesh.h"
#include "Camera.h"
#include "GameEntity.h"
#include "DXCore.h"
#include "SimpleShader.h"
#include "Material.h"
#include "Lights.h"
#include "Sky.h"
#include "Chunk.h"

#include <vector>
#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <memory>

class Game 
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Should we use vsync to limit the frame rate?
	bool vsync;

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders(); 
	void CreateBasicGeometry();

	void CreateTerrain(XMFLOAT3 pos, XMINT2 size);
	void CreateTerrain(XMFLOAT3 pos, XMINT3 size);

	// Note the usage of ComPtr below
	//  - This is a smart pointer for objects that abide by the
	//    Component Object Model, which DirectX objects do
	//  - More info here: https://github.com/Microsoft/DirectXTK/wiki/ComPtr
		
	// Shaders and shader-related constructs
	Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rockyTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rockyTextureNormal;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> woodPlanksTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> woodPlanksTextureNormal;

	std::shared_ptr<SimpleVertexShader> vertexShader;
	std::shared_ptr<SimplePixelShader> pixelShader;
	std::shared_ptr<SimplePixelShader> terrainPixelShader;

	std::shared_ptr<Camera> mainCamera;
	std::vector<Mesh*> meshes;
	std::vector<GameEntity*> entities;
	std::vector<Chunk*> chunks;

	std::shared_ptr<Material> mat0;
	std::shared_ptr<Material> mat1;
	std::shared_ptr<Material> mat2;
	std::shared_ptr<Material> mat3;
	std::shared_ptr<Material> mat4;
	std::shared_ptr<Material> mat5;
	std::shared_ptr<Material> mat6;
	std::shared_ptr<Material> terrainMaterial;

	DirectX::XMFLOAT3 ambientColor;

	std::vector<Light> lights;

	std::shared_ptr<Sky> sky;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> skyResourceView;

	// --------------------------------------------------------
	// Author: Chris Cascioli
	// Purpose: Creates a cube map on the GPU from 6 individual textures
	// 
	// - You are allowed to directly copy/paste this into your code base
	//   for assignments, given that you clearly cite that this is not
	//   code of your own design.
	//
	// - Note: This code assumes you’re putting the function in Game.cpp, 
	//   you’ve included WICTextureLoader.h and you have an ID3D11Device 
	//   ComPtr called “device”.  Make any adjustments necessary for
	//   your own implementation.
	// --------------------------------------------------------
	
	// Helper for creating a cubemap from 6 individual textures
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> CreateCubemap(
		const wchar_t* right,
		const wchar_t* left,
		const wchar_t* up,
		const wchar_t* down,
		const wchar_t* front,
		const wchar_t* back);
};

