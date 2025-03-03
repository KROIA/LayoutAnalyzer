#pragma once

#include "RibbonWidget.h"

class RibbonImpl : public RibbonWidget::Ribbon
{
	Q_OBJECT
	public:
		RibbonImpl(QWidget* parent = nullptr);


	
		struct SettingsButtons
		{
			RibbonWidget::RibbonButton* settings1;
			RibbonWidget::RibbonButton* settingsOK;
			RibbonWidget::RibbonButton* settingsCancel;
			RibbonWidget::RibbonButton* settingsSave;
		};

		struct WorkButtons
		{
			RibbonWidget::InformativeToolButton* open;
			RibbonWidget::InformativeToolButton* save;
		};

		struct EditButtons
		{
			RibbonWidget::RibbonButton* undo;
			RibbonWidget::RibbonButton* redo;
		};

		struct ViewButtons
		{
			RibbonWidget::InformativeToolButton* view1;
			RibbonWidget::InformativeToolButton* view2;
			RibbonWidget::InformativeToolButton* view3;
		};


		SettingsButtons& settingsButtons();
		WorkButtons& workButtons();
		EditButtons& editButtons();
		ViewButtons& viewButtons();
	private:
		SettingsButtons m_settingsButtons;
		WorkButtons m_workButtons;
		EditButtons m_editButtons;
		ViewButtons m_viewButtons;
};