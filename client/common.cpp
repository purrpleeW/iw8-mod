#include "common.hpp"
#include <utility/nt.hpp>

std::string Client::AndRel(std::uintptr_t address) {
	Common::Utility::NT::Library assignedLib = Common::Utility::NT::Library::GetByAddress(reinterpret_cast<void*>(address));
	if (assignedLib) {
		return std::format("0x{:016X} ({}+0x{:016X})", address, assignedLib.GetName(), address - reinterpret_cast<std::uintptr_t>(assignedLib.GetPtr()));
	}
	return std::format("0x{:016X}", address);
}
