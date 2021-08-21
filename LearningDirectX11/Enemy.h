#pragma once
#include"Plane.h"
#include"Bullet.h"

class Enemy : public Plane
{
public:
	Enemy(Graphics& gfx, float xPos, float yPos, float zPos, std::string surfacePath);
	~Enemy() = default;
	void RotateHUD(float _pitch, float _yaw, float _roll);
	void UpdateFireTimer(float dt);
	bool WantsToShoot();
	DirectX::XMVECTOR GetForward();
private:
	float fireRate = 60.0f;
	float fireTimer = 1000.0f;
};

