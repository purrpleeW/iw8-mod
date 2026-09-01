#include "common.hpp"
#include "hooks/hook.hpp"

template <>
void Client::Hook::Hooks::HK_UI_ShowKeyboard::hkCallback(int controllerIndex, const char* title, const char* defaultText, int maxlen, bool verifyString,
	bool checkProfanity, bool allowEmpty, IW8::UI_KEYBOARD_TYPE type, void(__fastcall* callback)(IW8::LocalClientNum_t, IW8::UI_KEYBOARD_RESULT, const char*),
	bool openAsModal, bool useAutoAlign)
{
	_Unreferenced_parameter_(checkProfanity);
	return m_Original(controllerIndex, title, defaultText, maxlen, verifyString, false /* checkProfanity */, allowEmpty, type, callback,
		openAsModal, useAutoAlign);
}
