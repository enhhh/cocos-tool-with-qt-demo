#ifndef __SpineParticle_H__
#define __SpineParticle_H__

#include <spine/spine.h>
#include "cocos2d.h"
#include <spine/SkeletonRenderer.h>
#include <spine/SkeletonAnimation.h>
#include "2d/CCParticleSystem.h"

USING_NS_CC;

typedef struct s_slotData
{
	std::string slotsName;//挂载点名称
	std::string particleName;//粒子名称
	Vec2 offset;//偏移
} s_spSlot;

typedef struct s_SpineParticleAction
{
	std::string actionName;
	std::string spineActionName;//对应spine中的动画信息
	std::map<std::string,s_spSlot> slotsData; //挂载信息
} s_spAction;

class SpineParticle
	: public cocos2d::Node
{
public:
	static SpineParticle* create(const std::string &jsonFile);

	void playAnimation(int trackIndex,const std::string &name,bool loop);

//工具使用的方法
public:
	static SpineParticle* create();

	void clearNode();

	void saveToJsonFile(const std::string &path);

	std::string getResoucePath(const std::string &path);

	void setSpine(const std::string &jsonPath,const std::string &atlasPath);

protected:
	SpineParticle();

	~SpineParticle();

	void unloadAllParticle();

	void loadParticle(const std::map<std::string,s_spSlot> & data);
public:
	std::string m_spineAtlasFile;
	std::string m_spineJsonFile;
	spine::SkeletonAnimation* m_skeleton;
	std::map<std::string, ParticleSystem*> m_particles;
	std::vector<s_spAction*> m_actions; //动作
	std::vector<ParticleSystem*> m_loadedParticles;//已经挂载的
};
#endif // !__SpineParticle_H__
