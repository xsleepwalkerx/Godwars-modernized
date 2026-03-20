# GodWars Modernized — C++ Migration Plan

## Vision

Transform the existing ~149,000-line C99 MUD codebase into a modern, maintainable C++20
codebase without sacrificing the game's character or requiring a full rewrite. The goal is
an **incremental, always-compilable migration** that improves safety, readability, and
extensibility while preserving every game mechanic.

---

## Guiding Principles

- **Never break the running game.** Each phase leaves the server in a playable, compilable state.
- **Prefer clarity over cleverness.** Replace C idioms with idiomatic C++ — not over-engineered abstractions.
- **Incremental by design.** Phases can be done independently by different contributors.
- **Zero-cost modernization first.** Reach for C++ features that cost nothing at runtime (RAII, `enum class`, `std::string_view`) before heavier machinery.

---

## Phase 0 — Compiler Switch (Days 1–3)

**Goal:** Compile the entire codebase with `g++` instead of `gcc` with no logic changes.

### Makefile Changes

```makefile
CC      = g++
CFLAGS  = -std=c++20 -Wall -Wextra -Wno-unused-parameter -g \
          -DMERC -Dlinux -Dunix -DMEMWATCH
```

### Common C → C++ Fixups Required

| C Pattern | Fix Required |
|-----------|-------------|
| `void *` implicitly cast to typed pointer | Add explicit cast: `(CHAR_DATA *)` |
| `int` used as enum value without cast | Add cast or use the enum directly |
| Variable-length arrays (VLAs) | Replace with `std::vector` or fixed-size arrays |
| Duplicate `typedef struct` definitions | Consolidate into single declaration |
| `//` comments in `.c` files | Already valid in C99; no change needed |
| `designated initializers` in non-trivial order | Reorder to match struct field order |

### Validation

```bash
make clean && make 2>&1 | grep -c error   # target: 0 errors
```

---

## Phase 1 — Headers and Type Safety (Week 1–2)

**Goal:** Eliminate implicit type punning, raw `void*` abuse, and integer-as-bool patterns.

### 1.1 — Replace `#define` Constants with `constexpr`

```cpp
// Before
#define MAX_STRING_LENGTH   4608
#define MAX_LEVEL           200

// After
constexpr int MAX_STRING_LENGTH = 4608;
constexpr int MAX_LEVEL         = 200;
```

### 1.2 — Replace Flag `#define` Macros with `enum class`

```cpp
// Before
#define IMM_SLASH       (A)
#define IMM_STAB        (B)
#define IMM_SMASH       (C)

// After
enum class Immunity : uint32_t {
    Slash  = 1u << 0,
    Stab   = 1u << 1,
    Smash  = 1u << 2,
};

// Bitwise helpers via operator overloads
constexpr Immunity operator|(Immunity a, Immunity b) {
    return static_cast<Immunity>(
        static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}
constexpr bool has_flag(Immunity flags, Immunity test) {
    return (static_cast<uint32_t>(flags) & static_cast<uint32_t>(test)) != 0;
}
```

### 1.3 — Replace `int` Booleans with `bool`

```cpp
// Before
int is_affected(CHAR_DATA *ch, int sn);

// After
bool is_affected(CHAR_DATA *ch, int sn);
```

### 1.4 — Introduce `<cstdint>` Throughout

Replace all `long`, `unsigned long` flag fields with `uint32_t` / `uint64_t` for
portability across 32/64-bit platforms.

---

## Phase 2 — String Safety (Week 2–3)

**Goal:** Eliminate buffer overflows from fixed `char[]` arrays and raw `strcpy`/`sprintf`.

### 2.1 — Replace Fixed Character Buffers

```cpp
// Before (in CHAR_DATA)
char    name[MAX_STRING_LENGTH];
char    description[MAX_STRING_LENGTH];

// After
std::string name;
std::string description;
```

> **Strategy:** Change struct fields one at a time. Use `name.c_str()` at call sites
> that still expect `const char*` — these compile cleanly and can be cleaned up later.

### 2.2 — Replace `sprintf` with `std::format` (C++20)

```cpp
// Before
char buf[MAX_STRING_LENGTH];
sprintf(buf, "%s hits %s for %d damage.\n\r", attacker, defender, dam);
send_to_char(buf, ch);

// After
send_to_char(std::format("{} hits {} for {} damage.\n\r",
    attacker, defender, dam), ch);
```

### 2.3 — Replace `str_dup` / `free_string` with RAII

The existing `str_dup` / `free_string` memory pool becomes unnecessary once string
fields are `std::string`. Remove the pool gradually as each struct is migrated.

### 2.4 — Use `std::string_view` for Read-Only Parameters

```cpp
// Before
void send_to_char(const char *txt, CHAR_DATA *ch);

// After
void send_to_char(std::string_view txt, CHAR_DATA *ch);
```

---

## Phase 3 — Memory Safety (Week 3–5)

**Goal:** Replace manual `new_char()` / `free_char()` memory pools with RAII and smart pointers.

### 3.1 — Replace Memory Pools with `std::unique_ptr`

The codebase uses custom allocators (`new_char`, `new_obj`, `new_room`) with manual
free lists. Replace with `std::unique_ptr` and let the destructor handle cleanup.

```cpp
// Before
CHAR_DATA *ch = new_char();
// ... use ch ...
free_char(ch);

// After
auto ch = std::make_unique<CharData>();
// ch is automatically freed when it goes out of scope
```

### 3.2 — Global Lists: Move to `std::list` with Non-Owning Observers

The game's master lists (`char_list`, `object_list`) own their entities. Other
references (room occupants, combat targets) are **non-owning observers**.

```cpp
// Global owning list
std::list<std::unique_ptr<CharData>> char_list;

// Non-owning reference stored in room, combat, etc.
CharData* fighting;   // raw pointer — valid because char_list owns the lifetime
```

> **Why not `shared_ptr` everywhere?** Shared ownership obscures who is responsible for
> entity lifetime and adds reference-counting overhead. In a MUD, lifetime is clear:
> characters live until they disconnect or are destroyed. Raw observer pointers are
> correct here; `shared_ptr` would be over-engineering.

### 3.3 — Replace `memwatch` with AddressSanitizer

```makefile
# During development
CFLAGS += -fsanitize=address,undefined
```

Remove the `MEMWATCH` compile-time flag and `memwatch.c`/`memwatch.h` once
ASAN is integrated into the CI build.

---

## Phase 4 — Encapsulation: C Structs → C++ Classes (Week 4–8)

**Goal:** Move game logic from free functions into the types they operate on.

### 4.1 — Rename and Restructure Core Types

| Old Name | New Name | File |
|----------|----------|------|
| `CHAR_DATA` | `CharData` | `char_data.hpp` |
| `OBJ_DATA` | `ObjData` | `obj_data.hpp` |
| `ROOM_INDEX_DATA` | `RoomData` | `room_data.hpp` |
| `AREA_DATA` | `AreaData` | `area_data.hpp` |
| `DESCRIPTOR_DATA` | `Descriptor` | `descriptor.hpp` |

> Use `using CHAR_DATA = CharData;` compatibility aliases in `merc.h` during the
> transition so old code still compiles untouched.

### 4.2 — Move Related Functions into Classes

```cpp
// Before: free functions everywhere
bool    is_affected(CHAR_DATA *ch, int sn);
int     get_curr_stat(CHAR_DATA *ch, int stat);
bool    can_see(CHAR_DATA *ch, CHAR_DATA *victim);
void    affect_to_char(CHAR_DATA *ch, AFFECT_DATA *paf);

// After: methods on CharData
class CharData {
public:
    bool is_affected(int sn) const;
    int  get_stat(Stat stat) const;
    bool can_see(const CharData& target) const;
    void apply_affect(const Affect& af);
};
```

### 4.3 — Command Dispatch: Function Pointers → `std::function`

```cpp
// Before
typedef void DO_FUN(CHAR_DATA *ch, char *argument);
struct cmd_type { const char *name; DO_FUN *do_fun; /* ... */ };

// After
using CommandFn = std::function<void(CharData&, std::string_view)>;
struct Command {
    std::string_view name;
    CommandFn        handler;
    int              min_level;
    int              log_flag;
    bool             show;
};
```

This enables lambda-based command registration and easy unit testing of individual
commands without a full game state.

### 4.4 — Spell/Skill System: Table → Registry

```cpp
// Before: parallel arrays in const.c indexed by skill number
struct skill_type skill_table[MAX_SKILL];

// After: a SkillRegistry with named lookup
class SkillRegistry {
public:
    void            register_skill(SkillDef def);
    const SkillDef* find(std::string_view name) const;
    const SkillDef* find(int sn) const;
private:
    std::vector<SkillDef>                    skills_;
    std::unordered_map<std::string, int>     by_name_;
};
```

---

## Phase 5 — Architecture Modernization (Month 2–4)

**Goal:** Replace the monolithic game loop and global state with a cleaner, testable structure.

### 5.1 — Game World as a Class

Move all global state (`char_list`, `obj_list`, `room_index`, `area_list`) into a
`World` singleton that is explicitly passed to systems that need it.

```cpp
class World {
public:
    static World& instance();

    std::list<std::unique_ptr<CharData>>    characters;
    std::list<std::unique_ptr<ObjData>>     objects;
    std::unordered_map<int, RoomData*>      rooms;       // vnum → room
    std::vector<std::unique_ptr<AreaData>>  areas;

    CharData* find_character(std::string_view name);
    RoomData* get_room(int vnum);
};
```

### 5.2 — Affect System: Typed Affects

```cpp
// Before: integer sn + union value — type unsafe
struct AFFECT_DATA { int type; int location; int modifier; /* ... */ };

// After: variant-based typed system
struct Affect {
    int       skill_num;
    int       duration;
    ApplyType location;
    int       modifier;
    uint32_t  bitvector;
};
```

### 5.3 — Event System

Replace the monolithic `update_handler()` tick with a lightweight event queue:

```cpp
class EventScheduler {
public:
    using Handler = std::function<void()>;

    void schedule(std::chrono::milliseconds delay, Handler fn);
    void tick();   // call every game pulse

private:
    struct Event {
        std::chrono::steady_clock::time_point fire_at;
        Handler handler;
    };
    std::priority_queue<Event, std::vector<Event>, EventCompare> queue_;
};
```

Each subsystem (combat rounds, regen ticks, NPC wanders) registers its own recurring
event rather than being hardcoded into `update.c`.

### 5.4 — Networking: Isolate the I/O Layer

Wrap the raw BSD socket I/O from `comm.c` behind an interface:

```cpp
class Connection {
public:
    virtual ~Connection() = default;
    virtual void send(std::string_view data) = 0;
    virtual std::optional<std::string> receive() = 0;
    virtual void close() = 0;
};

class TelnetConnection : public Connection { /* existing socket logic */ };
```

This makes it trivial to later add WebSocket support or a test harness that
simulates player connections without real sockets.

### 5.5 — Area/Database Loading: Replace Custom Parser

The `.are` file parser in `db.c` is fragile and hard to extend. Replace with a
two-stage approach:

1. **Short term:** Wrap the existing parser in a `AreaLoader` class with proper
   error reporting (`std::expected<AreaData, ParseError>`).
2. **Long term:** Add a parallel JSON/YAML loader for new content, keeping `.are`
   support for legacy areas.

```cpp
class AreaLoader {
public:
    std::expected<AreaData, std::string> load_are(const std::filesystem::path& path);
    std::expected<AreaData, std::string> load_json(const std::filesystem::path& path);
};
```

---

## Phase 6 — Class System Refactor (Month 3–5)

**Goal:** Replace the 28 hardcoded class files with a data-driven class/power system.

### 6.1 — Polymorphic Character Classes

```cpp
class CharClass {
public:
    virtual ~CharClass() = default;
    virtual std::string_view name() const = 0;
    virtual void on_login(CharData& ch) const {}
    virtual void on_death(CharData& ch) const {}
    virtual void gain_level(CharData& ch) const {}
    virtual std::span<const PowerDef> powers() const = 0;
};

class Vampire : public CharClass {
public:
    std::string_view name() const override { return "Vampire"; }
    void on_death(CharData& ch) const override;
    std::span<const PowerDef> powers() const override;
};
```

### 6.2 — Power/Ability System

Replace per-class hardcoded ability handlers with a `Power` registry:

```cpp
struct Power {
    std::string              name;
    std::string              description;
    int                      cost;        // blood/rage/quintessence/etc.
    std::function<void(CharData& user, CharData* target)> activate;
    std::function<bool(const CharData& user)>             can_use;
};
```

This lets new powers be defined in one place (even loaded from data files) rather
than scattered across a dozen `.c` files.

### 6.3 — Werewolf Subsystems as Composable Traits

The werewolf system (auspice + breed + tribe = 3 separate files) maps cleanly
to a composable trait system:

```cpp
class CharData {
    std::unique_ptr<CharClass>      char_class;
    std::unique_ptr<WerewolfAuspice> auspice;   // nullptr if not werewolf
    std::unique_ptr<WerewolfBreed>   breed;
    std::unique_ptr<WerewolfTribe>   tribe;
};
```

---

## Phase 7 — Testing Infrastructure (Ongoing from Phase 0)

**Goal:** Build a regression harness so refactoring never breaks game mechanics.

### 7.1 — Add a Test Framework

```cmake
# CMakeLists.txt (replace Makefile long-term)
find_package(Catch2 3 REQUIRED)
add_executable(godwars_tests
    tests/test_combat.cpp
    tests/test_affects.cpp
    tests/test_skills.cpp
)
target_link_libraries(godwars_tests PRIVATE godwars_lib Catch2::Catch2WithMain)
```

### 7.2 — Unit Test Priority Areas

| System | Why Test First |
|--------|---------------|
| Damage calculation (`fight.c`) | Complex formulas, easy to regress |
| Affect application/expiry | Touches almost every ability |
| Skill/spell lookup | Core to all class systems |
| Save/load player files | Data integrity |
| Command parser (`interp.c`) | Player-facing correctness |

### 7.3 — Integration: Simulated Player Sessions

Using the `Connection` interface from Phase 5.4:

```cpp
TEST_CASE("login and look around") {
    MockConnection conn;
    World world;
    world.load_test_area("tests/areas/test_start.are");

    auto ch = world.create_character("Tester");
    ch->send_command("look");

    REQUIRE(conn.output_contains("You are in the Test Room"));
}
```

---

## Migration Sequencing Summary

| Phase | Duration | Outcome |
|-------|----------|---------|
| **0** — Compiler switch | 3 days | Compiles with `g++`, zero logic change |
| **1** — Type safety | 1–2 weeks | `constexpr`, `enum class`, `bool` |
| **2** — String safety | 1–2 weeks | `std::string`, `std::format`, no buffer overflows |
| **3** — Memory safety | 2–3 weeks | `unique_ptr`, ASAN, no memory pools |
| **4** — Encapsulation | 4–6 weeks | C++ classes, `std::function` dispatch |
| **5** — Architecture | 6–10 weeks | World class, event system, isolated I/O |
| **6** — Class system | 6–10 weeks | Polymorphic classes, power registry |
| **7** — Testing | Ongoing | Regression safety throughout |

> Phases 5, 6, and 7 can be worked in parallel by different contributors once
> Phase 4 is complete.

---

## File Structure After Full Migration

```
GodWars_Modern/
├── CMakeLists.txt
├── src/
│   ├── core/
│   │   ├── world.hpp / world.cpp          # Global game state
│   │   ├── event_scheduler.hpp / .cpp     # Tick/event system
│   │   ├── command_registry.hpp / .cpp    # Command table
│   │   └── skill_registry.hpp / .cpp      # Skill/spell table
│   ├── entities/
│   │   ├── char_data.hpp / .cpp           # CharData class
│   │   ├── obj_data.hpp / .cpp            # ObjData class
│   │   ├── room_data.hpp / .cpp           # RoomData class
│   │   └── affect.hpp / .cpp              # Affect system
│   ├── classes/
│   │   ├── char_class.hpp                 # Base class interface
│   │   ├── vampire.hpp / .cpp
│   │   ├── werewolf.hpp / .cpp
│   │   ├── mage.hpp / .cpp
│   │   └── ...                            # One file per class
│   ├── systems/
│   │   ├── combat.hpp / .cpp              # fight.c → Combat system
│   │   ├── magic.hpp / .cpp               # magic.c → Magic system
│   │   ├── db_loader.hpp / .cpp           # db.c → Area loading
│   │   └── player_save.hpp / .cpp         # save.c → Persistence
│   ├── net/
│   │   ├── connection.hpp                 # Abstract connection
│   │   ├── telnet_connection.hpp / .cpp   # Raw socket impl
│   │   └── server.hpp / .cpp             # comm.c → Server loop
│   └── main.cpp
└── tests/
    ├── test_combat.cpp
    ├── test_affects.cpp
    └── test_skills.cpp
```

---

## Key C++ Features Used Throughout

| Feature | Use Case |
|---------|----------|
| `std::string` / `std::string_view` | All text, no more `char[]` overflows |
| `std::unique_ptr` | Entity ownership, no manual free |
| `std::list` | Character and object master lists |
| `std::unordered_map` | VNUM lookups, name→command dispatch |
| `std::function` | Command handlers, spell functions, event callbacks |
| `std::span` | Pass arrays without size parameter |
| `std::optional` | Nullable lookups instead of `nullptr` checks |
| `std::expected` (C++23) | Error-returning functions without exceptions |
| `std::format` | Safe string formatting, replaces `sprintf` |
| `std::filesystem` | Area file path handling |
| `enum class` | Type-safe bit flags |
| `constexpr` | Compile-time constants |
| Virtual dispatch | Character class polymorphism |
| RAII | Automatic resource cleanup everywhere |

---

## What This Achieves

By the end of all phases, the codebase will have:

- **Zero buffer overflows** — `std::string` replaces all fixed `char[]` buffers
- **Zero memory leaks** — RAII and `unique_ptr` replace manual pools
- **Type-safe flags** — `enum class` replaces `#define A/B/C` bit macros
- **Testable systems** — isolated classes can be unit tested without spinning up the full server
- **Extensible class system** — new playable classes added without touching core engine files
- **Modern tooling** — CMake, Catch2, AddressSanitizer, clang-format
- **Same game, same feel** — every mechanic preserved, just built on a solid foundation
