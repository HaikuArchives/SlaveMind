#include "seq.h"
//---------------------------------------------------------------------------
bool Sequence::Compatible(const Sequence& S, int A, int B)const
{
	//S sequence must pass testA and testB to stay as a possibility.
	return (TestA(S, A) && TestB(S, B));
}
//---------------------------------------------------------------------------
bool Sequence::TestA(const Sequence& S, int A)const
{
	//True if this sequence a exactly A colors at the same place as in S.
	int Result = 0;

	for(int i=0; i<5; i++)
   		if(color[i] == S.color[i])
      		Result++;

	return A==Result;
}
//---------------------------------------------------------------------------
bool Sequence::TestB(const Sequence& S, int B)const
{
	//True if this sequence has exactly B color not in the same place as in S.
   int Result = 0;

   	bool colorI[5] = {false, false, false, false, false};
	bool colorS[5] = {false, false, false, false, false};

   	//colors in the same place do not count
	for(int i=0; i<5; i++)
   		if(color[i] == S.color[i])
		{
      		colorI[i] = true;  //the 2 position are used
  			colorS[i] = true;
		}

	for(int i=0; i<5; i++)
		for(int j=0; j<5; j++)
	   		if(!colorI[i] && !colorS[j]) //not-used position
				if(color[i] == S.color[j])
				{
		      		colorI[i] = true;
      				colorS[j] = true;
					Result++; //same color in diffrent position
	            }

	return Result == B;
}
//---------------------------------------------------------------------------
Sequence::Sequence(int a, int b, int c, int d, int e)
{
	color[0] = a;
	color[1] = b;
	color[2] = c;
	color[3] = d;
	color[4] = e;
}
//---------------------------------------------------------------------------
Sequence::Sequence(const Sequence& S)
{//copy a sequence
	for(int i=0; i<5; i++)
	   	color[i] = S.color[i];
}
//---------------------------------------------------------------------------
void Sequence::Set(int a, int b, int c, int d, int e)
{
	color[0] = a;
	color[1] = b;
	color[2] = c;
	color[3] = d;
	color[4] = e;
}
//---------------------------------------------------------------------------
bool Sequence::operator==(const Sequence& S)const
{
	bool equal = true;
	for(int i=0; i<5; i++)
	   	if(color[i] != S.color[i])
    	  	equal = false;

	return equal;
}
//---------------------------------------------------------------------------
Sequence Sequence::operator++(int)
{
	Sequence old(*this);

	//incrementation
   	color[0]++;

	for(int i=0; i<4; i++)
    	if(color[i] == 9)
   		{
   			color[i] = 0;
			color[i+1]++;
   		}

	if(color[4] == 9)
		color[4] = 0;

   return old;
}
//---------------------------------------------------------------------------
void Sequence::Check(const Sequence& S, int& A, int& B)const
{
	A = 0; //black pins count
   	B = 0; //white pins count

    bool colorRep[5] = {false, false, false, false, false};
	bool colorSeq[5] = {false, false, false, false, false};

	for(int i=0; i<5; i++)
   		if(color[i] == S.color[i])
		{
      		colorRep[i] = true;  //the position are used
  			colorSeq[i] = true;
			A++; //same position
		}

	for(int i=0; i<5; i++)
		for(int j=0; j<5; j++)
	   		if(!colorRep[i] && !colorSeq[j]) //not-used position
         		if(color[i] == S.color[j])
				{
		      		colorRep[i] = true;
      				colorSeq[j] = true;
					if(i!=j) B++; //same color, different position
            	}
}
//---------------------------------------------------------------------------

