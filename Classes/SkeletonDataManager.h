#ifndef SkeletonDataManager_h__
#define SkeletonDataManager_h__
#include <map>
#include <string>
#include "cocos2d.h"
#include "spine/SkeletonData.h"


using namespace std;

#define SP_CLEAR_INTERVAL 0.5f

typedef map<string, spSkeletonData*> MAP_SKELETON_DATA;

class SpSkeletonDataManager: public cocos2d::Ref
{
public:
	SpSkeletonDataManager();
	~SpSkeletonDataManager();

	static SpSkeletonDataManager* getInstance();
	static void destroy();

public:
	void Tick(float dt);

public:
	spSkeletonData* LoadSkeletonData(const string& jsonFile, const string& atlasFile, float scale = 1.0f);

	spSkeletonData* GetSkeletonData(const string& jsonFile);

	void RemoveSkeletonData(const string& jsonFile);
	
	void ClearCacheSkeletonData();

	void ClearUnusedSkeletonData();

private:
	void OpenTick();
	void CloseTick();

private:
	MAP_SKELETON_DATA m_mapSkeletonData;
	float m_fDuration;			// 清理的时间间隔记录
};
#endif // SkeletonDataManager_h__
