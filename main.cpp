#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::endl;
using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;

class meta_data;
class behaviour;
class node;
class engine;

static size_t global_uid = 0;

class vector2 {
public:
    float x, y;
};

class meta_data {
private:
    size_t uid;
    string name;
    bool is_active;

public:
    size_t uid_get() const {
        return uid;
    }

    const string& name_get() const {
        return name;
    }
    void name_set(const string& new_name) {
        name = new_name;
    }

    bool active_get() const {
        return is_active;
    }
    void active_set(bool new_active) {
        is_active = new_active;
    }

    meta_data(const string& new_name)
    : uid(global_uid++),
    name(new_name),
    is_active(true) {}
};

class behaviour {
public:
    virtual void start() = 0;
    virtual void update() = 0;
    virtual ~behaviour() = default;
};

class node {
private:
    meta_data meta;

    node* parent;
    vector<unique_ptr<node>> children;

    unique_ptr<behaviour> script;

public:
    size_t uid_get() const {
        return meta.uid_get();
    }

    const string& name_get() const {
        return meta.name_get();
    }
    void name_set(const string& new_name) {
        meta.name_set(new_name);
    }

    bool active_get() const {
        return meta.active_get();
    }
    void active_set(bool new_active) {
        meta.active_set(new_active);
    }

    node* parent_get() const {
        return parent;
    }

    size_t child_count() const {
        return children.size();
    }
    node* child_get_by_id(size_t id) const {
        for (const auto& child : children) {
            if (child->uid_get() == id) {
                return child.get();
            }
        }
        return nullptr;
    }
    void child_add(unique_ptr<node> child) {
        if (!child)
            return;

        child->parent = this;
        children.push_back(std::move(child));
    }
    unique_ptr<node> child_remove_by_id(size_t id) {
        for (auto it = children.begin(); it != children.end(); ++it) {
            if ((*it)->uid_get() == id) {
                unique_ptr<node> removed = std::move(*it);
                children.erase(it);

                removed->parent = nullptr;

                return removed;
            }
        }
        return nullptr;
    }

    behaviour* script_get() const {
        return script.get();
    }
    void script_set(unique_ptr<behaviour> new_script) {
        script = std::move(new_script);
    }

    node(const string& name)
    : meta(name),
    parent(nullptr),
    children(),
    script(nullptr) {}
    virtual ~node() = default;
};

class engine {
private:
    meta_data meta;
    unique_ptr<node> root;

public:
    node* root_get() const {
        return root.get();
    }
    void root_set(unique_ptr<node> new_root) {
        root = std::move(new_root);
    }

    engine(const string& name)
    : meta(name),
    root(nullptr)
    {}
};

///

// engine prefabs
class node2d;
class sprite;

class node2d : public node {
public:
    vector2 position;

    node2d(const string& name = "Node2D")
    : node(name),
    position{0.0f, 0.0f}
    {}

    virtual ~node2d() = default;
};

class sprite : public node2d {
public:
    vector<vector<char>> pixels;

    sprite(const string& name = "Sprite") : node2d(name) {}
};

// game prefabs
class player;

class player : public node2d {
public:
    player() : node2d("Player") {
        child_add(make_unique<sprite>());
    }
};


int main() {
    engine game("Dungeon RPG");

    auto game_manager = make_unique<node>("Game Manager");
    auto level = make_unique<node>("Level 01");
    auto player = make_unique<player>();

    level->child_add(std::move(player));
    game_manager->child_add(std::move(level));

    game.root_set(std::move(game_manager));

    ///

    cout << endl << endl << "Hello World!" << endl << endl;
    return 0;
}


///
