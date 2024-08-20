#pragma once

#include "../../Graphics/Sprite.h"
#include "../../Util/Misc.h"

namespace ms {
    // A background animation on the world/channel select view.
    class UIWorldSelectBackground {
    private:
        std::string name_;

    public:
        UIWorldSelectBackground(std::string name) : name_(name) {}
        virtual ~UIWorldSelectBackground() {}

        virtual void init(nl::node& background_node, Point<int16_t>& target_dim, std::vector<Sprite>& sprites) const {}

        nl::node get_background_node(const nl::node& WorldSelect) const {
            return WorldSelect[name_];
        }
    };

    class UINeoCityBackground : public UIWorldSelectBackground {
    public:
        UINeoCityBackground() : UIWorldSelectBackground("neoCity") {}

        void init(nl::node& background_node, Point<int16_t>& target_dim, std::vector<Sprite>& sprites) const override;
    };
}
#pragma once
