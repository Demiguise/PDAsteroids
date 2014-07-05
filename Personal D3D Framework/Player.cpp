#include "Player.h"
#include "Colliders.h"
#include "Events.h"
#include "IEventManager.h"
#include "FileManager.h"
#include "GameLog.h"
#include "Projectile.h"

Player::Player(ModelData newMesh, EnVector3 initPos, EnVector3 initRot)
{
	position = initPos;
	rotation = initRot;
	mesh = newMesh;
	Init();
}

Player::Player(ModelData newMesh, EnVector3 initPos)
{
	position = initPos;
	rotation = EnVector3::Zero();
	mesh = newMesh;
	Init();
}

Player::Player(ModelData newMesh)
{
	position = EnVector3::Zero();
	rotation = EnVector3::Zero();
	mesh = newMesh;
	Init();
}

Player::~Player()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveAllListenersFromEnt(receiver);
}

void Player::Init()
{
	name = "Player";
	
	Event::PhysicsCreationEvent* e = new Event::PhysicsCreationEvent();
	e->eType = "PhysicsObj Created";
	e->entity = this;
	e->mass = 1.0f;
	e->scale = 1.0f;
	e->rbType = ColliderType::Sphere;
	IEventManager::GetInstance()->QueueEvent(e);

	//Init all projectiles in the pool to this one type;
	projectilePool.reserve(MAX_PLAYER_PROJECTILES);
	for (UINT i = 0 ; i < MAX_PLAYER_PROJECTILES ; ++i)
	{
		projectilePool.push_back(new Projectile(FileManager::GetInstance()->LoadModelData("Models/Projectile.line", FileType::LineFile)));
		projectilePool[i]->name = "Projectile";
	}
	maxInvulTime = 3.0f;
	lives = 3;
	maxReloadTime = 0.1f;
	AddListeners();
}

void Player::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("UserKeysActive", receiver);
	eventMan->AddListener("Collision Event", receiver);
}

void Player::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, receiver);
}

void Player::Update()
{
	Entity::Update();
	projectileHelper = EnVector3(0.0f, 1.8f, 0.0f).MatrixMult4x4(localToWorld);
	if (curReloadTime > 0.0f) { curReloadTime -= GAME_STEP; }
	if (curInvulTime > 0.0f) { curInvulTime -= GAME_STEP; }
}

void Player::FireShot()
{
  	for (UINT i = 0 ; i < MAX_PLAYER_PROJECTILES ; ++i)
	{
		if(!projectilePool[i]->active)
		{
			projectilePool[i]->OnActivated(projectileHelper, GetLocalAxis(1));
			return;
		}
	}
}

bool Player::OnEvent(Event::IEvent* e)
{
	if (e->eType == "UserKeysActive")
	{
		std::vector<GameKey> keyPresses = *static_cast<Event::InputEvent*>(e)->keyEvents;
		for (UINT i = 0 ; i < keyPresses.size() ; i++)
		{
			switch (keyPresses[i])
			{
			case GameKey::A:
				rotation += EnVector3(0.0f, 0.0f, -5.0f);
				break;
			case GameKey::D:
				rotation += EnVector3(0.0f, 0.0f, 5.0f);
				break;
			case GameKey::S:
				AddForce(GetLocalAxis(1) , -40);
				break;
			case GameKey::W:
				AddForce(GetLocalAxis(1), 40);
				break;
			case GameKey::SPACE:
				if (curReloadTime <= 0.0f)
				{
					FireShot();
					curReloadTime = maxReloadTime;
				}
				break;
			}
		} 
		return false;
	}
	else if(e->eType == "Collision Event")
	{
		Event::CollisionEvent* eCol = static_cast<Event::CollisionEvent*>(e);
		if ((	eCol->entityA->uID == this->uID &&
				eCol->entityB->name == "Asteroid") || (
				eCol->entityA->name == "Asteroid" &&
				eCol->entityB->uID == this->uID))
		{
			if (curInvulTime <= 0.0f) //If the invulnerability timer has run out.
			{
				--lives;
				if (lives == 0) 
				{
					GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Low, "[Player] Player has lost all lives.");
					return false;
					//Game needs to end/restart
				}
				GameLog::GetInstance()->Log(DebugChannel::Main, DebugLevel::Low, "[Player] Player has lost a life. (%i) remaining.", lives);
				curInvulTime = maxInvulTime; 
			}
			else
			{
				//Ignore collision due to invulnerability;
			}
		}
	}
	return false;
}
