#include "DragRect.h"
#define DRAG_RECT_ITEM_NAME "dragRect"


DragRectItem::DragRectItem()
: m_target(nullptr)
, m_rightTopCircle(nullptr)
, m_rightDownCircle(nullptr)
, m_leftTopCircle(nullptr)
, m_leftDownCircle(nullptr)
, m_anchorPoint(nullptr)
, m_isMoving(false)
{

}

DragRectItem * DragRectItem::create(Node * node)
{
	DragRectItem * ptr = new DragRectItem();
	if (!ptr)
		return nullptr;
	ptr->m_target = node;
	if (ptr->init())
	{
		node->addChild(ptr);
		ptr->setName(DRAG_RECT_ITEM_NAME);
		return ptr;
	}
	CC_SAFE_DELETE(ptr);
	return nullptr;
}


bool DragRectItem::init()
{
	if (!Node::init())
		return false;

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

	//init circle event
	reSize();
	auto rectEvent = EventListenerTouchOneByOne::create();

	rectEvent->setSwallowTouches(true);//吞噬事件

	//touch began
	rectEvent->onTouchBegan = std::bind(&DragRectItem::onRectTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
  
	rectEvent->onTouchMoved = std::bind(&DragRectItem::onRectTouchMove, this, std::placeholders::_1, std::placeholders::_2);

	rectEvent->onTouchEnded = std::bind(&DragRectItem::onRectTouchEnd, this, std::placeholders::_1, std::placeholders::_2);

	this->getEventDispatcher()->addEventListenerWithFixedPriority(rectEvent, -1);



	return true;
}

void DragRectItem::reSize()
{
	int nWidth = m_target->getContentSize().width;
	if (nWidth < 0)
		nWidth *= -1;
	int nHeight = m_target->getContentSize().height;
	if (nHeight < 0)
		nHeight *= -1;
	this->setContentSize(Size(nWidth, nHeight));
	m_leftTopCircle->setPosition(Vec2(0, nHeight));
	m_rightTopCircle->setPosition(Vec2(nWidth, nHeight));
	m_leftDownCircle->setPosition(0, 0);
	m_rightDownCircle->setPosition(nWidth, 0);
	Vec2 anchorPoint = m_target->getAnchorPoint();

	m_anchorPoint->setPosition(anchorPoint.x * nWidth, anchorPoint.y*nHeight);

	auto worldPos = m_target->getParent()->convertToWorldSpace(m_target->getPosition());
	this->setAnchorPoint(Vec2::ZERO);
	this->setPosition(Vec2::ZERO);
	//this->setRotation(m_target->getRotation());

	m_leftDownCircle->setScaleX(1.0f / m_target->getScaleX());
	m_leftDownCircle->setScaleY(1.0f / m_target->getScaleY());

	m_leftTopCircle->setScaleX(1.0f / m_target->getScaleX());
	m_leftTopCircle->setScaleY(1.0f / m_target->getScaleY());

	m_anchorPoint->setScaleX(1.0f / m_target->getScaleX());
	m_anchorPoint->setScaleY(1.0f / m_target->getScaleY());

	m_rightDownCircle->setScaleX(1.0f / m_target->getScaleX());
	m_rightDownCircle->setScaleY(1.0f / m_target->getScaleY());

	m_rightTopCircle->setScaleX(1.0f / m_target->getScaleX());
	m_rightTopCircle->setScaleY(1.0f / m_target->getScaleY());

}

bool DragRectItem::onRectTouchBegan(Touch *touch, Event * event)
{
	if (!this->isVisible())
		return false;
	m_originPos = touch->getLocation();
	Vec2 locationInNode = this->convertToNodeSpace(m_originPos);
	Size s = this->getContentSize();
	Rect rect = Rect(0, 0, s.width, s.height);
	if (rect.containsPoint(locationInNode))
	{
		log("event began... x = %f, y = %f", locationInNode.x, locationInNode.y);
		return true;
	}
	return false;
}

void DragRectItem::onRectTouchMove(Touch *touch, Event * event)
{
	m_isMoving = true;
	Vec2 offset = touch->getLocation() - m_originPos;
	DragRect::getInstance()->onMoving(offset);
}

void DragRectItem::onRectTouchEnd(Touch *touch, Event * event)
{
	if (m_isMoving)
	{
		Vec2 offset = touch->getLocation() - m_originPos;
		DragRect::getInstance()->onMoveEnd(offset);
		m_isMoving = false; 
	}
}



//====================================================================================
//
//
//
//
//DragRect
//
//
//
//====================================================================================
static DragRect * s_pDragRect = nullptr;

DragRect::DragRect()
: m_movingEndCall(nullptr)
, m_movingFun(nullptr)
, m_isMoving(false)
, m_multiSelectMode(false)
, m_keyPressed(false)
{

}

DragRect::~DragRect()
{
	
}


bool DragRect::init()
{
	//init event


	auto keyboardEventListener = EventListenerKeyboard::create();
	keyboardEventListener->onKeyPressed = std::bind(&DragRect::onKeyPressed, this, std::placeholders::_1, std::placeholders::_2);
	keyboardEventListener->onKeyReleased = std::bind(&DragRect::onkeyReleased, this, std::placeholders::_1, std::placeholders::_2);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardEventListener, Director::getInstance()->getRunningScene());


	return true;
}


DragRect * DragRect::getInstance()
{
	if (!s_pDragRect)
	{
		s_pDragRect = new DragRect();
		s_pDragRect->init();
	}
	return s_pDragRect;
}

void DragRect::addSelectNode(Node * node)
{
	if (!m_multiSelectMode)
		removeAllSelctedNodes();

	if (!node)
		return;
	auto rect = node->getChildByName(DRAG_RECT_ITEM_NAME);
	if (rect)
		rect->setVisible(true);
	else
	{
		DragRectItem::create(node);
	}
	m_allSectedNodes.push_back(node);
}

void DragRect::removeAllSelctedNodes()
{
	for (auto it : m_allSectedNodes)
	{
		auto rect = it->getChildByName(DRAG_RECT_ITEM_NAME);
		if (rect)
			rect->setVisible(false);
	}
	m_allSectedNodes.clear();

}


void DragRect::onMoving(Vec2 offset)
{
	//开始移动时记录所有的原始点用于undo
	if (!m_isMoving)
	{
		m_isMoving = true;
		m_allNodesOriginLocalPos.clear();
		for (Node* it : m_allSectedNodes)
		{
			m_allNodesOriginLocalPos[it] = it->getPosition();
		}
	}

	for (auto it : m_allSectedNodes)
	{
		auto pos = m_allNodesOriginLocalPos[it] + offset;
		it->setPosition(pos);
		if (m_movingFun)
			m_movingFun(it, pos);
	}
}

void DragRect::onMoveEnd(Vec2 offset)
{
	m_isMoving = false;
	for (auto it : m_allSectedNodes)
	{
		auto localEndPos = m_allNodesOriginLocalPos[it] + offset;
		auto localBeginPos = m_allNodesOriginLocalPos[it];
		it->setPosition(localEndPos);
		if (m_movingEndCall)
			m_movingEndCall(it, localBeginPos, localEndPos);
	}
}

Node* DragRect::getLastSelctedNode()
{
	if (m_allSectedNodes.empty())
		return nullptr;
	return m_allSectedNodes[m_allSectedNodes.size() - 1];
}


void DragRect::onKeyPressed(EventKeyboard::KeyCode keycode, Event*)
{
	//方向键处理
	if (keycode == EventKeyboard::KeyCode::KEY_UP_ARROW)
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			m_keyPressOffset = Vec2::ZERO;
		}
		m_keyPressOffset += Vec2(0, 1);
		DragRect::getInstance()->onMoving(m_keyPressOffset);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			m_keyPressOffset = Vec2::ZERO;
		}
		m_keyPressOffset += Vec2(0, -1);
		DragRect::getInstance()->onMoving(m_keyPressOffset);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW)
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			m_keyPressOffset = Vec2::ZERO;
		}
		m_keyPressOffset += Vec2(-1, 0);
		DragRect::getInstance()->onMoving(m_keyPressOffset);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW)
	{
		if (!m_keyPressed)
		{
			m_keyPressed = true;
			m_keyPressOffset = Vec2::ZERO;
		}
		m_keyPressOffset += Vec2(1, 0);
		DragRect::getInstance()->onMoving(m_keyPressOffset);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_CTRL)
	{
		DragRect::getInstance()->enterMultiSelcteMode(true);
	}
}

void DragRect::onkeyReleased(EventKeyboard::KeyCode keycode, Event*)
{
	if (keycode == EventKeyboard::KeyCode::KEY_CTRL)
	{
		DragRect::getInstance()->enterMultiSelcteMode(false);
	}
	else if (keycode == EventKeyboard::KeyCode::KEY_LEFT_ARROW
		|| keycode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW
		|| keycode == EventKeyboard::KeyCode::KEY_UP_ARROW
		|| keycode == EventKeyboard::KeyCode::KEY_DOWN_ARROW)
	{
		if (m_keyPressed)
		{
			m_keyPressed = false;
			DragRect::getInstance()->onMoveEnd(m_keyPressOffset);
		}
	}

}