#include "engine/engine_common.hpp"
#include "engine/iw8/XUID.hpp"
#include "game/game.hpp"

namespace IW8 {
	void XUID::Deserialize(const msg_t* msg) {
		this->m_ID = Client::g_Pointers->m_MSG_ReadInt64(msg);
	}

	void XUID::Serialize(const msg_t* msg) {
		Client::g_Pointers->m_MSG_WriteInt64(msg, this->m_ID);
	}

	XUID* XUID::FromHexString(const char* xuidString) {
		this->m_ID = Client::g_Pointers->m_I_atoui64_hex(xuidString);
		return this;
	}

	XUID* XUID::FromMsg(const msg_t* msg) {
		this->m_ID = Client::g_Pointers->m_MSG_ReadInt64(msg);
		return this;
	}

	XUID* XUID::FromString(const char* xuidString) {
		this->m_ID = Client::g_Pointers->m_I_atoui64(xuidString);
		return this;
	}

	XUID* XUID::FromUInt64(std::uint64_t id) {
		this->m_ID = id;
		return this;
	}

	XUID* XUID::FromUniversalId(std::uint64_t id) {
		this->m_ID = id;
		return this;
	}

	std::uint64_t XUID::GetUniversalId() {
		return this->m_ID;
	}

	bool XUID::IsNull() {
		return this->m_ID == 0;
	}

	bool XUID::IsValid() {
		return this->m_ID != 0;
	}

	XUID* XUID::NullXUID() {
		this->m_ID = 0;
		return this;
	}

	XUID* XUID::RandomXUID() {
		std::uint32_t* randSeed = Client::g_Pointers->m_holdrand;
		std::uint32_t backupRandSeed = *randSeed;
		this->m_ID = 0ull;
		*randSeed = Client::g_Pointers->m_Sys_Microseconds() & 0xFFFFFFFF;
		this->m_ID = Client::g_Pointers->m_I_irand(1, INT_MAX);
		*randSeed = backupRandSeed;
		return this;
	}

	const char* XUID::ToDevString() {
		return Client::g_Pointers->m_j_va("%zu", this->m_ID);
	}

	const char* XUID::ToHexString() {
		return Client::g_Pointers->m_j_va("%zx", this->m_ID);
	}

	const char* XUID::ToString() {
		return Client::g_Pointers->m_j_va("%zu", this->m_ID);
	}

	bool XUID::operator !=(const XUID* xuid) {
		return this->m_ID != xuid->m_ID;
	}

	XUID* XUID::operator =(const XUID* xuid) {
		this->m_ID = xuid->m_ID;
		return this;
	}

	bool XUID::operator ==(const XUID* xuid) {
		return this->m_ID == xuid->m_ID;
	}
}
