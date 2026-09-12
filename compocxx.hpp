// compocxx/compocxx.hpp

#include <cstddef>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using std::make_unique;
using std::string;
using std::unique_ptr;
using std::vector;

class meta_data;
class script;
class node;
class engine;

static size_t global_uid = 0;

struct vector2 {
    union {
        struct {
            float x, y;
        };
    };
};

struct vector3 {
    union {
        struct {
            float x, y, z;
        };

        struct {
            float r, g, b;
        };
    };
};

struct vector4 {
    union {
        struct {
            float x, y, z, w;
        };

        struct {
            float r, g, b, a;
        };
    };
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

class script {
private:
    node* owner;

public:
    node* owner_get() const {
        return owner;
    }

    void owner_set(node* new_owner) {
        owner = new_owner;
    }

    virtual void start() = 0;
    virtual void update() = 0;

    virtual ~script() = default;

    script()
    : owner(nullptr)
    {}
};

class node {
private:
    meta_data meta;

    node* parent;
    vector<unique_ptr<node>> children;

    unique_ptr<script> attached_script;

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
    node* child_get_by_name(const string& name) const {
        for (const auto& child : children) {
            if (child->name_get() == name) {
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

    script* script_get() const {
        return attached_script.get();
    }
    void script_set(unique_ptr<script> new_script) {
        if (attached_script)
            attached_script->owner_set(nullptr);

        if (new_script)
            new_script->owner_set(this);

        attached_script = std::move(new_script);
    }

    void start() {
        if (!meta.active_get())
            return;

        if (attached_script)
            attached_script->start();

        for (auto& child : children)
            child->start();
    }

    void update() {
        if (!meta.active_get())
            return;

        if (attached_script)
            attached_script->update();

        for (auto& child : children)
            child->update();
    }

    node(const string& name)
    : meta(name),
    parent(nullptr),
    children(),
    attached_script(nullptr) {}
    virtual ~node() = default;
};

class engine {
private:
    meta_data meta;
    unique_ptr<node> root;
    bool running;

public:
    node* root_get() const {
        return root.get();
    }
    void root_set(unique_ptr<node> new_root) {
        root = std::move(new_root);
    }

    void stop() {
        running = false;
    }

    void run() {
        if (!root)
            return;

        root->start();

        while (running) {
            root->update();
        }
    }

    engine(const string& name, bool running = true)
    : meta(name),
    running(running),
    root(nullptr)
    {}
};

///

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
