#include <Windows.h>
#include <stdlib.h>
#include <string>

int WinMain(
    HINSTANCE hInstance,		// 現在のインスタンスハンドル
    HINSTANCE hPrevInstance,	// 以前のインスタンスハンドル
    LPSTR lpCmdLine,			// コマンドライン
    int nCmdShow				// 表示状態
) {
    char first = lpCmdLine[0];
    if (first != '\0') {
        std::string str = lpCmdLine;
        std::string rep_str;
        std::string::size_type pos = str.find("\\");
        while (pos != std::string::npos) {
            switch (str[pos + 1]) {
            case 'n':
                rep_str = "\n"; break;
            case 't':
                rep_str = "\t"; break;
            case '\\':
                rep_str = "\\"; break;
            default:
                rep_str = ""; break;
            }
            str.replace(pos, 2, rep_str);
            pos = str.find("\\", rep_str.length());
        }
        MessageBox(NULL, str.c_str(), "確認", MB_OK);
    }
    else {
        MessageBox(NULL, "コマンドライン引数からダイアログに表示されます", "使用方法", MB_OK);
    }
}
