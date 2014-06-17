#include "Player.h"

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
	eventMan->RemoveAllListenersFromEnt(this);
}

void Player::Init()
{
	name = "Player";
	SceneManager::GetInstance()->RegisterEntity(this, uID);
	PhysicsManager::GetInstance()->RegisterEntity(this, ColliderType::Planar, 1);

	AddListeners();
}

void Player::AddListeners()
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->AddListener("UserKeysActive", this);
	eventMan->AddListener("Collision", this);
}

void Player::RemoveListener(std::string eventType)
{
	IEventManager* eventMan = IEventManager::GetInstance();
	eventMan->RemoveListener(eventType, this);
}

void Player::Update()
{
	UpdateQuaternion();
	UpdateLocalToWorldMatrix();
	if(rigidBody != 0)
	{
		rigidBody->ReCalculateAABB(AABB);
	}
}

void Player::FireShot()
{
	Projectile* newShot = new Projectile(	FileManager::GetInstance()->LoadModelData("Models/Ship.line", FileType::LineFile),
											position);
	
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
				rotation += EnVector3(0.0f, 0.0f, -3.0f);
				break;
			case GameKey::D:
				rotation += EnVector3(0.0f, 0.0f, 3.0f);
				break;
			case GameKey::S:
				AddForce(GetLocalAxis(1) , -10);
				break;
			case GameKey::W:
				AddForce(GetLocalAxis(1), 10);
				break;
			case GameKey::SPACE:
				FireShot();
				break;
			}
		}
		return true;
	}
	else if(e->eType == "Collision")
	{
		GameLog::GetInstance()->Log(DebugChannel::All, DebugLevel::Normal, "Player has recieved event regarding a collision");
	}
	return false;
}
