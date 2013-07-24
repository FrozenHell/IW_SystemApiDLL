// Messages.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <assert.h>

#define DEBUG

#if defined(DEBUG)
	#define WARN(message)				\
	{									\
		MessageBox(						\
			NULL,						\
			message,					\
			TEXT("������������ �����"),	\
			MB_OK | MB_ICONERROR		\
		);								\
		exit(EXIT_FAILURE);				\
	}

	#define WARN_IF(condition, message)		\
	{										\
		if (condition)						\
		{									\
			MessageBox(						\
				NULL,						\
				TEXT(message),				\
				TEXT(#condition),			\
				MB_OK | MB_ICONERROR		\
			);								\
			exit(EXIT_FAILURE);				\
		}									\
	}
#else
	#define WARN(message)
	#define WARN_IF(condition, message)
#endif


extern "C"
{
	struct FString
	{
		wchar_t* Data;
		int ArrayNum;
		int ArrayMax;

		void UpdateArrayNum()
		{
			ArrayNum = wcslen(Data)+1;
			assert(ArrayNum <= ArrayMax);
		}
	};

	struct Message1
	{
		FString Title;
		FString Text;		
	};

	struct TimeString
	{
		FString TimeStr;
		FString Format;
	};

	// ����������� char* � FString
	bool CharToFStr(FString* exitString, char* inputString)
	{
		// ����� ����� ������, ������� ���� ��������
		int stringLength = strlen(inputString);
		// ���� ������ ������� �������
		if (stringLength >= exitString->ArrayMax)
		{	// ���������� ��������������, �� ��������� ������
			return false;
		}
		// �������� ������ ��� ������ Unicode ��������
		wchar_t* wideString = new wchar_t[stringLength];
		// ������������ � Unicode
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, inputString,
			stringLength + 1, wideString, stringLength + 1);
		// ��������� ������ � FString
		wcscpy_s(exitString->Data, exitString->ArrayMax, wideString);
		// ������������� �������� ������ ����� �����
		exitString->UpdateArrayNum();
		// ������� ���������� ������
		delete[] wideString;
		return true;
	}

	void FStrToChar(FString* inputString, char* exitString)
	{
		wchar_t wideString[1024];
		// ������������ FString � wchar_t
		swprintf_s(wideString, 1024, L"%s",
			inputString->Data ? inputString->Data : L"");
		// ��������������� ������������ ��������� � ������������
		WideCharToMultiByte(CP_OEMCP, WC_COMPOSITECHECK, wideString,
			inputString->ArrayNum - 1, exitString, inputString->ArrayNum - 1,
			NULL, NULL);
	}

	// ---------------------------------------------- ���������
	__declspec(dllexport) int ShowOKMessage(Message1* Mess)
	{
		wchar_t temp[1024];
		swprintf_s(temp, 1024, L"%s",
			Mess->Text.Data ? Mess->Text.Data : L"");
		wchar_t temp2[1024];
		swprintf_s(temp2, 1024, L"%s",
			Mess->Title.Data ? Mess->Title.Data : L"");
		return MessageBox(0, temp, temp2, MB_OK);
	}

	__declspec(dllexport) int ShowYESNOMessage(Message1* Mess)
	{
		wchar_t temp[1024];
		swprintf_s(temp, 1024, L"%s",
			Mess->Text.Data ? Mess->Text.Data : L"");
		wchar_t temp2[1024];
		swprintf_s(temp2, 1024, L"%s",
			Mess->Title.Data ? Mess->Title.Data : L"");
		return MessageBox(0, temp, temp2, MB_YESNO);
	}

	__declspec(dllexport) int ShowYESNOCANCELMessage(Message1* Mess)
	{
		wchar_t temp[1024];
		swprintf_s(temp, 1024, L"%s",
			Mess->Text.Data ? Mess->Text.Data : L"");
		wchar_t temp2[1024];
		swprintf_s(temp2, 1024, L"%s",
			Mess->Title.Data ? Mess->Title.Data : L"");
		return MessageBox(0, temp, temp2, MB_YESNOCANCEL);
	}

	__declspec(dllexport) int ShowABORTRETRYIGNOREMessage(Message1* Mess)
	{
		wchar_t temp[1024];
		swprintf_s(temp, 1024, L"%s",
			Mess->Text.Data ? Mess->Text.Data : L"");
		wchar_t temp2[1024];
		swprintf_s(temp2, 1024, L"%s",
			Mess->Title.Data ? Mess->Title.Data : L"");
		return MessageBox(0, temp, temp2, MB_ABORTRETRYIGNORE);
		
	}

	// ------------------------------------------------------------------- �������� ����
	__declspec(dllexport) void GetFormattedTime(TimeString* Time)
	{
		char* formatString = new char[Time->Format.ArrayNum];
		// ������������ Time->Format � char*, ��������� �������� � formatString
		FStrToChar(&Time->Format, formatString);

		// �������� ������� �����
		char stringTime[20];
		time_t currentTime = time(NULL);

		// ��������������� currentTime � ������, �������� ��������� � stringTime
		strftime(stringTime, 20, formatString, localtime(&currentTime));

		// ������������ ������ � FString
		CharToFStr(&Time->TimeStr, stringTime);
	}
}