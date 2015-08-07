#pragma  once
#include "preInclude.h"

typedef struct _UI_BASE_DATA
{
public:

	Node	*pNode;//对应cocos2dx中的元件

	bool	locked;//是否锁定（禁止编辑）

	std::vector<_UI_BASE_DATA*> children;

	_UI_BASE_DATA* parent;

	_UI_BASE_DATA(_UI_BASE_DATA*parent = nullptr)
		: pNode(nullptr)
		, locked(false)
		, parent(nullptr)
	{

	}

	void destroy()//销毁调用,不直接析构
	{
		if (parent)
		{
			auto idx = parent->children.begin();
			for (; idx != parent->children.end(); idx++)
			{
				if (*idx == this)
				{
					parent->children.erase(idx);
					break;
				}
			}
			delete this;
		}
	}
protected:

	~_UI_BASE_DATA()
	{
		for (auto it : children)
		{
			delete it;
		}
	}

} UI_DATA;


enum widgetProperty
{
	name,
	color,
	opacity,
	position,
	rotation,
	scale,
};

//单帧动画数据
typedef struct _actionFrame
{
public:
	std::string className;//"classname":default null
	std::string name;//"name":default null
	Color3B color; //save with "colorb","colorg","colorr"
	int frameid;//farmeid
	int opacity;
	Vec2 position;//save with "positionx","positiony"
	float rotation;
	Vec2 scale;//save with "scalex","scaley"
	float starttime;//????
	std::vector<float> tweenParameter;
	int tweenType;
	bool visible;//value:true || false,default:false

	_actionFrame()
		: className("null")
		, name("null")
		, color(Color3B::WHITE)
		, frameid(0)
		, opacity(255)
		, position(Vec2::ZERO)
		, rotation(0)
		, scale(Vec2::ZERO)
		, starttime(0)
		, tweenType(0)
		, visible(false)
	{
	}

	_actionFrame * clone()
	{
		_actionFrame* result = new _actionFrame();
		result->className = className;
		result->name = name;
		result->color = color;
//		result->frameid = frameid;
		result->opacity = opacity;
		result->position = position;
		result->rotation = rotation;
		result->scale = scale;
		result->starttime = starttime;
		result->tweenType = tweenType;
		for (auto tween : tweenParameter)
		{
			result->tweenParameter.push_back(tween);
		}
		result->visible = visible;
		return result;
	}

	void operator=(_actionFrame *from)
	{
		className = from->className;
		name = from->name;
		color = from->color;
//		frameid = from->frameid;
		opacity = from->opacity;
		position = from->position;
		rotation = from->rotation;
		scale = from->scale;
		starttime = from->starttime;
		tweenType = from->tweenType;
		tweenParameter.clear();
		for(auto tween : from->tweenParameter)
			tweenParameter.push_back(tween);
	}

	_actionFrame(_actionFrame *otherFrame)
	{
		*this = otherFrame;
	}


} ACTION_FRAME;


typedef struct _actionNode
{
public:
	std::string classname;// default : null
	std::string name; //default null
	int ActionTag;
	std::map<int,ACTION_FRAME*> actionframelist;//<frameid,actionFrame>
	Widget*target;

	_actionNode()
		: classname("null")
		, name("null")
		, ActionTag(0)
		, target(nullptr)
	{

	}

	_actionNode *clone()
	{
		_actionNode *result = new _actionNode();
		result->classname = classname;
		result->ActionTag = ActionTag;
		result->name = name;
		result->target = target;
		for (auto &it : actionframelist)
		{
			if (!it.second)
				continue;
			auto frame = it.second->clone();
			result->actionframelist[it.first] = frame;
		}
		return result;
	}

	//注意，这里只拷贝动画数据，不包括名字等信息
	void operator=(_actionNode * otherNode)
	{
		for (auto &it : actionframelist)
		{
			if (it.second)
				delete it.second;
		}
		actionframelist.clear();
		for (auto &it : otherNode->actionframelist)
		{
			if (!it.second)
				continue;
			auto frame = it.second->clone();
			frame->frameid = it.first;
			actionframelist[it.first] = frame;
		}
	}

	_actionNode(_actionNode *otherNode)
	{
		*this = otherNode;
	}

} ACTION_NODE;

typedef struct _animation
{
public:
	std::string classname;// default : null
	std::string name;// default : null
	//std::map<int,ACTION_NODE> actionnodelist;//<actionTag,ActionNode>
	std::vector<ACTION_NODE*> actionnodelist;
	bool loop; //default : false
	float unittime; // default:0.1

	_animation()
		: loop(false)
		, unittime(0.1f)
		, classname("null")
		, name("null")
	{
	}
} ANIMATION;


//Q_DECLARE_METATYPE(UI_DATA);
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif
//采用非字节对齐


