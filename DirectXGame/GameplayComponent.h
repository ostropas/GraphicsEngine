#pragma once
#include "BaseComponent.h"

class GameplayComponent : public BaseComponent
{
protected:
	virtual void Update(float deltaTime) override;
	virtual void BeginPlay() override;
	virtual void OnDestroy() override;
};

