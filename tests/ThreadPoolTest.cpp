#include <catch2/catch_all.hpp>

#include <sbe/Engine.hpp>
#include <sbe/util/ThreadPool.hpp>

#include <atomic>
#include <functional>
#include <vector>

// ---------------------------------------------------------------------------
// Engine setup — ThreadPool calls Engine::out() when threads are interrupted
// during destruction, so we need loggers initialised for the whole test run.
// ---------------------------------------------------------------------------

struct EngineListener : Catch::EventListenerBase {
    using Catch::EventListenerBase::EventListenerBase;

    void testRunStarting(Catch::TestRunInfo const&) override {
        engine = new sbe::Engine();
        engine->CreateSubSystems();
    }

    void testRunEnded(Catch::TestRunStats const&) override {
        engine->UnloadSubSystems();
        delete engine;
        engine = nullptr;
    }

    static sbe::Engine* engine;
};

sbe::Engine* EngineListener::engine = nullptr;
CATCH_REGISTER_LISTENER(EngineListener)

// ---------------------------------------------------------------------------
// chunkInts tests
// ---------------------------------------------------------------------------

TEST_CASE("chunkInts produces the requested number of chunks", "[chunkInts]") {
    auto chunks = sbe::chunkInts(12, 3);
    REQUIRE(chunks.size() == 3);
}

TEST_CASE("chunkInts divides evenly", "[chunkInts]") {
    auto chunks = sbe::chunkInts(10, 2);
    REQUIRE(chunks.size() == 2);
    CHECK(chunks[0].first  == 0);
    CHECK(chunks[0].second == 5);
    CHECK(chunks[1].first  == 5);
    CHECK(chunks[1].second == 10);
}

TEST_CASE("chunkInts last chunk absorbs the remainder", "[chunkInts]") {
    // 10 / 3 = 3 per chunk, but the final chunk must reach 10
    auto chunks = sbe::chunkInts(10, 3);
    REQUIRE(chunks.size() == 3);
    CHECK(chunks[0].first  == 0);
    CHECK(chunks[0].second == 3);
    CHECK(chunks.back().second == 10);
}

TEST_CASE("chunkInts with a single thread spans the whole range", "[chunkInts]") {
    auto chunks = sbe::chunkInts(7, 1);
    REQUIRE(chunks.size() == 1);
    CHECK(chunks[0].first  == 0);
    CHECK(chunks[0].second == 7);
}

// ---------------------------------------------------------------------------
// chunk<It> template tests
// ---------------------------------------------------------------------------

TEST_CASE("chunk produces the requested number of iterator ranges", "[chunk]") {
    std::vector<int> v = {1, 2, 3, 4, 5, 6};
    auto chunks = sbe::chunk(v.begin(), v.end(), 3);
    REQUIRE(chunks.size() == 3);
}

TEST_CASE("chunk last range ends at the original end iterator", "[chunk]") {
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto chunks = sbe::chunk(v.begin(), v.end(), 2);
    REQUIRE(chunks.size() == 2);
    CHECK(chunks.back().second == v.end());
}

TEST_CASE("chunk covers every element exactly once", "[chunk]") {
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
    auto chunks = sbe::chunk(v.begin(), v.end(), 3);
    std::size_t total = 0;
    for (auto& c : chunks)
        total += static_cast<std::size_t>(std::distance(c.first, c.second));
    CHECK(total == v.size());
}

// ---------------------------------------------------------------------------
// ThreadPool tests
// ---------------------------------------------------------------------------

TEST_CASE("ThreadPool processes all elements of a vector job", "[ThreadPool]") {
    sbe::ThreadPool pool;
    pool.InitThreads(2);

    std::vector<int> data = {1, 2, 3, 4, 5, 6};
    std::atomic<int> sum{0};

    std::function<void(int&)> job = [&sum](int& val) {
        sum.fetch_add(val, std::memory_order_relaxed);
    };

    pool.setVectorJob<int>(job);
    pool.runVectorJob(data);

    CHECK(sum.load() == 21);
}

TEST_CASE("ThreadPool can be reinitialized with a different thread count", "[ThreadPool]") {
    sbe::ThreadPool pool;
    pool.InitThreads(2);
    pool.InitThreads(4);   // should tear down the 2-thread pool and start 4

    std::vector<int> data(8, 1);
    std::atomic<int> count{0};

    std::function<void(int&)> job = [&count](int&) {
        count.fetch_add(1, std::memory_order_relaxed);
    };

    pool.setVectorJob<int>(job);
    pool.runVectorJob(data);

    CHECK(count.load() == 8);
}

TEST_CASE("ThreadPool custom job is invoked once per thread", "[ThreadPool]") {
    const unsigned int numThreads = 3;
    sbe::ThreadPool pool;
    pool.InitThreads(numThreads);

    std::atomic<int> callCount{0};

    std::function<void(boost::any&, int)> job = [&callCount](boost::any&, int) {
        callCount.fetch_add(1, std::memory_order_relaxed);
    };

    pool.setCustomJob(job);
    pool.runCustomJob(boost::any{});

    CHECK(callCount.load() == static_cast<int>(numThreads));
}
