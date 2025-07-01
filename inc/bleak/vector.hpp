#pragma once

#include <bleak/typedef.hpp>

#include <bleak/concepts.hpp>
#include <bleak/utility.hpp>

namespace bleak {
	template<Numeric T, usize Size> using gcc_vec __attribute__((__vector_size__(n_of<T>(Size)))) = T;
	template<Numeric T, usize Size> using cl_vec __attribute__((__ext_vector_type__(Size))) = T;

	template<Numeric T, usize Size> using vec = gcc_vec<T, Size>;

	template<Numeric T> using vec2 = vec<T, 2>;
	template<Numeric T> using vec3 = vec<T, 3>;
	template<Numeric T> using vec4 = vec<T, 4>;

	using vec2i = vec2<ihalf>;
	using vec2u = vec2<uhalf>;
	using vec2f = vec2<fhalf>;

	using vec3i = vec3<ihalf>;
	using vec3u = vec3<uhalf>;
	using vec3f = vec3<fhalf>;

	using vec4i = vec4<ihalf>;
	using vec4u = vec4<uhalf>;
	using vec4f = vec4<fhalf>;
} // namespace bleak
