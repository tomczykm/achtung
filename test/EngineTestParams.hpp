#pragma once

#include <functional>
#include <tuple>
#include <utility>

#include "testable/EngineTestable.hpp"
#include "testable/InputSequence.hpp"

using SetupFunctor = std::function<void(EngineTestable&, InputSequence&)>;
using TestPreviewParam = std::pair<SetupFunctor, Ticks>;

constexpr std::size_t totalTestcases = 9;

extern const std::array<TestPreviewParam, totalTestcases> setupAndTicks;
