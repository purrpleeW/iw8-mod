#include "common.hpp"
#include "demonware/byte_buffer.hpp"
#include "engine/iw8/enums/bdBitBufferDataType.hpp"

namespace Client::Demonware {
	bool ByteBuffer::ReadBool(bool* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_BOOL_TYPE))) {
			return false;
		}
		return this->Read(1, output);
	}

	bool ByteBuffer::ReadByte(char* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_CHAR8_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadUbyte(unsigned char* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_UNSIGNED_CHAR8_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadInt16(short* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_INTEGER16_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadUint16(unsigned short* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_UNSIGNED_INTEGER16_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadInt32(int* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_INTEGER32_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadUint32(unsigned int* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_UNSIGNED_INTEGER32_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadInt64(__int64* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_INTEGER64_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadUint64(unsigned __int64* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_UNSIGNED_INTEGER64_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadFloat(float* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_FLOAT32_TYPE))) {
			return false;
		}
		return this->Read(sizeof(*output), output);
	}

	bool ByteBuffer::ReadString(std::string* output) {
		char* outData;
		if (this->ReadString(&outData)) {
			output->clear();
			output->append(outData);
			return true;
		}

		return false;
	}

	bool ByteBuffer::ReadString(char** output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_CHAR8_STRING_TYPE))) {
			return false;
		}

		*output = const_cast<char*>(this->m_Buffer.data()) + this->m_CurrentByte;
		this->m_CurrentByte += strlen(*output) + 1;

		return true;
	}

	bool ByteBuffer::ReadString(char* output, const int length) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_CHAR8_STRING_TYPE))) {
			return false;
		}

		strcpy_s(output, length, const_cast<char*>(this->m_Buffer.data()) + this->m_CurrentByte);
		this->m_CurrentByte += strlen(output) + 1;

		return true;
	}

	bool ByteBuffer::ReadBlob(std::string* output)
	{
		char* outData;
		int length;
		if (this->ReadBlob(&outData, &length)) {
			output->clear();
			output->append(outData, length);
			return true;
		}

		return false;
	}

	bool ByteBuffer::ReadBlob(char** output, int* length) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_BLOB_TYPE))) {
			return false;
		}

		unsigned int size;
		this->ReadUint32(&size);

		*output = const_cast<char*>(this->m_Buffer.data()) + this->m_CurrentByte;
		*length = static_cast<int>(size);

		this->m_CurrentByte += size;

		return true;
	}

	bool ByteBuffer::ReadStruct(void* output) {
		if (!this->ReadDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_STRUCTURED_DATA_TYPE))) {
			return false;
		}

		unsigned int size;
		this->ReadUint32(&size);

		auto data = const_cast<char*>(this->m_Buffer.data()) + this->m_CurrentByte;
		memcpy(output, data, size);

		this->m_CurrentByte += size;

		return true;
	}

	bool ByteBuffer::ReadDataType(const unsigned char expected) {
		if (!this->m_UseDataTypes) {
			return true;
		}

		unsigned char type;
		this->Read(sizeof(type), &type);
		return type == expected;
	}

	bool ByteBuffer::ReadArrayHeader(const unsigned char expected, unsigned int* elementCount, unsigned int* elementSize) {
		if (elementCount) {
			*elementCount = 0;
		}

		if (elementSize) {
			*elementSize = 0;
		}

		if (!this->ReadDataType(expected + 100)) {
			return false;
		}

		uint32_t arraySize, elCount;
		if (!this->ReadUint32(&arraySize)) {
			return false;
		}

		this->SetUseDataTypes(false);
		this->ReadUint32(&elCount);
		this->SetUseDataTypes(true);

		if (elementCount) {
			*elementCount = elCount;
		}

		if (elementSize) {
			*elementSize = arraySize / elCount;
		}

		return true;
	}

	bool ByteBuffer::WriteBool(bool data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_BOOL_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteByte(char data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_CHAR8_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteUbyte(unsigned char data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_UNSIGNED_CHAR8_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteInt16(short data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_INTEGER16_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteUint16(unsigned short data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_UNSIGNED_INTEGER16_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteInt32(int data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_INTEGER32_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteUint32(unsigned int data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_UNSIGNED_INTEGER32_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteInt64(__int64 data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_INTEGER64_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteUint64(unsigned __int64 data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_UNSIGNED_INTEGER64_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteDataType(unsigned char data) {
		if (!this->m_UseDataTypes) {
			return true;
		}
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteFloat(float data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_FLOAT32_TYPE));
		return this->Write(sizeof(data), &data);
	}

	bool ByteBuffer::WriteString(const std::string& data) {
		return this->WriteString(data.data());
	}

	bool ByteBuffer::WriteString(const char* data) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_SIGNED_CHAR8_STRING_TYPE));
		return this->Write(static_cast<int>(strlen(data)) + 1, data);
	}

	bool ByteBuffer::WriteBlob(const std::string& data) {
		return this->WriteBlob(data.data(), INT(data.size()));
	}

	bool ByteBuffer::WriteBlob(const char* data, const int length) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_BLOB_TYPE));
		this->WriteUint32(length);

		return this->Write(length, data);
	}

	bool ByteBuffer::WriteStruct(void* data, const int length) {
		this->WriteDataType(ENUM_UNDER(IW8::bdBitBufferDataType::BD_BB_STRUCTURED_DATA_TYPE));
		this->WriteUint32(length);

		return this->Write(length, data);
	}

	bool ByteBuffer::WriteArrayHeader(const unsigned char type, const unsigned int elementCount, const unsigned int elementSize) {
		const auto using_types = this->IsUsingDataTypes();
		this->SetUseDataTypes(false);

		auto result = this->WriteUbyte(type + 100);

		this->SetUseDataTypes(true);
		result &= this->WriteUint32(elementCount * elementSize);
		this->SetUseDataTypes(false);

		result &= this->WriteUint32(elementCount);

		this->SetUseDataTypes(using_types);
		return result;
	}

	bool ByteBuffer::Read(const int bytes, void* output) {
		if (bytes + this->m_CurrentByte > this->m_Buffer.size()) {
			return false;
		}

		std::memmove(output, this->m_Buffer.data() + this->m_CurrentByte, bytes);
		this->m_CurrentByte += bytes;

		return true;
	}

	bool ByteBuffer::Write(const int bytes, const void* data) {
		this->m_Buffer.append(static_cast<const char*>(data), bytes);
		this->m_CurrentByte += bytes;
		return true;
	}

	bool ByteBuffer::Write(const std::string& data) {
		return this->Write(static_cast<int>(data.size()), data.data());
	}

	void ByteBuffer::SetUseDataTypes(const bool use_data_types) {
		this->m_UseDataTypes = use_data_types;
	}

	size_t ByteBuffer::Size() const {
		return this->m_Buffer.size();
	}

	bool ByteBuffer::IsUsingDataTypes() const {
		return this->m_UseDataTypes;
	}

	std::string& ByteBuffer::GetBuffer() {
		return this->m_Buffer;
	}

	std::string ByteBuffer::GetRemaining() {
		return std::string(this->m_Buffer.begin() + this->m_CurrentByte, this->m_Buffer.end());
	}

	bool ByteBuffer::HasMoreData() const {
		return this->m_Buffer.size() > this->m_CurrentByte;
	}
}
