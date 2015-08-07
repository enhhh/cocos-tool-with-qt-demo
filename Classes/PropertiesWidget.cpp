#include "PropertiesWidget.h"
#include "qttreepropertybrowser.h"

PropertiesWidget::PropertiesWidget(QWidget *parent /*= nullptr*/)
{
	initSizeProperties();
}

void PropertiesWidget::initSizeProperties()
{
	variantManager = new QtVariantPropertyManager();
}

void PropertiesWidget::propertyValueChanged(QtProperty *property, const QVariant &val)
{
	QtVariantEditorFactory *variantFactory = new QtVariantEditorFactory();
	m_browser = new QtTreePropertyBrowser();

	m_browser->setFactoryForManager(variantManager, variantFactory);

	m_browser->addProperty(sizeGroup);
	m_browser->addProperty(normalGroup);
	m_browser->addProperty(layoutGroup);
	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(m_browser);
	this->setLayout(layout);
}

void PropertiesWidget::updateView()
{

}

