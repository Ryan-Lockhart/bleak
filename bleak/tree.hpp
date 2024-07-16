#pragma once

#include <bleak/typedef.hpp>

#include <initializer_list>
#include <utility>
#include <iterator>
#include <queue>
#include <stack>

namespace bleak {
	template<typename T, usize Branches> struct tree_t;
	template<typename T, usize Branches> struct branch_t;

	template<typename T> using bintree_t = tree_t<T, 2>;
	template<typename T> using binbranch_t = branch_t<T, 2>;

	template<typename T> using quadtree_t = tree_t<T, 4>;
	template<typename T> using quadbranch_t = branch_t<T, 4>;

	template<typename T> using octree_t = tree_t<T, 8>;
	template<typename T> using octbranch_t = branch_t<T, 8>;

	template<typename T, usize Branches> struct tree_t {
		branch_t<T, Branches> root;

		inline tree_t() noexcept = delete;

		inline tree_t(cref<T> value) noexcept : root { value } {}

		inline tree_t(rval<T> value) noexcept : root { std::move(value) } {}

		inline tree_t(cref<branch_t<T, Branches>> root) noexcept : root { root } {}

		inline tree_t(rval<branch_t<T, Branches>> root) noexcept : root { std::move(root) } {}

		inline tree_t(cref<T> value, std::initializer_list<branch_t<T, Branches>> branches) noexcept : root { value, branches } {}

		inline tree_t(rval<T> value, std::initializer_list<branch_t<T, Branches>> branches) noexcept : root { std::move(value), branches } {}

		inline tree_t(cref<branch_t<T, Branches>> root, std::initializer_list<branch_t<T, Branches>> branches) noexcept : root { root, branches } {}

		inline tree_t(rval<branch_t<T, Branches>> root, std::initializer_list<branch_t<T, Branches>> branches) noexcept : root { std::move(root), branches } {}

		inline ~tree_t() noexcept = default;

		inline ref<tree_t> operator=(cref<tree_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			root = other.root;
			return *this;
		}

		inline ref<tree_t> operator=(rval<tree_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			root = std::move(other.root);
			return *this;
		}
	};

	template<typename T, usize Branches> struct branch_t {
		static constexpr usize max_branches { Branches };

		ptr<branch_t<T, max_branches>> branches[max_branches];

		T value;

		inline branch_t() noexcept = delete;

		inline branch_t(cref<T> value) noexcept : value { value } {
			for (usize i { 0 }; i < max_branches; ++i) {
				branches[i] = nullptr;
			}
		}

		inline branch_t(rval<T> value) noexcept : value { std::move(value) } {
			for (usize i { 0 }; i < max_branches; ++i) {
				branches[i] = nullptr;
			}
		}

		inline branch_t(cref<branch_t<T, Branches>> other) : value { other.value } {
			for (usize i { 0 }; i < max_branches; ++i) {
				branches[i] = other.branches[i] ? new branch_t<T, max_branches> { *other.branches[i] } : nullptr;
			}
		}

		inline branch_t(rval<branch_t<T, Branches>> other) noexcept : value { std::move(other.value) } {
			for (usize i { 0 }; i < max_branches; ++i) {
				branches[i] = std::move(other.branches[i]);
				other.branches[i] = nullptr;
			}
		}

		inline branch_t(cref<T> value, std::initializer_list<branch_t<T, Branches>> elements) : value { value } {
			for (usize i { 0 }; i < max_branches; ++i) {
				branches[i] = i >= elements.size() ? nullptr : new branch_t<T, max_branches> { *(elements.begin() + i) };
			}
		}

		inline branch_t(rval<T> value, std::initializer_list<branch_t<T, Branches>> elements) : value { std::move(value) } {
			for (usize i { 0 }; i < max_branches; ++i) {
				branches[i] = i >= elements.size() ? nullptr : new branch_t<T, max_branches> { *(elements.begin() + i) };
			}
		}

		inline ~branch_t() noexcept {
			for (usize i { 0 }; i < max_branches; ++i) {
				if (branches[i]) {
					delete branches[i];
					branches[i] = nullptr;
				}
			}
		}

		inline ref<branch_t> operator=(cref<branch_t> other) {
			if (this == &other) {
				return *this;
			}

			value = other.value;

			for (usize i { 0 }; i < max_branches; ++i) {
				if (branches[i]) {
					delete branches[i];
					branches[i] = nullptr;
				}

				branches[i] = other.branches[i] ? new branch_t<T, max_branches> { *other.branches[i] } : nullptr;
			}

			return *this;
		}

		inline ref<branch_t> operator=(rval<branch_t> other) noexcept {
			if (this == &other) {
				return *this;
			}

			value = std::move(other.value);

			for (usize i { 0 }; i < max_branches; ++i) {
				if (branches[i]) {
					delete branches[i];
					branches[i] = nullptr;
				}

				branches[i] = std::move(other.branches[i]);
				other.branches[i] = nullptr;
			}

			return *this;
		}

		ptr<branch_t<T, max_branches>> operator[](usize index) noexcept {
			if (index >= max_branches) {
				return nullptr;
			}

			return branches[index];
		}

		cptr<branch_t<T, max_branches>> operator[](usize index) const noexcept {
			if (index >= max_branches) {
				return nullptr;
			}

			return branches[index];
		}

		inline ptr<branch_t<T, max_branches>> create(cref<T> value, usize index) noexcept {
			if (index >= max_branches) {
				return nullptr;
			}

			if (!branches[index]) {
				branches[index] = new branch_t<T, max_branches> { value };
				return branches[index];
			}

			return nullptr;
		}

		inline ptr<branch_t<T, max_branches>> create(rval<T> value, usize index) noexcept {
			if (index >= max_branches) {
				return nullptr;
			}

			if (!branches[index]) {
				branches[index] = new branch_t<T, max_branches> { std::move(value) };
				return branches[index];
			}

			return nullptr;
		}

		inline bool destroy(usize index) noexcept {
			if (index >= max_branches) {
				return false;
			}

			if (branches[index]) {
				delete branches[index];
				branches[index] = nullptr;

				return true;
			}

			return false;
		}

		inline bool has_branch(u8 index) const noexcept { return this[index] != nullptr; }

		inline bool is_leaf() const noexcept {
			for (usize i { 0 }; i < max_branches; ++i) {
				if (branches[i]) {
					return false;
				}
			}

			return true;
		}
	};

	enum class search_type_t { BreadthFirst, DepthFirst };

	template<typename T, usize Branches, search_type_t Search> struct tree_iter_t;

	template<typename T, search_type_t Search> using bintree_iter_t = tree_iter_t<T, 2, Search>;

	template<typename T> using bintree_bfs_t = bintree_iter_t<T, search_type_t::BreadthFirst>;
	template<typename T> using bintree_dfs_t = bintree_iter_t<T, search_type_t::DepthFirst>;

	template<typename T, search_type_t Search> using quadtree_iter_t = tree_iter_t<T, 4, Search>;

	template<typename T> using quadtree_bfs_t = tree_iter_t<T, 4, search_type_t::BreadthFirst>;
	template<typename T> using quadtree_dfs_t = tree_iter_t<T, 4, search_type_t::DepthFirst>;

	template<typename T, search_type_t Search> using octree_iter_t = tree_iter_t<T, 8, Search>;

	template<typename T> using octree_bfs_t = tree_iter_t<T, 8, search_type_t::BreadthFirst>;
	template<typename T> using octree_dfs_t = tree_iter_t<T, 8, search_type_t::DepthFirst>;

	template<typename T, usize Branches> struct tree_iter_t<T, Branches, search_type_t::BreadthFirst> {
	  public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = branch_t<T, Branches>;
		using pointer = ptr<value_type>;
		using reference = ref<value_type>;

	  private:
		std::queue<pointer> queue;

	  public:
		constexpr tree_iter_t() = delete;

		constexpr tree_iter_t(pointer handle) : queue {} {
			if (handle) {
				queue.push(handle);
			}
		}

		constexpr tree_iter_t(cref<tree_iter_t> other) : queue { other.queue } {}

		constexpr tree_iter_t(rval<tree_iter_t> other) : queue { std::move(other.queue) } {}

		constexpr ref<tree_iter_t> operator=(cref<tree_iter_t> other) {
			if (this == &other) {
				return *this;
			}

			queue = other.queue;
			return *this;
		}

		constexpr ref<tree_iter_t> operator=(rval<tree_iter_t> other) {
			if (this == &other) {
				return *this;
			}

			queue = std::move(other.queue);
			return *this;
		}

		constexpr ~tree_iter_t() {};

		constexpr reference operator*() const noexcept { return *queue.front(); }

		constexpr pointer operator->() const noexcept { return queue.front(); }

		constexpr pointer operator++() noexcept {
			auto current = queue.front();
			queue.pop();

			for (usize i { 0 }; i < Branches; ++i) {
				if (current->branches[i]) {
					queue.push(current->branches[i]);
				}
			}

			return peek();
		}

		constexpr pointer operator++(int) noexcept {
			auto current = queue.front();
			queue.pop();

			for (usize i { 0 }; i < Branches; ++i) {
				if (current->branches[i]) {
					queue.push(current->branches[i]);
				}
			}

			return current;
		}

		constexpr pointer peek() const noexcept {
			if (queue.empty()) {
				return nullptr;
			}

			return queue.front();
		}

		constexpr bool operator==(cref<tree_iter_t> other) const noexcept { return peek() == other.peek(); }

		constexpr bool operator!=(cref<tree_iter_t> other) noexcept { return peek() != other.peek(); }
	};

	template<typename T, usize Branches> struct tree_iter_t<T, Branches, search_type_t::DepthFirst> {
	  public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type = std::ptrdiff_t;
		using value_type = branch_t<T, Branches>;
		using pointer = ptr<value_type>;
		using reference = ref<value_type>;

	  private:
		std::stack<pointer> stack;

	  public:
		constexpr tree_iter_t() = delete;

		constexpr tree_iter_t(pointer handle) : stack {} {
			if (handle) {
				stack.push(handle);
			}
		}

		constexpr tree_iter_t(cref<tree_iter_t> other) : stack { other.stack } {}

		constexpr tree_iter_t(rval<tree_iter_t> other) : stack { std::move(other.stack) } {}

		constexpr ref<tree_iter_t> operator=(cref<tree_iter_t> other) {
			if (this == &other) {
				return *this;
			}

			stack = other.stack;
			return *this;
		}

		constexpr ref<tree_iter_t> operator=(rval<tree_iter_t> other) {
			if (this == &other) {
				return *this;
			}

			stack = std::move(other.stack);
			return *this;
		}

		constexpr ~tree_iter_t() {};

		constexpr reference operator*() const noexcept { return *stack.top(); }

		constexpr pointer operator->() const noexcept { return stack.top(); }

		constexpr pointer operator++() noexcept {
			auto current = stack.top();
			stack.pop();

			usize i { Branches - 1 };
			do {
				if (current->branches[i]) {
					stack.push(current->branches[i]);
				}
				--i;
			} while (i >= 0 && i < Branches);

			return peek();
		}

		constexpr pointer operator++(int) noexcept {
			auto current = stack.top();
			stack.pop();

			usize i { Branches - 1 };
			do {
				if (current->branches[i]) {
					stack.push(current->branches[i]);
				}
				--i;
			} while (i >= 0 && i < Branches);

			return current;
		}

		constexpr pointer peek() const noexcept {
			if (stack.empty()) {
				return nullptr;
			}

			return stack.top();
		}

		constexpr bool operator==(cref<tree_iter_t> other) const noexcept { return peek() == other.peek(); }

		constexpr bool operator!=(cref<tree_iter_t> other) noexcept { return peek() != other.peek(); }
	};
} // namespace Bleakdepth
