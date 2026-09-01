#include "common_core.hpp"
#include "utility/registry.hpp"

namespace Common::Registry {
	LONG SetRegValue(HKEY hKeyIn, const char* path, const char* name, DWORD value) {
		LONG status;
		HKEY hKey;

		status = RegOpenKeyExA(hKeyIn, path, 0, KEY_ALL_ACCESS, &hKey);
		if (hKey != NULL) {
			if (status == ERROR_SUCCESS) {
				status = RegSetValueExA(hKey, name, 0, REG_DWORD, (BYTE*)&value, sizeof(DWORD));
				RegCloseKey(hKey);
			}
			else {
				HKEY createHKey;
				LONG setStatus = RegCreateKeyExA(hKey, name, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &createHKey, NULL);
				if (createHKey != NULL && setStatus == REG_CREATED_NEW_KEY) {
					return SetRegValue(hKeyIn, path, name, value);
				}
			}
		}
		return status;
	}

	LONG GetRegValue(HKEY hKeyIn, const char* path, const char* name, DWORD& nValue, DWORD nDefaultValue) {
		nValue = nDefaultValue;
		DWORD dwBufferSize(sizeof(DWORD));
		DWORD nResult(0);
		LONG status;
		HKEY hKey;

		status = RegOpenKeyExA(hKeyIn, path, 0, KEY_ALL_ACCESS, &hKey);
		if (hKey != NULL) {
			LONG nError = ::RegQueryValueExA(hKey, name, 0, NULL, reinterpret_cast<LPBYTE>(&nResult), &dwBufferSize);
			if (ERROR_SUCCESS == nError) {
				nValue = nResult;
			}
			return nError;
		}
		return status;
	}
}
