#ifndef __DocumentMgr__H
#define __DocumentMgr__H

#include <spine/spine.h>
#include "cocos2d.h"
#include <spine/SkeletonRenderer.h>
#include <spine/SkeletonAnimation.h>
#include "2d/CCParticleSystem.h"
#include "qobject.h"
#include "SpineParticle.h"

USING_NS_CC;

class DocumentMgr
	: public QObject
{
	Q_OBJECT

public:
	static DocumentMgr* getInstance();

	int loadSpineAnimation(const std::string & path);

	int createProjectFile(const std::string &path);

	int loadProject(const std::string &path);

	int loadParticle(const std::string &path);

	void closeProject();

	void saveProject();

	void createProject(const std::string &path);



//=======

	s_spAction* createAction(const std::string &actionName);

	void addAction();

	void setCurrentAction(const std::string & actionName);

	void removeAction(const std::string &actionName);

signals:
	void updateActionView();

protected:
	DocumentMgr();

public:

	SpineParticle * m_pSpineParticle;
	std::string m_savePath;
	s_spAction* m_currentAction;
};
#endif // !__DocumentMgr__H
