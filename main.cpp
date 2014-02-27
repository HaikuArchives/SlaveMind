#include "main.h"
//--------------------------------------------------------------
int main()
{
	SlaveMind* Prog;
	Prog = new SlaveMind;
	Prog->Run();
	return 0;
}
//--------------------------------------------------------------
SlaveMind::SlaveMind() : BApplication("application/x-vnd-SlaveMind")
{
	MainWindow = new SMWindow();
	MainWindow->Show();
	About = new AboutBox();
	About->Run();
}
//--------------------------------------------------------------
