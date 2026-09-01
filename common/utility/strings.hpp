#pragma once
#include "common_core.hpp"
#include "utility/memory.hpp"

template <class Type, size_t n>
constexpr auto ARRAY_COUNT(Type(&)[n]) {
	return n;
}

namespace Common::Utility::Strings {
	template <size_t Buffers, size_t MinBufferSize>
	class VAProvider final {
	public:
		static_assert(Buffers != 0 && MinBufferSize != 0, "Buffers and MinBufferSize mustn't be 0");

		VAProvider()
			: m_CurrentBuffer(0)
		{}

		char* Get(const char* format, const va_list ap) {
			++this->m_CurrentBuffer %= ARRAY_COUNT(this->m_StringPool);
			auto entry = &this->m_StringPool[this->m_CurrentBuffer];

			if (!entry->m_Size || !entry->m_Buffer) {
				throw std::runtime_error("String pool not initialized");
			}

			while (true) {
				const int res = vsnprintf_s(entry->m_Buffer, entry->m_Size, _TRUNCATE, format, ap);
				if (res > 0) {
					// Success
					break;
				}
				if (res == 0) {
					// Error
					return nullptr;
				}

				entry->DoubleSize();
			}

			return entry->m_Buffer;
		}

	private:
		class Entry final {
		public:
			explicit Entry(const size_t size = MinBufferSize)
				: m_Size(size)
				, m_Buffer(nullptr)
			{
				if (this->m_Size < MinBufferSize) {
					this->m_Size = MinBufferSize;
				}
				this->Allocate();
			}

			~Entry() {
				if (this->m_Buffer != nullptr) {
					Memory::GetAllocator()->Free(this->m_Buffer);
				}
				this->m_Size = 0;
				this->m_Buffer = nullptr;
			}

			void Allocate() {
				if (this->m_Buffer != nullptr) {
					Memory::GetAllocator()->Free(this->m_Buffer);
				}
				this->m_Buffer = Memory::GetAllocator()->AllocateArray<char>(this->m_Size + 1);
			}

			void DoubleSize() {
				this->m_Size *= 2;
				this->Allocate();
			}

			size_t m_Size;
			char* m_Buffer;
		};

		size_t m_CurrentBuffer;
		Entry m_StringPool[Buffers];
	};

	const char* va(const char* fmt, ...);

	std::vector<std::string> Split(const std::string& s, char delim);

	std::string ToLower(const std::string& text);
	std::string ToUpper(const std::string& text);
	bool StartsWith(const std::string& text, const std::string& substring);
	bool EndsWith(const std::string& text, const std::string& substring);

	std::string DumpHex(const std::string& data, const std::string& separator = " ");

	std::string GetClipboardData();

	void Strip(const char* in, char* out, size_t max);

	std::string Convert(const std::wstring& wstr);
	std::wstring Convert(const std::string& str);

	std::string Replace(std::string str, const std::string& from, const std::string& to);
}

#ifdef UNICODE
#	define OPT_TO_WIDE(s) Common::Utility::Strings::Convert(s)
#else
#	define OPT_TO_WIDE(s) s
#endif
