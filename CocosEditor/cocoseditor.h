#ifndef COCOSEDITOR_H
#define COCOSEDITOR_H

#include <QtWidgets/QMainWindow>
#include "ui_cocoseditor.h"

class AppDelegate;
namespace Ui {
	class CocosEditor;
}

class CocosEditor : public QMainWindow
{
	Q_OBJECT

public:
	CocosEditor(QWidget *parent = 0);
	~CocosEditor();

	void setGLView(QWidget *glWidget);

	void createAction();

	void createMenus();

	void createToolBars();

	void initActionListWidget();

protected:
	void closeEvent(QCloseEvent *);

	

private slots:
	void newProject();
	void loadModel();
	void loadParticle();
	void openProject();
	bool saveProject();
	bool saveAs();
	void about();


private:
	Ui::CocosEditorClass	ui;
	AppDelegate				*_appDelegate;
	QWidget					*_glWidget;


protected:
	QMenu		*fileMenu;
	QMenu		*editMenu;
	QMenu		*helpMenu;
	QToolBar	*fileToolBar;
	QToolBar	*editToolBar;
	QAction		*newProjAct;
	QAction		*loadModelAct;
	QAction		*saveAct;
	QAction		*saveAsAct;
	QAction		*exitAct;
	QAction		*aboutAct;
	QAction		*aboutQtAct;
	QAction		*undoAct;
	QAction		*redoAct;
	QAction		*restoreFromBak;
	QAction		*loadParticleAct;
	QAction		*openProjAct;
};

#endif // COCOSEDITOR_H
