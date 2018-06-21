//허지수 3학년 B115233
#include "polyb.h"

istream& operator>>(istream& is, Polynomial &p)
{
	int noofterms;
	float coef;
	int exp;

	is >> noofterms; // .dat 파일로부터 항의 수를 입력

	for ( int i =0 ; i < noofterms ; i ++ )
	{
		is >> coef >> exp; // .dat 파일로부터 coef(계수)와 exp(지수) 입력
		p.NewTerm(coef, exp);	// p가 참조하는 객체를 .dat파일의 입력값으로 초기화
	}

	return is;
}

ostream& operator<<(ostream& os, Polynomial &p) //Polynomial을 출력하는 함수
{
	for( int i = 0 ; i < p.terms ; i ++ ) 
	{
		if( i == 0 && p.termArray[0].exp != 0) // 첫번째 항의 경우 && 지수가 0이 아닐 때
		{
			if( p.termArray[0].coef == 1) os << "x^" << p.termArray[0].exp;
			else if( p.termArray[0].coef == -1) os << "-x^" << p.termArray[0].exp;
			else os << p.termArray[0].coef << "x^" << p.termArray[0].exp;
		}
		else if( i == 0 && p.termArray[0].exp == 0) os << p.termArray[0].coef; // 첫번째 항의 경우 && 지수가 0 일 때
		
		if( i > 0 && p.termArray[i].exp != 0) // 첫번째 항이 아닌경우 && 지수가 0이 아닐 때
		{
			if(p.termArray[i].coef > 0) // 계수가 양수일 때...
			{
				if( p.termArray[i].coef == 1) os << " +x^" << p.termArray[i].exp;
				else	os << " +" << p.termArray[i].coef << "x^" << p.termArray[i].exp;
			}
			else if(p.termArray[i].coef < 0) /*와 계수가 음수일 때를 구분한 이유 : .dat파일로부터 Polynomial 객체를 초기화할 때 ...
							 양수 계수는 '+'부호를 생략한 채로 초기화 됨, 반면 음의 계수는 '-'부호를 포함한 채로 초기화 됨 */
			{
				if( p.termArray[i].coef == -1 ) os << " -x^" << p.termArray[i].exp;
				else	os << " " << p.termArray[i].coef << "x^" << p.termArray[i].exp;
			}
			else;	//혹시나 .dat파일로 부터 입력받을 0계수를 대비하기 위하여
		}
		else if( i > 0 && p.termArray[i].exp == 0) // 첫번째 항이 아닌경우 && 지수가 0 일 때
		{	
			if(p.termArray[i].coef > 0) os << " +" << p.termArray[i].coef;
			else if(p.termArray[i].coef < 0) os << " " << p.termArray[i].coef;
			else;
		}
	}
	cout << endl;
	return os;
}

Polynomial :: Polynomial() : capacity(4), terms(0)
{
	termArray = new Term[capacity];
}

void Polynomial :: NewTerm(const float theCoeff, const int theExp)
{
	if(terms == capacity)  // capacity가 꽉찼을 경우 늘리기 위한 장치
	{	
		capacity *= 2;
		Term * temp = new Term[capacity];
		copy(termArray, termArray + terms, temp);
		delete [] termArray;
		termArray = temp;
	}
	termArray[terms].coef = theCoeff; //객체의 Term형 멤버변수는 NewTerm함수의 인자값들로 초기화 된다. 
	termArray[terms].exp  = theExp;
	terms++;
}

Polynomial Polynomial :: operator+ (Polynomial& b)
{
	Polynomial c;

	int aPos = 0, bPos = 0;
	while( ( aPos<terms ) && ( bPos<b.terms ) ) // Polynomial형 객체들의'+'연산에서 c = a + b의 경우 ..... a 또는 b
	{								// 어느 한 객체라도 덧셈연산이 모두 처리 된경우 이 while문을 탈출한다.
		if(termArray[aPos].exp == b.termArray[bPos].exp)  // a와 b의 지수가 같은 경우의 덧셈연산 처리
		{
			float t = termArray[aPos].coef + b.termArray[bPos].coef;
			if(t) c.NewTerm(t, termArray[aPos].exp); // 이 if문은 더해서 계수가 0이 되는경우를 처리하기위한 장치이다.
			aPos++; bPos++;
		}
		
		else if(termArray[aPos].exp < b.termArray[bPos].exp) // a보다 b의 지수가 큰 경우의 덧셈연산 처리
		{
			c.NewTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
			bPos++;
		}
		
		else						// a가 b보다 지수가 큰 경우의 덧셈연산 처리
		{
			c.NewTerm(termArray[aPos].coef, termArray[aPos].exp);
			aPos++;
		}
	}

	for(;aPos < terms ; aPos++) // b가 다 처리된 경우 남은 a의 처리
		c.NewTerm(termArray[aPos].coef,termArray[aPos].exp);
	
	for(;bPos < b.terms ; bPos++) // a가 다 처리된 경우의 남은 b의 처리
		c.NewTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);

	return c;	
}

Polynomial Polynomial::operator*(Polynomial& b)
{
	Polynomial c;
	float temp_coef;
	int temp_exp;
	
	for(int aPos = 0 ; aPos < terms ; aPos++) // '분배법칙' 과 Polynomial 객체간의 '+'연산을 조합한 for 루프이다.
	{
		Polynomial temp_c; //(해석순서 3) 분배법칙의 한 마디가 끝나면 임시 Polynomial 객체를 다시 초기화시켜야한다. .... 가장 핵심부분 

		for(int bPos = 0 ; bPos < b.terms ; bPos++)
		{
			temp_coef = termArray[aPos].coef * b.termArray[bPos].coef;
			temp_exp = termArray[aPos].exp + b.termArray[bPos].exp;
			temp_c.NewTerm(temp_coef, temp_exp);		
		}	// (해석순서 1) a의 각 항에 대하여 b를 분배법칙하여 임시형 Polynomial 객체를 생성하였다.
		c = c + temp_c; // (해석순서 2) 임시 Polynomial 객체를 c객체와 덧셈연산을 한다. 가장중요한 것은 덧셈의 결과를 추 후 반환될 c에 저장하는 것이다.
		
	}
	
	return c;
}

/*
 
3x^5 + 2x^3 -4
x^8 -7x^5 -x^3 -3
3x^13 +2x^11 -21x^10 -21x^8 -2x^6 +19x^5 -2x^3 +12

*/
