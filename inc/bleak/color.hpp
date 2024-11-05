#pragma once

#include <bleak/typedef.hpp>

#include <format>
#include <string>

#include <bleak/hash.hpp>

extern "C" {
	typedef struct c_color_t {
		union {
			struct {
				// 8-bit red component
				bleak::u8 r{ 0x00 };
				// 8-bit green component
				bleak::u8 g{ 0x00 };
				// 8-bit blue component
				bleak::u8 b{ 0x00 };
				// 8-bit alpha component
				bleak::u8 a{ 0x00 };
			};

			// 32-bit packed color value
			unsigned int packed;
		};
	} c_color_t;

	static_assert(sizeof(c_color_t) == 4, "size of c_color_t must be four bytes");
}

namespace bleak {
	struct color_t : public c_color_t {
	  private:
		static constexpr u8 wrap_cast(f32 value) noexcept { return static_cast<u8>(static_cast<uhalf>(value * 255.0f) % 256); }

		static constexpr u8 wrap_cast(f64 value) noexcept { return static_cast<u8>(static_cast<usize>(value * 255.0f) % 256); }

	  public:
		constexpr color_t() noexcept {}

		constexpr color_t(u8 r, u8 g, u8 b, u8 a = u8{ 0xFF }) noexcept : c_color_t{ .r = r, .g = g, .b = b, .a = a } {}

		constexpr color_t(u8 rgb, u8 a = u8{ 0xFF }) noexcept : c_color_t{ .r = rgb, .g = rgb, .b = rgb, .a = a } {}

		constexpr color_t(f32 r, f32 g, f32 b, f32 a = 1.0f) noexcept :
			c_color_t{ .r = wrap_cast(r), .g = wrap_cast(g), .b = wrap_cast(b), .a = wrap_cast(a) } {}

		constexpr color_t(f64 r, f64 g, f64 b, f64 a = 1.0) noexcept :
			c_color_t{ .r = wrap_cast(r), .g = wrap_cast(g), .b = wrap_cast(b), .a = wrap_cast(a) } {}

		constexpr bool operator==(cref<color_t> other) const noexcept { return packed == other.packed; }

		constexpr bool operator!=(cref<color_t> other) const noexcept { return packed != other.packed; }

		constexpr bool operator<(cref<color_t> other) const noexcept = delete;
		constexpr bool operator<=(cref<color_t> other) const noexcept = delete;

		constexpr bool operator>(cref<color_t> other) const noexcept = delete;
		constexpr bool operator>=(cref<color_t> other) const noexcept = delete;

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

		constexpr explicit operator u32() const noexcept { return packed; }

		inline operator std::string() const noexcept { return std::format("[{}, {}, {}, {}]", r, g, b, a); }

		struct hasher {
			static constexpr usize operator()(cref<color_t> color) noexcept { return hash_combine(color.r, color.g, color.b, color.a); }
		};
	};
} // namespace bleak
