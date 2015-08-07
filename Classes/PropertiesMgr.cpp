#include "PropertiesMgr.h"
#include "ViewMgr.h"
#include "QtTreePropertyBrowser"
#include "DocumentMgr.h"
#include "qpoint.h"

static PropertiesMgr * s_pPropertiesMgr = nullptr;
PropertiesMgr::PropertiesMgr()
	: m_actionNameProperty(nullptr)
	, m_variantManager(nullptr)
	, m_particleSlotsProperty(nullptr)
{

}

PropertiesMgr::~PropertiesMgr()
{

}

PropertiesMgr * PropertiesMgr::getInstance()
{
	if (s_pPropertiesMgr == nullptr)
		s_pPropertiesMgr = new PropertiesMgr();
	return s_pPropertiesMgr;
}

void PropertiesMgr::initPropertyView()
{

	QtTreePropertyBrowser * tb = (QtTreePropertyBrowser*)ViewMgr::getInstance()->getWidgetWithName("propertiesWidget");
	tb->clear();

	if (m_variantManager == nullptr)
		m_variantManager = new QtVariantPropertyManager();
	else
		disconnect(m_variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)), this, SLOT(propertyValueChanged(QtProperty *, const QVariant &)));
	m_variantManager->clear();
	m_particlesSlotProperty.clear();
	m_spineActionNames.clear();
	m_slotsNames.clear();
	QtProperty * actionGroup = m_variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("action properties"));

	m_actionNameProperty = m_variantManager->addProperty(QtVariantPropertyManager::enumTypeId(), "actionName:");
	QStringList spineActionName;
	spineActionName << "";
	m_spineActionNames.push_back("");
	spine::SkeletonAnimation * pSkeleton = DocumentMgr::getInstance()->m_pSpineParticle->m_skeleton;
	for (int i = 0; i < pSkeleton->getSkeleton()->data->animationsCount; ++i)
	{
		spineActionName << pSkeleton->getSkeleton()->data->animations[i]->name;
		m_spineActionNames.push_back(pSkeleton->getSkeleton()->data->animations[i]->name);
	}
	m_actionNameProperty->setAttribute(QLatin1String("enumNames"), spineActionName);
	actionGroup->addSubProperty(m_actionNameProperty);
	int pos = 0;
	if (DocumentMgr::getInstance()->m_currentAction)
	for (auto it : m_spineActionNames)
	{
		if (DocumentMgr::getInstance()->m_currentAction->spineActionName == it)
		{
			m_actionNameProperty->setValue(pos);
			break;
		}
		pos++;
	}
	QtProperty *particleGroup = m_variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("particle properties"));

	QStringList slotsName;
	slotsName << "none";
	m_slotsNames.push_back("none");
	for (int i = 0; i < pSkeleton->getSkeleton()->data->slotsCount; i++)
	{
		slotsName << pSkeleton->getSkeleton()->data->spineSlots[i]->name;
		m_slotsNames.push_back(pSkeleton->getSkeleton()->data->spineSlots[i]->name);
	}

	for (auto & it : DocumentMgr::getInstance()->m_pSpineParticle->m_particles)
	{
		QtProperty *particleinfoGroup = m_variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), it.first.c_str());
		auto particleProperty = m_variantManager->addProperty(QtVariantPropertyManager::enumTypeId(),"slot");

		particleProperty->setAttribute(QLatin1String("enumNames"), slotsName);
		particleinfoGroup->addSubProperty(particleProperty);
		m_particlesSlotProperty.insert(std::make_pair(particleProperty, it.first));
		if (DocumentMgr::getInstance()->m_currentAction)
		{
			auto &slotsData = DocumentMgr::getInstance()->m_currentAction->slotsData;
			auto p = slotsData.find(it.first);
			if (p != slotsData.end())
			{
				int index = 0;
				for (auto &name : m_slotsNames)
				{
					if (name == p->second.slotsName)
					{
						particleProperty->setValue(index);
						break;
					}
						
					index++;
				}
			}
		}

		auto offsetProperty = m_variantManager->addProperty(QVariant::PointF, tr("offset:"));
		offsetProperty->setAttribute(QLatin1String("decimals"), 2);
		if (DocumentMgr::getInstance()->m_currentAction)
		{
			auto &slotsData = DocumentMgr::getInstance()->m_currentAction->slotsData;
			auto p = slotsData.find(it.first);
			if (p != slotsData.end())
			{
				offsetProperty->setValue(QPointF(p->second.offset.x, p->second.offset.y));
			}
		}

		particleinfoGroup->addSubProperty(offsetProperty);
		m_particleOffsetProperty.insert(std::make_pair(offsetProperty, it.first));
		particleGroup->addSubProperty(particleinfoGroup);
	}
	QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();

	tb->setFactoryForManager(m_variantManager, variantFactory);

	tb->addProperty(actionGroup);
	tb->addProperty(particleGroup);

	connect(m_variantManager, SIGNAL(valueChanged(QtProperty *, const QVariant &)), this, SLOT(propertyValueChanged(QtProperty *, const QVariant &)));
}

void PropertiesMgr::propertyValueChanged(QtProperty *from, const QVariant &value)
{
	QtVariantProperty *prop = (QtVariantProperty*)from;
	if (prop == m_actionNameProperty)
	{
		DocumentMgr::getInstance()->m_currentAction->spineActionName = m_spineActionNames[value.toInt()];
		return;
	}

	auto it = m_particlesSlotProperty.find(prop);
	if (it != m_particlesSlotProperty.end())
	{
		auto i = DocumentMgr::getInstance()->m_currentAction->slotsData.find(it->second);
		if (i != DocumentMgr::getInstance()->m_currentAction->slotsData.end())
		{

			if (m_slotsNames[value.toInt()] != "none")
			{
				i->second.slotsName = m_slotsNames[value.toInt()];
			}
			else
				DocumentMgr::getInstance()->m_currentAction->slotsData.erase(i);
			return;
		}
		else
		{
			if (m_slotsNames[value.toInt()] == "none")
				return;
			s_spSlot slotData;
			slotData.particleName = it->second;
			slotData.slotsName = m_slotsNames[value.toInt()];
			
			DocumentMgr::getInstance()->m_currentAction->slotsData.insert(std::make_pair(it->second, slotData));
			return;
		}
	}
	auto it2 = m_particleOffsetProperty.find(prop);
	if (it2 != m_particleOffsetProperty.end())
	{
		auto i = DocumentMgr::getInstance()->m_currentAction->slotsData.find(it2->second);
		if (i != DocumentMgr::getInstance()->m_currentAction->slotsData.end())
		{
			QPointF p = value.value<QPointF>();
			i->second.offset = Vec2(p.x(), p.y());
			return;
		}
	}
}
