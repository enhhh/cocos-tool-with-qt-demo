#include "cocoseditor.h"
#include <QtWidgets/QApplication>
#include "AppDelegate.h"

int main(int argc, char *argv[])
{
	AppDelegate app(argc, argv);
	return cocos2d::CCQApplication::getInstance()->run();
}
