#include "Main.h"

int WinMain(
	HINSTANCE hInstance,		// ���݂̃C���X�^���X�n���h��
	HINSTANCE hPrevInstance,	// �ȑO�̃C���X�^���X�n���h��
	LPSTR lpCmdLine,			// �R�}���h���C��
	int nCmdShow				// �\�����
) {
	char* str = lpCmdLine;
	int count = StrCount(lpCmdLine);
	if (str[0] != '\0') {
		MessageBox(NULL, escape_str(++str), "�m�F", MB_OK);
	}
	else {
		MessageBox(NULL, "�R�}���h���C����������A"
			"�ŏ��̈ꕶ���������ă_�C�A���O�ɕ\������܂�", "�g�p���@", MB_OK);
	}
}
