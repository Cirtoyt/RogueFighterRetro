#include "Game.h"
#include "GDIPlusManager.h"
#include "Surface.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <random>

GDIPlusManager gdipm;

Game::Game()
	:
	wnd(800, 600, "Rogue Fighter Retro DirectX 11 Game")
{
	Init();
}

int Game::Run()
{
	// Game loop
	while (true)
	{
		if (const auto ecode = Window::ProcessMessages())
		{
			return *ecode;
		}

		Update();

		Render();
	}
}

void Game::Init()
{
	float fovDegrees = 80.0f;
	float fovRadians = DegreeToRadian(fovDegrees);
	float aspectRatio = static_cast<float>(800) / static_cast<float>(600);
	float nearZ = 0.1f;
	float farZ = 1000.0f;
	wnd.Gfx().SetProjection(DirectX::XMMatrixPerspectiveFovLH(fovRadians, aspectRatio, nearZ, farZ));

	// Load overlays
	mainMenuOverlay = std::make_unique<Plane>(wnd.Gfx(), 0.0f, 0.0f, 3.5f, "GameData\\MainMenuOverlay.png");
	mainMenuOverlay->SetScale(4.0f, 3.0f, 1.0f);
	loadingLevelOverlay = std::make_unique<Plane>(wnd.Gfx(), 0.0f, 0.0f, 3.0f, "GameData\\LoadingLevelOverlay.png");
	loadingLevelOverlay->SetScale(4.0f, 3.0f, 1.0f);

	//Load HUD
	hudElements.push_back(std::make_unique<HUD>(wnd.Gfx(), 0.0f, 0.0f, 0.0f, "GameData\\HUD.png"));
	hudElements.back()->SetHUDPosition(0.0f, -0.12f, 0.15f);
	hudElements.back()->SetScale(0.18f, 0.055f, 1.0f);
	hudElements.push_back(std::make_unique<HUD>(wnd.Gfx(), 0.0f, 0.0f, 0.0f, "GameData\\HUDBeam.png"));
	hudElements.back()->SetHUDPosition(-0.1f, 0.0f, 0.12f);
	hudElements.back()->SetHUDRotation(0.0f, 0.0f, DegreeToRadian(10));
	hudElements.back()->SetScale(0.012f, 0.2f, 1.0f);
	hudElements.push_back(std::make_unique<HUD>(wnd.Gfx(), 0.0f, 0.0f, 0.0f, "GameData\\HUDBeam.png"));
	hudElements.back()->SetHUDPosition(0.1f, 0.0f, 0.12f);
	hudElements.back()->SetHUDRotation(0.0f, 0.0f, DegreeToRadian(-10));
	hudElements.back()->SetScale(0.012f, 0.2f, 1.0f);
	hudElements.push_back(std::make_unique<HUD>(wnd.Gfx(), 0.0f, 0.0f, 0.0f, "GameData\\Crosshair.png"));
	hudElements.back()->SetHUDPosition(0.0f, 0.0f, 0.15f);
	hudElements.back()->SetHUDRotation(0.0f, 0.0f, DegreeToRadian(45));
	hudElements.back()->SetScale(0.0025f, 0.0025f, 1.0f);
	hudElements.push_back(std::make_unique<HUD>(wnd.Gfx(), 0.0f, 0.0f, 0.0f, "GameData\\HUDHealth.png"));
	hudElements.back()->SetHUDPosition(0.02f, -0.077f, 0.12f);
	hudElements.back()->SetScale(0.02f, 0.003f, 1.0f);
	hudElements.push_back(std::make_unique<HUD>(wnd.Gfx(), 0.0f, 0.0f, 0.0f, "GameData\\HUDShield.png"));
	hudElements.back()->SetHUDPosition(0.02f, -0.091f, 0.12f);
	hudElements.back()->SetScale(0.02f, 0.003f, 1.0f);

	// Load goal object
	goal = std::make_unique<Box>(wnd.Gfx(), 0.0f, -30.0f, 0.0f, "GameData\\goalSpaceBox.png");
	goal->SetScale(0.4f, 0.4f, 0.4f);
	goal->SetRotation(DegreeToRadian(90.0f), 0.0f, 0.0f);

	// Generate test billboard
	testBillboard = std::make_unique<Plane>(wnd.Gfx(), 3.0f, 2.0f, 5.0f, "GameData\\Conrad.jpg");
}

void Game::GenerateLevel(std::string source, std::string itemSource)
{
	std::ifstream levelFile(source);

	// Map out level space & walls
	if (levelFile.is_open())
	{
		std::string line;
		unsigned int z = 0;
		while (std::getline(levelFile, line))
		{
			for (unsigned int x = 0; x < line.length(); x++)
			{
				switch (line.at(x))
				{
				case '1':
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 1.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, -1.0f, (float)z, GetWallTexture(std::rand() % 8)));
					break;
				case '2':
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, -1.0f, (float)z, GetWallTexture(std::rand() % 8)));
					break;
				case '3':
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, -2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					break;
				case '4':
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 1.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, -2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					break;
				case '5':
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 0.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, -2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					break;
				case '6':
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 0.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, -1.0f, (float)z, GetWallTexture(std::rand() % 8)));
					break;
				case '7':
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 1.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 0.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, -2.0f, (float)z, GetWallTexture(std::rand() % 8)));
					break;
				case '#':
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 1.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, 0.0f, (float)z, GetWallTexture(std::rand() % 8)));
					environment.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, -1.0f, (float)z, GetWallTexture(std::rand() % 8)));
					break;
				}
				/*std::string debugMsg = "Wall found at (x,z): " + std::to_string(x) + ", " + std::to_string(z) + "\n";
				OutputDebugString(debugMsg.c_str());*/
			}
			z++;
		}
		levelFile.close();
	}
	else
	{
		std::string error = "Failed to load LevelFile";
		OutputDebugString(error.c_str());
	}

	// Load items into scene (enemies, the spawn point, keys, the goal, doors, etc)
	std::ifstream levelItemsFile(itemSource);

	if (levelItemsFile.is_open())
	{
		std::string line;
		int z = 0;
		while (std::getline(levelItemsFile, line))
		{
			for (int x = 0; x < line.length(); x++)
			{
				switch (line.at(x))
				{
				case 'S':
					cam.SetPosition(-(float)x, 0.0f, (float)z);
					break;
				case 'G':
					goal->SetPosition(-(float)x, 0.0f, (float)z);
					break;
				case 'K':
					keys.push_back(std::make_unique<Box>(wnd.Gfx(), -(float)x, GetYBasedOnWalls(x, z), (float)z, "GameData\\KeyBox.png"));
					keys.back()->SetScale(0.25f, 0.25f, 0.25f);
					break;
				case 'E':
					enemies.push_back(std::make_unique<Enemy>(wnd.Gfx(), -(float)x, GetYBasedOnWalls(x, z), (float)z, std::rand() % 2 == 1 ? "GameData\\Enemy1.png" : "GameData\\Enemy2.png"));
					enemies.back()->SetScale(0.25f, 0.1f, 1.0f);
					break;
				}
			}
			z++;
		}
		levelItemsFile.close();
	}
	else
	{
		std::string error = "Failed to load LevelItemsFile";
		OutputDebugString(error.c_str());
	}
}

std::string Game::GetWallTexture(int _type)
{
	switch (_type)
	{
	case 0:
		return "GameData\\GreySpaceBox.jpg";
	case 1:
		return "GameData\\GreySpaceBox.jpg";
	case 2:
		return "GameData\\GreySpaceBox.jpg";
	case 3:
		return "GameData\\GreySpaceBox.jpg";
	case 4:
		return "GameData\\GreySpaceBox.jpg";
	case 5:
		return "GameData\\BlueAllSparkCube.jpg";
	case 6:
		return "GameData\\GreyWoodenBox.jpg";
	case 7:
		return "GameData\\GreyAllSparkCube.jpg";
	default:
		return "GameData\\TestLuckyBlockTexture.jpg";
	}
}

float Game::GetYBasedOnWalls(int _x, int _z)
{
	bool wallMiddle = false;
	bool wallAbove = false;
	bool wallBelow = false;
	for (auto& wall : environment)
	{
		if ((int)wall->GetXPos() == -_x && (int)wall->GetZPos() == _z)
		{
			if ((int)wall->GetYPos() == 1)
			{
				wallAbove = true;
			}
			else if (wall->GetYPos() == 0)
			{
				wallMiddle = true;
			}
			else if ((int)wall->GetYPos() == -1)
			{
				wallBelow = true;
			}
		}
	}
	if (wallMiddle && wallBelow)
	{
		return 1.0f;
	}
	else if (wallMiddle && wallAbove)
	{
		return -1.0f;
	}
	else if (wallMiddle)
	{
		return 1.0f;
	}
	return 0.0f;
}

void Game::Update()
{
	float dt = 1;

	UniversalInputs(dt);

	if (mainMenu)
	{
		if (!wnd.CursorEnabled())
		{
			wnd.EnableCursor();
			wnd.mouse.DisableRaw();
		}
	}
	else // In-game
	{
		LoadingLevelChecks(dt);

		PlayerInputs(dt);
		PassiveSpinGameObjects(dt);
		UpdateEnemies(dt);
		UpdateBullets(dt);

		KeyCheck();
		WinCheck();
	}
}

void Game::LoadingLevelChecks(float dt)
{
	if (primeLoadingLevel)
	{
		// Display loading overlay visual, freeing controls
		cam.SetPosition(0.0f, 0.0f, 0.0f);
		cam.SetRotation(0.0f, 0.0f, 0.0f);
		for (auto& hObject : hudElements)
		{
			hObject->SetPosition(hObject->GetXHUDPos(), hObject->GetYHUDPos(), hObject->GetZHUDPos());
			hObject->SetRotation(hObject->GetPitchHUDRot(), hObject->GetYawHUDRot(), hObject->GetRollHUDRot());
		}
		for (auto& enemy : enemies)
		{
			enemy->SetPosition(0.0f, 0.0f, 0.0f);
			enemy->SetRotation(0.0f, 0.0f, 0.0f);
		}

		if (!wnd.CursorEnabled())
		{
			wnd.EnableCursor();
			wnd.mouse.DisableRaw();
		}
		loadingLevel++;
	}
	if (loadingLevel >= 10 * dt)
	{
		// After visual has appeared...
		// Clear old level data
		environment.clear();
		keys.clear();
		enemies.clear();
		bullets.clear();
		showGoal = false;

		// Load new level data
		std::string levelFileName = "Level" + std::to_string(currentLevel);
		GenerateLevel("GameData\\" + levelFileName + ".txt", "GameData\\" + levelFileName + "i.txt");

		// Remove loading visual and confine controls to game
		primeLoadingLevel = false;
		loadingLevel = 0.0f;
		wnd.DisableCursor();
		wnd.mouse.EnableRaw();
	}
}

void Game::UniversalInputs(float dt)
{
	// KEYBOARD
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}
		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (!wnd.CursorEnabled())
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			else
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			break;
		}
	}
	// MOUSE
	while (!wnd.mouse.IsEmpty())
	{
		const auto e = wnd.mouse.Read();
		switch (e.GetType())
		{
		case Mouse::Event::Type::LPress:
			if (mainMenu)
			{
				mainMenu = false;
				primeLoadingLevel = true;
			}
			break;
		}
	}
}

void Game::PlayerInputs(float dt)
{
	// MOVEMENT
	if (!wnd.CursorEnabled())
	{
		// HUD movement
		DirectX::XMFLOAT3 camForward;
		DirectX::XMStoreFloat3(&camForward, cam.GetForward());
		DirectX::XMFLOAT3 camRight;
		DirectX::XMStoreFloat3(&camRight, cam.GetRight());
		DirectX::XMFLOAT3 camUp;
		DirectX::XMStoreFloat3(&camUp, cam.GetUp());
		for (auto& hObject : hudElements)
		{
			DirectX::XMFLOAT3 hudPos = DirectX::XMFLOAT3(cam.GetXPos(), cam.GetYPos(), cam.GetZPos());
			hudPos.x += (camRight.x * hObject->GetXHUDPos()) + (camUp.x * hObject->GetYHUDPos()) + (camForward.x * hObject->GetZHUDPos());
			hudPos.y += (camRight.y * hObject->GetXHUDPos()) + (camUp.y * hObject->GetYHUDPos()) + (camForward.y * hObject->GetZHUDPos());
			hudPos.z += (camRight.z * hObject->GetXHUDPos()) + (camUp.z * hObject->GetYHUDPos()) + (camForward.z * hObject->GetZHUDPos());

			hObject->SetPosition(hudPos.x, hudPos.y, hudPos.z);
		}
		
		// Player (camera) movement
		float forwardMovement = wnd.kbd.KeyIsPressed('W') ? movementSpeed : 0.0f;
		float backwardMovement = wnd.kbd.KeyIsPressed('S') ? -movementSpeed : 0.0f;
		float zMovement = (forwardMovement + backwardMovement) * dt;

		float leftMovement = wnd.kbd.KeyIsPressed('A') ? -movementSpeed : 0.0f;
		float rightMovement = wnd.kbd.KeyIsPressed('D') ? movementSpeed : 0.0f;
		float xMovement = (leftMovement + rightMovement) * dt;

		cam.Translate(xMovement, 0.0f, zMovement);

		// Collision detection
		bool collisionDetected = false;
		for (auto& wall : environment)
		{
			if (cam.GetXPos() + 0.25f >= wall->GetXPos() - 0.5f
				&& wall->GetXPos() + 0.5f >= cam.GetXPos() - 0.25f
				&& cam.GetYPos() + 0.25f >= wall->GetYPos() - 0.5f
				&& wall->GetYPos() + 0.5f >= cam.GetYPos() - 0.25f
				&& cam.GetZPos() + 0.25f >= wall->GetZPos() - 0.5f
				&& wall->GetZPos() + 0.5f >= cam.GetZPos() - 0.25f)
			{
				collisionDetected = true;
			}
		}
		if (collisionDetected)
		{
			cam.Translate(-xMovement, 0.0f, -zMovement);
		}

		// Shooting
		if (wnd.mouse.LeftIsPressed())
		{
			TryToShoot(dt);
		}
	}

	// ROTATION
	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			// HUD rotation
			for (auto& hObject : hudElements)
			{
				hObject->RotateHUD(delta->y * cameraRotSpeed * dt, delta->x * cameraRotSpeed * dt, 0.0f);
			}

			// Enemy rotation (so they always face you)
			for (auto& enemy : enemies)
			{
				enemy->RotateHUD(delta->y * cameraRotSpeed * dt, delta->x * cameraRotSpeed * dt, 0.0f);
			}

			// Player (camera) rotation
			cam.Rotate(delta->y * cameraRotSpeed * dt, delta->x * cameraRotSpeed * dt, 0.0f);
		}
	}
}

void Game::TryToShoot(float dt)
{
	fireTimer += dt;
	if (fireTimer >= fireRate)
	{
		DirectX::XMFLOAT3 camForward;
		DirectX::XMStoreFloat3(&camForward, cam.GetForward());
		bullets.push_back(std::make_unique<Bullet>(wnd.Gfx(), cam.GetXPos() + (camForward.x * 0.55f),
															  cam.GetYPos() + (camForward.y * 0.55f),
															  cam.GetZPos() + (camForward.z * 0.55f), "GameData\\BlueSpaceBox.jpg"));
		bullets.back()->SetScale(0.1f, 0.1f, 0.1f);
		bullets.back()->SetForwardDirection(camForward.x, camForward.y, camForward.z);
		fireTimer = 0.0f;
	}
}

void Game::PassiveSpinGameObjects(float dt)
{
	goal->Rotate(0.0f, 0.02f * dt, 0.0f);

	for (auto& key : keys)
	{
		key->Rotate(0.03f * dt, 0.03f * dt, 0.0f);
	}
}

void Game::UpdateEnemies(float dt)
{
	for (auto& enemy : enemies)
	{
		enemy->UpdateFireTimer(dt);

		if (enemy->WantsToShoot())
		{
			// Check if player is close enough to be shot at
			float inRangeDistance = 3.5f;
			if (cam.GetXPos() + 0.25f >= enemy->GetXPos() - inRangeDistance
				&& enemy->GetXPos() + inRangeDistance >= cam.GetXPos() - 0.25f
				&& cam.GetYPos() + 0.25f >= enemy->GetYPos() - inRangeDistance
				&& enemy->GetYPos() + inRangeDistance >= cam.GetYPos() - 0.25f
				&& cam.GetZPos() + 0.25f >= enemy->GetZPos() - inRangeDistance
				&& enemy->GetZPos() + inRangeDistance >= cam.GetZPos() - 0.25f)
			{
				DirectX::XMVECTOR enemytargetingForward = DirectX::XMVectorSet(cam.GetXPos() - enemy->GetXPos(), cam.GetYPos() - enemy->GetYPos(), cam.GetZPos() - enemy->GetZPos(), 0);
				enemytargetingForward = DirectX::XMVector3Normalize(enemytargetingForward);
				DirectX::XMFLOAT3 enemytargetingForwardNorm;
				DirectX::XMStoreFloat3(&enemytargetingForwardNorm, enemytargetingForward);

				bullets.push_back(std::make_unique<Bullet>(wnd.Gfx(), enemy->GetXPos() + (enemytargetingForwardNorm.x * 0.3f),
					enemy->GetYPos() + (enemytargetingForwardNorm.y * 0.3f),
					enemy->GetZPos() + (enemytargetingForwardNorm.z * 0.3f), "GameData\\RedSpaceBox.jpg"));
				bullets.back()->SetScale(0.1f, 0.1f, 0.1f);
				bullets.back()->SetForwardDirection(enemytargetingForwardNorm.x, enemytargetingForwardNorm.y, enemytargetingForwardNorm.z);
			}
		}
	}
}

void Game::UpdateBullets(float dt)
{
	bool wallCollisionDetected = false;
	bool enemyCollisionDetected = false;
	bool playerCollisionDetected = false;
	bool bulletLifeIsUp = false;
	std::vector<int> bulletHitIdxs;
	std::vector<int> enemyHitIdxs;

	for (int bi = 0; bi < bullets.size(); bi++)
	{
		bullets.at(bi)->UpdatePosition(dt);
		bullets.at(bi)->Rotate(-0.09f * dt, -0.09f * dt, 0.0f);

		if (bullets.at(bi)->IsLifeUp(dt))
		{
			bulletLifeIsUp = true;
			bulletHitIdxs.push_back(bi);
		}

		bool deleteBullet = false;

		// COLLISIONS
		// Check against walls
		for (auto& wall : environment)
		{
			if (bullets.at(bi)->GetXPos() + 0.05f >= wall->GetXPos() - 0.5f
				&& wall->GetXPos() + 0.5f >= bullets.at(bi)->GetXPos() - 0.05f
				&& bullets.at(bi)->GetYPos() + 0.05f >= wall->GetYPos() - 0.5f
				&& wall->GetYPos() + 0.5f >= bullets.at(bi)->GetYPos() - 0.05f
				&& bullets.at(bi)->GetZPos() + 0.05f >= wall->GetZPos() - 0.5f
				&& wall->GetZPos() + 0.5f >= bullets.at(bi)->GetZPos() - 0.05f)
			{
				wallCollisionDetected = true;
				deleteBullet = true;
			}
		}
		
		// Check against enemies
		for (int ei = 0; ei < enemies.size(); ei++)
		{
			if (bullets.at(bi)->GetXPos() + 0.05f >= enemies.at(ei)->GetXPos() - 0.125f
				&& enemies.at(ei)->GetXPos() + 0.125f >= bullets.at(bi)->GetXPos() - 0.05f
				&& bullets.at(bi)->GetYPos() + 0.05f >= enemies.at(ei)->GetYPos() - 0.125f
				&& enemies.at(ei)->GetYPos() + 0.125f >= bullets.at(bi)->GetYPos() - 0.05f
				&& bullets.at(bi)->GetZPos() + 0.05f >= enemies.at(ei)->GetZPos() - 0.125f
				&& enemies.at(ei)->GetZPos() + 0.125f >= bullets.at(bi)->GetZPos() - 0.05f)
			{
				enemyCollisionDetected = true;
				enemyHitIdxs.push_back(ei);
				deleteBullet = true;
			}
		}
		
		// Check against player (camera)
		if (bullets.at(bi)->GetXPos() + 0.05f >= cam.GetXPos() - 0.25f
			&& cam.GetXPos() + 0.25f >= bullets.at(bi)->GetXPos() - 0.05f
			&& bullets.at(bi)->GetYPos() + 0.05f >= cam.GetYPos() - 0.25f
			&& cam.GetYPos() + 0.25f >= bullets.at(bi)->GetYPos() - 0.05f
			&& bullets.at(bi)->GetZPos() + 0.05f >= cam.GetZPos() - 0.25f
			&& cam.GetZPos() + 0.25f >= bullets.at(bi)->GetZPos() - 0.05f)
		{
			playerCollisionDetected = true;
			deleteBullet = true;
		}

		if (deleteBullet)
		{
			bulletHitIdxs.push_back(bi);
		}
	}

	if (wallCollisionDetected || enemyCollisionDetected || playerCollisionDetected || bulletLifeIsUp)
	{
		for (int idx = bulletHitIdxs.size() - 1; idx >= 0; idx--)
		{
			bullets.erase(bullets.begin() + bulletHitIdxs[idx]);
		}
	}
	if (enemyCollisionDetected)
	{
		for (int idx = enemyHitIdxs.size() - 1; idx >= 0; idx--)
		{
			enemies.erase(enemies.begin() + enemyHitIdxs[idx]);
		}
	}
	if (playerCollisionDetected)
	{
		health -= 10;
		hudElements.at(4)->SetScale(0.02f * (health / 100.0f), 0.003f, 1.0f);
	}
}

void Game::KeyCheck()
{
	if (keys.size() >= 0)
	{
		bool keyHit = false;
		int keyVecIdx = 0;
		int keyHitIdx = -1;
		for (auto& key : keys)
		{
			// Check if player (camera) is within key area
			if (cam.GetXPos() <= key->GetXPos() + 0.4f
				&& cam.GetXPos() >= key->GetXPos() - 0.4f
				&& cam.GetYPos() <= key->GetYPos() + 0.4f
				&& cam.GetYPos() >= key->GetYPos() - 0.4f
				&& cam.GetZPos() <= key->GetZPos() + 0.4f
				&& cam.GetZPos() >= key->GetZPos() - 0.4f)
			{
				keyHit = true;
				keyHitIdx = keyVecIdx;
			}
			keyVecIdx++;
		}

		if (keyHit)
		{
			// Destroy key
			keys.erase(keys.begin() + keyHitIdx);
		}
	}

	// Check if all keys have been collected (destroyed)
	if (keys.size() == 0 && !showGoal)
	{
		showGoal = true;
	}
}

void Game::WinCheck()
{
	if (showGoal)
	{
		// Check if player (camera) is within goal area
		if (cam.GetXPos() <= goal->GetXPos() + 0.5f
			&& cam.GetXPos() >= goal->GetXPos() - 0.5f
			&& cam.GetYPos() <= goal->GetYPos() + 0.5f
			&& cam.GetYPos() >= goal->GetYPos() - 0.5f
			&& cam.GetZPos() <= goal->GetZPos() + 0.5f
			&& cam.GetZPos() >= goal->GetZPos() - 0.5f)
		{
			// Increment current level & prime to load it, otherwise if last level reached, end game
			if (currentLevel++ != 3)
			{
				primeLoadingLevel = true;
			}
			else
			{
				// End game!
				currentLevel = 1;
				cam.SetPosition(0.0f, 0.0f, 0.0f);
				cam.SetRotation(0.0f, 0.0f, 0.0f);
				for (auto& hObject : hudElements)
				{
					hObject->SetPosition(hObject->GetXHUDPos(), hObject->GetYHUDPos(), hObject->GetZHUDPos());
					hObject->SetRotation(hObject->GetPitchHUDRot(), hObject->GetYawHUDRot(), hObject->GetRollHUDRot());
				}
				mainMenu = true;
				health = 100.0f;
				hudElements.at(4)->SetScale(0.02f * (health / 100.0f), 0.003f, 1.0f);
				std::string debugMsg = "Game has been won!";
				OutputDebugString(debugMsg.c_str());
			}
		}
	}

	if (health <= 0)
	{
		currentLevel = 1;
		cam.SetPosition(0.0f, 0.0f, 0.0f);
		cam.SetRotation(0.0f, 0.0f, 0.0f);
		for (auto& hObject : hudElements)
		{
			hObject->SetPosition(hObject->GetXHUDPos(), hObject->GetYHUDPos(), hObject->GetZHUDPos());
			hObject->SetRotation(hObject->GetPitchHUDRot(), hObject->GetYawHUDRot(), hObject->GetRollHUDRot());
		}
		mainMenu = true;
		health = 100.0f;
		hudElements.at(4)->SetScale(0.02f * (health / 100.0f), 0.003f, 1.0f);
		std::string debugMsg = "GAMEOVER";
		OutputDebugString(debugMsg.c_str());
	}
}

void Game::Render()
{
	// Set background colour & clear depth buffer
	wnd.Gfx().ClearBuffers(0.1f, 0.1f, 0.1f);

	if (mainMenu)
	{
		mainMenuOverlay->UpdateMatrices(wnd.Gfx());
		mainMenuOverlay->Draw(wnd.Gfx());
	}
	else if (primeLoadingLevel)
	{
		loadingLevelOverlay->UpdateMatrices(wnd.Gfx());
		loadingLevelOverlay->Draw(wnd.Gfx());
	}
	else
	{
		// Draw HUD elements
		for (auto& hObject : hudElements)
		{
			hObject->UpdateMatrices(wnd.Gfx());
			hObject->Draw(wnd.Gfx());
		}

		// Draw objects
		for (auto& object : environment)
		{
			object->UpdateMatrices(wnd.Gfx());
			object->Draw(wnd.Gfx());
		}
		for (auto& key : keys)
		{
			key->UpdateMatrices(wnd.Gfx());
			key->Draw(wnd.Gfx());
		}
		for (auto& enemy : enemies)
		{
			enemy->UpdateMatrices(wnd.Gfx());
			enemy->Draw(wnd.Gfx());
		}
		for (auto& bullet : bullets)
		{
			bullet->UpdateMatrices(wnd.Gfx());
			bullet->Draw(wnd.Gfx());
		}

		if (showGoal)
		{
			goal->UpdateMatrices(wnd.Gfx());
			goal->Draw(wnd.Gfx());
		}
	}

	testBillboard->UpdateMatrices(wnd.Gfx());
	testBillboard->Draw(wnd.Gfx());

	// Update camera matrices
	wnd.Gfx().SetCamera(cam.GetMatrix());

	// Present to screen
	wnd.Gfx().PresentFrame();
}

float Game::DegreeToRadian(float _degree)
{
	return (_degree / 360.0f) * DirectX::XM_2PI;
}
