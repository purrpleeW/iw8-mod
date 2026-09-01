#pragma once
#include "common.hpp"

namespace Client::Demonware {
	class ByteBuffer final {
	public:
		ByteBuffer() = default;

		explicit ByteBuffer(std::string buffer)
			: m_Buffer(std::move(buffer))
		{}

		bool ReadBool(bool* output);
		bool ReadByte(char* output);
		bool ReadUbyte(unsigned char* output);
		bool ReadInt16(short* output);
		bool ReadUint16(unsigned short* output);
		bool ReadInt32(int* output);
		bool ReadUint32(unsigned int* output);
		bool ReadInt64(__int64* output);
		bool ReadUint64(unsigned __int64* output);
		bool ReadFloat(float* output);
		bool ReadString(char** output);
		bool ReadString(char* output, int length);
		bool ReadString(std::string* output);
		bool ReadBlob(char** output, int* length);
		bool ReadBlob(std::string* output);
		bool ReadStruct(void* output);
		bool ReadDataType(unsigned char expected);

		bool ReadArrayHeader(unsigned char expected, unsigned int* elementCount,
			unsigned int* elementSize = nullptr);

		bool WriteBool(bool data);
		bool WriteByte(char data);
		bool WriteUbyte(unsigned char data);
		bool WriteInt16(short data);
		bool WriteUint16(unsigned short data);
		bool WriteInt32(int data);
		bool WriteUint32(unsigned int data);
		bool WriteInt64(__int64 data);
		bool WriteUint64(unsigned __int64 data);
		bool WriteDataType(unsigned char data);
		bool WriteFloat(float data);
		bool WriteString(const char* data);
		bool WriteString(const std::string& data);
		bool WriteBlob(const char* data, int length);
		bool WriteBlob(const std::string& data);
		bool WriteStruct(void* data, int length);

		bool WriteArrayHeader(unsigned char type, unsigned int elementCount, unsigned int elementSize);

		bool Read(int bytes, void* output);
		bool Write(int bytes, const void* data);
		bool Write(const std::string& data);

		void SetUseDataTypes(bool useDataTypes);
		size_t Size() const;

		bool IsUsingDataTypes() const;

		std::string& GetBuffer();
		std::string GetRemaining();

		bool HasMoreData() const;

	private:
		std::string m_Buffer;
		size_t m_CurrentByte = 0;
		bool m_UseDataTypes = true;
	};
}
