#pragma once

#include <functional>
#include <tuple>
#include <utility>

#include "testable/EngineTestable.hpp"
#include "testable/InputSequence.hpp"

using SetupFunctor = std::function<void(EngineTestable&, InputSequence&)>;
using AssertFunctor = std::function<void(EngineTestable&)>;

using EngineTestParam = std::tuple<SetupFunctor, AssertFunctor, Ticks>;
using TestPreviewParam = std::pair<SetupFunctor, Ticks>;

constexpr std::size_t totalTestcases = 3;

extern const std::array<TestPreviewParam, totalTestcases> setupAndTicks;
