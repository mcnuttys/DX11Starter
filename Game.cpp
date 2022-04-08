#include "Game.h"
#include "Vertex.h"
#include "Input.h"
#include "GameEntity.h"

#include <math.h>
#include <iostream>
#include "WICTextureLoader.h"

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true),			   // Show extra stats (fps) in title bar?
	vsync(false)
{
#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

	mainCamera = 0;
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// Note: Since we're using smart pointers (ComPtr),
	// we don't need to explicitly clean up those DirectX objects
	// - If we weren't using smart pointers, we'd need
	//   to call Release() on each DirectX object created in Game
	for (auto& m : meshes) { delete m; }
	for (auto& e : entities) { delete e; }
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Load Textures
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/river_small_rocks_diff_1k.png").c_str(), 0, rockyTexture.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/river_small_rocks_nor_gl_1k.png").c_str(), 0, rockyTextureNormal.GetAddressOf());
	//CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/weathered_brown_planks_diff_1k.png").c_str(), 0, woodPlanksTexture.GetAddressOf());
	//CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/weathered_brown_planks_nor_gl_1k.png").c_str(), 0, woodPlanksTextureNormal.GetAddressOf());
	
	//CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cobblestone.png").c_str(), 0, rockyTexture.GetAddressOf());
	//CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cobblestone_normals.png").c_str(), 0, rockyTextureNormal.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cushion.png").c_str(), 0, woodPlanksTexture.GetAddressOf());
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/cushion_normals.png").c_str(), 0, woodPlanksTextureNormal.GetAddressOf());


	Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
	D3D11_SAMPLER_DESC sampDescription = {};
	sampDescription.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDescription.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDescription.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDescription.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDescription.MaxAnisotropy = 16;
	sampDescription.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDescription, sampler.GetAddressOf());
	
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();

	mat1 = std::make_shared<Material>(XMFLOAT3(1, 1, 1), 0.75f, XMFLOAT2(1, 1), XMFLOAT2(0,0), vertexShader, pixelShader);
	mat2 = std::make_shared<Material>(XMFLOAT3(1, 1, 1), 0.5f, XMFLOAT2(2, 2), XMFLOAT2(0,0), vertexShader, pixelShader);
	mat3 = std::make_shared<Material>(XMFLOAT3(1, 1, 1), 0.25f, XMFLOAT2(1, 1), XMFLOAT2(0,0), vertexShader, pixelShader);
	matFancy = std::make_shared<Material>(XMFLOAT3(0, 0, 1), 1.0f, XMFLOAT2(1, 1), XMFLOAT2(0,0), vertexShader, fancyPixelShader);

	mat1->AddTextureSRV("SurfaceTexture", rockyTexture);
	mat1->AddTextureSRV("NormalMap", rockyTextureNormal);
	mat1->AddSampler("BasicSampler", sampler);

	mat2->AddTextureSRV("SurfaceTexture", woodPlanksTexture);
	mat2->AddTextureSRV("NormalMap", woodPlanksTextureNormal);
	mat2->AddSampler("BasicSampler", sampler);

	mat3->AddTextureSRV("SurfaceTexture", rockyTexture);
	mat3->AddTextureSRV("NormalMap", rockyTextureNormal);
	mat3->AddSampler("BasicSampler", sampler);

	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	mainCamera = std::make_shared<Camera>((float)width / height, DirectX::XMFLOAT3(0.0f, 0.0f, -10.0f));
	ambientColor = XMFLOAT3(0.25f, 0.25f, 0.25f);

	std::shared_ptr<SimpleVertexShader> skyVS = std::make_shared<SimpleVertexShader>(device, context, GetFullPathTo_Wide(L"SkyVS.cso").c_str());
	std::shared_ptr<SimplePixelShader> skyPS = std::make_shared<SimplePixelShader>(device, context, GetFullPathTo_Wide(L"SkyPS.cso").c_str());

	sky = std::make_shared<Sky>(
		CreateCubemap(
			GetFullPathTo_Wide(L"../../Assets/Textures/Clouds Blue/right.png").c_str(),
			GetFullPathTo_Wide(L"../../Assets/Textures/Clouds Blue/left.png").c_str(),
			GetFullPathTo_Wide(L"../../Assets/Textures/Clouds Blue/up.png").c_str(),
			GetFullPathTo_Wide(L"../../Assets/Textures/Clouds Blue/down.png").c_str(),
			GetFullPathTo_Wide(L"../../Assets/Textures/Clouds Blue/front.png").c_str(),
			GetFullPathTo_Wide(L"../../Assets/Textures/Clouds Blue/back.png").c_str()
		),
		meshes[0],
		skyVS,
		skyPS,
		sampler,
		device,
		context);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = std::make_shared<SimpleVertexShader>(device, context, GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = std::make_shared<SimplePixelShader>(device, context, GetFullPathTo_Wide(L"PixelShader.cso").c_str());
	fancyPixelShader = std::make_shared<SimplePixelShader>(device, context, GetFullPathTo_Wide(L"FancyShader.cso").c_str());
}



// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	Vertex triVertices[] =
	{
		{ XMFLOAT3(+0.0f, +0.5f, +0.0f) },
		{ XMFLOAT3(+0.5f, -0.5f, +0.0f) },
		{ XMFLOAT3(-0.5f, -0.5f, +0.0f) },
	};

	unsigned int triIndices[] = { 0, 1, 2 };

	//Mesh* triangle = new Mesh(triVertices, 3, triIndices, 3, device, context);
	//meshes.push_back(triangle);

	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/cube.obj").c_str(), device, context));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/cylinder.obj").c_str(), device, context));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/helix.obj").c_str(), device, context));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/quad.obj").c_str(), device, context));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/quad_double_sided.obj").c_str(), device, context));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device, context));
	meshes.push_back(new Mesh(GetFullPathTo("../../Assets/Models/torus.obj").c_str(), device, context));

	GameEntity* entity0 = new GameEntity(meshes[0], mat1);
	GameEntity* entity1 = new GameEntity(meshes[1], mat2);
	GameEntity* entity2 = new GameEntity(meshes[2], mat3);
	GameEntity* entity3 = new GameEntity(meshes[3], mat1);
	GameEntity* entity4 = new GameEntity(meshes[4], mat2);
	GameEntity* entity5 = new GameEntity(meshes[5], mat3);
	GameEntity* entity6 = new GameEntity(meshes[6], mat1);

	entities.push_back(entity0);
	entities.push_back(entity1);
	entities.push_back(entity2);
	entities.push_back(entity3);
	entities.push_back(entity4);
	entities.push_back(entity5);
	entities.push_back(entity6);

	for (int i = -4; i < 3; i++) {
		int j = i + 4;
		entities[j]->GetTransform()->SetPosition(i * 3, 0, 0);
	}

	Light dirLight0 = {};
	dirLight0.Color = XMFLOAT3(1.0f, 1.0f, 1.0f);
	dirLight0.Type = LIGHT_TYPE_DIRECTIONAL;
	dirLight0.Intensity = 0.5f;
	dirLight0.Direction = XMFLOAT3(1, -1, 0);

	//Light dirLight1 = {};
	//dirLight1.Color = XMFLOAT3(1, 1, 1);
	//dirLight1.Type = LIGHT_TYPE_DIRECTIONAL;
	//dirLight1.Intensity = 0.5f;
	//dirLight1.Direction = XMFLOAT3(0, 1, 0);
	//
	//Light dirLight2 = {};
	//dirLight2.Color = XMFLOAT3(1, 1, 1);
	//dirLight2.Type = LIGHT_TYPE_DIRECTIONAL;
	//dirLight2.Intensity = 0.5f;
	//dirLight2.Direction = XMFLOAT3(0, 0, 1);

	Light pointLight0 = {};
	pointLight0.Color = XMFLOAT3(1, 0, 0);
	pointLight0.Type = LIGHT_TYPE_POINT;
	pointLight0.Intensity = 1.0f;
	pointLight0.Position = XMFLOAT3(-4, 1, 0);
	pointLight0.Range = 5;

	Light pointLight1 = {};
	pointLight1.Color = XMFLOAT3(1, 1, 0);
	pointLight1.Type = LIGHT_TYPE_POINT;
	pointLight1.Intensity = 1.0f;
	pointLight1.Position = XMFLOAT3(1.5f, 1, 0);
	pointLight1.Range = 2;

	lights.push_back(dirLight0);
	//lights.push_back(dirLight1);
	//lights.push_back(dirLight2);
	lights.push_back(pointLight0);
	lights.push_back(pointLight1);
}

// --------------------------------------------------------
// Loads six individual textures (the six faces of a cube map), then
// creates a blank cube map and copies each of the six textures to
// another face.  Afterwards, creates a shader resource view for
// the cube map and cleans up all of the temporary resources.
// --------------------------------------------------------
Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Game::CreateCubemap(
	const wchar_t* right,
	const wchar_t* left,
	const wchar_t* up,
	const wchar_t* down,
	const wchar_t* front,
	const wchar_t* back)
{
	// Load the 6 textures into an array.
	// - We need references to the TEXTURES, not the SHADER RESOURCE VIEWS!
	// - Specifically NOT generating mipmaps, as we usually don't need them for the sky!
	// - Order matters here!  +X, -X, +Y, -Y, +Z, -Z
	ID3D11Texture2D* textures[6] = {};
	CreateWICTextureFromFile(device.Get(), right, (ID3D11Resource**)&textures[0], 0);
	CreateWICTextureFromFile(device.Get(), left, (ID3D11Resource**)&textures[1], 0);
	CreateWICTextureFromFile(device.Get(), up, (ID3D11Resource**)&textures[2], 0);
	CreateWICTextureFromFile(device.Get(), down, (ID3D11Resource**)&textures[3], 0);
	CreateWICTextureFromFile(device.Get(), front, (ID3D11Resource**)&textures[4], 0);
	CreateWICTextureFromFile(device.Get(), back, (ID3D11Resource**)&textures[5], 0); 

	// We'll assume all of the textures are the same color format and resolution,
	// so get the description of the first shader resource view
	D3D11_TEXTURE2D_DESC faceDesc = {};
	textures[0]->GetDesc(&faceDesc);

	// Describe the resource for the cube map, which is simply 
	// a "texture 2d array".  This is a special GPU resource format, 
	// NOT just a C++ array of textures!!!
	D3D11_TEXTURE2D_DESC cubeDesc = {};
	cubeDesc.ArraySize = 6; // Cube map!
	cubeDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE; // We'll be using as a texture in a shader
	cubeDesc.CPUAccessFlags = 0; // No read back
	cubeDesc.Format = faceDesc.Format; // Match the loaded texture's color format
	cubeDesc.Width = faceDesc.Width;  // Match the size
	cubeDesc.Height = faceDesc.Height; // Match the size
	cubeDesc.MipLevels = 1; // Only need 1
	cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE; // A CUBE MAP, not 6 separate textures
	cubeDesc.Usage = D3D11_USAGE_DEFAULT; // Standard usage
	cubeDesc.SampleDesc.Count = 1;
	cubeDesc.SampleDesc.Quality = 0;

	// Create the actual texture resource
	ID3D11Texture2D* cubeMapTexture = 0;
	device->CreateTexture2D(&cubeDesc, 0, &cubeMapTexture);

	// Loop through the individual face textures and copy them,
	// one at a time, to the cube map texure
	for (int i = 0; i < 6; i++)
	{
		// Calculate the subresource position to copy into
		unsigned int subresource = D3D11CalcSubresource(
			0,	// Which mip (zero, since there's only one)
			i,	// Which array element?
			1); 	// How many mip levels are in the texture?

		// Copy from one resource (texture) to another
		context->CopySubresourceRegion(
			cubeMapTexture, // Destination resource
			subresource,	// Dest subresource index (one of the array elements)
			0, 0, 0,		// XYZ location of copy
			textures[i],	// Source resource
			0,	// Source subresource index (we're assuming there's only one)
			0);	// Source subresource "box" of data to copy (zero means the whole thing)
	}

	// At this point, all of the faces have been copied into the 
	// cube map texture, so we can describe a shader resource view for it
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = cubeDesc.Format; 	// Same format as texture
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE; // Treat this as a cube!
	srvDesc.TextureCube.MipLevels = 1;	// Only need access to 1 mip
	srvDesc.TextureCube.MostDetailedMip = 0; // Index of the first mip we want to see

	// Make the SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> cubeSRV;
	device->CreateShaderResourceView(cubeMapTexture, &srvDesc, cubeSRV.GetAddressOf());

	// Now that we're done, clean up the stuff we don't need anymore
	cubeMapTexture->Release();  // Done with this particular reference (the SRV has another)
	for (int i = 0; i < 6; i++)
		textures[i]->Release();

	// Send back the SRV, which is what we need for our shaders
	return cubeSRV;
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	if (mainCamera) {
		mainCamera->UpdateProjectionMatrix((float)this->width / this->height);
	}
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Example input checking: Quit if the escape key is pressed
	if (Input::GetInstance().KeyDown(VK_ESCAPE))
		Quit();

	//float offsetX = sin(totalTime) * deltaTime;
	//float offsetY = cos(totalTime) * deltaTime;

	for (auto& e : entities)
	{
		//e->GetTransform()->SetScale(sin(totalTime), cos(totalTime), 1.0f);
		e->GetTransform()->Rotate(0, deltaTime, 0);
	}

	mainCamera->Update(deltaTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Ensure the pipeline knows how to interpret the data (numbers)
	// from the vertex buffer.  
	// - If all of your 3D models use the exact same vertex layout,
	//    this could simply be done once in Init()
	// - However, this isn't always the case (but might be for this course)
	context->IASetInputLayout(inputLayout.Get());

	for (auto& e : entities)
	{
		std::shared_ptr<SimplePixelShader> ps = e->GetMaterial()->GetPixelShader();

		ps->SetFloat3("ambient", ambientColor);
		ps->SetData("lights", &lights[0], sizeof(Light) * (int)lights.size());

		e->Draw(context, mainCamera, totalTime);
	}

	sky->Draw(mainCamera.get());

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(vsync ? 1 : 0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}