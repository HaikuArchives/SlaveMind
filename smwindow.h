#ifndef SMWINDOW_H
#define SMWINDOW_H

#include <Window.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include "smview.h"
//------------------------------------------------------------------------
class SMWindow : public BWindow
{
 private:
	SMView* SlaveView;
	BMenuBar* Menu;
	BMenu* SubMenu;

 public:
	SMWindow();
	virtual	bool QuitRequested();
	virtual void MessageReceived(BMessage* message);
};
//------------------------------------------------------------------------
#endif
