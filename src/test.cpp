#include "tests.hpp"

#include <iostream>

#include "tree.hpp"

using namespace Bleakdepth;

void test_trees() {
    bintree_test();
    quadtree_test();
    octree_test();
}

void bintree_test() {
	std::cout << "Binbranch Test:" << std::endl;
	bintree_t<int> tree {
		binbranch_t<int> {
			0,
			std::initializer_list<binbranch_t<int>> {
				binbranch_t<int> {
					1, std::initializer_list<binbranch_t<int>> {
						binbranch_t<int> { 3 },
						binbranch_t<int> { 4 }
					}
				},
				binbranch_t<int> { 
					2, std::initializer_list<binbranch_t<int>> {
						binbranch_t<int> { 5 },
						binbranch_t<int> { 6 }
					}
				}
			}
		}
	};

	std::cout << "Breadth First Search:" << std::endl;

	for (bintree_bfs_t<int> iter { &tree.root }; iter != nullptr; ++iter) {
		std::cout << iter->value << std::endl;
	}

	std::cout << "\nDepth First Search:" << std::endl;

	for (bintree_dfs_t<int> iter { &tree.root }; iter != nullptr; ++iter) {
		std::cout << iter->value << std::endl;
	}
}

void quadtree_test() {
	std::cout << "Quadtree Test:" << std::endl;

	quadtree_t<int> tree {
		quadbranch_t<int> {
			0,
			std::initializer_list<quadbranch_t<int>> {
				quadbranch_t<int> {
					1, std::initializer_list<quadbranch_t<int>> {
						quadbranch_t<int> { 5 },
						quadbranch_t<int> { 6 },
						quadbranch_t<int> { 7 },
						quadbranch_t<int> { 8 }
					}
				},
				quadbranch_t<int> {
					2, std::initializer_list<quadbranch_t<int>> {
						quadbranch_t<int> { 9 },
						quadbranch_t<int> { 10 },
						quadbranch_t<int> { 11 },
						quadbranch_t<int> { 12 }
					}
				},
				quadbranch_t<int> {
					3, std::initializer_list<quadbranch_t<int>> {
						quadbranch_t<int> { 13 },
						quadbranch_t<int> { 14 },
						quadbranch_t<int> { 15 },
						quadbranch_t<int> { 16 }
					}
				},
				quadbranch_t<int> {
					4, std::initializer_list<quadbranch_t<int>> {
						quadbranch_t<int> { 17 },
						quadbranch_t<int> { 18 },
						quadbranch_t<int> { 19 },
						quadbranch_t<int> { 20 }
					}
				}
			}
		}
	};

	std::cout << "Breadth First Search:" << std::endl;

	for (quadtree_bfs_t<int> iter { &tree.root }; iter != nullptr; ++iter) {
		std::cout << iter->value << std::endl;
	}

	std::cout << "\nDepth First Search:" << std::endl;

	for (quadtree_dfs_t<int> iter { &tree.root }; iter != nullptr; ++iter) {
		std::cout << iter->value << std::endl;
	}
}

void octree_test() {
    std::cout << "Octree Test:" << std::endl;

    octree_t<int> tree {
        octbranch_t<int> {
            0,
            std::initializer_list<octbranch_t<int>> {
                octbranch_t<int> {
                    1, std::initializer_list<octbranch_t<int>> {
                        octbranch_t<int> { 9 },
                        octbranch_t<int> { 10 },
                        octbranch_t<int> { 11 },
                        octbranch_t<int> { 12 },
                        octbranch_t<int> { 13 },
                        octbranch_t<int> { 14 },
                        octbranch_t<int> { 15 },
                        octbranch_t<int> { 16 }
                    }
                },
                octbranch_t<int> {
                    2, std::initializer_list<octbranch_t<int>> {
                        octbranch_t<int> { 17 },
                        octbranch_t<int> { 18 },
                        octbranch_t<int> { 19 },
                        octbranch_t<int> { 20 },
                        octbranch_t<int> { 21 },
                        octbranch_t<int> { 22 },
                        octbranch_t<int> { 23 },
                        octbranch_t<int> { 24 }
                    }
                },
                octbranch_t<int> {
                    3, std::initializer_list<octbranch_t<int>> {
                        octbranch_t<int> { 25 },
                        octbranch_t<int> { 26 },
                        octbranch_t<int> { 27 },
                        octbranch_t<int> { 28 },
                        octbranch_t<int> { 29 },
                        octbranch_t<int> { 30 },
                        octbranch_t<int> { 31 },
                        octbranch_t<int> { 32 }
                    }
                },
                octbranch_t<int> {
                    4, std::initializer_list<octbranch_t<int>> {
                        octbranch_t<int> { 33 },
                        octbranch_t<int> { 34 },
                        octbranch_t<int> { 35 },
                        octbranch_t<int> { 36 },
                        octbranch_t<int> { 37 },
                        octbranch_t<int> { 38 },
                        octbranch_t<int> { 39 },
                        octbranch_t<int> { 40 }
                    }
                },
                octbranch_t<int> {
                    5, std::initializer_list<octbranch_t<int>> {
                        octbranch_t<int> { 41 },
                        octbranch_t<int> { 42 },
                        octbranch_t<int> { 43 },
                        octbranch_t<int> { 44 },
                        octbranch_t<int> { 45 },
                        octbranch_t<int> { 46 },
                        octbranch_t<int> { 47 },
                        octbranch_t<int> { 48 }
                    }
                },
                octbranch_t<int> {
                    6, std::initializer_list<octbranch_t<int>> {
                        octbranch_t<int> { 49 },
                        octbranch_t<int> { 50 },
                        octbranch_t<int> { 51 },
                        octbranch_t<int> { 52 },
                        octbranch_t<int> { 53 },
                        octbranch_t<int> { 54 },
                        octbranch_t<int> { 55 },
                        octbranch_t<int> { 56 }
                    }
                },
                octbranch_t<int> {
                    7, std::initializer_list<octbranch_t<int>> {
                        octbranch_t<int> { 57 },
                        octbranch_t<int> { 58 },
                        octbranch_t<int> { 59 },
                        octbranch_t<int> { 60 },
                        octbranch_t<int> { 61 },
                        octbranch_t<int> { 62 },
                        octbranch_t<int> { 63 },
                        octbranch_t<int> { 64 }
                    }
                },
                octbranch_t<int> {
                    8, std::initializer_list<octbranch_t<int>> {
                        octbranch_t<int> { 65 },
                        octbranch_t<int> { 66 },
                        octbranch_t<int> { 67 },
                        octbranch_t<int> { 68 },
                        octbranch_t<int> { 69 },
                        octbranch_t<int> { 70 },
                        octbranch_t<int> { 71 },
                        octbranch_t<int> { 72 }
                    }
                }
            }
        }
    };

    std::cout << "Breadth First Search:" << std::endl;

    for (octree_bfs_t<int> iter { &tree.root }; iter != nullptr; ++iter) {
        std::cout << iter->value << std::endl;
    }

    std::cout << "\nDepth First Search:" << std::endl;

    for (octree_dfs_t<int> iter { &tree.root }; iter != nullptr; ++iter) {
        std::cout << iter->value << std::endl;
    }
}
