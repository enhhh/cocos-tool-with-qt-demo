#ifndef __DRAG_CIRCLE_H__
#define __DRAG_CIRCLE_H__
#include "preInclude.h"


class DragCircle
	: public Node
{
public:
	static DragCircle* getInstance();

	void resize();

	void setSelectWidget(Widget * widget);

	Widget * getSelctedWidget(){ return m_selectedWidget; }

	bool isHitSuccess(){ return m_hitSuccess; }

	bool onTouchBegan(Touch* touch, Event* event);

	void onTouchMoved(Touch* touch, Event* event);

	void onTouchEnded(Touch* touch, Event* event);

	void onKeyPressed(EventKeyboard::KeyCode keycode, Event*);

	void onkeyReleased(EventKeyboard::KeyCode keycode, Event*);

	void startMove();

	void endMove();
protected:
	DragCircle();

	~DragCircle();

	virtual bool init();

	void createRectWithWidget(Widget *widget);

protected:
	Sprite * m_anchorPoint;//ÃªµãÍ¼±ê
	Sprite* m_leftTopCircle;
	Sprite* m_rightTopCircle;
	Sprite * m_leftDownCircle;
	Sprite* m_rightDownCircle;

	Widget *	m_selectedWidget;
	bool		m_hitSuccess;
	bool		m_keyPressed;

	Vec2	m_oldPos;
	std::vector<Widget*>	m_selectedWidgets;
};
#endif // !__DRAG_CIRCLE_H__
