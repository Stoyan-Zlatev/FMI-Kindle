#pragma once
#include "MyString.h"

bool isPrefix(const MyString& text, const MyString& prefix);

bool isDigit(char element);

int parseCharToInt(char element);

int parseStringToInt(const MyString& data);

size_t getCommandLength(const MyString& text); 

void getField1Data(size_t startIndex, const MyString& text, MyString& field1);

void getField2Data(size_t startIndex, const MyString& text, MyString& title, MyString& data);