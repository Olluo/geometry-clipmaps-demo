/****************************************************************************
basic OpenGL demo modified from http://qt-project.org/doc/qt-5.0/qtgui/openglwindow.html
****************************************************************************/
#include "NGLScene.h"
#include <QtGui/QGuiApplication>
#include <iostream>

int main(int argc, char **argv)
{
	// if(argc <2 )
	// {
	// 	std::cerr <<"Usage ImageGrid [name]\n";
	// 	exit(EXIT_FAILURE);
	// }
	QGuiApplication app(argc, argv);
	// create an OpenGL format specifier
	QSurfaceFormat format;
	// set the number of samples for multisampling
	// will need to enable glEnable(GL_MULTISAMPLE); once we have a context
	format.setSamples(4);
#if defined(__APPLE__)
	// at present mac osx Mountain Lion only supports GL3.2
	// the new mavericks will have GL 4.x so can change
	format.setMajorVersion(4);
	format.setMinorVersion(1);
#else
	// with luck we have the latest GL version so set to this
	format.setMajorVersion(4);
	format.setMinorVersion(3);
#endif
	// now we are going to set to CoreProfile OpenGL so we can't use and old Immediate mode GL
	format.setProfile(QSurfaceFormat::CoreProfile);
	// now set the depth buffer to 24 bits
	format.setDepthBufferSize(24);
	// now we are going to create our scene window
	terraindeformer::NGLScene window("./img/MountainBig.bmp");
	// and set the OpenGL format
	window.setFormat(format);
	// we can now query the version to see if it worked
	std::cout << "Profile is " << format.majorVersion() << " " << format.minorVersion() << "\n";
	// set the window size
	window.resize(1024, 720);
	// and finally show
	window.show();

	return app.exec();
}
