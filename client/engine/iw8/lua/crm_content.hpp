#pragma once
#include "engine/engine_common.hpp"
#include "engine/iw8/lua/crm_locations.hpp"

#include <nlohmann/json.hpp>

#define CRM_SETTER(n) \
	CRMContent& Set##n(typename std::decay_t<decltype(IW8::Lua::CRMContent::m_##n)>::value_type value) { \
		this->m_##n = value; \
		return *this; \
	}

#define CRM_TO_JSON(n, r) \
	if (this->m_##n.has_value()) { \
		j[r] = this->m_##n.value(); \
	}

namespace IW8::Lua {
	class CRMContent {
	public:
		void AssignJson() {
			nlohmann::json j{};

			CRM_TO_JSON(Action, "action");
			CRM_TO_JSON(ActionLocation, "action_location");
			CRM_TO_JSON(ActionTitle, "action_title");
			CRM_TO_JSON(Checksum, "checksum");
			CRM_TO_JSON(ContentLong, "content_long");
			CRM_TO_JSON(ContentShort, "content_short");
			CRM_TO_JSON(EndDate, "end_date");
			CRM_TO_JSON(ForceViewTime, "force_view_time");
			CRM_TO_JSON(Image, "image");
			CRM_TO_JSON(LayoutType, "layout_type");
			CRM_TO_JSON(LocationID, "location_id");
			CRM_TO_JSON(MessageID, "message_id");
			CRM_TO_JSON(MessageIndex, "message_index");
			CRM_TO_JSON(OptionItem, "item");
			CRM_TO_JSON(PopupImage, "popup_image");
			CRM_TO_JSON(StartDate, "start_date");
			CRM_TO_JSON(StoreItem, "item");
			CRM_TO_JSON(Title, "title");

			this->m_JsonData = j;
		}

		void GenerateChecksum(std::size_t parent) {
			this->SetChecksum(std::format("iw8-{}{}{}{}{}{}",
				parent, this->m_MessageIndex.value_or(0), this->m_MessageID.value_or("?").size(), this->m_Title.value_or("?").size(),
				this->m_ContentLong.value_or("?").size(), this->m_ContentShort.value_or("?").size()));
		}

		static void AutoFillIndexes(std::vector<CRMContent>* v, std::size_t parent = 99) {
			std::size_t idx = 0;
			for (auto& c : *v) {
				int idx32 = idx & 0xFFFFFFFF;
				c.SetMessageID(std::to_string(idx32 + 1)).SetMessageIndex(idx32).GenerateChecksum(parent);
				idx++;
			}
		}
	private:
		std::optional<std::string> m_Action = std::nullopt;
		std::optional<std::string> m_ActionLocation = std::nullopt;
		std::optional<std::string> m_ActionTitle = std::nullopt;
		std::optional<std::string> m_Checksum = std::nullopt;
		std::optional<std::string> m_ContentLong = std::nullopt;
		std::optional<std::string> m_ContentShort = std::nullopt;
		std::optional<std::string> m_EndDate = std::nullopt;	// unused
		std::optional<int> m_ForceViewTime = std::nullopt;
		std::optional<std::string> m_Image = std::nullopt;
		std::optional<int> m_LayoutType = std::nullopt;
		std::optional<int> m_LocationID = std::nullopt;
		std::optional<std::string> m_MessageID = std::nullopt;
		std::optional<int> m_MessageIndex = std::nullopt;
		std::optional<std::string> m_OptionItem = std::nullopt;
		std::optional<std::string> m_PopupImage = std::nullopt;
		std::optional<std::string> m_StartDate = std::nullopt;	// unused
		std::optional<std::string> m_StoreItem = std::nullopt;
		std::optional<std::string> m_Title = std::nullopt;
	public:
		nlohmann::json m_JsonData{};

		// these bug out intellisense if above the field definitions
		CRM_SETTER(Action)
		CRM_SETTER(ActionLocation)
		CRM_SETTER(ActionTitle)
		CRM_SETTER(Checksum)
		CRM_SETTER(ContentLong)
		CRM_SETTER(ContentShort)
		CRM_SETTER(EndDate)
		CRM_SETTER(ForceViewTime)
		CRM_SETTER(Image)
		CRM_SETTER(LayoutType)
		CRM_SETTER(LocationID)
		CRM_SETTER(MessageID)
		CRM_SETTER(MessageIndex)
		CRM_SETTER(OptionItem)
		CRM_SETTER(PopupImage)
		CRM_SETTER(StartDate)
		CRM_SETTER(StoreItem)
		CRM_SETTER(Title)
	};
}

#undef CRM_TO_JSON
#undef CRM_SETTER
