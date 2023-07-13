#include "stdafx.h"
#include "AnimationController.h"
#include "ResourceMgr.h"

void AnimationController::AddClip(const AnimationClip& newClip)
{
	if (clips.find(newClip.id) == clips.end())
	{
		clips.insert({ newClip.id, newClip });
	}
}

void AnimationController::Update(float dt)
{
	if (!isPlaying)
	{
		return;
	}

	accumTime += dt * speed;
	if (accumTime < clipDuration)
	{
		return;
	}

	accumTime = 0.f;
	++currentFrame;

	if (currentFrame == totalFrame)
	{
		if(!queue.empty())
		{
			std::string id = queue.front();
			queue.pop();
			Play(id, false);
			return;
		}
		else
		{
			switch (currentClip->loopType)
			{
			case AnimationLoopTypes::Single:
				currentFrame = totalFrame - 1;
				return;
			case AnimationLoopTypes::Loop:
				currentFrame = 0;
				break;
			}
		}
	}

	if (currentClip->frames[currentFrame].action != nullptr) // 함수를 참조하고있지 않을 때 널 반환
	{
		currentClip->frames[currentFrame].action();
	}

	if (target != nullptr)
	{
		SetFrame(currentClip->frames[currentFrame]);
	}
}

void AnimationController::Play(const std::string& clipId, bool clearQueue)
{
	if (clearQueue)
	{
		while (!queue.empty())
		{
			queue.pop(); // 하나 빼기
		}
	}

	auto find = clips.find(clipId);
	if (find == clips.end())
	{
		Stop();
		std::cout << "ERR: NOT EXIST CLIP" << std::endl;
		return;
	}

	isPlaying = true;

	currentClip = &find->second;
	currentFrame = 0;
	totalFrame = currentClip->frames.size();
	clipDuration = 1.f / currentClip->fps;
	accumTime = 0.f;

	SetFrame(currentClip->frames[currentFrame]);
}

void AnimationController::PlayQueue(const std::string& clipId)
{
	queue.push(clipId);
}

void AnimationController::Stop()
{
	isPlaying = false;
}

void AnimationController::SetFrame(const AnimationFrame& frame)
{
	sf::Texture* tex = RESOURCE_MGR.GetTexture(frame.textureId);
	target->setTexture(*tex);
	target->setTextureRect(frame.texCoord);
}
