#include "cpu.h"
#include "seq.h"
#include "smview.h"
#include <list> //list from Standard Template Library
#include <stdio.h>
#include <Application.h>
#include "main.h"
//---------------------------------------------------------------------------
typedef	list<Sequence>::iterator ITER;
//---------------------------------------------------------------------------
bool DropSequence(Sequence& S, SMView* V)
{
	for(int i=0; i<5; i++)
	{
		V->Balls[V->CurrentRow][i].Bitmap = V->AllBalls[S.GetColor(i)];
		V->Balls[V->CurrentRow][i].Color = S.GetColor(i);
		((SlaveMind*)be_app)->MainWindow->Lock();
		V->Invalidate();
		V->Window()->UpdateIfNeeded();
		((SlaveMind*)be_app)->MainWindow->Sync();
		((SlaveMind*)be_app)->MainWindow->Unlock();
		snooze(500000); //wait half a sec
	}


	((SlaveMind*)be_app)->MainWindow->Lock();
	bool correct = V->CheckSequence();
	V->Invalidate();
	V->Window()->UpdateIfNeeded();
	((SlaveMind*)be_app)->MainWindow->Sync();
	((SlaveMind*)be_app)->MainWindow->Unlock();

	if(correct)
		return true; //Computer wins !

	V->CurrentRow--;
	if(V->CurrentRow < 0)
		return true; //computer loses !

	return false;
}
//---------------------------------------------------------------------------
int32 CPUPlayer(void* data)
{
	SMView* V = (SMView*) data;
	
	//Build a list containing all combinations
	bool firstTry = true;
	bool buildList = false;
	list<Sequence> SlaveList;
	ITER start, runner, terminator;
	
	Sequence SequenceTest(0,1,2,3,4); //default test
	Sequence Seq(SequenceTest);

	while(V->CPU)
	{
		if(firstTry)
		{//Send the default test sequence
			firstTry = false;
			buildList = true; //build the list on next turn
			if(DropSequence(SequenceTest, V))
			{
				V->Playing = false; 
				V->CPU = false;
			}
				
			V->Answer.Check(SequenceTest, V->APins, V->BPins);
		}		

		else if(buildList)
		{//we build our initial possibilities list.
			do
			{
				if(Seq.Compatible(SequenceTest, V->APins, V->BPins))
					SlaveList.push_back(Seq);
				Seq++;
			}while(!(SequenceTest == Seq));
			buildList = false;
		}
		
		else
		{//try next guess
			start = SlaveList.begin();
			SequenceTest = (*start);

			if(DropSequence(SequenceTest, V))
			{
				V->Playing = false; 
				V->CPU = false;
			}

			V->Answer.Check(SequenceTest, V->APins, V->BPins);

			//remove the ones that don't fit previous answer
			runner = SlaveList.begin();
			while(runner != SlaveList.end())
			{
				Seq = (*runner);
				if(!Seq.Compatible(SequenceTest, V->APins, V->BPins))
				{	
					terminator = runner;					
					runner++;
					SlaveList.erase(terminator);
				}
				else runner++;
			}						
		}
	}

	return 0;
}
//---------------------------------------------------------------------------
