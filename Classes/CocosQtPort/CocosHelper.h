#ifndef __COCOS_HELPER_H__
#define __COCOS_HELPER_H__
#include "preInclude.h"


class CocosHelper
{
public:
	static QSizeF CSizeToQSize(const Size &size);

	static QColor CColorToQColor(const Color3B &color);

	static QPointF VecToQPoint(const Vec2 &vec);

};
#endif // !__COCOS_HELPER_H__
