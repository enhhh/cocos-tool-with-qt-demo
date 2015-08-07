#include "DocumentMgr.h"
#include "cocos2d.h"
#include "DragRect.h"
#include "ViewMgr.h"
#include "PropertiesMgr.h"
#include "json\stringbuffer.h"
#include "json\prettywriter.h"
#include <io.h>
#include "qfiledialog.h"
USING_NS_CC;

static DocumentMgr * s_pDocumentMgr = nullptr;


DocumentMgr::DocumentMgr()
:m_currentAction(nullptr)
, m_pSpineParticle(nullptr)
{

}


DocumentMgr * DocumentMgr::getInstance()
{
	if (s_pDocumentMgr == nullptr)
		s_pDocumentMgr = new DocumentMgr();
	return s_pDocumentMgr;
}

int DocumentMgr::loadSpineAnimation(const std::string & path)
{ 
	if (!m_pSpineParticle)
		createProject(path);
	else
		m_pSpineParticle->cleanup();

	size_t pos = path.find_first_of(".");
	std::string te = path.substr(0, pos);
	std::string tf = te + ".atlas";
	if (access(tf.c_str(), 0) == -1)
	{
		QString fileName = QFileDialog::getOpenFileName(nullptr, tr("Open spine atlas file"), "", tr("spine atlas Files(*.atlas)"));
		tf = fileName.toStdString();
		if (tf.empty())
			return -1;
	}
	size_t jnp = path.find("Resources");
	std::string spineJsonFileName = path.substr(jnp + 10, path.length());
	jnp = tf.find("Resources");
	std::string spineAtlasFileName = tf.substr(jnp + 10, tf.length());
	m_pSpineParticle->setSpine(spineJsonFileName, spineAtlasFileName);
	ViewMgr::getInstance()->updateSpineActionsListview();
	PropertiesMgr::getInstance()->initPropertyView();
	return 0;
}

int DocumentMgr::loadProject(const std::string &path)
{
	//解析工程数据
	if (m_pSpineParticle != nullptr)
		closeProject();
	m_savePath = path;
	m_pSpineParticle = SpineParticle::create(path);
	auto scene = Director::getInstance()->getRunningScene();
	auto winSize = Director::getInstance()->getWinSize();
	Vec2 modelPos = Vec2(winSize.width / 2, winSize.height / 2);
	m_pSpineParticle->setPosition(modelPos);
	Director::getInstance()->getRunningScene()->addChild(m_pSpineParticle);
	DragRect::getInstance()->addSelectNode(m_pSpineParticle);
	if (m_pSpineParticle->m_actions.empty())
		m_currentAction = nullptr;
	else
	{
		m_currentAction = m_pSpineParticle->m_actions[0];
	}
	ViewMgr::getInstance()->updateSpineActionsListview();
	PropertiesMgr::getInstance()->initPropertyView();
	ViewMgr::getInstance()->updateParticleView();
	emit updateActionView();
	return 0;
}

void DocumentMgr::createProject(const std::string &path)
{
	closeProject();
	m_currentAction = nullptr;
	m_savePath = path;
	m_pSpineParticle = SpineParticle::create();
	auto scene = Director::getInstance()->getRunningScene();
	auto winSize = Director::getInstance()->getWinSize();
	Vec2 modelPos = Vec2(winSize.width / 2, winSize.height / 2);
	m_pSpineParticle->setPosition(modelPos);
	Director::getInstance()->getRunningScene()->addChild(m_pSpineParticle);
	DragRect::getInstance()->addSelectNode(m_pSpineParticle);
}

void DocumentMgr::closeProject()
{
	if (m_pSpineParticle)
	{
		DragRect::getInstance()->removeAllSelctedNodes();
		m_pSpineParticle->cleanup();
		m_pSpineParticle->removeFromParent();
	}
}

int DocumentMgr::loadParticle(const std::string &path)
{
	if (!m_pSpineParticle)
		return -1;
	size_t pos1 = path.find("Resources");
	std::string name = path.substr(pos1 + 10, path.length());
	auto it = m_pSpineParticle->m_particles.find(name);
	if (it != m_pSpineParticle->m_particles.end())
		return -1;
	auto ps = ParticleSystem::create(path);
	ps->retain();
	m_pSpineParticle->m_particles.insert(std::make_pair(name, ps));
	ViewMgr::getInstance()->updateParticleView();

	return 0;
}

s_spAction* DocumentMgr::createAction(const std::string &actionName)
{
	if (!m_pSpineParticle)
		return nullptr;
	for (auto &it : m_pSpineParticle->m_actions)
	{
		if (it->actionName == actionName)
			return nullptr;
	}
	s_spAction *action = new s_spAction();
	action->actionName = actionName;
	action->spineActionName = "";
	m_pSpineParticle->m_actions.push_back(action);
	m_currentAction = action;
	PropertiesMgr::getInstance()->initPropertyView();
	emit updateActionView();
	return action;
}

void DocumentMgr::setCurrentAction(const std::string & actionName)
{
	if (m_pSpineParticle == nullptr)
		return;
	for (auto &it : m_pSpineParticle->m_actions)
	{
		if (it->actionName == actionName)
		{
			m_currentAction = it;
			m_pSpineParticle->playAnimation(1, it->actionName, false);
			PropertiesMgr::getInstance()->initPropertyView();
			return;
		}
	}
}

void DocumentMgr::addAction()
{
	if (m_pSpineParticle == nullptr)
		return;
	int i = 0;
	char name[128] = { 0 };
	do 
	{
		sprintf(name, "newAction_%d", i);
		if (createAction(name))
			return;
		i++;

	} while (true);
}

void DocumentMgr::saveProject()
{
	if (!m_pSpineParticle)
		return;
	m_pSpineParticle->saveToJsonFile(m_savePath);
}

void DocumentMgr::removeAction(const std::string &actionName)
{
	if (!m_pSpineParticle)
		return;
	for (auto it = m_pSpineParticle->m_actions.begin(); it != m_pSpineParticle->m_actions.end(); it++)
	{
		if ((*it)->actionName == actionName)
		{
			m_pSpineParticle->m_actions.erase(it);
			if (!m_pSpineParticle->m_actions.empty())
				m_currentAction = *m_pSpineParticle->m_actions.begin();
			else
				m_currentAction = nullptr;
			break;
		}
	}
	PropertiesMgr::getInstance()->initPropertyView();
	emit updateActionView();
}
