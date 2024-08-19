//////////////////////////////////////////////////////////////////////////////////
//	This file is part of the continued Journey MMORPG client					//
//	Copyright (C) 2015-2019  Daniel Allendorf, Ryan Payton						//
//																				//
//	This program is free software: you can redistribute it and/or modify		//
//	it under the terms of the GNU Affero General Public License as published by	//
//	the Free Software Foundation, either version 3 of the License, or			//
//	(at your option) any later version.											//
//																				//
//	This program is distributed in the hope that it will be useful,				//
//	but WITHOUT ANY WARRANTY; without even the implied warranty of				//
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the				//
//	GNU Affero General Public License for more details.							//
//																				//
//	You should have received a copy of the GNU Affero General Public License	//
//	along with this program.  If not, see <https://www.gnu.org/licenses/>.		//
//////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "../PacketHandler.h"

#include "../../IO/UI.h"
#include "../../IO/UITypes/UIChatBar.h"

namespace ms {
    class MessageHandler : public PacketHandler {
        struct Message {
            const char* text;
            UIChatBar::MessageType type;

            Message(UIChatBar::MessageType type, const char* text) : text(text), type(type) {
            }
        };

        std::vector<Message> message_buffer;

        void add_to_buffer(UIChatBar::MessageType type, const char* text) {
            message_buffer.emplace_back(type, text);
        }

        void flush() {
            auto uiChatBar = UI::get().get_element<UIChatBar>();
            if (uiChatBar) {
                for (auto message : message_buffer) {
                    uiChatBar->show_message(message.text, message.type);
                }
                message_buffer.clear();
            }
        }

    protected:
        void show_message(const char* text, UIChatBar::MessageType type) {
            auto uiChatBar = UI::get().get_element<UIChatBar>();
            if (uiChatBar) {
                if (!message_buffer.empty()) {
                    flush();
                }
                uiChatBar->show_message(text, type);
            } else {
                add_to_buffer(type, text);
            }
        }
    };

    // Show a status message
    // Opcode: SHOW_STATUS_INFO(39)
    class ShowStatusInfoHandler : public PacketHandler {
    public:
        void handle(InPacket& recv) override;

    private:
        void show_status(Color::Name color, const std::string& message) const;
    };

    // Show a server message
    // Opcode: SERVER_MESSAGE(68)
    class ServerMessageHandler : public MessageHandler {
        void handle(InPacket& recv) override;
    };

    // Show another type of server message
    // Opcode: WEEK_EVENT_MESSAGE(77)
    class WeekEventMessageHandler : public MessageHandler {
        void handle(InPacket& recv) override;
    };

    // Show a chat message
    // CHAT_RECEIVED(162)
    class ChatReceivedHandler : public MessageHandler {
        void handle(InPacket& recv) override;
    };

    // Shows the effect of a scroll
    // Opcode: SCROLL_RESULT(167)
    class ScrollResultHandler : public MessageHandler {
        void handle(InPacket& recv) override;
    };

    // Can contain numerous different effects and messages
    // Opcode: SHOW_ITEM_GAIN_INCHAT(206)
    class ShowItemGainInChatHandler : public MessageHandler {
        void handle(InPacket& recv) override;
    };
}
