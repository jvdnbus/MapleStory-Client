#include "UIWorldSelectBackground.h"

#include "../UI.h"

namespace ms {
	void UINeoCityBackground::init(nl::node& background_node, Point<int16_t>& target_dim, std::vector<Sprite>& sprites) const {
        auto bg_sprite = background_node["0"]["0"].get_bitmap();
        uint16_t w = bg_sprite.width();
        float scale = static_cast<float>(target_dim.x()) / static_cast<float>(w);

        // Assuming Map.nx contains 2x resolution sprites for "neoCity"

        // apply same scale to all frames based on the background sprite
        DrawArgument base_draw_arg = DrawArgument(target_dim / 2);

        // Background sprite
        sprites.emplace_back(background_node["0"], base_draw_arg + DrawArgument(Point<int16_t>( - 1, -1), scale, scale));

        // Fairy animation
        sprites.emplace_back(Animation(background_node["1"]), base_draw_arg + DrawArgument(266, 152));

        // Fairy foreground sprite
        sprites.emplace_back(background_node["2"], base_draw_arg + DrawArgument(170,  107));

        // Fairy eyes animation
        sprites.emplace_back(Animation(background_node["3"]), base_draw_arg + DrawArgument(150, 80));

        // Sparkles animation
        sprites.emplace_back(Animation(background_node["4"]), base_draw_arg + DrawArgument(-250, -38));

        // NeoCity logo animation
        sprites.emplace_back(Animation(background_node["5"]), base_draw_arg + DrawArgument(-180, 200));
    }
}
