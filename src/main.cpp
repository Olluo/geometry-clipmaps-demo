/**
 * @file main.cpp
 * @author Ollie Nicholls
 * @brief Main window application based on 
 * http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include <iostream>

#include <QtGui/QGuiApplication>

#include "NGLScene.h"

int main(int argc, char **argv)
{
	if(argc <2 )
	{
		std::cerr <<"Usage: GeoClipmapDemo.exe <heightmap_file>\n";
		exit(EXIT_FAILURE);
	}

	QGuiApplication app(argc, argv);
	QSurfaceFormat format;
	
	format.setSamples(4);
	format.setMajorVersion(4);
	format.setMinorVersion(3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(24);

	// geoclipmap::NGLScene window("./img/grand_canyon.png");
	// geoclipmap::NGLScene window("./img/poole_harbour.png");
	// geoclipmap::NGLScene window("./img/cheddar.png");
	// geoclipmap::NGLScene window("./img/ben_nevis.png");
	geoclipmap::NGLScene window(argv[1]);
	
	window.setFormat(format);

	std::cout << "Profile is " << format.majorVersion() << " " << format.minorVersion() << "\n";

	window.resize(1024, 720);
	window.show();

	return app.exec();
}
