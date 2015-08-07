#include "ActionListWidget.h"
#include "qboxlayout.h"
#include "qevent.h"
#include "DocumentMgr.h"
#include "qmenu.h"
#include "qaction.h"


ActionListWidget::ActionListWidget(QWidget * parent /*= nullptr*/)
:QWidget(parent)
, m_dataModel(nullptr)
{
	QHBoxLayout * layout = new QHBoxLayout(this);
	m_listView = new AnimationListView();
	m_listView->setEditTriggers(QAbstractItemView::EditTrigger::DoubleClicked);
	layout->addWidget(m_listView);
	m_dataModel = new QStandardItemModel();
	connect(m_dataModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(animationChanged(QStandardItem*)));
	connect(DocumentMgr::getInstance(), SIGNAL(updateActionView()), this, SLOT(catchAnimationUpdate()));
	// 
	// 	QStandardItemModel *model = new QStandardItemModel();
	// 	model->setItem(1, 0, new QStandardItem("1"));
	m_listView->setModel(m_dataModel);
	this->setLayout(layout);
}

void ActionListWidget::animationChanged(QStandardItem* item)
{
	s_spAction *ani = (s_spAction*)(item->data(Qt::UserRole).value<void*>());
	if (!ani)
		return;
	ani->actionName = item->index().data(Qt::DisplayRole).toString().toStdString();
}

void ActionListWidget::catchAnimationUpdate()
{
	m_dataModel->clear();
	const std::vector<s_spAction*> &list = DocumentMgr::getInstance()->m_pSpineParticle->m_actions;
	int i = 0;
	for (auto it : list)
	{
		QStandardItem *item = new QStandardItem(it->actionName.c_str());
		item->setData(QVariant::fromValue((void*)it), Qt::UserRole);
		m_dataModel->setItem(i, 0, item);
		i++;
	}
}

//model
AnimationListView::AnimationListView(QWidget* parent)
: QListView(parent)
, m_popMenu(nullptr)
{
	initPopMenu();
}

bool AnimationListView::edit(const QModelIndex &index, EditTrigger trigger, QEvent *event)
{
	if (!event)
		return false;

	if (event->type() == QEvent::MouseButtonPress)
	{
		m_currentIdx = index;
		QMouseEvent * mouseEvent = (QMouseEvent *)(event);
		if (mouseEvent->button() == Qt::RightButton)
			m_popMenu->exec(QCursor::pos());
		else if (mouseEvent->button() == Qt::LeftButton && index.isValid())
		{

			DocumentMgr::getInstance()->setCurrentAction(index.data(Qt::DisplayRole).toString().toStdString());
		}
	}
	QListView::edit(index, trigger, event);
	return false;
}

void AnimationListView::initPopMenu()
{
	m_popMenu = new QMenu();
	QAction *addAni = new QAction(this);
	QAction *copyAni = new QAction(this);
	QAction *delAni = new QAction(this);
	connect(addAni, SIGNAL(triggered(bool)), this, SLOT(addAnimation()));
	connect(delAni, SIGNAL(triggered(bool)), this, SLOT(removeAnimation()));
	connect(copyAni, SIGNAL(triggered(bool)), this, SLOT(copyAnimation()));
	addAni->setText(tr("add animation"));
	copyAni->setText(tr("copy animation"));
	delAni->setText(tr("remove animation"));
	m_popMenu->addAction(addAni);
	m_popMenu->addAction(copyAni);
	m_popMenu->addAction(delAni);
}

void AnimationListView::addAnimation()
{
	//AnimationDoc::getInstance()->addAnimationWithName("ani");
	DocumentMgr::getInstance()->addAction();
}

void AnimationListView::removeAnimation()
{
	DocumentMgr::getInstance()->removeAction(m_currentIdx.data(Qt::DisplayRole).value<QString>().toStdString());

}

void AnimationListView::copyAnimation()
{

}

void AnimationListView::onSelectedIndexChanged()
{

}


AnimationListModel::AnimationListModel(QObject *parent /*= nullptr*/)
: QAbstractItemModel(parent)
{

}

int AnimationListModel::rowCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	if (DocumentMgr::getInstance()->m_pSpineParticle == nullptr)
		return 0;
	return DocumentMgr::getInstance()->m_pSpineParticle->m_actions.size();
}

int AnimationListModel::columnCount(const QModelIndex &parent /*= QModelIndex()*/) const
{
	return 1;
}

QVariant AnimationListModel::data(const QModelIndex &index, int role /*= Qt::DisplayRole*/) const
{
	if (!index.isValid() || index.column() != 0 || role != Qt::DisplayRole)
		return QVariant();
	std::vector<s_spAction*> vec = DocumentMgr::getInstance()->m_pSpineParticle->m_actions;
	if (index.row() >= vec.size())
		return QVariant();
	return QString::fromStdString(vec[index.row()]->actionName);
}

QModelIndex AnimationListModel::index(int row, int column, const QModelIndex &parent /*= QModelIndex()*/) const
{
	if (row == -1 || column != 0)
		return QModelIndex();
	std::vector<s_spAction*> vec = DocumentMgr::getInstance()->m_pSpineParticle->m_actions;;
	if (row >= vec.size())
		return QModelIndex();
	return createIndex(row, column, vec[row]);
}

QModelIndex AnimationListModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

Qt::ItemFlags AnimationListModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}
