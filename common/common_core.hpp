#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <d3d11.h>
#include <d3d12.h>
#include <DbgHelp.h>
#include <ErrorRep.h>
#include <ExDisp.h>
#include <intrin.h>
#include <MsHtmHst.h>
#include <MsHTML.h>
#include <Psapi.h>
#include <shellapi.h>
#include <TlHelp32.h>
#include <ws2tcpip.h>
#include <wrl/client.h>

#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <mutex>
#include <ostream>
#include <queue>
#include <random>
#include <ranges>
#include <regex>
#include <string>
#include <thread>
#include <vector>

#include <dxgi1_6.h>

#pragma intrinsic(_ReturnAddress)

#pragma comment(lib, "DbgHelp.lib")
#pragma comment(lib, "Faultrep.lib")
#pragma comment(lib, "Ws2_32.lib")

#define GIT_DESCRIBE "0000000"
#define GIT_DIRTY 1
#define GIT_HASH "0000000000000000000000000000000000000000"
#define GIT_TAG ""
#define GIT_BRANCH "unknown"
#include "version.hpp"

#define CLASS_GETTER(field) \
	auto& Get##field##() { \
		return this->m_##field##; \
	}

#define CLASS_SETTER(field, type) \
	auto& Set##field##(type field) { \
		this->m_##field = field; \
		return *this; \
	}

#define CLASS_GETTER_SETTER(field, type) \
	CLASS_GETTER(field) \
	CLASS_SETTER(field, type)
