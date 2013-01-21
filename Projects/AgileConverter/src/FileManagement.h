#ifndef FILEMANAGEMENT_H
#define FILEMANAGEMENT_H

#include <string>
#include <windows.h>
#include <commdlg.h>

using namespace std;

string openfilename(char *filter, HWND owner) 
{
	OPENFILENAMEA ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = owner;
	ofn.lpstrFilter = filter;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "";
	string fileNameStr;
	if ( GetOpenFileNameA(&ofn) )
		fileNameStr = fileName;
	return fileNameStr;
}
string savefilename(char *filter, HWND owner) 
{
	OPENFILENAMEA ofn;
	char fileName[MAX_PATH] = "";
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize   = sizeof(ofn);
	ofn.hwndOwner = owner;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrFilter = "Agile (*.agl)\0*.agl\0";   
	ofn.lpstrTitle = "Save File As";
	ofn.Flags = OFN_HIDEREADONLY; 
	ofn.lpstrDefExt = "txt";
	string fileNameStr;
	if ( GetSaveFileNameA(&ofn) )
		fileNameStr = fileName;
	return fileNameStr;
}

#endif