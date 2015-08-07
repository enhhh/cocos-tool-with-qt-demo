#ifndef __ViewMgr_H_
#define __ViewMgr_H_
#include "qlistwidget.h"
#include "qtvariantproperty.h"

class ViewMgr : QObject
{
	Q_OBJECT

public:
	static ViewMgr * getInstance();

	void registerWidget(std::string name, QWidget *widget);

	QWidget * getWidgetWithName(const std::string &name);

	void updateSpineActionsListview();

	void updatePropertiesView();

	void updateParticleView();

public slots:
void onSpineActionsClick(QListWidgetItem * item);

protected:
	ViewMgr();

protected:
	std::map<std::string, QWidget*> m_widgets;
	QtVariantPropertyManager * m_variantManager;
};
#endif // !__ViewMgr_H_
