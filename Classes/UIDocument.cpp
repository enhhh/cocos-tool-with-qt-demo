#include "UIDocument.h"
#include "cocos2d.h"
#include "AnimationDoc.h"
#include <utility>
#include "json/stringbuffer.h"
#include "json/writer.h"
#include "json/prettywriter.h"
#include "ChangeProperty.h"
#include "AddRemoveFrame.h"

USING_NS_CC;

using namespace cocostudio;

static UIDoucument * s_DataExchange = nullptr;
UIDoucument::UIDoucument()
: m_rootWidget(nullptr)
, m_rootUI(nullptr)
, m_jsonDict(nullptr)
, m_resultJson(nullptr)
{
	m_undoStack = new QUndoStack();
}

UIDoucument::~UIDoucument()
{
}

UIDoucument* UIDoucument::getInstance()
{
	if (!s_DataExchange)
	{
		s_DataExchange = new (std::nothrow) UIDoucument();
		s_DataExchange->initEventListener();
	}

	return s_DataExchange;
}

bool UIDoucument::openFile(std::string &path)
{
	DragRect::getInstance()->setMovingCallBack([](Node*target, Vec2 offset){
		UIDoucument::getInstance()->UISelected(target);
		UIDoucument::getInstance()->updateCurrentKeyFrame();
	});

	DragRect::getInstance()->setMovingEndCall([=](Node*target, Vec2 begin, Vec2 end){
		UIDoucument::getInstance()->excuteCommand(new ChangePosition(target, begin, end));
	});

	DragRect::getInstance()->removeAllSelctedNodes();

	ActionManagerEx::getInstance()->releaseActions();
	cocos2d::ui::Widget* root = widgetFromJsonFile(path.c_str());
	if (root)
	{
		if (m_rootWidget)
			m_rootWidget->removeFromParentAndCleanup(true);

		m_rootWidget = root;
		//DragCircle::getInstance();
		Director::getInstance()->getRunningScene()->addChild(root,1);

		if (m_rootUI)
			m_rootUI->destroy();
		m_rootUI = new UI_DATA();
		auto ptr = createDataTree(root);
		ptr->parent = m_rootUI;

		m_rootUI->children.push_back(ptr);
		AnimationDoc::getInstance()->parseAnimationData(m_strFilePath, m_fileName);
		emit FileOpened();
		m_undoStack->clear();
		return true;
	}

	return false;
}

UI_DATA* UIDoucument::createDataTree(Node *pWidget)
{
	if (!pWidget)
		return nullptr;
	UI_DATA * ptr = new UI_DATA();
	ptr->pNode = pWidget;
	ptr->pNode->getEventDispatcher()->removeEventListenersForTarget(ptr->pNode);
	ptr->pNode->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_eventListener->clone(),ptr->pNode);
	for (auto &it : pWidget->getChildren())
	{
		UI_DATA *child = createDataTree(it);
		if (child)
		{
			child->parent = ptr;
			ptr->children.push_back(child);
		}
			
	}
	return ptr;
}

void UIDoucument::initEventListener()
{
	//init dragrect event



	//keyboard event

	m_eventListener = EventListenerTouchOneByOne::create();
	m_eventListener->setSwallowTouches(true);
	m_eventListener->onTouchBegan = [=](Touch* touch, Event* event){
		auto target = static_cast<Widget*>(event->getCurrentTarget());
		if (!target->isVisible())
			return false;
		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);

		if (rect.containsPoint(locationInNode))
		{
			log("event began... x = %f, y = %f", locationInNode.x, locationInNode.y);
			selectWidget(target);
			DragRect::getInstance()->addSelectNode(target);
			return true;
		}
		return false;
	};

	m_eventListener->onTouchMoved = [=](Touch* touch, Event* event){
		return;
		auto target = static_cast<Widget*>(event->getCurrentTarget());
		target->setPosition(target->getPosition() + touch->getDelta());

		updateCurrentKeyFrame();
		selectWidget(target);
	};

	m_eventListener->onTouchEnded = [=](Touch* touch, Event* event){
		auto target = static_cast<Widget*>(event->getCurrentTarget());
//		updateCurrentKeyFrame();
		selectWidget(target);
	};

	m_eventListener->retain();

}

Widget* UIDoucument::widgetFromJsonFile(const char *fileName)
{
	std::string jsonpath;
	jsonpath = fileName;
	if (m_jsonDict)
		CC_SAFE_DELETE(m_jsonDict);
	m_jsonDict = new rapidjson::Document();
	//    jsonpath = CCFileUtils::getInstance()->fullPathForFilename(fileName);
	size_t pos = jsonpath.find_last_of('/');
	size_t lastpointpos = jsonpath.find_last_of('.');
	m_strFilePath = jsonpath.substr(0, pos + 1);
	m_fileName = jsonpath.substr(pos + 1, lastpointpos - pos - 1);
	GUIReader::getInstance()->setFilePath(m_strFilePath);
	std::string contentStr = FileUtils::getInstance()->getStringFromFile(jsonpath);
	m_jsonDict->Parse<0>(contentStr.c_str());
	if (m_jsonDict->HasParseError())
	{
		CCLOG("GetParseError %s\n", m_jsonDict->GetParseError());
	}
	Widget* widget = nullptr;
	const char* fileVersion = DICTOOL->getStringValue_json(*m_jsonDict, "version");
	WidgetPropertiesReader * pReader = nullptr;
	if (fileVersion)
	{
		int versionInteger = GUIReader::getInstance()->getVersionInteger(fileVersion);
		if (versionInteger < 250)
		{
			pReader = new WidgetPropertiesReader0250();
			widget = pReader->createWidget(*m_jsonDict, m_strFilePath.c_str(), fileName);
		}
		else
		{
			pReader = new WidgetPropertiesReader0300();
			widget = pReader->createWidget(*m_jsonDict, m_strFilePath.c_str(), fileName);
		}
	}
	else
	{
		pReader = new WidgetPropertiesReader0250();
		widget = pReader->createWidget(*m_jsonDict, m_strFilePath.c_str(), fileName);
	}

	
	CC_SAFE_DELETE(pReader);
	return widget;
}


void UIDoucument::updateUIWithFrameid(int frameid)
{
	ANIMATION * ani = AnimationDoc::getInstance()->getCurrentAnimation();
	if (!ani)
		return;
	for (auto it : ani->actionnodelist)
	{
		ACTION_FRAME *frame = nullptr;
		for (int i = frameid; i >= 0; i--)
		{
			frame = it->actionframelist[i];
			if (frame)
				break;
		}
		if (!frame)
			break;
		Widget *target = it->target;
		if (!target)
			continue;
		AnimationDoc::getInstance()->setValueFromFrameToWidget(frame, target);
		emit UISelected(target);
	}
	
}

void UIDoucument::updateCurrentKeyFrame()
{
	int keyframe = AnimationDoc::getInstance()->getCurrentKeyFrame();
	ANIMATION * ani = AnimationDoc::getInstance()->getCurrentAnimation();
	if (!ani)
		return;
	int curRow = 0;
	for (auto it : ani->actionnodelist)
	{
		for (auto selctedWidget : DragRect::getInstance()->getAllSelectedNodes())
		{
			if (it->target == selctedWidget)
			{
				ACTION_FRAME *frame = it->actionframelist[keyframe];
				if (!frame)
				{
					excuteCommand(new AddFrame(curRow, keyframe));
				}
				
				AnimationDoc::getInstance()->setValueFromWidgetToFrame((Widget*)selctedWidget, frame);
			}
			
		}
		curRow++;
	}
}

void UIDoucument::createJson(rapidjson::Document *doc)
{
	if (!doc)
	{
		if (m_resultJson)
			CC_SAFE_DELETE(m_resultJson);
		m_resultJson = new rapidjson::Document();
		m_resultJson->SetObject();
		doc = m_resultJson;
	}

	
	rapidjson::Document::AllocatorType& allocator = doc->GetAllocator();
	rapidjson::Value animation(rapidjson::kObjectType);

	animation.AddMember("classname", rapidjson::kNullType, allocator);
	animation.AddMember("name", "AnimationManager", allocator);

	rapidjson::Value actionlist(rapidjson::kArrayType);
	std::vector<ANIMATION*> aniList = AnimationDoc::getInstance()->getAnimationList();
	for (auto ani : aniList)
	{
		rapidjson::Value action(rapidjson::kObjectType);
		action.AddMember("classname", rapidjson::kNullType, allocator);
		action.AddMember("name", ani->name.c_str(), allocator);
		rapidjson::Value actionnodelist(rapidjson::kArrayType);
		for (auto node : ani->actionnodelist)
		{
			rapidjson::Value actionnode(rapidjson::kObjectType);
			actionnode.AddMember("classname", rapidjson::kNullType, allocator);
			actionnode.AddMember("name", node->name.c_str(), allocator);
			actionnode.AddMember("ActionTag", node->ActionTag, allocator);
			rapidjson::Value actionframelist(rapidjson::kArrayType);
			for (auto it : node->actionframelist)
			{
				auto frame = it.second;
				if (!frame)
					continue;
				rapidjson::Value actionframe(rapidjson::kObjectType);
				actionframe.AddMember("classname", rapidjson::kNullType, allocator);
				actionframe.AddMember("name", rapidjson::kNullType, allocator);
				actionframe.AddMember("colorb", frame->color.b, allocator);
				actionframe.AddMember("colorg", frame->color.g, allocator);
				actionframe.AddMember("colorr", frame->color.r, allocator);
				actionframe.AddMember("frameid", frame->frameid, allocator);
				actionframe.AddMember("opacity", frame->opacity, allocator);
				actionframe.AddMember("positionx", frame->position.x, allocator);
				actionframe.AddMember("positiony", frame->position.y, allocator);
				actionframe.AddMember("rotation",frame->rotation, allocator);
				actionframe.AddMember("scalex", frame->scale.x, allocator);
				actionframe.AddMember("scaley", frame->scale.y, allocator);
				actionframe.AddMember("starttime", frame->starttime, allocator);
				rapidjson::Value tweenParameter(rapidjson::kArrayType);//‘§¡Ù°£‘›Œ¥”√
				for (int i = 0; i < frame->tweenParameter.size(); i++)
				{
					tweenParameter.PushBack(frame->tweenParameter[i], allocator);
				}
				actionframe.AddMember("tweenParameter", tweenParameter, allocator);
				actionframe.AddMember("tweenType", 0, allocator);
				actionframe.AddMember("visible", frame->visible, allocator);
				actionframelist.PushBack(actionframe, allocator);
			}
			actionnode.AddMember("actionframelist",actionframelist, allocator);
			actionnodelist.PushBack(actionnode, allocator);
		}
		action.AddMember("actionnodelist", actionnodelist, allocator);
		action.AddMember("loop", ani->loop, allocator);
		action.AddMember("unittime", ani->unittime, allocator);
		actionlist.PushBack(action, allocator);
	}
	animation.AddMember("actionlist", actionlist, allocator);
	

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer>writer(buffer);
	animation.Accept(writer);
	std::string bakFilePath = m_strFilePath + m_fileName + ".AniBak";
	FILE*bakFile = fopen(bakFilePath.c_str(), "wb");
	if (bakFile)
	{
		fputs(buffer.GetString(), bakFile);
		fclose(bakFile);
	}
	doc->AddMember("animation", animation, allocator);
}

void UIDoucument::saveAnimation()
{
	if (!m_jsonDict)
		return;
	m_jsonDict->RemoveMember("animation");
	createJson(m_jsonDict);

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer>writer(buffer);
	m_jsonDict->Accept(writer);
	std::string filePath = m_strFilePath + m_fileName + ".ExportJson";
	FILE*file = fopen(filePath.c_str(), "wb");
	if (file)
	{
		fputs(buffer.GetString(), file);
		fclose(file);
	}

}

void UIDoucument::playCurrentAnimation()
{
	createJson();
	ANIMATION * ani = AnimationDoc::getInstance()->getCurrentAnimation();
	ActionManagerEx::getInstance()->releaseActions();
	const rapidjson::Value &animation = DICTOOL->getSubDictionary_json(*m_resultJson, "animation");
	ActionManagerEx::getInstance()->initWithDictionary(m_fileName.c_str(), animation, m_rootWidget);
	//ActionManagerEx::getInstance()->playActionByName(ani->name.c_str());
	std::string filename = m_fileName + ".json";
	ActionObject* action1 = ActionManagerEx::getInstance()->getActionByName(m_fileName.c_str(), ani->name.c_str());
	if (action1)
		action1->play(CallFunc::create([](){log("test!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"); }));
}

void UIDoucument::excuteCommand(QUndoCommand *command)
{
	m_undoStack->push(command);
}

void UIDoucument::undo()
{
	m_undoStack->undo();
}

void UIDoucument::redo()
{
	m_undoStack->redo();
}

void UIDoucument::resetAnimationFromBackFile()
{
	AnimationDoc::getInstance()->clearAllData();

	AnimationDoc::getInstance()->parseAnimationData(m_strFilePath, m_fileName,true);
}


