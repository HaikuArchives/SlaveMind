#ifndef MAIN_H
#define MAIN_H

#include <Application.h>
#include "smwindow.h"
#include "about.h"

class SlaveMind : public BApplication
{
 public:
 	SMWindow* MainWindow;
	AboutBox* About;
	SlaveMind();
};

#endif