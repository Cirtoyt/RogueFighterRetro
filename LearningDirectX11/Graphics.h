#pragma once
#include "CirtWin.h"
#include <d3d11.h>
#include <wrl.h>
#include <DirectXMath.h>

class Graphics
{
public:
	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;
	void ClearBuffers(float red, float green, float blue) noexcept;
	void PresentFrame();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	void SetProjection(DirectX::FXMMATRIX proj);
	DirectX::XMMATRIX GetProjection();
	void SetCamera(DirectX::FXMMATRIX cam);
	DirectX::XMMATRIX GetCamera();
private:
	Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDSV;
	DirectX::XMMATRIX projection;
	DirectX::XMMATRIX camera;
};

