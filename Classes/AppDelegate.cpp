#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "CocosQtPort\CCQGLView.h"
#include "qdir.h"
USING_NS_CC;

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
  //  GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    //GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
	auto director = Director::getInstance();
	auto glview = CCQGLView::getInstance();
	//glview->setBgColor(Color4B(50, 50, 50, 255));

	director->setOpenGLView(glview);
	_mainWindow.setGLView(glview->getGLWidget());
	_mainWindow.initActionListWidget();
	_mainWindow.show();

    // turn on display FPS
    director->setDisplayStats(false);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = Scene::create();
	scene->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	
	scene->setContentSize(Director::getInstance()->getVisibleSize());
	//scene->setPosition(Director::getInstance()->getVisibleSize() / 2);
	QString path;
	QDir dir;
	path = dir.currentPath();
	FileUtils::getInstance()->addSearchPath(path.toStdString() + "/Resources");
	FileUtils::getInstance()->addSearchPath("D://Resources");
	auto keyboardEvent = EventListenerKeyboard::create();

	keyboardEvent->onKeyPressed = [=](EventKeyboard::KeyCode code, Event*event)
	{
		if (code == EventKeyboard::KeyCode::KEY_CTRL)
		{
			m_dragMode = true;
		}
	};

	keyboardEvent->onKeyReleased = [=](EventKeyboard::KeyCode code, Event*event)
	{
		if (code == EventKeyboard::KeyCode::KEY_CTRL)
		{
			m_dragMode = false;
		}
	};

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(keyboardEvent, -1);


	auto scrollEvent = EventListenerMouse::create();
	scrollEvent->onMouseScroll = [=](Event* event)
	{
		EventMouse* e = (EventMouse*)event;
		int scrollX = e->getScrollX();
		int scrollY = e->getScrollY();
		scene->setScale(scene->getScale() + (float)scrollY / 100.0f);
		Size currentSize = scene->getContentSize() * scene->getScale();
	//	scene->setScale();
	};

	auto eventListener = EventListenerTouchOneByOne::create();

	eventListener->onTouchBegan = [=](Touch*touch, Event*event)->bool{
		if (!m_dragMode)
			return false;
		return true;
	};

	eventListener->onTouchMoved = [=](Touch*touch, Event*event)
	{
		scene->setPosition(scene->getPosition() + touch->getDelta());
	};
	eventListener->setSwallowTouches(true);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(eventListener, -3);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(scrollEvent, -2);
    // run
    director->runWithScene(scene);

	CCRect rect(0, 0, scene->getContentSize().width, scene->getContentSize().height);

	CCSprite* grid = CCSprite::create("grid1_16x16_new_aaa.png");

	grid->setTextureRect(rect);

	Texture2D::TexParams params = {

		GL_LINEAR,

		GL_LINEAR,

		GL_REPEAT,

		GL_REPEAT

	};

	grid->getTexture()->setTexParameters(&params);
	grid->setAnchorPoint(Vec2::ZERO);
	grid->setPosition(Vec2::ZERO);

	scene->addChild(grid,0);

	//

	return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
