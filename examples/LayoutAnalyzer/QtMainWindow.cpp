#include "QtMainWindow.h"
#include <QDir>
#include <QFileDialog>


QtMainWindow::QtMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_ribbon = new RibbonImpl(ui.ribbon_widget);
	m_spacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);
	if (ui.layerSelector_frame->layout() == nullptr)
	{
		QVBoxLayout* layout = new QVBoxLayout();
		ui.layerSelector_frame->setLayout(layout);
	}
	ui.layerSelector_frame->layout()->addItem(m_spacer);

	setupScene();
	RibbonImpl::LayerButtons& layerButtons = m_ribbon->getLayerButtons();
	connect(layerButtons.addLayer, &RibbonWidget::RibbonButton::clicked, [this]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("Open Layer Image"), "", tr("Image Files (*.png *.jpg *.bmp *.dat)"));
			if (fileName.isEmpty())
				return;
			fileName.replace("/", "\\");
			
			LayerData layerData;
			layerData.layer = new LayoutAnalyzer::PrintLayer();
			std::string filePath = fileName.toStdString();
			std::string name = filePath.substr(filePath.find_last_of("\\") + 1);
			
			bool loaded = false;

			if (name.find(".dat") != std::string::npos)
			{
				loaded = layerData.layer->loadDataFromFile(filePath);
			}
			else
			{
				loaded = layerData.layer->loadLayer(filePath);
			}
			if(loaded)
			{
				layerData.button = new QPushButton(name.c_str());
				ui.layerSelector_frame->layout()->removeItem(m_spacer);
				ui.layerSelector_frame->layout()->addWidget(layerData.button);
				ui.layerSelector_frame->layout()->addItem(m_spacer);

				
				
				m_scene->addObject(layerData.layer);

				connect(layerData.button, &QPushButton::clicked, [this, index = m_layers.size()]()
					{
						selectLayer(index);
					});

				std::string maskPath = LayoutAnalyzer::GlobalSettings::getFilterMaskPath();
				if (!maskPath.empty())
					layerData.layer->loadFilter(maskPath);

				m_layers.push_back(layerData);
				selectLayer(m_layers.size()-1);
			}

		});

	connect(layerButtons.removeSelectedLayer, &RibbonWidget::RibbonButton::clicked, [this]()
		{
			if (m_layers.size() == 0)
				return;

			delete m_layers[m_selectedLayer].button;
			m_layers[m_selectedLayer].layer->deleteLater();
			m_layers.erase(m_layers.begin() + m_selectedLayer);
			if (m_selectedLayer != 0)
				m_selectedLayer--;
			selectLayer(m_selectedLayer);
		});
	connect(layerButtons.saveLayersToFile, &RibbonWidget::RibbonButton::clicked, [this]()
		{
			// File dialog to select a folder
			QString folderPath = QFileDialog::getExistingDirectory(this, tr("Select Folder to save Layers"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
			if (folderPath.isEmpty())
				return;
			std::string path = folderPath.toStdString();
			for (size_t i = 0; i < m_layers.size(); ++i)
			{
				std::string name = path + "\\" + m_layers[i].layer->getName() + ".dat";
				m_layers[i].layer->saveDataToFile(name);
			}
		});


	RibbonImpl::InputButtons& inputButtons = m_ribbon->getInputButtons();
	connect(inputButtons.loadMask, &RibbonWidget::RibbonButton::clicked, [this]()
		{
			QString fileName = QFileDialog::getOpenFileName(this, tr("Open Filter Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
			if (fileName.isEmpty())
				return;
			LayoutAnalyzer::GlobalSettings::setFilterMaskPath(fileName.toStdString());
			for (size_t i = 0; i < m_layers.size(); ++i)
				m_layers[i].layer->loadFilter(LayoutAnalyzer::GlobalSettings::getFilterMaskPath());
		});

	RibbonImpl::AnalysisButtons& analysisButtons = m_ribbon->getAnalysisButtons();
	connect(analysisButtons.startViaDetection, &RibbonWidget::RibbonButton::clicked, [this]()
		{
			if (m_layers.size() > 0)
				getSelectedLayer().layer->applyFilter();
		});

	RibbonImpl::ToolButtons& toolButtons = m_ribbon->toolButtons();
	connect(toolButtons.pathFinder, &RibbonWidget::InformativeToolButton::clicked, [this]()
		{
			LayoutAnalyzer::Toolbox::setCurrentTool(LayoutAnalyzer::Toolbox::Tool::pathFinder);
			m_ribbon->setToolButtonActive(LayoutAnalyzer::Toolbox::getCurrentTool());
		});

	connect(toolButtons.viaSetter, &RibbonWidget::InformativeToolButton::clicked, [this]()
		{
			LayoutAnalyzer::Toolbox::setCurrentTool(LayoutAnalyzer::Toolbox::Tool::viaSetter);
			m_ribbon->setToolButtonActive(LayoutAnalyzer::Toolbox::getCurrentTool());
		});

	connect(toolButtons.viaDeleter, &RibbonWidget::InformativeToolButton::clicked, [this]()
		{
			LayoutAnalyzer::Toolbox::setCurrentTool(LayoutAnalyzer::Toolbox::Tool::viaDeleter);
			m_ribbon->setToolButtonActive(LayoutAnalyzer::Toolbox::getCurrentTool());
		});

	connect(toolButtons.silkscreenColorPicker, &RibbonWidget::InformativeToolButton::clicked, [this]()
		{
			LayoutAnalyzer::Toolbox::setCurrentTool(LayoutAnalyzer::Toolbox::Tool::silkscreenColorPicker);
			m_ribbon->setToolButtonActive(LayoutAnalyzer::Toolbox::getCurrentTool());
		});
	connect(toolButtons.viaColorPicker, &RibbonWidget::InformativeToolButton::clicked, [this]()
		{
			LayoutAnalyzer::Toolbox::setCurrentTool(LayoutAnalyzer::Toolbox::Tool::viaColorPicker);
			m_ribbon->setToolButtonActive(LayoutAnalyzer::Toolbox::getCurrentTool());
		});
	connect(toolButtons.backgroundColorPicker, &RibbonWidget::InformativeToolButton::clicked, [this]()
		{
			LayoutAnalyzer::Toolbox::setCurrentTool(LayoutAnalyzer::Toolbox::Tool::backgroundColorPicker);
			m_ribbon->setToolButtonActive(LayoutAnalyzer::Toolbox::getCurrentTool());
		});


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
	defaultEditor->getCamera()->setMinZoom(0.01);
	m_scene->addObject(defaultEditor);


	//layer = new LayoutAnalyzer::PrintLayer();
	//m_scene->addObject(layer);

    //std::string layersPath = "D:\\Users\\Alex\\Dokumente\\SoftwareProjects\\LayoutAnalyzer\\Layout";
    /*std::string layersPath = "..\\Layout";
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
	}*/


	m_scene->start();

}

void QtMainWindow::selectLayer(size_t index)
{
	if (m_layers.size() <= index)
		return;

	m_layers[m_selectedLayer].layer->setRenderLayer(QSFML::RenderLayer::layer_0);
	m_layers[index].layer->setRenderLayer(QSFML::RenderLayer::layer_1);

	for (size_t i = 0; i < m_layers.size(); ++i)
	{
		if (i == index)
		{
			m_layers[i].button->setDown(true);
			//m_layers[i].layer->setEnabled(true);
			m_layers[i].layer->enableInteractions(true);

		}
		else
		{
			m_layers[i].button->setDown(false);
		    //m_layers[i].layer->setEnabled(false);
		    m_layers[i].layer->stopPathFinder();
			m_layers[i].layer->enableInteractions(false);
		}
	}
	m_selectedLayer = index;
}
QtMainWindow::LayerData& QtMainWindow::getSelectedLayer()
{
	return m_layers[m_selectedLayer];
}