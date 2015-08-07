#ifndef __PROPERTIES_MGR_H__
#define __PROPERTIES_MGR_H__

#include "QObject"
#include "QtVariantProperty"

class PropertiesMgr
	: public QObject
{
	Q_OBJECT
public:
	static PropertiesMgr * getInstance();

	void initPropertyView();

public slots:
	void propertyValueChanged(QtProperty *, const QVariant &);
protected:
	PropertiesMgr();

	~PropertiesMgr();

public:
	QtVariantProperty *m_actionNameProperty;
	QtVariantProperty *m_particleSlotsProperty;

	QtVariantPropertyManager* m_variantManager;
	std::map<QtVariantProperty*, std::string> m_particlesSlotProperty;
	std::map<QtVariantProperty*, std::string> m_particleOffsetProperty;
	std::vector<std::string> m_spineActionNames;
	std::vector<std::string> m_slotsNames;
};
#endif // !__PROPERTIES_MGR_H__
