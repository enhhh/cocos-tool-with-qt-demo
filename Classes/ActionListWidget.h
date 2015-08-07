#ifndef __ACTION_LIST_WIDGET_H_
#define __ACTION_LIST_WIDGET_H_
#include "qstandarditemmodel.h"
#include "qlistview.h"

class ActionListWidget
	: public QWidget
{
	Q_OBJECT

public:
	ActionListWidget(QWidget * parent = nullptr);

	public slots:
	void animationChanged(QStandardItem* item);

	void catchAnimationUpdate();
protected:
	QStandardItemModel *m_dataModel;
	QListView * m_listView;
};


class AnimationListView
	: public QListView
{
	Q_OBJECT
public:
	AnimationListView(QWidget* parent = nullptr);

	virtual bool edit(const QModelIndex &index, EditTrigger trigger, QEvent *event) override;

protected:
	void initPopMenu();

	public slots:
	void addAnimation();

	void removeAnimation();

	void copyAnimation();

	void onSelectedIndexChanged();



protected:
	QMenu * m_popMenu;
	QModelIndex m_currentIdx;
};

class AnimationListModel
	: public QAbstractItemModel
{
	Q_OBJECT

public:

	AnimationListModel(QObject *parent = nullptr);

	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

	virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	virtual QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

	virtual QModelIndex parent(const QModelIndex &child) const;

	virtual Qt::ItemFlags flags(const QModelIndex &index) const;

};
#endif // !__ACTION_LIST_WIDGET_H_
