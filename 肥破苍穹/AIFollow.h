#pragma once
#include "stdafx.h"
#include "Entity.h"

class AIFollow
{
private:
    Entity& self;
    Entity& entity;
public:
    AIFollow(Entity& self, Entity& entity);
    ~AIFollow();

    void update(const float& dt);
};