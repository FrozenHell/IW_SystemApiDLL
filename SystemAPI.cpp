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
			TEXT("Ќедопустима€ ветка"),	\
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

	// преобразуем char* в FString
	bool CharToFStr(FString* exitString, char* inputString)
	{
		// узнаЄм длину строки, которую надо записать
		int stringLength = strlen(inputString);
		// если строка слишком длинна€
		if (stringLength >= exitString->ArrayMax)
		{	// прекращаем преобразование, не дожида€сь ошибок
			return false;
		}
		// выдел€ем пам€ть под массив Unicode символов
		wchar_t* wideString = new wchar_t[stringLength];
		// конвертируем в Unicode
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, inputString,
			stringLength + 1, wideString, stringLength + 1);
		// переносим строку в FString
		wcscpy_s(exitString->Data, exitString->ArrayMax, wideString);
		// устонавливаем конечной строке новую длину
		exitString->UpdateArrayNum();
		// очищаем выделенную пам€ть
		delete[] wideString;
		return true;
	}

	void FStrToChar(FString* inputString, char* exitString)
	{
		wchar_t wideString[1024];
		// конвертируем FString в wchar_t
		swprintf_s(wideString, 1024, L"%s",
			inputString->Data ? inputString->Data : L"");
		// преобразовываем двухбайтовую кодировку к однобайтовой
		WideCharToMultiByte(CP_OEMCP, WC_COMPOSITECHECK, wideString,
			inputString->ArrayNum - 1, exitString, inputString->ArrayNum - 1,
			NULL, NULL);
	}

	// ---------------------------------------------- сообщени€
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

	// ------------------------------------------------------------------- получить дату
	__declspec(dllexport) void GetFormattedTime(TimeString* Time)
	{
		char* formatString = new char[Time->Format.ArrayNum];
		// конвертируем Time->Format в char*, результат помещаем в formatString
		FStrToChar(&Time->Format, formatString);

		// получаем текущее врем€
		char stringTime[20];
		time_t currentTime = time(NULL);

		// преобразовываем currentTime в строку, помещаем результат в stringTime
		strftime(stringTime, 20, formatString, localtime(&currentTime));

		// конвертируем строку в FString
		CharToFStr(&Time->TimeStr, stringTime);
	}
}