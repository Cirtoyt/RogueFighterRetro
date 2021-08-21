#pragma once
#include "Graphics.h"
#include <string>

class Box
{
public:
	Box(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath);
	~Box() = default;
	void UpdateMatrices(Graphics& gfx);
	void Draw(Graphics& gfx);
	void SetPosition(float _x, float _y, float _z);
	// Sets in radians
	void SetRotation(float _pitch, float _yaw, float _roll);
	void SetScale(float _x, float _y, float _z);
	void Rotate(float _pitch, float _yaw, float _roll);
	float GetXPos();
	float GetYPos();
	float GetZPos();
	float GetPitch();
	float GetYaw();
	float GetRoll();
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

	Vertex vertices[14] =
	{
		vertices[0].pos = { -0.5f, -0.5f, -0.5f },
		vertices[0].tex = { 2.0f / 3.0f, 0.0f / 4.0f },
		vertices[1].pos = { 0.5f, -0.5f, -0.5f },
		vertices[1].tex = { 1.0f / 3.0f, 0.0f / 4.0f },
		vertices[2].pos = { -0.5f, 0.5f, -0.5f },
		vertices[2].tex = { 2.0f / 3.0f, 1.0f / 4.0f },
		vertices[3].pos = { 0.5f, 0.5f, -0.5f },
		vertices[3].tex = { 1.0f / 3.0f, 1.0f / 4.0f },
		vertices[4].pos = { -0.5f, -0.5f, 0.5f },
		vertices[4].tex = { 2.0f / 3.0f, 3.0f / 4.0f },
		vertices[5].pos = { 0.5f, -0.5f, 0.5f },
		vertices[5].tex = { 1.0f / 3.0f, 3.0f / 4.0f },
		vertices[6].pos = { -0.5f, 0.5f, 0.5f },
		vertices[6].tex = { 2.0f / 3.0f, 2.0f / 4.0f },
		vertices[7].pos = { 0.5f, 0.5f, 0.5f },
		vertices[7].tex = { 1.0f / 3.0f, 2.0f / 4.0f },
		vertices[8].pos = { -0.5f, -0.5f, -0.5f },
		vertices[8].tex = { 2.0f / 3.0f, 4.0f / 4.0f },
		vertices[9].pos = { 0.5f, -0.5f, -0.5f },
		vertices[9].tex = { 1.0f / 3.0f, 4.0f / 4.0f },
		vertices[10].pos = { -0.5f, -0.5f, -0.5f },
		vertices[10].tex = { 3.0f / 3.0f, 1.0f / 4.0f },
		vertices[11].pos = { -0.5f, -0.5f, 0.5f },
		vertices[11].tex = { 3.0f / 3.0f, 2.0f / 4.0f },
		vertices[12].pos = { 0.5f, -0.5f, -0.5f },
		vertices[12].tex = { 0.0f / 3.0f, 1.0f / 4.0f },
		vertices[13].pos = { 0.5f, -0.5f, 0.5f },
		vertices[13].tex = { 0.0f / 3.0f, 2.0f / 4.0f },

	};

	const unsigned short indices[36] =
	{
		0,2,1,   2,3,1,
		4,8,5,   5,8,9,
		2,6,3,   3,6,7,
		4,5,7,   4,7,6,
		2,10,11, 2,11,6,
		12,3,7,  12,7,13,
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