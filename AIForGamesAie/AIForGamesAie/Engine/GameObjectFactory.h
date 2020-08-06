#pragma once
#include "GameObject.h"
#include "LevelState.h"
#include <vector>
#include <typeindex>

class GameObjectFactory
{
public:
	GameObjectFactory();
	virtual ~GameObjectFactory();

	virtual GameObject* CreateGameObject(std::type_index index, LevelState* level);
	virtual GameObject* CreateGameObject(std::string index, LevelState* level);

	virtual void GetAllGameObjectTypes(std::vector<std::type_index> &output);

protected:

};

