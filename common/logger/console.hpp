#pragma once
#include "common_core.hpp"
#include "global/vector2.hpp"

namespace Common {
	class Console {
	public:
		HANDLE GetOutputHandle();
		DWORD GetConsoleMode();
		void SetConsoleMode(DWORD consoleMode);

		void SetCursorPos(Vector2<int> pos);
		Vector2<int> GetCursorPos();
		std::string GetTextAtPos(Vector2<int> pos, int length);

		static void InputThread();
		static void StartInputThread();

		void Write(std::string text);

		std::function<void(std::string)> m_OnInput{};
		std::mutex m_Mutex{};
	};

	inline Console g_Console{};
}
