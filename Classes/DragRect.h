#ifndef __DRAG_RECT_H__
#define __DRAG_RECT_H__
#include "preInclude.h"


class DragRect
	: public Ref
{
public:
	static DragRect * getInstance();

	//添加选中节点
	void addSelectNode(Node * node);

	//取消选中所有节点
	void removeAllSelctedNodes();

	//取消选中节点
	void removeSelectedNode(Node *node);

	//设置移动回调
	void setMovingCallBack(std::function<void(Node*, Vec2)> fun){ m_movingFun = fun; }

	//设置移动结束回调
	//target,localBeginPos,localEndPos
	void setMovingEndCall(std::function<void(Node*, Vec2, Vec2)> fun){ m_movingEndCall = fun; }

	//获取所有选中节点
	const std::vector<Node*> & getAllSelectedNodes(){ return m_allSectedNodes; }

	//获取最后一个选中的节点
	Node* getLastSelctedNode();

	//进入多选模式，false退出
	void enterMultiSelcteMode(bool bValue){ m_multiSelectMode = bValue; }
protected:
	DragRect();

	~DragRect();

	virtual bool init();

	void onMoving(Vec2 offset);

	void onMoveEnd(Vec2 endPos);


	//方向键处理
	void onKeyPressed(EventKeyboard::KeyCode keycode, Event*);

	void onkeyReleased(EventKeyboard::KeyCode keycode, Event*);


protected:
	std::vector<Node*>						m_allSectedNodes;
	std::function<void(Node*, Vec2)>		m_movingFun;
	std::function<void(Node*, Vec2, Vec2)>	m_movingEndCall;
	bool									m_isMoving;
	std::map<Node*, Vec2>					m_allNodesOriginLocalPos;

	bool									m_keyPressed;
	bool									m_multiSelectMode;
	Vec2									m_keyPressOffset;
	friend class DragRectItem;
};



class DragRectItem
	: public Node
{
public:
	static DragRectItem *create(Node * node);

protected:
	DragRectItem();

	virtual bool init();

	void reSize();

	bool onRectTouchBegan(Touch *touch, Event * event);

	void onRectTouchMove(Touch *touch, Event * event);

	void onRectTouchEnd(Touch *touch, Event * event);


private:

	Sprite*		m_anchorPoint;//锚点图标
	Sprite*		m_leftTopCircle;
	Sprite*		m_rightTopCircle;
	Sprite*		m_leftDownCircle;
	Sprite*		m_rightDownCircle;

	Node*		m_target;

	bool		m_isMoving;
	
	Vec2		m_originPos;
};
#endif // !__DRAG_RECT_H__
