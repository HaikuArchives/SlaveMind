#include "smview.h"
#include <TranslationUtils.h>
#include <stdlib.h>
#include "convert.h"
#include <stdio.h>
//----------------------------------------------------------------------
SMView::SMView() : BView(BRect(0, 20, 160, 268), "Board", B_FOLLOW_NONE, B_WILL_DRAW)
{
	srand((long) system_time()); //randomize
	Background = BTranslationUtils::GetBitmap("bmp/board.jpg");

	//The view is not automatically erased before updating
	SetViewColor(B_TRANSPARENT_32_BIT); 

	//Offscreen bitmap to avoid flickering
	OffscreenBitmap  = new BBitmap(BRect(0, 0, 159, 248), B_RGB_32_BIT, true); 
	OffscreenView = new BView(BRect(0, 0, 159, 248), "", B_FOLLOW_NONE, (uint32)NULL); //our canvas
	OffscreenView->SetDrawingMode(B_OP_ALPHA);
	OffscreenView->SetBlendingMode(B_PIXEL_ALPHA, B_ALPHA_OVERLAY);
	OffscreenView->SetViewColor(B_TRANSPARENT_32_BIT);
	OffscreenBitmap->AddChild(OffscreenView);

	//Load the balls...
	AllBalls[0] = BTranslationUtils::GetBitmap("bmp/red.bmp");
	AllBalls[1] = BTranslationUtils::GetBitmap("bmp/yellow.bmp");
	AllBalls[2] = BTranslationUtils::GetBitmap("bmp/blue.bmp");
	AllBalls[3] = BTranslationUtils::GetBitmap("bmp/green.bmp");
	AllBalls[4] = BTranslationUtils::GetBitmap("bmp/purple.bmp");
	AllBalls[5] = BTranslationUtils::GetBitmap("bmp/orange.bmp");
	AllBalls[6] = BTranslationUtils::GetBitmap("bmp/grey.bmp");
	AllBalls[7] = BTranslationUtils::GetBitmap("bmp/black.bmp");
	AllBalls[8] = BTranslationUtils::GetBitmap("bmp/white.bmp");

	//And make them transparents
	for(int i=0; i<9; i++)
		MakeTransparent(AllBalls[i]);

	CPU = false;
		
	Ini();
}
//----------------------------------------------------------------------
void SMView::Restart()
{//Stop the CPU (if needed) and restart a game
	CPU=false;
	Playing=false;
	status_t* exitvalue=NULL;
	wait_for_thread(CPUId, exitvalue);
	Ini();
}
//----------------------------------------------------------------------
void SMView::Ini()
{//Start a new game
	Playing = true;
	CurrentRow = 13;

	for(int i=0; i<14; i++)
		for(int j=0; j<5; j++)
		{
			Balls[i][j].Color = -1;
			Balls[i][j].Bitmap = NULL;
			Pins[i][j] = 0;
		}

	int mysteryColor;
	for(int j=0; j<5; j++)
	{
		mysteryColor = rand() % 9;
		Mystery[j].Color = mysteryColor;
		Mystery[j].Bitmap = AllBalls[mysteryColor];
	}
	Answer.Set(Mystery[0].Color, Mystery[1].Color, Mystery[2].Color, Mystery[3].Color, Mystery[4].Color);
	Invalidate();
}
//----------------------------------------------------------------------
void SMView::Draw(BRect where) 
{
	OffscreenBitmap->Lock(); //protect the OffscreenBitmap against intruders
	RedrawBackground(OffscreenView); //draw the background offscreen

	//draw all balls that intersect where
	for(int i=0; i<14; i++)
		for(int j=0; j<5; j++)
		{
			if(Balls[i][j].Color != -1)
				OffscreenView->DrawBitmap(Balls[i][j].Bitmap, BRect(j*16+8, i*16+1, j*16+24, i*16+17));
		}
	
	//draw all pins
	for(int i=0; i<14; i++)
		for(int j=0; j<5; j++)
		{
			if(Pins[i][j] == 1) //white pin
			{
				OffscreenView->SetHighColor(255, 255, 255, 255);
				OffscreenView->FillEllipse(BRect(j*8+88, i*16+1, j*8+96, i*16+9));
			}
			
			else if(Pins[i][j] == 2) //black pin
			{
				OffscreenView->SetHighColor(0, 0, 0, 255);
				OffscreenView->FillEllipse(BRect(j*8+88, i*16+1, j*8+96, i*16+9));
			}
		}

	if(!Playing)
	{//player gived up, show the hidden balls
		int offset = 0;
		for(int j=0; j<5; j++)
		{
			OffscreenView->DrawBitmap(Mystery[j].Bitmap, BRect(offset + 8, 232, offset + 24, 248));
			offset += 16;
		}
	}

	//Draw the invalidated portion of the offscreen bitmap into the onscreen view
	OffscreenView->Sync(); //Synchronize offscreen view with onscreen view
	DrawBitmap(OffscreenBitmap, where, where); //Copy the updated part to screen
	OffscreenBitmap->Unlock(); //Unprotect the Offscreen bitmap
}
////----------------------------------------------------------------------
void SMView::RedrawBackground(BView* aView)
{//Redraw the entire background on 'aView'
	aView->DrawBitmap(Background, BRect(0, 0, 159, 248));
}
////----------------------------------------------------------------------
void SMView::MouseDown(BPoint where)
{//clicking somewhere on the board
	if(CPU || !Playing) return; //no effect
	
	int32 buttons;
	Window()->CurrentMessage()->FindInt32("buttons", &buttons);

	int n;
	//Check what color we are dragging
	if(BRect(136, 81, 152, 225).Contains(where))
	{//dragging a color from the 'palette'
		if(BRect(136, 81, 152, 97).Contains(where)) n = 0;
		else if(BRect(136, 97, 152, 113).Contains(where)) n = 1;
		else if(BRect(136, 113, 152, 129).Contains(where)) n = 2;
		else if(BRect(136, 129, 152, 145).Contains(where)) n = 3;
		else if(BRect(136, 145, 152, 161).Contains(where)) n = 4;
		else if(BRect(136, 161, 152, 177).Contains(where)) n = 5;
		else if(BRect(136, 177, 152, 193).Contains(where)) n = 6;
		else if(BRect(136, 193, 152, 209).Contains(where)) n = 7;
		else if(BRect(136, 209, 152, 225).Contains(where)) n = 8;
		else return; //not supposed to happen
	}
	else return;

	BPoint P;
	uint32 mod;
	while(true)
	{
		GetMouse(&P, &mod);
		if(!mod) //button released
		{Window()->Activate(true); return;}
		
		if(P != where) break; //start to drag
		snooze(40000); //wait a bit
	}

	BMessage Message(SLAVEMINDCOLOR);
	Message.AddInt32("SlaveMind:color", n);
	BBitmap* bmp = new BBitmap(AllBalls[n]);
	MakeSemiTransparent(bmp);
	DragMessage(&Message, bmp, B_OP_ALPHA, BPoint(10,10), Window());
}
//--------------------------------------------------------------------------------
void SMView::Drop(int c)
{
	if(CPU) return; //no effect

	BPoint P;
	uint32 mod;
	GetMouse(&P, &mod);
	int row = ((int)P.y - 1) / 16;
	int col = ((int)P.x - 8) / 16;

	if(row == CurrentRow && col < 5 && col >= 0)
	{
		Balls[row][col].Bitmap = AllBalls[c];
		Balls[row][col].Color = c;

		bool EndLine = true;
		for(int i=0; i<5; i++)
			if(Balls[CurrentRow][i].Color == -1)
				EndLine = false;
		
		if(EndLine) 
		{
			if(CheckSequence())
				Win();
			else
			{
				CurrentRow--;
				if(CurrentRow < 0) 
					GiveUp();
			}
		}
		Invalidate();
	}

	Window()->UpdateIfNeeded();
}
//--------------------------------------------------------------------------------
void SMView::GiveUp()
{
	if(CPU) return; //no effect
	Playing = false;
	Invalidate();
}
//--------------------------------------------------------------------------------
void SMView::Win()
{
	Playing = false;
	Invalidate();
}
//--------------------------------------------------------------------------------
bool SMView::CheckSequence()
{
	Sequence Guess(Balls[CurrentRow][0].Color, Balls[CurrentRow][1].Color, Balls[CurrentRow][2].Color,
					Balls[CurrentRow][3].Color, Balls[CurrentRow][4].Color);
	
	int A, B;
	Answer.Check(Guess, A, B);
	APins = A;
	BPins = B;
	int currentPin=0;

	while(A!=0)
	{//black pins
		Pins[CurrentRow][currentPin] = 2;
		currentPin++;
		A--;
	}

	while(B!=0)
	{//white pins
		Pins[CurrentRow][currentPin] = 1;
		currentPin++;
		B--;
	}
	
	while(currentPin != 5)
	{//no pins
		Pins[CurrentRow][currentPin] = 0;
		currentPin++;
	}

	return APins == 5;
}
//--------------------------------------------------------------------------------
void SMView::Quit()
{
	if(CPU)
	{
		CPU=false;
		status_t* exitvalue=NULL;
		wait_for_thread(CPUId, exitvalue);
	}
}
//--------------------------------------------------------------------------------
void SMView::ToggleCPU()
{
	if(!CPU)
	{//start CPU
		Ini();
		CPU = true;
		CPUId = spawn_thread(CPUPlayer, "CPU Player", B_NORMAL_PRIORITY, (void *) this);
		resume_thread(CPUId);
	}
}
//--------------------------------------------------------------------------------
