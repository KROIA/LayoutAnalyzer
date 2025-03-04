#include "RibbonImpl.h"


RibbonImpl::RibbonImpl(QWidget* parent)
	: RibbonWidget::Ribbon(parent)
{
	Q_INIT_RESOURCE(RibbonIcons); // Init the resource file
	Q_INIT_RESOURCE(icons); // Init the resource file
	// Create tabs
	RibbonWidget::RibbonTab* tabSettings	= new RibbonWidget::RibbonTab("Settings", ":/icons/developerTool.png", this);
	//RibbonWidget::RibbonTab* tabEdit		= new RibbonWidget::RibbonTab("Editing", ":/icons/create_new_2.png", this);

	// Create groups
	RibbonWidget::RibbonButtonGroup* layerGroup	= new RibbonWidget::RibbonButtonGroup("Layer", tabSettings);
	RibbonWidget::RibbonButtonGroup* filterGroup	= new RibbonWidget::RibbonButtonGroup("Filter", tabSettings);
	RibbonWidget::RibbonButtonGroup* analysisGroup	= new RibbonWidget::RibbonButtonGroup("Analysis", tabSettings);
	RibbonWidget::RibbonButtonGroup* toolGroup	= new RibbonWidget::RibbonButtonGroup("Tools", tabSettings);
	//RibbonWidget::RibbonButtonGroup* groupSettings	= new RibbonWidget::RibbonButtonGroup("groupSettings", tabSettings);
	//RibbonWidget::RibbonButtonGroup* groupWork		= new RibbonWidget::RibbonButtonGroup("groupWork", tabEdit);
	//RibbonWidget::RibbonButtonGroup* groupEdit		= new RibbonWidget::RibbonButtonGroup("groupEdit", tabEdit);

	// Create buttons
	m_layerButtons.addLayer = new RibbonWidget::RibbonButton("Add Layer", "Add a layer image", ":/icons/live_folder_2.png", true, layerGroup);
	m_layerButtons.removeSelectedLayer = new RibbonWidget::RibbonButton("Remove Layer", "Remove selected layer", ":/icons/delete.png", true, layerGroup);
	
	m_inputButtons.loadMask = new RibbonWidget::RibbonButton("Load Mask", "Load a mask image representig a via,\nused for via detection", ":/icons/live_folder_2.png", true, filterGroup);
	
	m_analysisButtons.startViaDetection = new RibbonWidget::RibbonButton("Start Via\ndetection", "Try to find the vias using the via mask", ":/icons/lupe.png", true, analysisGroup);
	
	m_toolButtons.pathFinder = new RibbonWidget::InformativeToolButton("Path Finder", "Rightclick on a color to find all its paths", ":/icons/lupe.png", true, toolGroup);
	m_toolButtons.viaDeleter = new RibbonWidget::InformativeToolButton("Delete Via", "Remove a wrong detected via", ":/icons/ViaRemove.png", true, toolGroup);
	m_toolButtons.viaSetter = new RibbonWidget::InformativeToolButton("Add Via", "Add a via manually", ":/icons/Via.png", true, toolGroup);
	m_toolButtons.silkscreenColorPicker = new RibbonWidget::InformativeToolButton("Pick sinkscreen color", "Pick the silkscreen color for bether path finding", ":/icons/lupe.png", true, toolGroup);
	m_toolButtons.viaColorPicker = new RibbonWidget::InformativeToolButton("Pick via color", "Pick the via color for bether path finding", ":/icons/lupe.png", true, toolGroup);
	m_toolButtons.backgroundColorPicker = new RibbonWidget::InformativeToolButton("Pick background color", "Pick the background color to make it transparent", ":/icons/lupe.png", true, toolGroup);

	//m_settingsButtons.settings1		= new RibbonWidget::RibbonButton("Settings 1", "Settings 1", ":/icons/engineering_1.png", true, groupSettings);
	//m_settingsButtons.settingsOK	= new RibbonWidget::RibbonButton("Settings OK", "Settings OK", ":/icons/accept.png", true, groupSettings);
	//m_settingsButtons.settingsCancel= new RibbonWidget::RibbonButton("Cancel", "Cancel", ":/icons/close.png", true, groupSettings);
	//m_settingsButtons.settingsSave	= new RibbonWidget::RibbonButton("Settings Save", "Settings Save", ":/icons/floppy_disk.png", true, groupSettings);

	//m_settingsButtons.settings1->setIconSize(QSize(40, 40));
	//
	//m_workButtons.open = new RibbonWidget::InformativeToolButton("Open", "Open", ":/icons/document.png", true, groupWork);
	//m_workButtons.save = new RibbonWidget::InformativeToolButton("Save", "Save", ":/icons/floppy_disk.png", true, groupWork);
	//
	//m_editButtons.undo = new RibbonWidget::RibbonButton("Undo", "Undo", ":/icons/arrowCClockwise.png", true, groupEdit);
	//m_editButtons.redo = new RibbonWidget::RibbonButton("Redo", "Redo", ":/icons/arrowClockwise.png", true, groupEdit);
	//
	//m_viewButtons.view1 = new RibbonWidget::InformativeToolButton("View 1", "View 1", ":/icons/file_2.png", true, groupEdit);
	//m_viewButtons.view1->setLoadingCircleColor(QColor(255, 0, 0));
	//m_viewButtons.view1->enableLoadingCircle(true);
	//
	//m_viewButtons.view2 = new RibbonWidget::InformativeToolButton("View 1", "View 1", ":/icons/file_2.png", true, groupEdit);
	//m_viewButtons.view2->setProgressColor(QColor(0, 255, 255));
	//m_viewButtons.view2->setProgress(50);
	//m_viewButtons.view3 = new RibbonWidget::InformativeToolButton("View 1", "View 1", ":/icons/file_2.png", true, groupEdit);
	//
	//// Add tabs
	//addTab(tabSettings);
	//addTab(tabEdit);
	addTab(tabSettings);
}

RibbonImpl::LayerButtons& RibbonImpl::getLayerButtons()
{
	return m_layerButtons;
}
RibbonImpl::InputButtons& RibbonImpl::getInputButtons()
{
	return m_inputButtons;
}
RibbonImpl::AnalysisButtons& RibbonImpl::getAnalysisButtons()
{
	return m_analysisButtons;
}
RibbonImpl::ToolButtons& RibbonImpl::toolButtons()
{
	return m_toolButtons;
}
void RibbonImpl::setToolButtonActive(LayoutAnalyzer::Toolbox::Tool tool)
{
	// enable overlay for the selected button
	m_toolButtons.pathFinder->setOverlayEnable(false);
	m_toolButtons.viaSetter->setOverlayEnable(false);
	m_toolButtons.viaDeleter->setOverlayEnable(false);
	m_toolButtons.silkscreenColorPicker->setOverlayEnable(false);
	m_toolButtons.viaColorPicker->setOverlayEnable(false);
	m_toolButtons.backgroundColorPicker->setOverlayEnable(false);
	switch (tool)
	{
	case LayoutAnalyzer::Toolbox::Tool::pathFinder:
		m_toolButtons.pathFinder->setOverlayEnable(true);
		break;
	case LayoutAnalyzer::Toolbox::Tool::viaSetter:
		m_toolButtons.viaSetter->setOverlayEnable(true);
		break;
	case LayoutAnalyzer::Toolbox::Tool::viaDeleter:
		m_toolButtons.viaDeleter->setOverlayEnable(true);
		break;
	case LayoutAnalyzer::Toolbox::Tool::silkscreenColorPicker:
		m_toolButtons.silkscreenColorPicker->setOverlayEnable(true);
		break;
	case LayoutAnalyzer::Toolbox::Tool::viaColorPicker:
		m_toolButtons.viaColorPicker->setOverlayEnable(true);
		break;
	case LayoutAnalyzer::Toolbox::Tool::backgroundColorPicker:
		m_toolButtons.backgroundColorPicker->setOverlayEnable(true);
		break;

	}
}
//RibbonImpl::SettingsButtons& RibbonImpl::settingsButtons()
//{
//	return m_settingsButtons;
//}
//RibbonImpl::WorkButtons& RibbonImpl::workButtons()
//{
//	return m_workButtons;
//}
//RibbonImpl::EditButtons& RibbonImpl::editButtons()
//{
//	return m_editButtons;
//}
//RibbonImpl::ViewButtons& RibbonImpl::viewButtons()
//{
//	return m_viewButtons;
//}