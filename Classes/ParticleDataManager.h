#ifndef __PARTICLE_DATA_MANAGER_H__
#define __PARTICLE_DATA_MANAGER_H__

#include "base/CCValue.h"
#include "2d/CCParticleSystemQuad.h"
USING_NS_CC;

class ParticleDataManager
{
public:
	static ParticleDataManager * getInstance();

	void loadParticleData(const std::string & plistFile);

	ParticleSystemQuad * getParticle(const std::string &plistFileName);

	void clearDataCache();

protected:
	ParticleDataManager();

	~ParticleDataManager();

protected:
	std::map<std::string, ValueMap> m_particleDataCache;
};
#endif // !__PARTICLE_DATA_MANAGER_H__
