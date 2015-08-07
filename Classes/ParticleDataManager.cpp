#include "ParticleDataManager.h"
#include "CCFileUtils.h"

static ParticleDataManager * s_pParticleDataManager = nullptr;

ParticleDataManager::ParticleDataManager()
{

}

ParticleDataManager::~ParticleDataManager()
{
	m_particleDataCache.clear();
}

ParticleDataManager * ParticleDataManager::getInstance()
{
	if (s_pParticleDataManager == nullptr)
		s_pParticleDataManager = new ParticleDataManager();
	return s_pParticleDataManager;
}

ParticleSystemQuad * ParticleDataManager::getParticle(const std::string &plistFileName)
{
	auto it = m_particleDataCache.find(plistFileName);
	if (it == m_particleDataCache.end())
	{
		loadParticleData(plistFileName);
		it = m_particleDataCache.find(plistFileName);
		if (it == m_particleDataCache.end())
			return nullptr;
	}
	return ParticleSystemQuad::create(it->second);
}

void ParticleDataManager::loadParticleData(const std::string & plistFile)
{
	auto it = m_particleDataCache.find(plistFile);
	if (it != m_particleDataCache.end())
		return;

	auto _plistFile = FileUtils::getInstance()->fullPathForFilename(plistFile);
	ValueMap dict = FileUtils::getInstance()->getValueMapFromFile(_plistFile.c_str());

	CCASSERT(!dict.empty(), "Particles: file not found");
	m_particleDataCache.insert(std::make_pair(plistFile, dict));
}

void ParticleDataManager::clearDataCache()
{
	m_particleDataCache.clear();
}
