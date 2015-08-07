#ifndef __DRAG_RECT_H__
#define __DRAG_RECT_H__
#include "preInclude.h"


class DragRect
	: public Ref
{
public:
	static DragRect * getInstance();

	//���ѡ�нڵ�
	void addSelectNode(Node * node);

	//ȡ��ѡ�����нڵ�
	void removeAllSelctedNodes();

	//ȡ��ѡ�нڵ�
	void removeSelectedNode(Node *node);

	//�����ƶ��ص�
	void setMovingCallBack(std::function<void(Node*, Vec2)> fun){ m_movingFun = fun; }

	//�����ƶ������ص�
	//target,localBeginPos,localEndPos
	void setMovingEndCall(std::function<void(Node*, Vec2, Vec2)> fun){ m_movingEndCall = fun; }

	//��ȡ����ѡ�нڵ�
	const std::vector<Node*> & getAllSelectedNodes(){ return m_allSectedNodes; }

	//��ȡ���һ��ѡ�еĽڵ�
	Node* getLastSelctedNode();

	//�����ѡģʽ��false�˳�
	void enterMultiSelcteMode(bool bValue){ m_multiSelectMode = bValue; }
protected:
	DragRect();

	~DragRect();

	virtual bool init();

	void onMoving(Vec2 offset);

	void onMoveEnd(Vec2 endPos);


	//���������
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

	Sprite*		m_anchorPoint;//ê��ͼ��
	Sprite*		m_leftTopCircle;
	Sprite*		m_rightTopCircle;
	Sprite*		m_leftDownCircle;
	Sprite*		m_rightDownCircle;

	Node*		m_target;

	bool		m_isMoving;
	
	Vec2		m_originPos;
};
#endif // !__DRAG_RECT_H__
