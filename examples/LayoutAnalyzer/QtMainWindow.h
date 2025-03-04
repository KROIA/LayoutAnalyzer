#pragma once

#include <QMainWindow>
#include <QPushButton>
#include "ui_QtMainWindow.h"
#include "LayoutAnalyzer.h"
#include "RibbonImpl.h"

class QtMainWindow : public QMainWindow
{
	Q_OBJECT
	struct LayerData
	{
		LayoutAnalyzer::PrintLayer* layer;
		QPushButton* button;
	};
public:
	QtMainWindow(QWidget *parent = nullptr);
	~QtMainWindow();

private:
	void setupScene();
	void selectLayer(size_t index);
	LayerData& getSelectedLayer();



	Ui::QtMainWindow ui;
	RibbonImpl* m_ribbon;

	QSFML::Scene* m_scene;

	
	std::vector<LayerData> m_layers;
	size_t m_selectedLayer = 0;
	QSpacerItem* m_spacer;
};
