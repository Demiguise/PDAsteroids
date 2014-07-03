#include "PhysicsManager.h"
#include "Entity.h"
#include "Colliders.h"
#include "Events.h"
#include "IEventManager.h"
#include "GameLog.h"
#include "FileManager.h"

//PhysicsEventReceiver Class
PhysicsEventReceiver::PhysicsEventReceiver(PhysicsManager* parentPhysics)
{
	parent = parentPhysics;
}

PhysicsEventReceiver::PhysicsEventReceiver() {}
PhysicsEventReceiver::~PhysicsEventReceiver() {}

bool PhysicsEventReceiver::Receive(Event::IEvent* e)
{
	return parent->OnEvent(e);
}

//PhysicsManager Class
PhysicsManager::PhysicsManager()
{
	gravAcceleration = 9.81f; //ms^-2
	LoadColliders();
	receiver = new PhysicsEventReceiver(this);
	IEventManager::GetInstance()->AddListener("PhysicsObj Created", receiver);
	IEventManager::GetInstance()->AddListener("PhysicsObj Destroyed", receiver);
	GameLog::GetInstance()->Log(DebugChannel::Physics, DebugLevel::Normal, "[Physics] Initialisation Complete.");
}

PhysicsManager::~PhysicsManager()
{
	IEventManager::GetInstance()->RemoveAllListenersFromEnt(receiver);
	delete receiver;
}

void PhysicsManager::LoadColliders()
{
	colliderModels.push_back(FileManager::GetInstance()->LoadModelData("Models/Colliders/RBCube.obj", FileType::ObjFile));
	colliderModels.push_back(FileManager::GetInstance()->LoadModelData("Models/Colliders/RBCylinder.obj", FileType::ObjFile));
	colliderModels.push_back(FileManager::GetInstance()->LoadModelData("Models/Colliders/RBSphere.obj", FileType::ObjFile));
	colliderModels.push_back(FileManager::GetInstance()->LoadModelData("Models/Colliders/RBPlane.line", FileType::LineFile));
}

void PhysicsManager::RegisterEntity(Entity* entity, ColliderType rbType,
									float mass, float scale)
{
	sceneCollideables.push_back(entity);
	switch (rbType)
	{
	case ColliderType::Box:
		entity->SetRigidBody(new BoxCollider(ScaleRBModel(colliderModels[0], scale), mass, entity));
		break;
	case ColliderType::Planar:
		entity->SetRigidBody(new PlanarCollider(ScaleRBModel(colliderModels[3], scale), mass, entity));
		break;
	case ColliderType::Sphere:
		entity->SetRigidBody(new SphereCollider(ScaleRBModel(colliderModels[2], scale), mass, entity));
		break;
	default:
		entity->SetRigidBody(new RigidBody(mass, entity));
		break;
	}
}

ModelData PhysicsManager::ScaleRBModel(ModelData rbModel, const float& scale)
{
	ModelData scaledModel = rbModel;
	for (UINT i = 0 ; i < rbModel.vData.size() ; ++i)
	{
		scaledModel.vData[i].position = Util::ScalarProduct3D(rbModel.vData[i].position, scale);
	}
	return scaledModel;
}

void PhysicsManager::RemoveEntity(Entity* entity)
{
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		if (sceneCollideables[i] == entity)
		{
			delete entity->rigidBody;
			sceneCollideables.erase(sceneCollideables.begin() + i);
		}
	}
}

void PhysicsManager::Update(const float& dt)
{
	Entity* curEnt;
	
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		curEnt = sceneCollideables[i];
		curEnt->rigidBody->centrePoint = curEnt->position;
		if (curEnt->rigidBody->affectedByGravity == true)
		{
			curEnt->AddForce(EnVector3(0.0f, -1.0f, 0.0f), (gravAcceleration * curEnt->rigidBody->mass));
		}
		curEnt->position += Util::ScalarProduct3D(curEnt->velocity, dt); //Find the new position from the old velocity.
		EnVector3 accel = Util::ScalarProduct3D(curEnt->forceAccum, (1/curEnt->rigidBody->mass)); // F = MA . Therefor A = F/M
		curEnt->velocity += Util::ScalarProduct3D(accel, dt); //Calculate the new velocity from the acceleration.
		curEnt->forceAccum = EnVector3(0.0f, 0.0f, 0.0f); //Null out the force accumulated in this frame
		//GameLog::GetInstance()->Log(DebugChannel::Physics, DebugLevel::Normal, "[Physics] %s | Velocity is (%f, %f, %f).", curEnt->name.c_str(), curEnt->velocity.x, curEnt->velocity.y, curEnt->velocity.z);
	}
	CollisionUpdate(dt);
}

bool PhysicsManager::OnEvent(Event::IEvent* e)
{
	if (e->eType == "PhysicsObj Created")
	{
		Event::PhysicsCreationEvent* event = static_cast<Event::PhysicsCreationEvent*>(e);
		RegisterEntity(event->entity, event->rbType, event->mass, event->scale);
		return true;
	}
	else if (e->eType == "PhysicsObj Destroyed")
	{
		Event::PhysicsCreationEvent* event = static_cast<Event::PhysicsCreationEvent*>(e);
		RemoveEntity(event->entity);
		return true;
	}
	return false;
}

void PhysicsManager::CollisionUpdate(const float& dt)
{
	//First off, let's begin our coarse collision detection. This tests for only intersections in each frame, no fancy stuff.
	std::vector<CollisionPair> sceneCollisions = CoarseCollisionDetection(sceneCollideables);

	if (!sceneCollisions.empty())
	{
		GenerateContacts(sceneCollisions);
		ResolveCollisions(sceneCollisions, dt);
	}
}

std::vector<CollisionPair> PhysicsManager::CoarseCollisionDetection(const std::deque<Entity*>& availableCollideables)
{
	Entity* curEnt;
	std::vector<CollisionPair> possibleCollisions;
	for (UINT i = 0 ; i < sceneCollideables.size() ; ++i)
	{
		curEnt = sceneCollideables[i];
		for (UINT j = 0 ; j < sceneCollideables.size() ; ++j)
		{
			if ((i != j) && (curEnt->TestAABBIntersection(sceneCollideables[j]->AABB))) 
			{ //Collision between two entities has occured.
				if (curEnt->rigidBody->isAwake && sceneCollideables[j]->rigidBody->isAwake)
				{
					bool collisionIsUnique = true;
					for (UINT k = 0 ; k < possibleCollisions.size() ; ++k)
					{
						if((possibleCollisions[k].a == sceneCollideables[j]) && (possibleCollisions[k].b == curEnt ))
						{ //Already a collision between these two entities in the system, ignore it.
							collisionIsUnique = false;
						}
					}
					if (collisionIsUnique) { possibleCollisions.push_back(CollisionPair(curEnt, sceneCollideables[j])); }
				}
			}
		}
	}
	return possibleCollisions;
}

void PhysicsManager::GenerateContacts(std::vector<CollisionPair>& coarseCollisions)
{
	for (UINT i = 0 ; i < coarseCollisions.size() ; ++i)
	{
		CollisionData* contacts = coarseCollisions[i].a->rigidBody->GenerateContacts(coarseCollisions[i].b->rigidBody);
		coarseCollisions[i].data = contacts;
	}
}

//Our game doesn't actually need this as we aren't going to be requiring any actual collsion resolutions.
//Thus this code will merely ensure that the collision is valid and propagate the requisite events.
void PhysicsManager::ResolveCollisions(std::vector<CollisionPair>& possibleCollisions, const float& dt)
{
	//This only considers the first contact data in the collision pair for the moment.
	for ( UINT i = 0 ; i < possibleCollisions.size() ; ++i)
	{
		if (possibleCollisions[i].data == 0 || 
			possibleCollisions[i].a->rigidBody->isAwake == false ||
			possibleCollisions[i].b->rigidBody->isAwake == false) { continue; }
		if (possibleCollisions[i].a->name == possibleCollisions[i].b->name) { continue; }
		Entity* entA = possibleCollisions[i].a;
		Entity* entB = possibleCollisions[i].b;

		Event::CollisionEvent* e = new Event::CollisionEvent();
		e->eType = "Collision Event";
		e->entityA = entA;
		e->entityB = entB;
		IEventManager::GetInstance()->QueueEvent(e);

		GameLog::GetInstance()->Log(DebugChannel::Physics, DebugLevel::Normal, "[Physics] %s has collided with %s.", entA->name.c_str(), entB->name.c_str());
	}
}

bool PhysicsManager::CastRay(EnVector3 pos, EnVector3 dir,  float dist, RayCastHit& raycastOut)
{
	//std::vector<RayCastHit> contactInfos;
	//ModelData curRigidBodyMdl;
	//EnMatrix3x3 linePlaneMatrix;
	//EnVector3 linePlaneVector;
	//EnVector3 linePlaneCoefficients;
	//curRigidBodyMdl = entityrigidBody->rbModel;
	////Jep, testing every single triangle! Fun times!
	////Testing every other triangle would work in this instance, unsure how that might apply. Changing to J+6 should do the trick.
	////I feel bad about just assuming the model data would work like this.
	//for (UINT j = 0 ; j < curRigidBodyMdl.iData.size() ; j += 3) 
	//{
	//	//==============
	//	//TODO:
	//	//Need to take into account the rotation of the object as well
	//	UINT v1 = curRigidBodyMdl.iData[j];
	//	UINT v2 = curRigidBodyMdl.iData[j+1];
	//	UINT v3 = curRigidBodyMdl.iData[j+2];
	//	linePlaneVector = pos - curRigidBodyMdl.vData[v1].position;
	//	linePlaneMatrix.r[0] = Util::ScalarProduct3D((testableEntities[i]->position - pos), dist);
	//	linePlaneMatrix.r[1] = (testableEntities[i]->position + curRigidBodyMdl.vData[v2].position) - (testableEntities[i]->position + curRigidBodyMdl.vData[v1].position);
	//	linePlaneMatrix.r[2] = (testableEntities[i]->position + curRigidBodyMdl.vData[v3].position) - (testableEntities[i]->position + curRigidBodyMdl.vData[v1].position);
	//	if (linePlaneMatrix.Invert()) //Line is intersecting the plane.
	//	{
	//		linePlaneCoefficients = linePlaneVector.MatrixMult3x3(linePlaneMatrix);
	//		if((linePlaneCoefficients.x > 0) && (linePlaneCoefficients.x < 1)) //Lies within our lines maximum distance
	//		{
	//			EnVector3 collisionNormal = (curRigidBodyMdl.vData[v2].position - curRigidBodyMdl.vData[v1].position).Cross(curRigidBodyMdl.vData[v3].position - curRigidBodyMdl.vData[v1].position).Normalized();
	//			contactInfos.push_back(RayCastHit(testableEntities[i],
	//									(pos + Util::ScalarProduct3D((testableEntities[i]->position - pos).Normalized(), linePlaneCoefficients.x)),
	//									collisionNormal));
	//		}

	//	}
	//	else //Line is parallel to the plane.
	//	{
	//		//==============
	//		//TODO:
	//		//Some extra checking to ensure it's not inside the actual plane.
	//	}
	//}
	//if (!contactInfos.empty())
	//{
	//	if (contactInfos.size() == 1) //Easy out
	//	{ 
	//		raycastOut = contactInfos[0];
	//		return true; 
	//	} 
	//	float minDist = dist;
	//	int posIndex = 0;
	//	for (UINT i = 0 ; i < contactInfos.size() ; ++i)
	//	{
	//		float distToContact = (pos - contactInfos[i].collisionPos).GetMagnitude();
	//		if ( distToContact < minDist) 
	//		{ 
	//			minDist = distToContact;
	//			posIndex = i; 
	//		}
	//	}
	//	raycastOut = contactInfos[posIndex];
	//	return true;
	//}
	return false;
}

