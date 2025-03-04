#pragma once

#include "RibbonWidget.h"
#include "LayoutAnalyzer.h"

class RibbonImpl : public RibbonWidget::Ribbon
{
	Q_OBJECT
	public:
		RibbonImpl(QWidget* parent = nullptr);

		struct LayerButtons
		{
			RibbonWidget::RibbonButton* addLayer;
			RibbonWidget::RibbonButton* removeSelectedLayer;
		};
		struct InputButtons
		{
			RibbonWidget::RibbonButton* loadMask;
		};

		struct AnalysisButtons
		{
			RibbonWidget::RibbonButton* startViaDetection;
		};

		struct ToolButtons
		{
			RibbonWidget::InformativeToolButton* pathFinder;
			RibbonWidget::InformativeToolButton* viaSetter;
			RibbonWidget::InformativeToolButton* viaDeleter;
			RibbonWidget::InformativeToolButton* silkscreenColorPicker;
			RibbonWidget::InformativeToolButton* viaColorPicker;
			RibbonWidget::InformativeToolButton* backgroundColorPicker;
		};
	
		//struct SettingsButtons
		//{
		//	RibbonWidget::RibbonButton* settings1;
		//	RibbonWidget::RibbonButton* settingsOK;
		//	RibbonWidget::RibbonButton* settingsCancel;
		//	RibbonWidget::RibbonButton* settingsSave;
		//};
		//
		//struct WorkButtons
		//{
		//	RibbonWidget::InformativeToolButton* open;
		//	RibbonWidget::InformativeToolButton* save;
		//};
		//
		//struct EditButtons
		//{
		//	RibbonWidget::RibbonButton* undo;
		//	RibbonWidget::RibbonButton* redo;
		//};
		//
		//struct ViewButtons
		//{
		//	RibbonWidget::InformativeToolButton* view1;
		//	RibbonWidget::InformativeToolButton* view2;
		//	RibbonWidget::InformativeToolButton* view3;
		//};

		LayerButtons& getLayerButtons();
		InputButtons& getInputButtons();
		AnalysisButtons& getAnalysisButtons();
		ToolButtons& toolButtons();
		void setToolButtonActive(LayoutAnalyzer::Toolbox::Tool tool);
		//SettingsButtons& settingsButtons();
		//WorkButtons& workButtons();
		//EditButtons& editButtons();
		//ViewButtons& viewButtons();
	private:
		LayerButtons m_layerButtons;
		InputButtons m_inputButtons;
		AnalysisButtons m_analysisButtons;
		ToolButtons m_toolButtons;
		//SettingsButtons m_settingsButtons;
		//WorkButtons m_workButtons;
		//EditButtons m_editButtons;
		//ViewButtons m_viewButtons;
};