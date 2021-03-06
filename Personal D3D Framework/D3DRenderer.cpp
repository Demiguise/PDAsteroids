#include "D3DRenderer.h"
#include "RenderableObject.h"
#include "GameLog.h"
#include "UI.h"
#include "ScopedTimer.h"

//Initialisations
D3DRenderer::D3DRenderer(HINSTANCE hInstance, HWND hWnd, UINT wHeight, UINT wWidth)
	:	mWindowHeight(wHeight),
		mWindowWidth (wWidth),
		mDepthStencilBuffer(0),
		mDepthStencilView(0),
		mRenderTargetView(0),
		mSwapChain(0)
{
	
	rendererName = "DirectX11";
	mHInst = hInstance;
	mHWnd = hWnd;	
	setWireframe = false;
	//Quickly initializing the Projection and View matrices.
	mProjMatrix = mCamViewMatrix = XMMatrixIdentity();
	Init();
}

D3DRenderer::~D3DRenderer()
{
	ReleaseCOM(md3dDevice);
	ReleaseCOM(mDeviceContext);
	ReleaseCOM(mSwapChain);
	IEventManager::GetInstance()->RemoveAllListenersFromEnt(receiver);
	delete receiver;
}

bool D3DRenderer::Init()
{
	GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal, "[D3DRenderer] Beginning Initialisation.");
	GameLog::GetInstance()->Log(DebugChannel::Rendering, DebugLevel::None, "[D3DRenderer] Testing Rendering Channel! %i", 2);
	if (!InitDirect3D()) { return false; }
	InitEffects();
	CreateInputLayer();
	mBufferManager = new D3DBufferManager(md3dDevice);
	UserInterface = new UI(md3dDevice);
	GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal, "[D3DRenderer] Initialisation Complete.");
	return true;
}

bool D3DRenderer::InitDirect3D()
{
	//Description for the swap chain we're using
	DXGI_SWAP_CHAIN_DESC sd;

	
	//Defining the swap chain buffer
	sd.BufferDesc.Width = mWindowWidth;
	sd.BufferDesc.Height = mWindowHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;

	//MSAA Options (Set to no MSAA)
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	
	//Other Options
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mHWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel;
	HR(D3D11CreateDeviceAndSwapChain(	0,
										D3D_DRIVER_TYPE_HARDWARE,
										0,
										D3D11_CREATE_DEVICE_DEBUG,
										0,
										0,
										D3D11_SDK_VERSION,
										&sd,
										&mSwapChain,
										&md3dDevice,
										&featureLevel,
										&mDeviceContext));

	if (!md3dDevice)
	{
		GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal,"[D3DRenderer] D3DDevice creation failed.");
		MessageBox(mHWnd, L"D3DDevice creation has failed.", 0, 0);
		return false;
	}
	OnResize(mWindowHeight, mWindowWidth);
	GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Normal,"[D3DRenderer] D3DDevice creation succeeded.");
	return true;
}

void D3DRenderer::CreateInputLayer()
{
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{"Position",	0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Normal",		0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TexCoord",	0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	D3DX11_PASS_DESC passDesc;
	mETech->GetPassByIndex(0)->GetDesc(&passDesc);
	md3dDevice->CreateInputLayout(vertexDesc, 3, passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize, &mInputLayout);

	D3D11_RASTERIZER_DESC rSD;
	rSD.FillMode = D3D11_FILL_WIREFRAME;
	rSD.CullMode = D3D11_CULL_NONE;
	rSD.FrontCounterClockwise = FALSE;
	rSD.DepthBias = 0;
	rSD.SlopeScaledDepthBias = 0.0f;
	rSD.DepthBiasClamp = 0.0f;
	rSD.DepthClipEnable = TRUE;
	rSD.ScissorEnable = FALSE;
	rSD.MultisampleEnable = FALSE;
	rSD.AntialiasedLineEnable = FALSE;

	md3dDevice->CreateRasterizerState(&rSD, &mRSD);
}

void D3DRenderer::InitEffects()
{
	ID3D10Blob* compiledShader = 0;
	ID3D10Blob* compilationMsgs = 0;
	HRESULT hr = D3DX11CompileFromFile(L"Effects/color.fx", 0, 0, 0, "fx_5_0", 0, 0, 0, &compiledShader, &compilationMsgs, 0);

	if (compilationMsgs !=0)
	{
		MessageBoxA(0, (char*)compilationMsgs->GetBufferPointer(), 0 ,0);
		ReleaseCOM(compilationMsgs);
	}

	if (FAILED(hr))
	{
		DXTrace(__FILE__,(DWORD)__LINE__, hr, L"D3DX11CompileFromFile", true);
	}

	HR(D3DX11CreateEffectFromMemory(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(),
		0, md3dDevice, &mEffect));

	ReleaseCOM(compiledShader);
	mETech = mEffect->GetTechniqueByName("ColorTech");
}

//Runtime

void D3DRenderer::OnResize(UINT newHeight, UINT newWidth)
{
	mWindowHeight = newHeight;
	mWindowWidth = newWidth;

	//Release old COM interfaces.
	ReleaseCOM(mDepthStencilBuffer);
	ReleaseCOM(mDepthStencilView);
	ReleaseCOM(mRenderTargetView);

	//Resize the buffers to the new sizes and rebind them to the pipeline through
	//the RenderTargetView.
	HR(mSwapChain->ResizeBuffers(1, mWindowWidth, mWindowHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(md3dDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);

	//Description for the Depth Stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	//Defining the buffer
	depthStencilDesc.Height = mWindowHeight;
	depthStencilDesc.Width = mWindowWidth;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//The MSAA settings for the depthStencilDesc MUST match the SwapChain.
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	//Other options
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;
	
	//Actually create the DepthStencilView
	md3dDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer);
	md3dDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView);

	//Bind the Render target and DepthStencilView to the pipline
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mScreenViewport.TopLeftX = 0.0f;
	mScreenViewport.TopLeftY = 0.0f;
	mScreenViewport.Width = static_cast<float>(newWidth);
	mScreenViewport.Height = static_cast<float>(newHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;
	
	mDeviceContext->RSSetViewports(1, &mScreenViewport);
	
	mProjMatrix = XMMatrixPerspectiveFovLH(0.25f*3.14f, newWidth/newHeight, 1.0f, 1000.0f);
}

void D3DRenderer::UpdateScene(const EnMatrix4x4& camPos, const EnVector3& target)
{
	XMVECTOR up = XMVectorSet(camPos.c[1].x, camPos.c[1].y, camPos.c[1].z, 1.0f);
	XMVECTOR camPosVector = XMVectorSet(camPos.c[3].x, camPos.c[3].y, camPos.c[3].z, 1.0f);
	XMVECTOR camTarget = XMVectorSet(target.x, target.y, target.z, 1.0f);
	mCamViewMatrix = XMMatrixLookAtLH(camPosVector, camTarget, up);
	//mCamViewMatrix = ConvertToXMMatrix(camPos);
}

bool D3DRenderer::OnEvent(Event::IEvent* e)
{
	if (e->eType == "RenderableObject Created")
	{
		CRenderableObject* newRenderableObject = static_cast<CRenderableObject*>(static_cast<Event::EntityEvent*>(e)->entity);
		mBufferManager->LinkEntities(newRenderableObject, newRenderableObject->mesh.semanticName);
		return true;
	}
	else if (e->eType == "RenderableObject Destroyed")
	{
		CRenderableObject* destroyedRenderableObject = static_cast<CRenderableObject*>(static_cast<Event::EntityEvent*>(e)->entity);
		mBufferManager->RemoveEntity(destroyedRenderableObject);
		return true;
	}
	else if (e->eType == "Asteroid Activated")
	{
		CRenderableObject* activatedAsteroid = static_cast<CRenderableObject*>(static_cast<Event::EntityEvent*>(e)->entity);
		mBufferManager->LinkEntities(activatedAsteroid, activatedAsteroid->mesh.semanticName);
		return false;
	}
	return false;
}

void D3DRenderer::DrawScene()
{

	std::vector<packedBufferData> sceneData = mBufferManager->GrabSceneBuffers();

	//float bgColor[4] = {0.69f, 0.77f, 0.87f, 1.0f};
	float bgColor[4] = {0.0f, 0.0f, 0.0f, 1.0f};
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, bgColor);
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL, 1.0f, 0);

	mDeviceContext->IASetInputLayout(mInputLayout);
	
	if (setWireframe) { mDeviceContext->RSSetState(mRSD); }

	ID3DX11EffectMatrixVariable* mEffectWorldViewProj = mEffect->GetVariableByName("gWorldViewProj")->AsMatrix();

	D3DX11_TECHNIQUE_DESC techDesc;
	mETech->GetDesc(&techDesc);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	for(UINT p = 0 ; p < techDesc.Passes ; ++p)
	{
		for (UINT i = 0 ; i < sceneData.size() ; ++i)
		{
			mDeviceContext->IASetPrimitiveTopology(sceneData[i].bufferContents.topologyType);
			mDeviceContext->IASetVertexBuffers(0, 1, &sceneData[i].bufferContents.vertexBuffer, &stride, &offset);
			mDeviceContext->IASetIndexBuffer(sceneData[i].bufferContents.indexBuffer , DXGI_FORMAT_R32_UINT, 0);
			XMMATRIX wvpMatrix = BuildWVPMatrix(sceneData[i].owningEntity->localToWorld, mCamViewMatrix, mProjMatrix);
			mEffectWorldViewProj->SetMatrix(reinterpret_cast<float*>(&wvpMatrix));
			mETech->GetPassByIndex(p)->Apply(0, mDeviceContext);
			mDeviceContext->DrawIndexed(sceneData[i].bufferContents.iCount, 0, 0);
		}
	}

	UserInterface->Render(mDeviceContext);

	mSwapChain->Present(0,0);
}

UI* D3DRenderer::GetUI()
{
	return UserInterface;
}

void D3DRenderer::CreateBuffer(ModelData model, std::string name)
{
	mBufferManager->InitNewBuffer(model, name);
}

XMMATRIX D3DRenderer::BuildWVPMatrix(EnMatrix4x4& entWorldMatrix, XMMATRIX& view, XMMATRIX& proj)
{
	XMMATRIX worldMatrix = ConvertToXMMatrix(entWorldMatrix);
	XMMATRIX lView = view;
	XMMATRIX lProj = proj;
	XMMATRIX newMatrix = worldMatrix * lView * lProj;
	return newMatrix;
}

XMMATRIX D3DRenderer::ConvertToXMMatrix(EnMatrix4x4 m)
{
	//We have to transpose as it appears that the XMMATRIX is expecting information in a per row basis, however we use per column.
	XMMATRIX newMatrix(	m.c[0].x, m.c[1].x, m.c[2].x, m.c[3].x, 
						m.c[0].y, m.c[1].y, m.c[2].y, m.c[3].y, 
						m.c[0].z, m.c[1].z, m.c[2].z, m.c[3].z, 
						m.c[0].w, m.c[1].w, m.c[2].w, m.c[3].w);
	return XMMatrixTranspose(newMatrix);
}