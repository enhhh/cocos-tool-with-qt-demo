#include "SpineParticle.h"
#include "json/prettywriter.h"
#include "json/document.h"
#include "cocostudio/DictionaryHelper.h"
#include "json/stringbuffer.h"
using namespace cocostudio;

SpineParticle::SpineParticle()
: m_skeleton(nullptr)
{

}

SpineParticle::~SpineParticle()
{
	m_skeleton = nullptr;
	for (auto &it : m_particles)
	{
		it.second->release();
	}
	m_spineAtlasFile = "";
	m_spineJsonFile = "";
	m_particles.clear();
	for (auto &it : m_actions)
	{
		delete it;
	}
	m_actions.clear();
}

void SpineParticle::clearNode()
{
	this->removeAllChildrenWithCleanup(true);
	m_skeleton = nullptr;
	for (auto &it : m_particles)
	{
		it.second->release();
	}
	m_spineAtlasFile = "";
	m_spineJsonFile = "";
	m_particles.clear();
	for (auto &it : m_actions)
	{
		delete it;
	}
	m_actions.clear();
}

void SpineParticle::playAnimation(int trackIndex, const std::string &name, bool loop)
{
	for (auto &it : m_actions)
	{
		if (it->actionName == name)
		{
			unloadAllParticle();
			loadParticle(it->slotsData);
			m_skeleton->setAnimation(trackIndex, it->spineActionName.c_str(), loop);
		}
	}
}

void SpineParticle::unloadAllParticle()
{
	for (auto it : m_loadedParticles)
	{
		it->removeFromParent();
	}
	m_loadedParticles.clear();
}

void SpineParticle::loadParticle(const std::map<std::string,s_spSlot> & data)
{
	for (auto &it : data)
	{
		auto particle = ParticleSystemQuad::create(it.second.particleName.c_str());
		m_skeleton->addChild(particle);
		m_loadedParticles.push_back(particle);
		int slotsCount = m_skeleton->getSkeleton()->data->slotsCount;
		auto spineSlots = m_skeleton->getSkeleton()->data->spineSlots;
		Vec2 pos = Vec2::ZERO;
		Vec2 offset = it.second.offset;
		for (int i = 0; i < slotsCount; i++)
		{
			if (spineSlots[i]->name == it.second.slotsName)
			{
				pos = Vec2(spineSlots[i]->boneData->x, spineSlots[i]->boneData->y);
				break;
			}
		}
		particle->setPosition(pos + offset);
	}

}

SpineParticle* SpineParticle::create(const std::string &jsonFile)
{
	SpineParticle * ptr = SpineParticle::create();
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonFile);
	rapidjson::Document resultJson;
	resultJson.Parse<0>(contentStr.c_str());
	if (resultJson.HasParseError())
	{
		log("GetParseError %s\n", resultJson.GetParseError());
		return nullptr;
	}
	const rapidjson::Value& spineInfo = DICTOOL->getSubDictionary_json(resultJson, "spineInfo");
	ptr->setSpine(DICTOOL->getStringValue_json(spineInfo, "jsonFile"), DICTOOL->getStringValue_json(spineInfo, "atlasFile"));
	int particleCount = DICTOOL->getArrayCount_json(resultJson, "particleInfo");
	for (int i = 0; i < particleCount; i++)
	{
		std::string path = DICTOOL->getStringValueFromArray_json(resultJson, "particleInfo", i);
		auto particle = ParticleSystem::create(path);
		if (particle)
		{
			particle->retain();
			ptr->m_particles.insert(std::make_pair(path, particle));
		}
	}
	int actionCount = DICTOOL->getArrayCount_json(resultJson, "actionsInfo");
	for (int i = 0; i < actionCount; i++)
	{
		const rapidjson::Value& actionValue = DICTOOL->getDictionaryFromArray_json(resultJson, "actionsInfo", i);
		s_spAction * action= new s_spAction();
		action->actionName = DICTOOL->getStringValue_json(actionValue, "actionName");
		action->spineActionName = DICTOOL->getStringValue_json(actionValue, "spineActionName");
		int slotsCount = DICTOOL->getArrayCount_json(actionValue, "slotsInfo");
		for (int j = 0; j < slotsCount; j++)
		{
			const rapidjson::Value& slotValue = DICTOOL->getDictionaryFromArray_json(actionValue, "slotsInfo", j);
			s_spSlot slot;
			slot.particleName = DICTOOL->getStringValue_json(slotValue, "particleName");
			slot.slotsName = DICTOOL->getStringValue_json(slotValue, "slotName");
			slot.offset = Vec2(DICTOOL->getFloatValue_json(slotValue, "offsetX"), DICTOOL->getFloatValue_json(slotValue, "offsetY"));
			action->slotsData.insert(std::make_pair(slot.particleName,slot));
		}
		ptr->m_actions.push_back(action);
	}
	return ptr;
}

SpineParticle* SpineParticle::create()
{
	SpineParticle * ptr = new SpineParticle();
	if (ptr && ptr->init())
	{
		ptr->autorelease();
		return ptr;
	}
	return nullptr;
}

void SpineParticle::saveToJsonFile(const std::string &path)
{
	char szValue[256][256];
	int charindex = 0;
	if (path.empty())
		return;
	auto doc = new rapidjson::Document();

	rapidjson::Document::AllocatorType& allocator = doc->GetAllocator();
	rapidjson::Value spineParticle(rapidjson::kObjectType);


	//spine文件信息
	rapidjson::Value spineInfo(rapidjson::kObjectType);
	spineInfo.AddMember("jsonFile", m_spineJsonFile.c_str(), allocator);
	spineInfo.AddMember("atlasFile", m_spineAtlasFile.c_str(), allocator);
	spineParticle.AddMember("spineInfo", spineInfo, allocator);
	//粒子文件信息
	rapidjson::Value particleInfo(rapidjson::kArrayType);
	for (auto &it : m_particles)
	{
		sprintf(szValue[charindex], "%s", it.first.c_str());
		particleInfo.PushBack(szValue[charindex++], allocator);
	}
	spineParticle.AddMember("particleInfo", particleInfo, allocator);
	//动作信息
	rapidjson::Value actionInfo(rapidjson::kArrayType);
	for (auto &it : m_actions)
	{
		rapidjson::Value actionNode(rapidjson::kObjectType);
		actionNode.AddMember("actionName", it->actionName.c_str(), allocator);
		actionNode.AddMember("spineActionName", it->spineActionName.c_str(), allocator);
		//挂载信息
		rapidjson::Value slotsArray(rapidjson::kArrayType);
		for (auto slot : it->slotsData)
		{
			rapidjson::Value slotsInfo(rapidjson::kObjectType);
			
			sprintf(szValue[charindex], "%s", slot.second.particleName.c_str());
			//slotsInfo.AddMember("particleName", slot.second.particleName.c_str(), allocator);
			slotsInfo.AddMember("particleName", szValue[charindex++], allocator);
			//slotsInfo.AddMember("slotName", slot.second.slotsName.c_str(), allocator);
			sprintf(szValue[charindex], "%s", slot.second.slotsName.c_str());
			slotsInfo.AddMember("slotName", szValue[charindex++], allocator);
			slotsInfo.AddMember("offsetX", slot.second.offset.x, allocator);
			slotsInfo.AddMember("offsetY", slot.second.offset.y, allocator);
			slotsArray.PushBack(slotsInfo, allocator);
		}
		actionNode.AddMember("slotsInfo", slotsArray, allocator);
		actionInfo.PushBack(actionNode, allocator);
	}
	spineParticle.AddMember("actionsInfo", actionInfo, allocator);

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer>writer(buffer);
	spineParticle.Accept(writer);
	FILE*bakFile = fopen(path.c_str(), "w");
	if (bakFile)
	{
		fputs(buffer.GetString(), bakFile);
		fclose(bakFile);
	}
}

void SpineParticle::setSpine(const std::string &jsonPath, const std::string &atlasPath)
{
	if (m_skeleton)
	{
		unloadAllParticle();
		m_skeleton->removeFromParent();
	}	
	m_spineJsonFile = jsonPath;
	m_spineAtlasFile = atlasPath;
	m_skeleton = spine::SkeletonAnimation::createWithFile(jsonPath, atlasPath);
	this->addChild(m_skeleton);
}
