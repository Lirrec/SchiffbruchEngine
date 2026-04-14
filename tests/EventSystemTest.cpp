#include <catch2/catch_test_macros.hpp>

#include <sbe/Engine.hpp>
#include <sbe/Module.hpp>
#include <sbe/event/Event.hpp>
#include <sbe/event/EventHelper.hpp>
#include <sbe/event/EventUser.hpp>
#include <sbe/util/Hash.hpp>

#include <boost/any.hpp>
#include <functional>
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// TestEventUser
// Subclass that promotes all protected EventUser API to public so tests can
// call Register*/Unregister* directly without additional boilerplate.
// ---------------------------------------------------------------------------

class TestEventUser : public sbe::EventUser {
public:
    int       callCount = 0;
    boost::any lastData;

    void HandleEvent(Event& e) override {
        ++callCount;
        lastData = e.Data();
    }

    // Promote the full RegisterForEvent overload set (including templates)
    using sbe::EventUser::RegisterForEvent;
    using sbe::EventUser::RegisterMemberAsEventCallback;
    using sbe::EventUser::UnregisterThis;
};

// ---------------------------------------------------------------------------
// ModuleFixture
// Creates a Module on the calling thread for each TEST_CASE_METHOD.
// Engine + EventCore are already alive courtesy of SBETestSetupListener.
// ---------------------------------------------------------------------------

struct ModuleFixture {
    sbe::Module module;

    ModuleFixture()  { module.SetupForCurrentThread("EventSystemTest"); }
    ~ModuleFixture() { module.TeardownCurrentThread(); }

    void processEvents() { module.ProcessEvents(); }
};

// ===========================================================================
// Event construction and Is()
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "Event constructed from string name is recognised by Is(name)",
    "[Event][construction]")
{
    Event e("EVT_STRING_CTOR");
    CHECK(e.Is("EVT_STRING_CTOR"));
    CHECK_FALSE(e.Is("EVT_OTHER_NAME"));
}

TEST_CASE_METHOD(ModuleFixture,
    "Event constructed from hash is recognised by Is(hash)",
    "[Event][construction]")
{
    // Registering the name first ensures EventCore knows the mapping.
    Event reg("EVT_HRSH_CTOR");
    constexpr sbe::Event::EventType h = sbe::hash_name("EVT_HRSH_CTOR");
    Event e(h);
    CHECK(e.Is(h));
    CHECK_FALSE(e.Is(sbe::hash_name("EVT_DIFFERENT")));
}

TEST_CASE_METHOD(ModuleFixture,
    "Event carries arbitrary typed data accessible via Data()",
    "[Event][data]")
{
    Event e("EVT_WITH_INT", 42);
    REQUIRE_FALSE(e.Data().empty());
    CHECK(e.Is("EVT_WITH_INT", typeid(int)));
    CHECK(boost::any_cast<int>(e.Data()) == 42);
}

TEST_CASE_METHOD(ModuleFixture,
    "Event::Is(name, typeid) returns false for wrong data type",
    "[Event][data]")
{
    Event e("EVT_TYPED_STR", std::string("hello"));
    CHECK_FALSE(e.Is("EVT_TYPED_STR", typeid(int)));
}

TEST_CASE_METHOD(ModuleFixture,
    "Event::Is returns false for a different event name",
    "[Event]")
{
    Event e("EVT_FOO");
    CHECK_FALSE(e.Is("EVT_BRR")); // 'B'–'Y' range: consistently lowercased
}

TEST_CASE_METHOD(ModuleFixture,
    "hash_name is case-insensitive for letters B-Y",
    "[hash]")
{
    // 'E','V','T' are all in the B-Y range so they are lowercased.
    CHECK(sbe::hash_name("EVT_TEST") == sbe::hash_name("evt_test"));
    CHECK(sbe::hash_name("EVT_TEST") == sbe::hash_name("Evt_Test"));
}

// ===========================================================================
// RegisterForEvent — by string name
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "RegisterForEvent(name) routes unhandled events to HandleEvent override",
    "[EventUser][registration][by-name]")
{
    TestEventUser user;
    user.RegisterForEvent("EVT_SIMPLE_NAME");

    Module::Get()->QueueEvent("EVT_SIMPLE_NAME");
    processEvents();

    CHECK(user.callCount == 1);
}

TEST_CASE_METHOD(ModuleFixture,
    "RegisterForEvent(name, callback) invokes the supplied callback",
    "[EventUser][registration][by-name]")
{
    bool called = false;
    TestEventUser user;
    std::function<void(Event&)> handler = [&called](Event&) { called = true; };
    user.RegisterForEvent("EVT_NAME_CB", handler);

    Module::Get()->QueueEvent("EVT_NAME_CB");
    processEvents();

    CHECK(called);
}

// ===========================================================================
// RegisterForEvent — by hash (EventType)
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "RegisterForEvent(hash) routes events to HandleEvent",
    "[EventUser][registration][by-hash]")
{
    // Ensure the name is known to EventCore before looking up its hash.
    Event reg("EVT_BY_HRSH");
    constexpr sbe::Event::EventType h = sbe::hash_name("EVT_BY_HRSH");

    TestEventUser user;
    user.RegisterForEvent(h);

    Module::Get()->QueueEvent(Event("EVT_BY_HRSH"));
    processEvents();

    CHECK(user.callCount == 1);
}

TEST_CASE_METHOD(ModuleFixture,
    "RegisterForEvent(hash, callback) invokes the supplied callback",
    "[EventUser][registration][by-hash]")
{
    Event reg("EVT_HRSH_CB");
    constexpr sbe::Event::EventType h = sbe::hash_name("EVT_HRSH_CB");

    bool called = false;
    TestEventUser user;
    std::function<void(Event&)> handler = [&called](Event&) { called = true; };
    user.RegisterForEvent(h, handler);

    Module::Get()->QueueEvent(Event("EVT_HRSH_CB"));
    processEvents();

    CHECK(called);
}

// ===========================================================================
// RegisterForEvent — typed single-argument lambda
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "RegisterForEvent<int> lambda receives the correct integer value",
    "[EventUser][typed-lambda]")
{
    int received = -1;
    TestEventUser user;
    std::function<void(int)> handler = [&received](int v) { received = v; };
    user.RegisterForEvent("EVT_INT_TYPED", handler);

    Module::Get()->QueueEvent("EVT_INT_TYPED", 99);
    processEvents();

    CHECK(received == 99);
}

TEST_CASE_METHOD(ModuleFixture,
    "RegisterForEvent<string> lambda receives the correct string value",
    "[EventUser][typed-lambda]")
{
    std::string received;
    TestEventUser user;
    std::function<void(std::string)> handler = [&received](std::string s) { received = s; };
    user.RegisterForEvent("EVT_STR_TYPED", handler);

    Module::Get()->QueueEvent("EVT_STR_TYPED", std::string("world"));
    processEvents();

    CHECK(received == "world");
}

TEST_CASE_METHOD(ModuleFixture,
    "RegisterForEvent typed lambda is not invoked for a different event",
    "[EventUser][typed-lambda]")
{
    int received = -1;
    TestEventUser user;
    std::function<void(int)> handler = [&received](int v) { received = v; };
    user.RegisterForEvent("EVT_INT_NOINVOKE", handler);

    Module::Get()->QueueEvent("EVT_DIFFERENT_INT", 7);
    processEvents();

    CHECK(received == -1);
}

// ===========================================================================
// RegisterForEvent — typed two-argument lambda (packed as a tuple)
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "RegisterForEvent<int,string> two-argument lambda receives both values",
    "[EventUser][typed-lambda][tuple]")
{
    int   a = 0;
    std::string b;
    TestEventUser user;
    std::function<void(int, std::string)> handler =
        [&](int i, std::string s) { a = i; b = s; };
    user.RegisterForEvent("EVT_TWO_INTS", handler);

    Module::Get()->QueueEvent("EVT_TWO_INTS", false, 7, std::string("hello"));
    processEvents();

    CHECK(a == 7);
    CHECK(b == "hello");
}

// ===========================================================================
// RegisterMemberAsEventCallback — no-param member function
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "RegisterMemberAsEventCallback with no-param member fires on matching event",
    "[EventUser][member-callback]")
{
    struct Handler : public sbe::EventUser {
        bool called = false;
        void onEvent() { called = true; }
        using sbe::EventUser::RegisterMemberAsEventCallback;
    };

    Handler h;
    h.RegisterMemberAsEventCallback(&h, &Handler::onEvent, "EVT_NO_PRMS");

    Module::Get()->QueueEvent("EVT_NO_PRMS");
    processEvents();

    CHECK(h.called);
}

TEST_CASE_METHOD(ModuleFixture,
    "RegisterMemberAsEventCallback no-param member is not invoked for unrelated event",
    "[EventUser][member-callback]")
{
    struct Handler : public sbe::EventUser {
        bool called = false;
        void onEvent() { called = true; }
        using sbe::EventUser::RegisterMemberAsEventCallback;
    };

    Handler h;
    h.RegisterMemberAsEventCallback(&h, &Handler::onEvent, "EVT_NPRM_GUARD");

    Module::Get()->QueueEvent("EVT_NPRM_OTHER");
    processEvents();

    CHECK_FALSE(h.called);
}

// ===========================================================================
// RegisterMemberAsEventCallback — with EventDef (typed params)
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "RegisterMemberAsEventCallback via EventDef invokes member with correct int data",
    "[EventUser][member-callback][EventDef]")
{
    struct Handler : public sbe::EventUser {
        int value = 0;
        void onValue(int v) { value = v; }
        using sbe::EventUser::RegisterMemberAsEventCallback;
    };

    constexpr sbe::HashType EVT = sbe::hash_name("EVT_MBDEF_INT");
    auto def = sbe::makeEventDef<EVT>(&Handler::onValue);

    Handler h;
    h.RegisterMemberAsEventCallback(&h, def, "EVT_MBDEF_INT");

    Module::Get()->QueueEvent(EVT, false, 55);
    processEvents();

    CHECK(h.value == 55);
}

TEST_CASE_METHOD(ModuleFixture,
    "RegisterMemberAsEventCallback via EventDef with string param",
    "[EventUser][member-callback][EventDef]")
{
    struct Handler : public sbe::EventUser {
        std::string received;
        void onStr(std::string s) { received = s; }
        using sbe::EventUser::RegisterMemberAsEventCallback;
    };

    constexpr sbe::HashType EVT = sbe::hash_name("EVT_MBDEF_STR");
    auto def = sbe::makeEventDef<EVT>(&Handler::onStr);

    Handler h;
    h.RegisterMemberAsEventCallback(&h, def, "EVT_MBDEF_STR");

    Module::Get()->QueueEvent(EVT, false, std::string("test_value"));
    processEvents();

    CHECK(h.received == "test_value");
}

// ===========================================================================
// Priority ordering
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "Lower-priority number handler is called before higher-priority number handler",
    "[EventUser][priority]")
{
    // EventQueue uses a std::multimap sorted ascending, so numerically lower
    // priority values are dispatched first (see EventQueue.hpp doc comment).
    std::vector<int> order;

    TestEventUser first;  // priority 0 — dispatched first
    std::function<void(Event&)> firstHandler = [&order](Event&) { order.push_back(1); };
    first.RegisterForEvent("EVT_PRIO_TEST", firstHandler, 0);

    TestEventUser second; // priority 10 — dispatched second
    std::function<void(Event&)> secondHandler = [&order](Event&) { order.push_back(2); };
    second.RegisterForEvent("EVT_PRIO_TEST", secondHandler, 10);

    Module::Get()->QueueEvent("EVT_PRIO_TEST");
    processEvents();

    REQUIRE(order.size() == 2);
    CHECK(order[0] == 1); // lower priority number fires first
    CHECK(order[1] == 2);
}

// ===========================================================================
// Multiple events per tick
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "All events queued before processEvents() are delivered in one tick",
    "[EventUser][queue]")
{
    int count = 0;
    TestEventUser user;
    std::function<void(Event&)> handler = [&count](Event&) { ++count; };
    user.RegisterForEvent("EVT_MULTI_TICK", handler);

    Module::Get()->QueueEvent("EVT_MULTI_TICK");
    Module::Get()->QueueEvent("EVT_MULTI_TICK");
    Module::Get()->QueueEvent("EVT_MULTI_TICK");
    processEvents();

    CHECK(count == 3);
}

TEST_CASE_METHOD(ModuleFixture,
    "Events queued after processEvents() are delivered on the next tick",
    "[EventUser][queue]")
{
    int count = 0;
    TestEventUser user;
    std::function<void(Event&)> handler = [&count](Event&) { ++count; };
    user.RegisterForEvent("EVT_NEXT_TICK", handler);

    Module::Get()->QueueEvent("EVT_NEXT_TICK");
    processEvents();
    CHECK(count == 1);

    Module::Get()->QueueEvent("EVT_NEXT_TICK");
    processEvents();
    CHECK(count == 2);
}

// ===========================================================================
// UnregisterThis
// ===========================================================================

TEST_CASE_METHOD(ModuleFixture,
    "UnregisterThis prevents the user from receiving further events",
    "[EventUser][unregister]")
{
    TestEventUser user;
    user.RegisterForEvent("EVT_UNREG");

    user.UnregisterThis();
    Module::Get()->QueueEvent("EVT_UNREG");
    processEvents();

    CHECK(user.callCount == 0);
}

TEST_CASE_METHOD(ModuleFixture,
    "Event is still delivered to remaining user after another unregisters",
    "[EventUser][unregister]")
{
    TestEventUser userA;
    TestEventUser userB;
    userA.RegisterForEvent("EVT_PARTIAL_UNREG");
    userB.RegisterForEvent("EVT_PARTIAL_UNREG");

    userA.UnregisterThis();
    Module::Get()->QueueEvent("EVT_PARTIAL_UNREG");
    processEvents();

    CHECK(userA.callCount == 0);
    CHECK(userB.callCount == 1);
}
