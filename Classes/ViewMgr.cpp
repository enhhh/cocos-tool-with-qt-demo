#include "ViewMgr.h"
#include "qlistwidget.h"
#include "DocumentMgr.h"
#include "qmenu.h"
#include "qaction.h"
#include "qttreepropertybrowser.h"

static ViewMgr * s_pViewMgr = nullptr;

ViewMgr * ViewMgr::getInstance()
{
	if (s_pViewMgr == nullptr)
	{
		s_pViewMgr = new ViewMgr();
	}
	return s_pViewMgr;
}

void ViewMgr::registerWidget(std::string name, QWidget *widget)
{
	auto it = m_widgets.find(name);
	if (it == m_widgets.end())
	{
		m_widgets.insert(std::make_pair(name, widget));
	}
}

QWidget * ViewMgr::getWidgetWithName(const std::string &name)
{
	auto it = m_widgets.find(name);
	if (it == m_widgets.end())
		return nullptr;
	return it->second;
}

void ViewMgr::updateSpineActionsListview()
{
	auto it = m_widgets.find("spineActonWidget");
	if (it == m_widgets.end())
		return;
	QListWidget *lw = (QListWidget*)it->second;
	lw->clear();
	spine::SkeletonAnimation * pSkeleton = DocumentMgr::getInstance()->m_pSpineParticle->m_skeleton;
	for (int i = 0; i < pSkeleton->getSkeleton()->data->animationsCount; ++i)
	{
		lw->addItem(new QListWidgetItem(pSkeleton->getSkeleton()->data->animations[i]->name, lw));
	}
	connect(lw, SIGNAL(itemPressed(QListWidgetItem*)), this, SLOT(onSpineActionsClick(QListWidgetItem*)));
}

void ViewMgr::onSpineActionsClick(QListWidgetItem * item)
{
	auto name = item->text();
	DocumentMgr::getInstance()->m_pSpineParticle->m_skeleton->setAnimation(0, name.toStdString(), false);
}

void ViewMgr::updatePropertiesView()
{
	auto it = m_widgets.find("propertiesWidget");
	if (it == m_widgets.end())
		return;
	QtTreePropertyBrowser * tb = (QtTreePropertyBrowser*)it->second;
	tb->clear();
	if (m_variantManager == nullptr)
		m_variantManager = new QtVariantPropertyManager();
	m_variantManager->clear();
	QtProperty * bonesGroup = m_variantManager->addProperty(QtVariantPropertyManager::groupTypeId(), tr("all bones"));

	spine::SkeletonAnimation * pSkeleton = DocumentMgr::getInstance()->m_pSpineParticle->m_skeleton;
	if (!pSkeleton)
		return;
	for (int i = 0; i < pSkeleton->getSkeleton()->data->slotsCount; i++)
	{
		pSkeleton->getSkeleton()->data->spineSlots[i]->name;
	}
	
}

void ViewMgr::updateParticleView()
{
	auto it = m_widgets.find("particleWidget");
	if (it == m_widgets.end())
		return;
	QListWidget *lw = (QListWidget*)it->second;
	lw->clear();
	for (auto &it : DocumentMgr::getInstance()->m_pSpineParticle->m_particles)
	{
		lw->addItem(new QListWidgetItem(it.first.c_str(),lw));
	}
}

ViewMgr::ViewMgr()
: m_variantManager(nullptr)
{

}



