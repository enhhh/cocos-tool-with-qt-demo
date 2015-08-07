#ifndef __DATA_STRUCT_H_
#define __DATA_STRUCT_H_
#include "cocos2d.h"
USING_NS_CC;

typedef struct s_slotData
{
	std::string slotsName;//挂载点名称
	std::string particleName;//粒子名称
	Vec2 offset;//偏移
} s_spSlot;

typedef struct s_SpineParticleAction
{
	std::string actionName; //动作名
	std::string spineActionName;//对应spine中的动画信息
	std::vector<s_spSlot> slotsData; //挂载信息
} s_spAction;

typedef struct s_SpineParticle
{
	std::string spineJsonFile;
	std::string spineAtlasFile;
	spine::SkeletonAnimation * skeleton;
	std::map<std::string, ParticleSystem*> particles;
	std::vector<s_spAction*> actions; //动作
public:
	s_spAction * getActionWithName(const std::string & name)
	{
		for (auto i : actions)
		{
			if (i->actionName == name)
				return i;
		}
		return nullptr;
	}

} s_spData;


#endif // !__DATA_STRUCT_H_
