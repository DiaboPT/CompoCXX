// compocxx/main.cpp

#include "compocxx.hpp"

#include <cstdlib>
#include <iostream>

using std::cout;
using std::endl;

class entity_script : public script {
public:
    int health;
    int damage;

    entity_script *target;

    entity_script(int health, int damage)
        : health(health), damage(damage), target(nullptr) {}

    bool alive() const { return health > 0; }

    void attack() {
        if (!target || !target->alive())
        return;

        cout << owner_get()->name_get() << " attacks "
             << target->owner_get()->name_get() << " for " << damage << " damage!"
             << endl;

        target->health -= damage;

        if (target->health < 0)
        target->health = 0;

        cout << target->owner_get()->name_get() << " has " << target->health
             << " HP remaining." << endl;
    }

    void start() override {}
    void update() override {}
};

class player_script : public entity_script {
public:
    player_script(int health, int damage) : entity_script(health, damage) {}
};

class opponent_script : public entity_script {
public:
    opponent_script(int health, int damage) : entity_script(health, damage) {}
};

class battle_script : public script {
private:
    engine *game;
    entity_script *player;
    entity_script *opponent;

    bool player_turn;

public:
    battle_script(engine *game)
        : game(game), player(nullptr), opponent(nullptr), player_turn(true) {}

    void start() override {
        node *battle = owner_get();

        if (!battle)
        return;

        node *player_node = battle->child_get_by_name("Player");

        node *opponent_node = battle->child_get_by_name("Opponent");

        if (!player_node || !opponent_node)
        return;

        player = dynamic_cast<entity_script *>(player_node->script_get());

        opponent = dynamic_cast<entity_script *>(opponent_node->script_get());

        if (!player || !opponent)
        return;

        player->target = opponent;
        opponent->target = player;

        cout << endl;
        cout << "============================" << endl;
        cout << "        BATTLE START        " << endl;
        cout << "============================" << endl;

        cout << player_node->name_get() << " HP: " << player->health
             << "  Damage: " << player->damage << endl;

        cout << opponent_node->name_get() << " HP: " << opponent->health
             << "  Damage: " << opponent->damage << endl;

        cout << "============================" << endl;
        cout << endl;
    }

    void update() override {
        if (!player || !opponent)
        return;

        if (!player->alive() || !opponent->alive()) {
        game->stop();
        return;
        }

        if (player_turn) {
        cout << "[Player Turn]" << endl;

        player->attack();

        if (!opponent->alive()) {
            cout << endl;
            cout << "Opponent has died!" << endl;
            cout << "PLAYER WINS!" << endl;
            return;
        }
        } else {
        cout << "[Opponent Turn]" << endl;

        opponent->attack();

        if (!player->alive()) {
            cout << endl;
            cout << "Player has died!" << endl;
            cout << "OPPONENT WINS!" << endl;

            game->stop();
            return;
        }
        }

        player_turn = !player_turn;

        cout << endl;
    }
};

class player_node : public node2d {
public:
    player_node() : node2d("Player") {}
};

class opponent_node : public node2d {
public:
    opponent_node() : node2d("Opponent") {}
};

class battle_node : public node2d {
public:
    battle_node() : node2d("Battle") {}
};

///

int main() {
    engine game("RPG");

    auto battle = make_unique<battle_node>();

    auto player = make_unique<player_node>();
    player->script_set(make_unique<player_script>(100, 20));

    auto opponent = make_unique<opponent_node>();
    opponent->script_set(make_unique<opponent_script>(80, 15));

    battle->child_add(std::move(player));
    battle->child_add(std::move(opponent));

    battle->script_set(make_unique<battle_script>(&game));

    game.root_set(std::move(battle));

    game.run();

    return EXIT_SUCCESS;
}
