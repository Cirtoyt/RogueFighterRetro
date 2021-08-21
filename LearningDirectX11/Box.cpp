#include "Box.h"
#include <d3dcompiler.h>
#include <sstream>
#include <DirectXMath.h>
#include <vector>
#include "Surface.h"

namespace dx = DirectX;

Box::Box(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath)
	:
	xPos(xPos),
	yPos(yPos),
	zPos(zPos),
	surfacePath(surfacePath)
{
	// Generate surface from file
	const auto s = Surface::FromFile(surfacePath);

	// Create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	textureDesc.Width = s.GetWidth();
	textureDesc.Height = s.GetHeight();
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA tsd = {};
	tsd.pSysMem = s.GetBufferPtr();
	tsd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);
	gfx.GetDevice()->CreateTexture2D(&textureDesc, &tsd, &pTexture);

	// Create the resoure view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	gfx.GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvDesc, &pTextureView);

	if (!pVertexBuffer)
	{
		// Create vertex buffer
		D3D11_BUFFER_DESC vbd = {};
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.CPUAccessFlags = 0u;
		vbd.MiscFlags = 0u;
		vbd.ByteWidth = sizeof(vertices);
		vbd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA vsd = {};
		vsd.pSysMem = vertices;
		gfx.GetDevice()->CreateBuffer(&vbd, &vsd, &pVertexBuffer);
	}

	// Create sampler state
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	gfx.GetDevice()->CreateSamplerState(&samplerDesc, &pSampler);

	// Create index buffer
	D3D11_BUFFER_DESC ibd{};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	gfx.GetDevice()->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = 64u;
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &vertices;
	gfx.GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer);

	// Create pixel shader
	D3DReadFileToBlob(L"TexturePS.cso", &pPSBlob);
	gfx.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);

	// Create vertex shader
	D3DReadFileToBlob(L"TextureVS.cso", &pVSBlob);
	gfx.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);

	// Input layout
	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"Position", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TexCoord", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	gfx.GetDevice()->CreateInputLayout(ied, (UINT)std::size(ied),
		pVSBlob->GetBufferPointer(),
		pVSBlob->GetBufferSize(),
		&pInputLayout);

	// Set primitive topology to triangle list (groups of 3 vertices)
	gfx.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Box::UpdateMatrices(Graphics& gfx)
{
	dx::XMMATRIX tf = dx::XMMatrixTranspose(
		dx::XMMatrixScaling(xScale, yScale, zScale) *
		dx::XMMatrixRotationRollPitchYaw(pitch, yaw, roll) *
		dx::XMMatrixTranslation(xPos, yPos, zPos) *
		gfx.GetCamera() *
		gfx.GetProjection()
	);

	D3D11_MAPPED_SUBRESOURCE msr;
	gfx.GetContext()->Map(pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
	memcpy(msr.pData, &tf, sizeof(tf));
	gfx.GetContext()->Unmap(pConstantBuffer.Get(), 0u);
}

void Box::Draw(Graphics& gfx)
{
	// Bind everything to pipeline
	gfx.GetContext()->PSSetShaderResources(0u, 1u, pTextureView.GetAddressOf());
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	gfx.GetContext()->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
	gfx.GetContext()->PSSetSamplers(0u, 1u, pSampler.GetAddressOf());
	gfx.GetContext()->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	gfx.GetContext()->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());
	gfx.GetContext()->PSSetShader(pPixelShader.Get(), nullptr, 0u);
	gfx.GetContext()->VSSetShader(pVertexShader.Get(), nullptr, 0u);
	gfx.GetContext()->IASetInputLayout(pInputLayout.Get());

	//Draw
	gfx.GetContext()->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

void Box::SetPosition(float _x, float _y, float _z)
{
	xPos = _x;
	yPos = _y;
	zPos = _z;
}
// Sets in radians
void Box::SetRotation(float _pitch, float _yaw, float _roll)
{
	pitch = _pitch;
	yaw = _yaw;
	roll = _roll;
}

void Box::SetScale(float _x, float _y, float _z)
{
	xScale = _x;
	yScale = _y;
	zScale = _z;
}

void Box::Rotate(float _pitch, float _yaw, float _roll)
{
	pitch += _pitch;
	yaw += _yaw;
	roll += _roll;
}

float Box::GetXPos()
{
	return xPos;
}

float Box::GetYPos()
{
	return yPos;
}

float Box::GetZPos()
{
	return zPos;
}

float Box::GetPitch()
{
	return pitch;
}

float Box::GetYaw()
{
	return yaw;
}

float Box::GetRoll()
{
	return roll;
}
