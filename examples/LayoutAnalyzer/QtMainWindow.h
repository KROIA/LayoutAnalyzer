#pragma once

#include <QMainWindow>
#include "ui_QtMainWindow.h"
#include "LayoutAnalyzer.h"
#include "RibbonImpl.h"

class QtMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QtMainWindow(QWidget *parent = nullptr);
	~QtMainWindow();

private:
	void setupScene();

	Ui::QtMainWindow ui;
	RibbonImpl* m_ribbon;

	QSFML::Scene* m_scene;
	LayoutAnalyzer::PrintLayer* layer;
};
