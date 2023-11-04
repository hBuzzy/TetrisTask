#ifndef FIGURE_GENERATOR_HPP
#define FIGURE_GENERATOR_HPP

#include <QObject>
#include <QColor>
#include <QVarLengthArray>

#include <random>
#include <concepts>
#include <execution>

#include "figure.hpp"

template <std::uniform_random_bit_generator RandGen = std::mt19937, std::size_t c = 7>
class FigureGenerator final {
public:
    explicit FigureGenerator(
        RandGen&& randGen = std::mt19937{std::random_device{}()},
        const QVarLengthArray<QColor, c>& colors = {
            {236, 39, 51},
            {246, 149, 34},
            {251, 224, 0},
            {78, 182, 72},
            {43, 171, 225},
            {0, 90, 156},
            {145, 43, 139}
        }
    ) : randGen_{std::forward<RandGen>(randGen)}, colors_(colors) {};
public:
    [[nodiscard]] Figure operator()() {
        return { randGen_() % 5, randGen_() % 4, colors_[randGen_() % c] };
    };
private:
    RandGen randGen_;
    const QVarLengthArray<QColor, c> colors_;
};

template <std::uniform_random_bit_generator RandGen, std::size_t c>
FigureGenerator(RandGen&&, const QVarLengthArray<QColor, c>&) -> FigureGenerator<RandGen, c>;

#endif
