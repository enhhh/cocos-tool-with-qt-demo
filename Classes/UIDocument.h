#ifndef __DATA_EXCHANGE_H__
#define __DATA_EXCHANGE_H__

#include "cocostudio/CocoStudio.h"
#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "DataStruct.h"
#include <QObject>
#include <QUndoStack>
#include "DragRect.h"

USING_NS_CC;
USING_NS_CC_EXT;
using namespace cocostudio;
using namespace ui;

//���ݳ־û����֣��������������Դ��ࣨ���ĵ�����
class UIDoucument
	: public QObject
{
	Q_OBJECT
public:
	static UIDoucument* getInstance();

	bool openFile(std::string &path);

	//
	Widget* getRootWidget(){ return m_rootWidget; }

	UI_DATA* getUITree(){ return m_rootUI; }

	void updateUIWithFrameid(int frameid);

	void updateCurrentKeyFrame();

	void createJson(rapidjson::Document *doc = nullptr);

	void saveAnimation();

	void excuteCommand(QUndoCommand *command);

	void selectWidget(Node *widget)
	{
		//DragCircle::getInstance()->setSelectWidget(widget);
		//DragRect::getInstance()->addSelectNode(widget);
		emit UISelected(widget);
	}

	

public slots:
	void playCurrentAnimation();

	void undo();

	void redo();

	void resetAnimationFromBackFile();
protected:

	Widget* widgetFromJsonFile(const char *fileName);
	
signals:
	void FileOpened();

	void UISelected(Node *target);
protected:
	UIDoucument();

	~UIDoucument();

	UI_DATA* createDataTree(Node *pWidget);//�����������ݴ������


protected:

	void initEventListener();



protected:

	Widget*			m_rootWidget;
	UI_DATA*		m_rootUI;

	rapidjson::Document * m_jsonDict;
	rapidjson::Document * m_resultJson;
	std::string m_fileName;
	std::string m_strFilePath;

	EventListenerTouchOneByOne*	m_eventListener;//use for call back 

	std::vector<ANIMATION> animationList;//�����б�
	QUndoStack * m_undoStack;
	bool	m_needExecuteMove;
};
#endif // !__DATA_EXCHANGE_H__
