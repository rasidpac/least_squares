// LeastSquares.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include "RingBuffer.h"
using namespace std;
#define W 0.003068	//omega = w = 2*pi/T, T = 2048

int main()
{
	ifstream in("..\\sample.csv");
	vector<vector<double>> data;
	RingBuffer<double> position;
	RingBuffer<double> signal;
	RingBuffer<double> calculated;
	RingBuffer<double> v;
	RingBuffer<double> cc;
	RingBuffer<double> ss;
	double vCurr=0.0, vPrev=0.0, xCurr=0.0, xPrev=0.0;
	double m11=0.0, m12=0.0, m21=0.0, w11=0.0, w12=0.0, w21=0.0, z1=0.0, z2=0.0, detM=0.0, a=0.0, c=0.0, b=0.0;


	int key;
	cout << "Enter a number to begin: ";
	cin >> key;
	cout << endl;

	if (in) {
		string line;
		while (getline(in, line)) {
			stringstream sep(line);
			string field;
			data.push_back(vector<double>());
			while (getline(sep, field, ',')) {
				data.back().push_back(stof(field));
			}
		}
	}
	int col = 0;
	for (auto row : data) {		
		for (auto field : row) {
			cout << field << ' ';
			if (col == 0)
			{
				position.push(field);
				xPrev = xCurr;
				xCurr = field;
			}
			else if (col == 1)
			{
				signal.push(field);
				vPrev = vCurr;
				vCurr = field;
			}
			col++;
		}
		col = 0;
		if (signal.getLength() < 2)
		{
			cout << vCurr << '\n';
			continue;
		}
		v.push(vCurr - vPrev);
		cc.push(cos(W*xCurr) - cos(W*xPrev));
		ss.push(sin(W*xPrev) - sin(W*xCurr));
		m11 = 0.0;
		m21 = 0.0;
		m12 = 0.0;
		z1 = 0.0;
		z2 = 0.0;
		for (unsigned short i = 0; i < ss.getLength(); i++)
		{
			m11 = m11 + ss.getIndex(i)*cc.getIndex(i);
			m12 = m12 + ss.getIndex(i)*ss.getIndex(i);
			m21 = m21 + cc.getIndex(i)*cc.getIndex(i);
			z1 = z1 + ss.getIndex(i)*v.getIndex(i);
			z2 = z2 + cc.getIndex(i)*v.getIndex(i);
		}
		detM = m11*m11 - m12*m21;
		if ( detM != 0.0 && ss.getLength()>1 )
		{
			w11 = m11 / detM;
			w12 = -m12 / detM;
			w21 = -m21 / detM;
			a = sqrt( pow(w11*z1 + w12*z2, 2) + pow(w21*z1 + w11*z2, 2) );
			c = atan2(w21*z1 + w11*z2, w11*z1 + w12*z2);
			b = 0.0;
			
			for (unsigned short i = 0; i < position.getLength(); i++)
			{
				b = b + signal.getIndex(i) - a*cos(W*position.getIndex(i) + c);
			}
			b = b / position.getLength();
			
			//b = signal.getIndex(v.getLength()-1) - a*cos(W*position.getIndex(position.getLength()-1) + c);
			calculated.push(b + a*cos(W*xCurr + c));
			cout << b + a*cos(W*xCurr + c);
		}
		else
		{
			calculated.push(vCurr);
			cout << vCurr;
		}
		cout << '\n';		
	}

	//position.print();
	//signal.print();
	//calculated.print();

	cin >> key;
    return 0;
}

