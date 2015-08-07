#include "CocosHelper.h"



QSizeF CocosHelper::CSizeToQSize(const Size &size)
{
	QSizeF ret;
	ret.setWidth(size.width);
	ret.setHeight(size.height);
	return ret;
}

QColor CocosHelper::CColorToQColor(const Color3B &color)
{
	QColor ret;
	ret.setRed(color.r);
	ret.setGreen(color.g);
	ret.setBlue(color.b);
	return ret;

}

QPointF CocosHelper::VecToQPoint(const Vec2 &vec)
{
	QPointF ret;
	ret.setX(vec.x);
	ret.setY(vec.y);
	return ret;
}

