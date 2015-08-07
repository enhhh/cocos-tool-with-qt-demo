#include "cocoseditor.h"
#include "AppDelegate.h"
#include "qfile.h"
#include <QMessageBox>
#include <QFileDialog>
#include "qstring.h"
#include "qsplitter.h"
#include "qgridlayout.h"
#include "DocumentMgr.h"
#include <QListWidget>
#include "ViewMgr.h"
#include "qttreepropertybrowser.h"
#include "ActionListWidget.h"

CocosEditor::CocosEditor(QWidget *parent)
: QMainWindow(parent)
, _appDelegate(NULL)
, _glWidget(NULL)
, fileMenu(nullptr)
, editMenu(nullptr)
, helpMenu(nullptr)
, fileToolBar(nullptr)
, editToolBar(nullptr)
, newProjAct(nullptr)
, loadModelAct(nullptr)
, saveAct(nullptr)
, saveAsAct(nullptr)
, exitAct(nullptr)
, aboutAct(nullptr)
, aboutQtAct(nullptr)
, openProjAct(nullptr)
{
	ui.setupUi(this);
	createAction();
	createMenus();
	createToolBars();
}

CocosEditor::~CocosEditor()
{

}

void CocosEditor::createAction()
{
	newProjAct = new QAction(QIcon(":/images/new.png"), tr("&New project"), this);
	newProjAct->setShortcuts(QKeySequence::New);
	newProjAct->setStatusTip(tr("Create a new file"));
	connect(newProjAct, SIGNAL(triggered()), this, SLOT(newProject()));
	 


	//! [19]
	loadModelAct = new QAction(QIcon(":/images/open.png"), tr("load &Model..."), this);
	loadModelAct->setStatusTip(tr("Open an existing file"));
	connect(loadModelAct, SIGNAL(triggered()), this, SLOT(loadModel()));
	//! [18] //! [19]

	saveAct = new QAction(QIcon(":/images/save.png"), tr("&Save project"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, SIGNAL(triggered()), this, SLOT(saveProject()));
	saveAsAct = new QAction(tr("Save &As..."), this);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));
	connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));


	undoAct = new QAction(QIcon(":/images/undo.png"), tr("&undo"),this);
	undoAct->setShortcut(QKeySequence::Undo);
	//connect(undoAct, SIGNAL(triggered()), UIDoucument::getInstance(), SLOT(undo()));

	redoAct = new QAction(QIcon(":/images/redo.png"), tr("&redo"), this);
	redoAct->setShortcut(QKeySequence::Redo);
	//connect(redoAct, SIGNAL(triggered()), UIDoucument::getInstance(), SLOT(redo()));

	restoreFromBak = new QAction(QIcon(":/images/bak.png"), tr("&restore"), this);
	restoreFromBak->setShortcut(QKeySequence::Replace);
	//connect(restoreFromBak, SIGNAL(triggered()), UIDoucument::getInstance(), SLOT(resetAnimationFromBackFile()));

	//! [20]
	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcuts(QKeySequence::Quit);
	//! [20]
	exitAct->setStatusTip(tr("Exit the application"));
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	openProjAct = new QAction(tr("&Open project"), this);
	openProjAct->setShortcuts(QKeySequence::Open);
	connect(openProjAct, SIGNAL(triggered()), this, SLOT(openProject()));

	loadParticleAct = new QAction(tr("load &Particle"),this);
	connect(loadParticleAct, SIGNAL(triggered()), this, SLOT(loadParticle()));

	//! [21]

// 	aboutAct = new QAction(tr("&About"), this);
// 	aboutAct->setStatusTip(tr("Show the application's About box"));
// 	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	//! [22]
	

	aboutQtAct = new QAction(tr("About &Qt"), this);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
	connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
	//! [22]

	//! [23]

}

void CocosEditor::createMenus()
//! [25] //! [27]
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(newProjAct);
	//! [28]
	

	fileMenu->addAction(openProjAct);

	fileMenu->addAction(loadModelAct);

	fileMenu->addAction(loadParticleAct);
	//! [28]
	fileMenu->addAction(saveAct);
	//! [26]
	fileMenu->addAction(saveAsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
	menuBar()->addSeparator();
	
	editMenu = menuBar()->addMenu(tr("&Edit"));
	editMenu->addAction(undoAct);
	editMenu->addAction(redoAct);
	editMenu->addAction(restoreFromBak);

	menuBar()->addSeparator();

	helpMenu = menuBar()->addMenu(tr("&Help"));
	helpMenu->addAction(aboutAct);
	helpMenu->addAction(aboutQtAct);
}

void CocosEditor::createToolBars()
{
	fileToolBar = ui.mainToolBar;
	fileToolBar->addAction(newProjAct);
	//! [29] //! [31]
	fileToolBar->addAction(loadModelAct);
	//! [31]
	fileToolBar->addAction(saveAct);

	fileToolBar->addAction(undoAct);

	fileToolBar->addAction(redoAct);

	fileToolBar->addAction(restoreFromBak);

	//editToolBar = addToolBar(tr("Edit"));
}


void CocosEditor::closeEvent(QCloseEvent *)
{
	cocos2d::Director::getInstance()->end();
	qApp->quit();
}

void CocosEditor::setGLView(QWidget *glWidget)
{
	_glWidget = glWidget;

	if (_glWidget)
	{
		setCentralWidget(_glWidget);
	}
}


void CocosEditor::loadModel()
//! [7] //! [8]
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open spine file"), "", tr("ExportJson Files(*.json)"));
		std::string file = fileName.toStdString();
		if (!fileName.isEmpty())
			DocumentMgr::getInstance()->loadSpineAnimation(file);
}

void CocosEditor::newProject()
{
	QString fileName = QFileDialog::getSaveFileName(this, tr("save sp file"), "", tr("spine particle Files(*.sp)"));
	std::string file = fileName.toStdString();
	if (!fileName.isEmpty())
		DocumentMgr::getInstance()->createProject(file);
}

bool CocosEditor::saveProject()
{
	DocumentMgr::getInstance()->saveProject();
	return true;
}
bool CocosEditor::saveAs()
{
	return true;
}
void CocosEditor::about()
{
}

void CocosEditor::initActionListWidget()
{
	auto * actionsListWidgets = new ActionListWidget();
	ui.actionWidget->setWidget(actionsListWidgets);
	ViewMgr::getInstance()->registerWidget("actionWidget", actionsListWidgets);

	QListWidget*spActionWidget = new QListWidget();
	ui.spineActionWidget->setWidget(spActionWidget);
	ViewMgr::getInstance()->registerWidget("spineActonWidget", spActionWidget);

	QListWidget*particleWidget = new QListWidget();
	ui.particleWidget->setWidget(particleWidget);
	ViewMgr::getInstance()->registerWidget("particleWidget", particleWidget);

	QtTreePropertyBrowser*propertiesWidget = new QtTreePropertyBrowser();
	ui.propertiesWidget->setWidget(propertiesWidget);
	ViewMgr::getInstance()->registerWidget("propertiesWidget", propertiesWidget);
}

void CocosEditor::loadParticle()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open particle file"), "", tr("particle Files(*.plist)"));
	std::string file = fileName.toStdString();
	if (!fileName.isEmpty())
		DocumentMgr::getInstance()->loadParticle(file);
}

void CocosEditor::openProject()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open spine particle file"), "", tr("spine particle Files(*.sp)"));
	std::string file = fileName.toStdString();
	if (!fileName.isEmpty())
		DocumentMgr::getInstance()->loadProject(file);
}
