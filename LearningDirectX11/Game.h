#pragma once
#include "Window.h"
#include <chrono>
#include "Box.h"
#include "Plane.h"
#include "Camera.h"
#include <map>
#include "HUD.h"
#include "Enemy.h"
#include "Bullet.h"

class Game
{
public:
	Game();
	// master frame / message loop
	int Run();
private:
	void Init();
	void GenerateLevel(std::string source, std::string itemSource);
	std::string GetWallTexture(int _type);
	float GetYBasedOnWalls(int _x, int _z);
	void Update();
	void LoadingLevelChecks(float dt);
	void UniversalInputs(float dt);
	void PlayerInputs(float dt);
	void TryToShoot(float dt);
	void PassiveSpinGameObjects(float dt);
	void UpdateEnemies(float dt);
	void UpdateBullets(float dt);
	void KeyCheck();
	void WinCheck();
	void Render();
	float DegreeToRadian(float _degree);

	Window wnd;
	Camera cam;
	std::vector<std::unique_ptr<class Box>> environment;
	std::unique_ptr<class Plane> testBillboard;
	std::unique_ptr<class Plane> mainMenuOverlay;
	std::unique_ptr<class Plane> loadingLevelOverlay;
	std::vector<std::unique_ptr<class HUD>> hudElements;
	std::unique_ptr<class Box> goal;
	std::vector<std::unique_ptr<class Box>> keys;
	std::vector<std::unique_ptr<class Enemy>> enemies;
	std::vector<std::unique_ptr<class Bullet>> bullets;
	float movementSpeed = 0.025f;
	float cameraRotSpeed = 0.003f;
	int currentLevel = 1;
	bool primeLoadingLevel = true;
	float loadingLevel = 0;
	bool mainMenu = true;
	int health = 100;
	int sheildEnergy = 100;
	bool shieldOn = false;
	bool showGoal = false;
	float fireRate = 22.0f;
	float fireTimer = 1000.0f;
};