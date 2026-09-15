#pragma once

#include <functional>
#include <optional>
#include <expected>

#include <QString>


template <typename T>
using optional = std::optional<T>;

template <typename T>
using ref = std::reference_wrapper<T>;

template <typename T>
using optional_ref = optional<ref<T>>;

template <typename T = void>
using result_t = std::expected<T, QString>;