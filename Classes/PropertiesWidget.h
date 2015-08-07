#ifndef __PROPERTIES_WIDGET_H__
#define __PROPERTIES_WIDGET_H__

#include <QWidget>
#include "qtpropertybrowser.h"
#include "qtvariantproperty.h"

class PropertiesWidget
	: public QWidget
{
	Q_OBJECT
public:
	PropertiesWidget(QWidget *parent = nullptr);

protected:
	void initSizeProperties();

public Q_SLOTS:
	void propertyValueChanged(QtProperty *property, const QVariant &val);

	void updateView();
private:
	QtVariantPropertyManager *variantManager;
	QtAbstractPropertyBrowser*		m_browser;


	QtVariantProperty *m_sizeModeProperty;
	QtVariantProperty *m_sizeProperty;

	QtVariantProperty *m_tagProperty;
	QtVariantProperty *m_catchEventProperty;
	QtVariantProperty *m_uiNameProperty;
	QtVariantProperty *m_zOrderProperty;
	QtVariantProperty *m_opacityProperty;
	QtVariantProperty *m_colorMixProperty;
	QtVariantProperty *m_percentPositionProperty;
	QtVariantProperty *m_positionProperty;

	QtVariantProperty *m_flipXProperty;
	QtVariantProperty *m_flipYProperty;

	QtVariantProperty *m_scaleProperty;
	QtVariantProperty *m_rotateProperty;
	QtVariantProperty *m_anchorPointProperty;
	QtVariantProperty *m_blendSrcProperty;
	QtVariantProperty *m_blendDstProperty;
};
#endif // !__PROPERTIES_WIDGET_H__
