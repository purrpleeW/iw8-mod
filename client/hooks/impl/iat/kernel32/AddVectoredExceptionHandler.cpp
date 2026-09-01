#include "common.hpp"
#include "hooks/hook.hpp"
#include "hooks/util/hook_util.hpp"

std::vector<std::uintptr_t> s_RegisteredExceptionHandlers{};

PVECTORED_EXCEPTION_HANDLER s_OriginalVectoredExceptionHandler = nullptr;
LONG WINAPI VectoredExceptionHandlerStub(EXCEPTION_POINTERS* inf) {
	if (inf->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT) {
		return s_OriginalVectoredExceptionHandler(inf);
	}

	return Client::Hook::Util::TopLevelExceptionFilter(inf);
}

template <>
PVOID WINAPI Client::Hook::Hooks::HK_AddVectoredExceptionHandler::hkCallback(ULONG first, PVECTORED_EXCEPTION_HANDLER handler) {
	// pro tip you cannot hook handlers oops
	/*
	auto res = AddVectoredExceptionHandler(first, handler);

	if (handler == nullptr || Common::Utility::NT::Library::GetByAddress(handler) != Common::Utility::NT::Library()) {
		return res;
	}
	
	std::uintptr_t handlerPtr = reinterpret_cast<std::uintptr_t>(handler);
	if (std::find(s_RegisteredExceptionHandlers.begin(), s_RegisteredExceptionHandlers.end(), handlerPtr) == s_RegisteredExceptionHandlers.end()) {
		(new Memory::MinHook(handler))->Hook(VectoredExceptionHandlerStub, &s_OriginalVectoredExceptionHandler);
		LOG("Game/AddVectoredExceptionHandler", DEBUG, "Patched vectored exception handler #{} @ 0x{:016X}",
			s_RegisteredExceptionHandlers.size() + 1, handlerPtr);
		s_RegisteredExceptionHandlers.push_back(handlerPtr);
	}

	return res;
	*/

	// what happens if we don't register the handler?
	// update: the game crashes
	/*
	return handler;
	*/

	return AddVectoredExceptionHandler(first, handler);
}
