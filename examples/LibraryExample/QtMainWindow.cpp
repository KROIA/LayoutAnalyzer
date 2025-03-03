#include "QtMainWindow.h"
#include <QDir>

QtMainWindow::QtMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_ribbon = new RibbonImpl(ui.ribbon_widget);

	setupScene();


}

QtMainWindow::~QtMainWindow()
{
	delete m_ribbon;
}


void QtMainWindow::setupScene()
{
    QSFML::SceneSettings settings;
    //settings.layout.autoAjustSize = false;
    settings.layout.fixedSize = sf::Vector2u(300, 100);
    settings.contextSettings.antialiasingLevel = 8;
    settings.timing.frameTime = 0.02;
    //settings.updateControlls.enableMultithreading = false;
    //settings.updateControlls.enablePaintLoop = false;
    //settings.updateControlls.enableEventLoop = false;
    //settings.updateControlls.enableUpdateLoop = false;
    m_scene = new QSFML::Scene(ui.engine_widget, settings);
	QSFML::Objects::DefaultEditor* defaultEditor = new QSFML::Objects::DefaultEditor("Editor", { 20000,20000 });
	m_scene->addObject(defaultEditor);


    //std::string layersPath = "D:\\Users\\Alex\\Dokumente\\SoftwareProjects\\LayoutAnalyzer\\Layout";
    std::string layersPath = "..\\Layout";
	// Get File list in directory
	QDir dir(QString::fromStdString(layersPath));
	QStringList filters;
	filters << "*.png";
	QStringList files = dir.entryList(filters, QDir::Files | QDir::NoDotAndDotDot);
	for (int i = 0; i < files.size(); ++i)
	{
		QString file = files.at(i);
		std::string path = layersPath + "\\" + file.toStdString();
		LayoutAnalyzer::PrintLayer* layer = new LayoutAnalyzer::PrintLayer(path);
		m_scene->addObject(layer);
		break;
	}


	m_scene->start();

}