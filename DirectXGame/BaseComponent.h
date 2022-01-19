#pragma once
class BaseComponent
{
public:
	virtual void Update(float deltaTime) = 0;
	virtual void BeginPlay() = 0;
	virtual void OnDestroy() = 0;
};

