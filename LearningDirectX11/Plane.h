#pragma once
#include "Graphics.h"
#include <string>

class Plane
{
public:
	Plane(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath);
	~Plane() = default;
	void UpdateMatrices(Graphics& gfx);
	void Draw(Graphics& gfx);
	void SetPosition(float _x, float _y, float _z);
	// Sets in radians
	void SetRotation(float _pitch, float _yaw, float _roll);
	void SetScale(float _x, float _y, float _z);
	float GetXPos();
	float GetYPos();
	float GetZPos();
protected:
	float xPos = 0.0f;
	float yPos = 0.0f;
	float zPos = 0.0f;
	float roll = 0.0f;
	float pitch = 0.0f;
	float yaw = 0.0f;
	float xScale = 1.0f;
	float yScale = 1.0f;
	float zScale = 1.0f;
	std::string surfacePath;
private:
	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;
		struct
		{
			float u;
			float v;
		} tex;
	};

	Vertex vertices[4] =
	{
		{ -1.0f, 1.0f, 0.0f, 0.0f, 0.0f},
		{ 1.0f, 1.0f, 0.0f, 1.0f, 0.0f},
		{ -1.0f, -1.0f, 0.0f, 0.0f, 1.0f},
		{ 1.0f, -1.0f, 0.0f, 1.0f, 1.0f},
	};

	const unsigned short indices[6] =
	{
		0,1,3, 0,3,2,
	};

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pTextureView;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pPSBlob;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pVSBlob;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};