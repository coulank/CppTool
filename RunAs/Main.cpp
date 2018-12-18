#include <Windows.h>
#include <stdlib.h>
#include <string>

HWND SimpleWindow(bool Visible = true, HINSTANCE hInstance = NULL) {
	char clsname[256] = "simple";
	LPSTR WINDOW_CLASS_NAME = clsname;
	HWND hWnd;
	WNDCLASS wc;
	/*ウィンドウクラスの登録*/
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = DefWindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = NULL;
	wc.hCursor = NULL;
	wc.hbrBackground = (HBRUSH)COLOR_BACKGROUND + 1;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WINDOW_CLASS_NAME;

	if (!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("ウィンドウクラスの作成に失敗しました"), NULL, MB_OK);
		return 0;
	}
	int Visible_opt = Visible ? WS_VISIBLE : 0;

	/*登録したクラスのウィンドウを生成*/
	hWnd = CreateWindow(
		WINDOW_CLASS_NAME, TEXT("Window Title"),
		WS_OVERLAPPEDWINDOW | Visible_opt,
		100, 100, 400, 300,
		NULL, NULL, hInstance, NULL
	);
	return hWnd;
}

// 特定の文字を隔てて左側を取得する、デフォルトで親ディレクトリ
// 戻り値は見つからなかったときは元の文字列が返される
std::string str_getLeft(std::string subject_str, std::string delim = "\\", bool LeftSearch = false) {
	auto i = std::string::npos;
	if (LeftSearch) {
		i = subject_str.find_first_of(delim);
	}
	else {
		i = subject_str.find_last_of(delim);
	}
	if (i == std::string::npos) {
		return subject_str;
	}
	else {
		return subject_str.substr(0, i);
	}
}

// 特定の文字を隔てて右側を取得する、デフォルトで拡張子、見つからなければ空文字列
std::string str_getRight(std::string subject_str, std::string delim = ".", bool LeftSearch = false) {
	auto i = std::string::npos;
	if (LeftSearch) {
		i = subject_str.find_first_of(delim);
	}
	else {
		i = subject_str.find_last_of(delim);
	}
	if (i == std::string::npos) {
		return "";
	}
	else {
		return subject_str.substr(++i, subject_str.size() - i);
	}
}

int WinMain(
	HINSTANCE hInstance, // 現在のインスタンスハンドル
	HINSTANCE hPrevInstance, // 以前のインスタンスハンドル
	LPSTR lpCmdLine, // コマンドライン
	int nCmdShow // 表示状態
) {
	bool hide_window = true;
	std::string run_app = "notepad";
	std::string run_open = lpCmdLine;
	if (run_open == "") run_open = "powershell";
	if (run_open != "") {
		std::string str_arg = str_getRight(run_open, "?");
		run_open = str_getLeft(run_open, "?", true);

		std::string ext_name = str_getRight(run_open, ".");
		bool registapp = (run_open.find("\\") == std::string::npos);
		if (ext_name == "" && registapp) ext_name = "exe";

		if (ext_name == "exe") {
			hide_window = false;
			run_app = run_open;
			run_open = str_arg;
		}
		else if (ext_name == "ps1") {
			run_app = "powershell";
		}
		int run_mode = SW_SHOWNORMAL;
		HWND hWnd = NULL;
		if (hide_window) {
			//hWnd = SimpleWindow(false);
			int run_mode = SW_HIDE;
		}
		ShellExecute(hWnd, "runas", run_app.c_str(), run_open.c_str(), NULL, run_mode);
		//if (hide_window) DestroyWindow(hWnd);
	}
	else {
		// ※デフォルトでPowerShellを実行するので、とりあえず下のは残しっぱのもの
		MessageBox(NULL, "コマンドライン引数のパスを管理者権限で実行する", "使用方法", MB_OK);
	}
}