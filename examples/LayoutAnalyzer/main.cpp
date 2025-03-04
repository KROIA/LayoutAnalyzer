#include <QApplication>
#include <iostream>
#include "QtMainWindow.h"



int main(int argc, char* argv[])
{
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
	QApplication app(argc, argv);

	Log::UI::NativeConsoleView::createStaticInstance();
	Log::UI::NativeConsoleView::getStaticInstance()->show();

	QtMainWindow window;
	window.show();

	int ret = app.exec();

	LayoutAnalyzer::Profiler::stop((std::string(LayoutAnalyzer::LibraryInfo::name) + ".prof").c_str());
	return ret;
}