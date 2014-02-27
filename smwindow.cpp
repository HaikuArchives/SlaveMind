#include "smwindow.h"
#include "enum.h"
#include <Application.h>
#include "main.h"
//-------------------------------------------------------------------
SMWindow::SMWindow()
	: BWindow(BRect(30, 30, 189, 298), "SlaveMind v1.2", B_TITLED_WINDOW, B_NOT_RESIZABLE | B_NOT_ZOOMABLE)
{
	SlaveView = new SMView();
	AddChild(SlaveView); 

	//Load the drop-down list
	Menu = new BMenuBar(BRect(0, 0, 160, 10), "MainMenu");
	SubMenu = new BMenu("Menu");
	SubMenu->AddItem(new BMenuItem("New Game", new BMessage(RESTART)));
	SubMenu->AddItem(new BMenuItem("Give Up", new BMessage(GIVEUP)));
	SubMenu->AddItem(new BMenuItem("CPU Play", new BMessage(CPUPLAY)));
	SubMenu->AddItem(new BMenuItem("About...", new BMessage(ABOUT)));
	SubMenu->AddItem(new BMenuItem("Quit", new BMessage(QUIT)));
	Menu->AddItem(SubMenu);
	AddChild(Menu);
}
//-------------------------------------------------------------------
bool SMWindow::QuitRequested()
{
	SlaveView->Quit(); //stop the CPU player thread if running
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
//-------------------------------------------------------------------
void SMWindow::MessageReceived(BMessage* message)
{
 	switch (message->what)
 	{
   		case SLAVEMINDCOLOR: SlaveView->Drop(message->FindInt32("SlaveMind:color"));break;
  		case GIVEUP: SlaveView->GiveUp();break;
		case RESTART: SlaveView->Restart();break;
		case CPUPLAY: SlaveView->ToggleCPU();break;
		case ABOUT: ((SlaveMind*)be_app)->About->MessageReceived(message);break;
		case QUIT: QuitRequested();break;
   		default:BWindow::MessageReceived(message);break;
	}
}
//-------------------------------------------------------------------
