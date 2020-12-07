/*
# ______       ____   ___
#   |     \/   ____| |___|    
#   |     |   |   \  |   |       
#-----------------------------------------------------------------------
# Copyright 2020, tyra - https://github.com/h4570/tyra
# Licenced under Apache License 2.0
# Sandro Sobczyński <sandro.sobczynski@gmail.com>
*/

#include "ui.hpp"

#include <utils/debug.hpp>

// ----
// Constructors/Destructors
// ----

Ui::Ui(TextureRepository *t_texRepo)
{
    PRINT_LOG("Initializing ui");

    isTask1Done = false;
    isTask2Done = false;

    info.size.set(128.0F, 32.0F);
    info.position.set(500.0F, 430.0F);

    reward.size.set(128.0F, 128.0F);
    reward.position.set(500.0F, 280.0F);
    reward.flipVertically(true);

    task1.size.set(128.0F, 32.0F);
    task1.position.set(500.0F, 10.0F);

    task2.size.set(128.0F, 32.0F);
    task2.position.set(500.0F, 50.0F);

    task1Checkbox.size.set(32.0F, 32.0F);
    task1Checkbox.position.set(460.0F, 10.0F);

    task2Checkbox.size.set(32.0F, 32.0F);
    task2Checkbox.position.set(460.0F, 50.0F);

    t_texRepo->add("ui/", "reward", BMP)->addLink(reward.getId());
    t_texRepo->add("ui/", "info-text", PNG)->addLink(info.getId());
    t_texRepo->add("ui/", "check1-text", PNG)->addLink(task1.getId());
    t_texRepo->add("ui/", "check2-text", PNG)->addLink(task2.getId());
    grayCheckboxTex = t_texRepo->add("ui/", "gray-checkmark", PNG);
    blueCheckboxTex = t_texRepo->add("ui/", "blue-checkmark", PNG);

    grayCheckboxTex->addLink(task1Checkbox.getId());
    grayCheckboxTex->addLink(task2Checkbox.getId());

    PRINT_LOG("Ui initialized!");
}

Ui::~Ui() {}

// ----
// Methods
// ----

void Ui::update(const Player &player)
{
    if (!isTask1Done && player.getJumpCount() >= 10)
    {
        isTask1Done = true;
        grayCheckboxTex->removeLinkBySprite(task1Checkbox.getId());
        blueCheckboxTex->addLink(task1Checkbox.getId());
    }
    if (!isTask2Done && player.getKilledEnemiesCount() >= 10)
    {
        isTask2Done = true;
        grayCheckboxTex->removeLinkBySprite(task2Checkbox.getId());
        blueCheckboxTex->addLink(task2Checkbox.getId());
    }
}

void Ui::render(Renderer *t_renderer)
{
    if (isTask1Done && isTask2Done)
        t_renderer->draw(reward);
    t_renderer->draw(info);
    t_renderer->draw(task1);
    t_renderer->draw(task2);
    t_renderer->draw(task1Checkbox);
    t_renderer->draw(task2Checkbox);
}
