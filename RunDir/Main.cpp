#include <Windows.h>
#include <iostream>
#include <stdlib.h>
#include <locale.h>
#include <ShlObj.h>
#include <string>
#include <regex>
#include <vector>

using namespace std;
const int MAX_PATH2 = MAX_PATH + 1;
const int MAX_FOLDER = 2;

typedef basic_regex<char> regex;

class Frun {
	char Path[MAX_PATH2] = "\0";
	string Pathes[MAX_FOLDER];
	char drive[MAX_PATH2]
		, dir[MAX_PATH2]
		, fname[MAX_PATH2]
		, ext[MAX_PATH2];
	HWND hWnd;
	int folder_cnt = 0;
	const char* ptn_arg = "^([^\\\\]*)(.*?)\\\\?\\*?$";
	const char* ptn_prt = "^(.*?)[^\\\\]*$";
	const char* ptn_prt2 = "^[^\\\\]*(.*?)[^\\\\]*$";
	const char* ptn_chk = "^.*?([^\\\\]*.?[^\\\\]*)$";
	const char* ptn_max = "^.*?max.*?$";
	const char* ptn_min = "^.*?min.*?$";
	const char* ptn_normal = "^.*?normal.*?$|^.*?mid.*?$";
	const char* ptn_hidden = "^.*?hid[de].*?$";

	void PathJoin(char* FileName, char* Output) {
		_splitpath_s(Path, ext, MAX_PATH2, Output, MAX_PATH2, ext, MAX_PATH2, ext, MAX_PATH2);
		strcat_s(Output, MAX_PATH2, FileName);
	}
	bool SelectFolder() {
		BROWSEINFO        brows;
		ITEMIDLIST        *pIDlist;
		char buf[512];
		LPTSTR lpSelectPath = buf;
		//BROWSEINFO�\���̂̏�����
		memset(&brows, NULL, sizeof(brows));
		//BROWSEINFO�\���̂ɒl���Z�b�g
		brows.hwndOwner = hWnd;
		brows.pszDisplayName = lpSelectPath;
		brows.lpszTitle = "�t�H���_�I��";
		brows.lParam = (LPARAM)"\\";
		brows.pidlRoot = NULL;
		brows.ulFlags = 0;
		brows.lpfn = &BrowseCallbackProc;
		brows.iImage = (int)NULL;
		//�t�H���_�I���_�C�A���O�̕\��
		pIDlist = SHBrowseForFolder(&brows);
		if (pIDlist == NULL) {
			return false;
		}
		else {
			SHGetPathFromIDList(pIDlist, lpSelectPath);
			strcpy_s(Path, lpSelectPath);
			strcat_s(Path, "\\*");
			return true;
		}
	}
	static int CALLBACK BrowseCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData) {
		switch (uMsg)
		{
		case BFFM_INITIALIZED:
			//����������(�����\���t�H���_�w��)
			SendMessage(hWnd, BFFM_SETSELECTION, (WPARAM)TRUE, lpData);
			break;
		}
		return 0;
	}
	// �f�B���N�g������t�@�C�������K������
	void GetFile(WIN32_FIND_DATA &Getf) {	// Getf�͎Q�Ɠn���APath�̓|�C���^�n��
		smatch sm;
		regex re_chk(ptn_chk);
		regex re_max(ptn_max, regex_constants::icase);
		regex re_min(ptn_min, regex_constants::icase);
		regex re_nml(ptn_normal, regex_constants::icase);
		regex re_hid(ptn_hidden, regex_constants::icase);
		int window_style = 1;
		regex re(ptn_prt2);
		if (!regex_match(Pathes[folder_cnt], sm, re)) return;
		string RunPath = sm[1];
		RunPath += Getf.cFileName;
		string input = RunPath;
		if (regex_match(input, sm, re_chk)) input = sm[1];
		try {
			if (regex_match(input, sm, re_chk)) {
				input = sm[0];
			}
			if (regex_match(input, sm, re_hid)) {
				window_style = SW_HIDE;	// 0
			}
			else if (regex_match(input, sm, re_nml)) {
				window_style = SW_SHOWNORMAL;	// 1
			}
			else if (regex_match(input, sm, re_min)) {
				window_style = SW_SHOWMINIMIZED;	// 2
			}
			else if (regex_match(input, sm, re_max)) {
				window_style = SW_SHOWMAXIMIZED;	// 3
			}
			else {
				window_style = SW_SHOW;
			}
			//input = RunPath;
			//input += ": " + to_string(window_style);
			ShellExecute(hWnd, "open", RunPath.c_str(), NULL, NULL, window_style);
			Sleep(1);
		}
		catch (...) {}		// ��O�H�m��Ȃ��q�ł��ˁi�޷ެ��j
	}
	void DirChk(WIN32_FIND_DATA &Getf) {
		// �f�B���N�g�����ǂ����r�b�g���Z�ŋ��߂�
		if (!(Getf.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)) {
			GetFile(Getf);
		}
		else {
			string chkPath = Getf.cFileName;
			regex re("^\\.\\.?$");
			smatch sm;
			if (!regex_match(chkPath, sm, re)) {
				regex re(ptn_prt);
				if (regex_match(Pathes[folder_cnt], sm, re)) {
					string dpath = sm[1];
					dpath += Getf.cFileName;
					dpath += "\\*";
					folder_cnt++;
					if (folder_cnt < MAX_FOLDER) {
						Pathes[folder_cnt] = dpath;
						Open_File();
					}
					folder_cnt--;
				}
			}
		}
	}
	void Open_File() {
		//if (Path[0] == '*') Path[0] = '\0';
		static HANDLE h;
		vector<WIN32_FIND_DATA> found;
		WIN32_FIND_DATA find; // ������������find���|�C���^�^����Ȃ����A�\���̂ł���
		h = FindFirstFile(Pathes[folder_cnt].c_str(), &find); // �̂ŁA�|�C���^�n����&���K�v
		if (h != INVALID_HANDLE_VALUE) {
			found.push_back(find);
			while (1) {
				if (FindNextFile(h, &find)) {
					found.push_back(find);
				}
				else {
					FindClose(h);
					break;
				}
			}
		}
		for (WIN32_FIND_DATA &find_i : found) {
			DirChk(find_i);
		}
	}
public:
	~Frun() {}
	void main(HINSTANCE hWnd, LPSTR lpCmdLine) {
		char ThisPath[MAX_PATH];
		GetModuleFileName(NULL, ThisPath, MAX_PATH);
		_splitpath_s(ThisPath, drive, MAX_PATH2, dir, MAX_PATH2, fname, MAX_PATH2, ext, MAX_PATH2);	// �|�C���^�n��
		strcat_s(Path, MAX_PATH2, dir);
		if (lpCmdLine[0] != '\0') {
			string cmdstr = lpCmdLine, input;
			smatch sm, dm;
			regex re(ptn_arg);
			if (!regex_match(cmdstr, sm, re)) return;
			input = sm[1];
			regex dre("^.*:$");
			if (!regex_match(input, dm, dre))
			{
				cmdstr = sm[1];
			}
			else {
				cmdstr = "";
			}
			cmdstr += sm[2];
			cmdstr += "\\*";

			if (cmdstr[0] != '\\') {
				strcat_s(Path, MAX_PATH2, cmdstr.c_str());
			}
			else {
				strcpy_s(Path, cmdstr.c_str());
			}
			Pathes[0] = Path;
			Open_File();
		}
		else {
			bool jg = SelectFolder();
			Pathes[0] = Path;
			if (jg) Open_File();
			//MessageBox(NULL, Path, "", MB_OK);
			//MessageBox(NULL, "�w�肵���p�X�̃A�v����S�ċN������", "�������", MB_OK);
		}
	}
};

int WinMain(
	HINSTANCE hInstance,		// ���݂̃C���X�^���X�n���h��
	HINSTANCE hPrevInstance,	// �ȑO�̃C���X�^���X�n���h��
	LPSTR lpCmdLine,			// �R�}���h���C��
	int nCmdShow				// �\�����
) {
	Frun *mainst = new Frun();
	// ���̂Ƃ��ɓ����Ƀ����o�ϐ��̒�`���Ȃ����
	// ���̃����o�ϐ��̓N���X���폜����Ƃ��ɂ܂Ƃ߂ď������̂�
	// �f�X�g���N�^�ɓ����K�v���Ȃ��Ƃ��������b�g���L��悤��
	mainst->main(hInstance, lpCmdLine);
	delete mainst;
}
