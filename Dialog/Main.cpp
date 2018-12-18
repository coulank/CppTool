#include <Windows.h>
#include <stdlib.h>
#include <string>

int WinMain(
    HINSTANCE hInstance,		// ���݂̃C���X�^���X�n���h��
    HINSTANCE hPrevInstance,	// �ȑO�̃C���X�^���X�n���h��
    LPSTR lpCmdLine,			// �R�}���h���C��
    int nCmdShow				// �\�����
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
        MessageBox(NULL, str.c_str(), "�m�F", MB_OK);
    }
    else {
        MessageBox(NULL, "�R�}���h���C����������_�C�A���O�ɕ\������܂�", "�g�p���@", MB_OK);
    }
}
