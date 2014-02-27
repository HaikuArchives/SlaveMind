#ifndef SMVIEW_H
#define SMVIEW_H
#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <Message.h>
#include "enum.h"
#include "seq.h"
#include "cpu.h"
//---------------------------------------------------------------------
struct SMColor
{
	BBitmap* Bitmap;
	int Color;
};
//---------------------------------------------------------------------
class SMView : public BView
{
 private:
	BBitmap* Background; //the board background
	BBitmap* OffscreenBitmap; //to avoid flicker while drawing
	BView* OffscreenView; //used by the offscreen bitmap
	BBitmap* AllBalls[9]; //all the possible color

	BPoint DropPoint;

	//matrix of balls and pins
	int Pins[14][5]; //(0=?) (1=same color, different position) (2=perfect)
	SMColor Balls[14][5]; //the try

	SMColor Mystery[5]; //concealed balls
	Sequence Answer;
	int CurrentRow;
	int APins;
	int BPins;

	bool CPU; //is the cpu playing ?
	bool Playing; //if false, we show the answer
	friend int32 CPUPlayer(void* data);
	friend bool DropSequence(Sequence& S, SMView* V);
	thread_id CPUId;

	void RedrawBackground(BView*); //redraw the entire background
	void Ini(); //Initialisation before the game begins
	bool CheckSequence(); //return true if correct sequence found
	
 public:
	SMView(); 
	virtual void Draw(BRect where);
	virtual void MouseDown(BPoint where);
	void Drop(int c);
	void Win();
	void GiveUp();
	void Restart();
	void ToggleCPU();
	void Quit();
};

#endif