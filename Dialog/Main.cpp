#include "Main.h"

int WinMain(
	HINSTANCE hInstance,		// 現在のインスタンスハンドル
	HINSTANCE hPrevInstance,	// 以前のインスタンスハンドル
	LPSTR lpCmdLine,			// コマンドライン
	int nCmdShow				// 表示状態
) {
	char* str = lpCmdLine;
	int count = StrCount(lpCmdLine);
	if (str[0] != '\0') {
		MessageBox(NULL, escape_str(++str), "確認", MB_OK);
	}
	else {
		MessageBox(NULL, "コマンドライン引数から、"
			"最初の一文字を除いてダイアログに表示されます", "使用方法", MB_OK);
	}
}
