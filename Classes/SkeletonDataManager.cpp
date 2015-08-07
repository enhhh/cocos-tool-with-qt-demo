#include "SkeletonDataManager.h"
#include "spine/Atlas.h"
#include "base/ccMacros.h"
#include "spine/SkeletonJson.h"

SpSkeletonDataManager::SpSkeletonDataManager()
: m_fDuration(0.0f)
{

}

SpSkeletonDataManager::~SpSkeletonDataManager()
{
	ClearCacheSkeletonData();
}

static SpSkeletonDataManager* instance = nullptr;
SpSkeletonDataManager* SpSkeletonDataManager::getInstance()
{
	if (instance == nullptr)
	{
		instance = new SpSkeletonDataManager;
		instance->OpenTick();
	}
	return instance;
}

void SpSkeletonDataManager::destroy()
{
	if (instance)
	{
		instance->ClearCacheSkeletonData();
		instance->CloseTick();
	}

	delete instance;
	instance = nullptr;
}

spSkeletonData* SpSkeletonDataManager::LoadSkeletonData(const string& jsonFile, const string& atlasFile, float scale /*= 1.0f*/)
{
	auto iter = m_mapSkeletonData.find(jsonFile);
	if ( iter == m_mapSkeletonData.end())
	{
		auto _atlas = spAtlas_createFromFile(atlasFile.c_str(), 0);
		CCASSERT(_atlas, "Error reading atlas file.");

		spSkeletonJson* json = spSkeletonJson_create(_atlas);
		if (json)
		{
			json->scale = 1.0f/*scale*/;//强制统一为1.0f，需要设置尺寸自行在使用的地方设置
			spSkeletonData* skeletonData = spSkeletonJson_readSkeletonDataFile(json, jsonFile.c_str());
			CCASSERT(skeletonData, json->error ? json->error : "Error reading skeleton data file.");
			skeletonData->refCount = 0;
			skeletonData->ifRefed = 0;
			spSkeletonJson_dispose(json);

			m_mapSkeletonData.insert(make_pair(jsonFile, skeletonData));
			return skeletonData;
		}
		
		return nullptr;
	}
	else
	{
		return iter->second;
	}
}

spSkeletonData* SpSkeletonDataManager::GetSkeletonData(const string& jsonFile)
{
	auto iter = m_mapSkeletonData.find(jsonFile);
	if (iter != m_mapSkeletonData.end())
	{
		return iter->second;
	}
	return nullptr;
}

void SpSkeletonDataManager::RemoveSkeletonData(const string& jsonFile)
{
	auto iter = m_mapSkeletonData.find(jsonFile);
	if (iter != m_mapSkeletonData.end())
	{
		spSkeletonData_dispose(iter->second);
		m_mapSkeletonData.erase(iter);
	}
}

void SpSkeletonDataManager::ClearCacheSkeletonData()
{
	for (auto _pair: m_mapSkeletonData)
	{
		spSkeletonData_dispose(_pair.second);
	}
	m_mapSkeletonData.clear();
}

void SpSkeletonDataManager::Tick(float dt)
{
	m_fDuration += dt;
	if (m_fDuration >= SP_CLEAR_INTERVAL)
	{
		m_fDuration = 0.0f;
		for (auto iter = m_mapSkeletonData.begin(); iter != m_mapSkeletonData.end(); )
		{
			if (iter->second->ifRefed == 1 && iter->second->refCount == 0)
			{
				spSkeletonData_dispose(iter->second);
				m_mapSkeletonData.erase(iter++);
			}
			else
				iter++;
		}
	}
}

void SpSkeletonDataManager::OpenTick()
{
	cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(SpSkeletonDataManager::Tick), this, 0.0f, false);
}

void SpSkeletonDataManager::CloseTick()
{
	cocos2d::Director::getInstance()->getScheduler()->unschedule(schedule_selector(SpSkeletonDataManager::Tick), this);
}

void SpSkeletonDataManager::ClearUnusedSkeletonData()
{
	for (auto iter = m_mapSkeletonData.begin(); iter != m_mapSkeletonData.end();)
	{
		if (iter->second->ifRefed == 0 && iter->second->refCount == 0)
		{
			spSkeletonData_dispose(iter->second);
			m_mapSkeletonData.erase(iter++);
		}
		else
			iter++;
	}
}
