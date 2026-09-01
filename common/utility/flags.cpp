#include "common_core.hpp"
#include "utility/flags.hpp"
#include "utility/strings.hpp"

namespace Common::Utility::Flags {
	void ParseFlags(std::vector<std::string>& flags) {
		int numArgs;
		auto* const argv = CommandLineToArgvW(GetCommandLineW(), &numArgs);

		flags.clear();

		if (argv) {
			for (auto i = 0; i < numArgs; ++i) {
				std::wstring wideFlag(argv[i]);
				if (wideFlag[0] == L'-') {
					wideFlag.erase(wideFlag.begin());
					const auto flag = Strings::Convert(wideFlag);
					flags.emplace_back(Strings::ToLower(flag));
				}
			}

			LocalFree(argv);
		}
	}

	bool HasFlag(const std::string& flag) {
		static auto parsed = false;
		static std::vector<std::string> enabledFlags;

		if (!parsed) {
			ParseFlags(enabledFlags);
			parsed = true;
		}

		return std::ranges::any_of(enabledFlags.cbegin(), enabledFlags.cend(), [flag](const auto& elem) {
			return elem == Strings::ToLower(flag);
		});
	}
}
