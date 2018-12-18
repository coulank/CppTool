#pragma once
#include <Windows.h>
#include <iostream>

int StrCount(char* str) {
	int i = 0;
	while (str[i++] != '\0');
	return i;
}

char* ToNum(int Num, char* OutStr) {
	const char* NumStr = "0123456789";
	int ModNum = 0, tmpNum = Num, i, f;
	for (f = 0; f++; tmpNum > 0) tmpNum /= 10;
	for (i = ++f; i--; i >= -1) {
		ModNum = Num % 10;
		OutStr[i] = NumStr[ModNum];
		Num = (Num - ModNum) / 10;
		if (Num == 0) break;
	}
	return OutStr;
}

char* replace_str(char* OutStr, const char* search, const char* replacestr) {
	int i = -1;
	while (OutStr[++i] != '\n') {

	}
	return OutStr;
}

char* escape_str(char* OutStr) {
	int i = -1, step;
	char jadge;
	while (OutStr[++i] != '\0') {
		if (OutStr[i] == '\\') {
			OutStr[i] = '\0';
			step = 1;
			jadge = OutStr[i + 1];
			switch (jadge)
			{
			case 'n':
				OutStr[i + 1] = '\n'; break;
			case 't':
				OutStr[i + 1] = '\t'; break;
			case 'a':
				OutStr[i + 1] = '\a'; break;
			default:
				step = 2; break;
			}
			strcat_s(OutStr, sizeof(OutStr) + 1, OutStr + (i + step));
			--i;
		}
	}
	return OutStr;
}
