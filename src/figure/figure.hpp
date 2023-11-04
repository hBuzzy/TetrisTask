#ifndef FIGURE_HPP
#define FIGURE_HPP

#include <QObject>
#include <QColor>
#include <QVarLengthArray>

#include <random>
#include <concepts>
#include <ranges>

class Figure final {
public:
    using pos_t = QVarLengthArray<std::pair<uint, uint>, 4>;
public:
    Figure() = default;
    Figure(uint preset, uint rotation, const QColor& color)
    : preset_(preset), rotation_(rotation), color_(color) {};
public:
    [[nodiscard]] Figure Rotated() const {
        return { preset_, (rotation_ + 1) % 4, color_ };
    }
    void Rotate() {
        rotation_ = (rotation_ + 1) % 4;
    }
    [[nodiscard]] auto GetGeometry() const {
        return presets_[preset_][rotation_];
    }
    [[nodiscard]] auto GetColor() const {
        return color_;
    }
    [[nodiscard]] uint GetLeft() const {
        return std::ranges::min(
            presets_[preset_][rotation_], {},
            std::_Select2nd<std::pair<uint, uint>>{}
        ).second;
    }
    [[nodiscard]] uint GetRight() const {
        return std::ranges::max(
            presets_[preset_][rotation_], {},
            std::_Select2nd<std::pair<uint, uint>>{}
        ).second;
    }
    [[nodiscard]] uint GetTop() const {
        return std::ranges::min(
            presets_[preset_][rotation_], {},
            std::_Select1st<std::pair<uint, uint>>{}
        ).first;
    }
    [[nodiscard]] uint GetBottom() const {
        return std::ranges::max(
            presets_[preset_][rotation_], {},
            std::_Select1st<std::pair<uint, uint>>{}
        ).first;
    }
private:
    static const QVarLengthArray<QVarLengthArray<pos_t, 4>, 5> presets_;
    uint preset_ = 0, rotation_ = 0;
    QColor color_;
};

#endif
