#ifndef __DATA_STRUCT_H_
#define __DATA_STRUCT_H_
#include "cocos2d.h"
USING_NS_CC;

typedef struct s_slotData
{
	std::string slotsName;//���ص�����
	std::string particleName;//��������
	Vec2 offset;//ƫ��
} s_spSlot;

typedef struct s_SpineParticleAction
{
	std::string actionName; //������
	std::string spineActionName;//��Ӧspine�еĶ�����Ϣ
	std::vector<s_spSlot> slotsData; //������Ϣ
} s_spAction;

typedef struct s_SpineParticle
{
	std::string spineJsonFile;
	std::string spineAtlasFile;
	spine::SkeletonAnimation * skeleton;
	std::map<std::string, ParticleSystem*> particles;
	std::vector<s_spAction*> actions; //����
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
