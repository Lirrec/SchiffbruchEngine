#include <catch2/catch_all.hpp>

#include <sbe/Engine.hpp>
#include "event/EventCore.hpp" // private SBE header, accessible via the src/ include path

/**
 * Global test-run listener that initialises the minimal SBE infrastructure
 * required by all tests in this binary:
 *   - sbe::Engine  (loggers, resource manager, …)
 *   - sbe::EventCore  (singleton event registry)
 *
 * EventCore is created before CreateSubSystems() so that any subsystem that
 * constructs Events during initialisation does not crash.
 * Both objects are torn down at the end of the test run.
 */
struct SBETestSetupListener : Catch::EventListenerBase {
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const&) override {
        engine    = new sbe::Engine();
        eventCore = new sbe::EventCore();   // leaked in production too
        engine->CreateSubSystems();
    }

    void testRunEnded(Catch::TestRunStats const&) override {
        engine->UnloadSubSystems();
        delete engine;
        engine = nullptr;
        // eventCore is intentionally leaked (matches production behaviour)
    }

    static sbe::Engine*    engine;
    static sbe::EventCore* eventCore;
};

sbe::Engine*    SBETestSetupListener::engine    = nullptr;
sbe::EventCore* SBETestSetupListener::eventCore = nullptr;

CATCH_REGISTER_LISTENER(SBETestSetupListener)
