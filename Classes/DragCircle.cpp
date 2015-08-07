#include "DragCircle.h"
#include "ChangeProperty.h"
#include "UIDocument.h"

static DragCircle *s_pDragCircle = nullptr;


DragCircle::DragCircle()
: m_anchorPoint(nullptr)
, m_hitSuccess(false)
, m_leftDownCircle(nullptr)
, m_leftTopCircle(nullptr)
, m_rightDownCircle(nullptr)
, m_rightTopCircle(nullptr)
, m_keyPressed(false)

{

}

DragCircle::~DragCircle()
{
	s_pDragCircle = nullptr;
}

DragCircle* DragCircle::getInstance()
{
	if (!s_pDragCircle)
	{
		s_pDragCircle = new DragCircle();
		if (s_pDragCircle && s_pDragCircle->init())
		{
			auto scene = Director::getInstance()->getRunningScene();
			if (!scene)
				return false;
			scene->addChild(s_pDragCircle,2);//放在最上层
			return s_pDragCircle;
		}
			
		else
		{
			CC_SAFE_DELETE(s_pDragCircle);
			return nullptr;
		}
	}
	return s_pDragCircle;
}

bool DragCircle::init()
{
	m_anchorPoint = Sprite::create("anchor_circle.png");
	this->addChild(m_anchorPoint);
	m_leftDownCircle = Sprite::create("stock_draw_circle.png");
	this->addChild(m_leftDownCircle);
	m_rightDownCircle = Sprite::create("stock_draw_circle.png");
	this->addChild(m_rightDownCircle);
	m_leftTopCircle = Sprite::create("stock_draw_circle.png");
	this->addChild(m_leftTopCircle);
	m_rightTopCircle = Sprite::create("stock_draw_circle.png");
	this->addChild(m_rightTopCircle);

	this->setVisible(false);

	//init event

	auto eventListener = EventListenerTouchOneByOne::create();
	eventListener->setSwallowTouches(false);//不吞噬事件
	eventListener->onTouchBegan = std::bind(&DragCircle::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
	
	eventListener->onTouchMoved = std::bind(&DragCircle::onTouchMoved, this,std::placeholders::_1, std::placeholders::_2);

	eventListener->onTouchEnded = std::bind(&DragCircle::onTouchEnded, this,std::placeholders::_1, std::placeholders::_2);


	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

	auto keyboardEventListener = EventListenerKeyboard::create();

	keyboardEventListener->onKeyPressed = std::bind(&DragCircle::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keyboardEventListener->onKeyReleased = std::bind(&DragCircle::onkeyReleased, this, std::placeholders::_1, std::placeholders::_2);

	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEventListener, this);

	return true;
}

void DragCircle::resize()
{
	if (!m_selectedWidget)
	{
		this->setVisible(false);
		return;
	}
	this->setVisible(true);

	int nWidth = m_selectedWidget->getContentSize().width * m_selectedWidget->getScaleX();
	if (nWidth < 0)
		nWidth *= -1;
	int nHeight = m_selectedWidget->getContentSize().height * m_selectedWidget->getScaleY();
	if (nHeight < 0)
		nHeight *= -1;
	this->setContentSize(Size(nWidth, nHeight));
	m_leftTopCircle->setPosition(Vec2(0, nHeight));
	m_rightTopCircle->setPosition(Vec2(nWidth, nHeight));
	m_leftDownCircle->setPosition(0, 0);
	m_rightDownCircle->setPosition(nWidth, 0);
	Vec2 anchorPoint = m_selectedWidget->getAnchorPoint();

	m_anchorPoint->setPosition(anchorPoint.x * nWidth, anchorPoint.y*nHeight);

	auto worldPos = m_selectedWidget->getParent()->convertToWorldSpace(m_selectedWidget->getPosition());
	this->setAnchorPoint(anchorPoint);
	this->setPosition(worldPos);
	this->setRotation(m_selectedWidget->getRotation());
}

void DragCircle::setSelectWidget(Widget * widget)
{
	m_selectedWidget = widget;
	if (!widget)
	{
		this->setVisible(false);
		return;
	}	
	resize();
}

void DragCircle::onTouchMoved(Touch* touch, Event* event)
{
	this->setPosition(this->getPosition() + touch->getDelta());
}

void DragCircle::onTouchEnded(Touch* touch, Event* event)
{
	m_hitSuccess = false;
	endMove();
}

bool DragCircle::onTouchBegan(Touch* touch, Event* event)
{
	m_hitSuccess = false;

	Vec2 locationInNode = this->convertToNodeSpace(touch->getLocation());
	Size s = this->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	startMove();
	if (rect.containsPoint(locationInNode))
	{
		log("event began... x = %f, y = %f", locationInNode.x, locationInNode.y);
		m_hitSuccess = true;
		return true;
	}
	return false;
}

void DragCircle::startMove()
{
	m_oldPos = m_selectedWidget->getPosition();
}

void DragCircle::endMove()
{
	if (m_oldPos == m_selectedWidget->getPosition())
		return;
	UIDoucument::getInstance()->excuteCommand(new ChangePosition(m_selectedWidget, m_oldPos, m_selectedWidget->getPosition()));
}

void DragCircle::onKeyPressed(EventKeyboard::KeyCode keycode, Event*)
{
	if (!m_selectedWidget)
		return;

	if (keycode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			startMove();
		}
		m_selectedWidget->setPosition(m_selectedWidget->getPosition() + Vec2(0, 1));
		UIDoucument::getInstance()->selectWidget(m_selectedWidget);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			startMove();
		}
		m_selectedWidget->setPosition(m_selectedWidget->getPosition() + Vec2(0, -1));
		UIDoucument::getInstance()->selectWidget(m_selectedWidget);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			startMove();
		}
		m_selectedWidget->setPosition(m_selectedWidget->getPosition() + Vec2(-1, 0));
		UIDoucument::getInstance()->selectWidget(m_selectedWidget);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			startMove();
		}
		m_selectedWidget->setPosition(m_selectedWidget->getPosition() + Vec2(1, 0));
		UIDoucument::getInstance()->selectWidget(m_selectedWidget);
	}
}

void DragCircle::onkeyReleased(EventKeyboard::KeyCode keycode, Event*)
{
	if (m_keyPressed)
	{
		m_keyPressed = false;
		endMove();
	}
}

void DragCircle::createRectWithWidget(Widget *widget)
{
	auto anchorPoint = Sprite::create("anchor_circle.png");
	this->addChild(m_anchorPoint);
	m_leftDownCircle = Sprite::create("stock_draw_circle.png");
	this->addChild(m_leftDownCircle);
	m_rightDownCircle = Sprite::create("stock_draw_circle.png");
	this->addChild(m_rightDownCircle);
	m_leftTopCircle = Sprite::create("stock_draw_circle.png");
	this->addChild(m_leftTopCircle);
	m_rightTopCircle = Sprite::create("stock_draw_circle.png");
	this->addChild(m_rightTopCircle);
}




