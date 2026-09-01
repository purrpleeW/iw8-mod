#pragma once
#include "common.hpp"
#include "memory/memory.hpp"

#include <utility/concurrency.hpp>

namespace Client {
	class SignatureStore {
		using Res = Memory::ScannedResult<void>;
		
		class Signature {
		public:
			Signature(const std::string& name, void** ptr, const std::string& signature, std::function<Res(Res)> mod)
				: m_Name(name)
				, m_Signature(signature)
				, m_Pointer(ptr)
				, m_Mod(std::move(mod))
			{}

			void Scan() const {
				Res scTemp = Memory::SigScan(this->m_Signature, g_GameModuleName, this->m_Name);
				if (scTemp.As<void*>() != nullptr) {
					scTemp = this->m_Mod(scTemp);
					*this->m_Pointer = scTemp.As<void*>();
				}
			}

			std::string m_Name;
			std::string m_Signature;
			void** m_Pointer;
			std::function<Res(Res)> m_Mod;
		};
	public:
		void Add(const std::string& name, void** ptr, const std::string& signature, std::function<Res(Res)> mod) {
			this->m_Signatures.push_back(Signature(name, ptr, signature, std::move(mod)));
		}
		
		void Add(const std::string& name, void** ptr, const std::string& signature) {
			this->m_Signatures.push_back(Signature(name, ptr, signature,
				[](Res r) { return r; }));
		}

		void ScanAll() {
			Common::Utility::Concurrency::AsyncForEach<Signature>(this->m_Signatures, [](auto& sig) {
				sig.Scan();
			});
		}
	private:
		std::vector<Signature> m_Signatures{};
	};
}
