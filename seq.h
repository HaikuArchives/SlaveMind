#ifndef SEQ_H
#define SEQ_H
//---------------------------------------------------------------------------
class Sequence
{
 private:
	int color[5];
	bool TestA(const Sequence& S, int A)const;
   	bool TestB(const Sequence& S, int B)const;

 public:
	Sequence() {};
	Sequence(int, int, int, int, int);
	Sequence(const Sequence& S);
	void Set(int, int, int, int, int);
	bool Compatible(const Sequence& S, int A, int B)const;
	void Check(const Sequence& S, int& A, int& B)const;
	bool operator==(const Sequence& S)const;
    Sequence operator++(int);
	int GetColor(int i) {return color[i];}
};
//---------------------------------------------------------------------------
#endif
