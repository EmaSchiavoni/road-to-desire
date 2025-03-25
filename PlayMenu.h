#pragma once
#include "Menu.h"

namespace rtd {

    class PlayMenu : public rtd::Menu
    {
    public:
        PlayMenu();
        void update(const float& delta_time, rtd::Game& game) override;
        void select(rtd::Game& game) override;
    };

}