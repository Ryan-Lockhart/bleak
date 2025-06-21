#pragma once

#include <bleak/typedef.hpp>

#include <format>
#include <string>

#include <bleak/concepts.hpp>
#include <bleak/hash.hpp>

namespace bleak {
	template<typename... Values>
		requires is_plurary<Values...>::value && is_homogeneous<u8, Values...>::value
	static constexpr u8 mix(Values... values) noexcept {
		return static_cast<u8>((static_cast<u16>(values) + ...) / sizeof...(Values));
	}

	struct color_t {
	  private:
		static constexpr u8 wrap_cast(f32 value) noexcept { return static_cast<u8>(static_cast<uhalf>(value * 255.0f) % 256); }

		static constexpr u8 wrap_cast(f64 value) noexcept { return static_cast<u8>(static_cast<usize>(value * 255.0f) % 256); }

	  public:
	  	u8 r{ 0x00 };
		u8 g{ 0x00 };
		u8 b{ 0x00 };
		u8 a{ 0x00 };

		constexpr color_t() noexcept {}

		constexpr color_t(cref<color_t> other) noexcept : r{ other.r }, g{ other.g }, b{ other.b }, a{ other.a } {}
		constexpr color_t(cref<color_t> rgb, u8 a) noexcept : r{ rgb.r }, g{ rgb.g }, b{ rgb.b }, a{ a } {}

		constexpr color_t(rval<color_t> other) noexcept : r{ std::move(other.r) }, g{ std::move(other.g) }, b{ std::move(other.b) }, a{ std::move(other.a) } {}
		constexpr color_t(rval<color_t> rgb, u8 a) noexcept : r{ std::move(rgb.r) }, g{ std::move(rgb.g) }, b{ std::move(rgb.b) }, a{ a } {}

		constexpr color_t(u8 r, u8 g, u8 b, u8 a = u8{ 0xFF }) noexcept : r{ r }, g{ g }, b{ b }, a{ a } {}

		constexpr color_t(u8 rgb, u8 a = u8{ 0xFF }) noexcept : r{ rgb }, g{ rgb }, b{ rgb }, a{ a } {}

		constexpr color_t(f32 r, f32 g, f32 b, f32 a = 1.0f) noexcept : r{ wrap_cast(r) }, g{ wrap_cast(g) }, b{ wrap_cast(b) }, a{ wrap_cast(a) } {}

		constexpr color_t(f32 rgb, f32 a = 1.0f) noexcept : r{ wrap_cast(rgb) }, g{ wrap_cast(rgb) }, b{ wrap_cast(rgb) }, a{ wrap_cast(a) } {}

		constexpr color_t(f64 r, f64 g, f64 b, f64 a = 1.0) noexcept : r{ wrap_cast(r) }, g{ wrap_cast(g) }, b{ wrap_cast(b) }, a{ wrap_cast(a) } {}

		constexpr color_t(f64 rgb, f64 a = 1.0) noexcept : r{ wrap_cast(rgb) }, g{ wrap_cast(rgb) }, b{ wrap_cast(rgb) }, a{ wrap_cast(a) } {}

		constexpr ref<color_t> operator=(cref<color_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			r = other.r;
			g = other.g;
			b = other.b;
			a = other.a;

			return *this;
		}

		constexpr ref<color_t> operator=(rval<color_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			r = std::move(other.r);
			g = std::move(other.g);
			b = std::move(other.b);
			a = std::move(other.a);

			return *this;
		}

		constexpr bool operator==(color_t other) const noexcept { return r == other.r && g == other.g && b == other.b && a == other.a; }

		constexpr bool operator!=(color_t other) const noexcept { return r != other.r || g != other.g || b != other.b || a != other.a; }

		constexpr bool operator<(color_t other) const noexcept = delete;
		constexpr bool operator<=(color_t other) const noexcept = delete;

		constexpr bool operator>(color_t other) const noexcept = delete;
		constexpr bool operator>=(color_t other) const noexcept = delete;

		constexpr void set_rgb(f32 red, f32 green, f32 blue) noexcept {
			r = wrap_cast(red);
			g = wrap_cast(green);
			b = wrap_cast(blue);
		}

		constexpr void set_rgb(f64 red, f64 green, f64 blue) noexcept {
			r = wrap_cast(red);
			g = wrap_cast(green);
			b = wrap_cast(blue);
		}

		constexpr void set_alpha(f32 alpha) noexcept { a = wrap_cast(alpha); }

		constexpr void set_alpha(f64 alpha) noexcept { a = wrap_cast(alpha); }

		constexpr color_t dimmed(f32 factor) const noexcept {
			if (factor < 0.0f) {
				return *this;
			}
			
			return color_t{ static_cast<u8>(r * factor), static_cast<u8>(g * factor), static_cast<u8>(b * factor), a };
		}

		constexpr ref<color_t> dim(f32 factor) noexcept {
			if (factor < 0.0f) {
				return *this;
			}

			r = static_cast<u8>(r * factor);
			g = static_cast<u8>(g * factor);
			b = static_cast<u8>(b * factor);

			return *this;
		}

		constexpr ref<color_t> mix(color_t other) noexcept {
			if (this == &other) {
				return *this;
			}

			r = bleak::mix(r, other.r);
			g = bleak::mix(g, other.g);
			b = bleak::mix(b, other.b);
			a = bleak::mix(a, other.a);

			return *this;
		}

		template<typename... Colors>
			requires is_plurary<Colors...>::value && is_homogeneous<color_t, Colors...>::value
		constexpr ref<color_t> mix(Colors... others) noexcept {
			r = bleak::mix(r, others.r...);
			g = bleak::mix(g, others.g...);
			b = bleak::mix(b, others.b...);
			a = bleak::mix(a, others.a...);

			return *this;
		}

		inline operator std::string() const noexcept { return std::format("[{}, {}, {}, {}]", r, g, b, a); }

		struct hasher {
			static constexpr usize operator()(color_t color) noexcept { return hash_combine(color.r, color.g, color.b, color.a); }
		};
	};

	template<typename... Colors>
		requires is_plurary<Colors...>::value && is_homogeneous<color_t, Colors...>::value
	static constexpr color_t mix(Colors... colors) noexcept {
		return color_t{ bleak::mix(colors.r...), bleak::mix(colors.g...), bleak::mix(colors.b...), bleak::mix(colors.a...) };
	}
} // namespace bleak
