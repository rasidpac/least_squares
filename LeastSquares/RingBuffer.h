#pragma once
#include <iostream>
#include "stdafx.h"
using std::cout;
using std::cin;
using std::endl;
#define BUFFER_SIZE 128

template <typename T>
class RingBuffer
{
private:
	unsigned short length;
	unsigned short headIndex;
	unsigned short tailIndex;
	T buffer[BUFFER_SIZE];
public:
	RingBuffer();
	void push(T data);
	void print(void);
	unsigned short getLength(void);
	T getIndex(unsigned short);
};

template <typename T>
RingBuffer<T>::RingBuffer(void)
{
	length = 0;
	headIndex = BUFFER_SIZE-1;
	tailIndex = 0;
}

template <typename T>
void RingBuffer<T>::push(T data)
{
	headIndex = (headIndex + 1) % BUFFER_SIZE;
	if (headIndex == tailIndex && length > 0)	
		tailIndex = (tailIndex + 1) % BUFFER_SIZE;	
	else
		length++;
	buffer[headIndex] = data;
}

template <typename T>
void RingBuffer<T>::print(void)
{
	int index = tailIndex;
	for (int i = 0; i < length; i++)
	{
		cout << buffer[index] << " ";
		index = (index + 1) % BUFFER_SIZE;
	}
	cout << endl;
}

template <typename T>
unsigned short RingBuffer<T>::getLength(void)
{
	return length;
}

template <typename T>
T RingBuffer<T>::getIndex(unsigned short i)
{
	unsigned short j = (tailIndex + i) % length;
	return buffer[j];
}