#include "typedef.hpp"

#include <raylib.h>
#include <fmt/format.h>

#include <cmath>
#include <algorithm>
#include <random>

#include "log.hpp"
#include "point.hpp"
#include "line.hpp"
#include "path.hpp"

constexpr cstr GAME_TITLE = "Bleakdepth";
constexpr cstr GAME_VERSION = "0.0.1";

constexpr i32 WINDOW_WIDTH = 1280;
constexpr i32 WINDOW_HEIGHT = 480;

constexpr f32 WINDOW_ASPECT_RATIO = static_cast<f32>(WINDOW_WIDTH) / static_cast<f32>(WINDOW_HEIGHT);

constexpr i32 WINDOW_ORIGIN_X = 0;
constexpr i32 WINDOW_ORIGIN_Y = 0;

constexpr i32 WINDOW_EXTENT_X = WINDOW_WIDTH - 1;
constexpr i32 WINDOW_EXTENT_Y = WINDOW_HEIGHT - 1;

constexpr i32 WINDOW_CENTER_X = WINDOW_WIDTH / 2;
constexpr i32 WINDOW_CENTER_Y = WINDOW_HEIGHT / 2;

constexpr i32 UI_WIDTH = 320;
constexpr i32 UI_HEIGHT = WINDOW_HEIGHT;

constexpr i32 UI_ORIGIN_X = WINDOW_WIDTH - UI_WIDTH;
constexpr i32 UI_ORIGIN_Y = 0;

constexpr i32 UI_EXTENT_X = WINDOW_WIDTH - 1;
constexpr i32 UI_EXTENT_Y = WINDOW_HEIGHT - 1;

constexpr i32 UI_CENTER_X = UI_ORIGIN_X + UI_WIDTH / 2;
constexpr i32 UI_CENTER_Y = WINDOW_CENTER_Y;

constexpr i32 GAME_WIDTH = WINDOW_WIDTH - UI_WIDTH;
constexpr i32 GAME_HEIGHT = WINDOW_HEIGHT;

constexpr i32 GAME_CENTER_X = GAME_WIDTH / 2;
constexpr i32 GAME_CENTER_Y = WINDOW_CENTER_Y;

constexpr i32 CELL_WIDTH = 12;
constexpr i32 CELL_HEIGHT = 12;

constexpr i32 UI_CELL_WIDTH = 8;
constexpr i32 UI_CELL_HEIGHT = 8;

constexpr i32 UI_GRID_WIDTH = UI_WIDTH / UI_CELL_WIDTH;
constexpr i32 UI_GRID_HEIGHT = UI_HEIGHT / UI_CELL_HEIGHT;

constexpr i32 UI_GRID_ORIGIN_X = UI_ORIGIN_X / UI_CELL_WIDTH;
constexpr i32 UI_GRID_ORIGIN_Y = 0;

constexpr i32 UI_GRID_EXTENT_X = UI_GRID_ORIGIN_X + UI_GRID_WIDTH - 1;
constexpr i32 UI_GRID_EXTENT_Y = UI_GRID_HEIGHT - 1;

constexpr i32 UI_GRID_CENTER_X = UI_GRID_ORIGIN_X + UI_GRID_WIDTH / 2;
constexpr i32 UI_GRID_CENTER_Y = UI_GRID_HEIGHT / 2;

constexpr i32 GRID_WIDTH = GAME_WIDTH / CELL_WIDTH;
constexpr i32 GRID_HEIGHT = GAME_HEIGHT / CELL_HEIGHT;

constexpr i32 GRID_ORIGIN_X = 0;
constexpr i32 GRID_ORIGIN_Y = 0;

constexpr i32 GRID_EXTENT_X = GRID_WIDTH - 1;
constexpr i32 GRID_EXTENT_Y = GRID_HEIGHT - 1;

constexpr i32 GRID_CENTER_X = GRID_WIDTH / 2;
constexpr i32 GRID_CENTER_Y = GRID_HEIGHT / 2;

constexpr usize GRID_SIZE = GRID_WIDTH * GRID_HEIGHT;

constexpr i32 MAP_WIDTH = 256;
constexpr i32 MAP_HEIGHT = 256;
constexpr i32 MAP_DEPTH = 1;

constexpr usize MAP_AREA = MAP_WIDTH * MAP_HEIGHT;
constexpr usize MAP_VOLUME = MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH;

constexpr i32 MAP_ORIGIN_X = 0;
constexpr i32 MAP_ORIGIN_Y = 0;
constexpr i32 MAP_ORIGIN_Z = 0;

constexpr i32 MAP_EXTENT_X = MAP_WIDTH - 1;
constexpr i32 MAP_EXTENT_Y = MAP_HEIGHT - 1;
constexpr i32 MAP_EXTENT_Z = MAP_DEPTH - 1;

constexpr i32 MAP_CENTER_X = MAP_WIDTH / 2;
constexpr i32 MAP_CENTER_Y = MAP_HEIGHT / 2;
constexpr i32 MAP_CENTER_Z = MAP_DEPTH / 2;

constexpr i32 MAP_BORDER_SIZE = 2;

static std::vector<bool> SOLID{ MAP_VOLUME, false, std::allocator<bool>{ } };
static std::vector<bool> OPAQUE{ MAP_VOLUME, false, std::allocator<bool>{ } };

static std::vector<bool> EXPLORED{ MAP_VOLUME, false, std::allocator<bool>{ } };
static std::vector<bool> VISIBLE{ MAP_VOLUME, false, std::allocator<bool>{ } };
static std::vector<bool> LIT{ MAP_VOLUME, false, std::allocator<bool>{ } };

static std::vector<bool> BLOODY{ MAP_VOLUME, false, std::allocator<bool>{ } };

static Log MESSAGES;
static Log ERRORS;

constexpr i32 HORIZONTAL_TAB_SIZE = 4;
constexpr i32 VERTICAL_TAB_SIZE = 4;

static Texture2D* UI_GLYPHS = nullptr;
static Texture2D* GAME_GLYPHS = nullptr;

static Texture2D* CURSOR = nullptr;

#ifndef NDEBUG
static bool SHOW_ERRORS = false;
#endif

static i32 MOUSE_X = 0;
static i32 MOUSE_Y = 0;

static i32 CURSOR_X = 0;
static i32 CURSOR_Y = 0;

static i32 UI_CURSOR_X = 0;
static i32 UI_CURSOR_Y = 0;

static bool CURSOR_INSIDE_WINDOW = false;

static bool CURSOR_INSIDE_GAME = false;
static bool CURSOR_INSIDE_UI = false;

static i32 PLAYER_X = 0;
static i32 PLAYER_Y = 0;

static bool PLAYER_ACTED = false;

static i32 CAMERA_X = 0;
static i32 CAMERA_Y = 0;

static i32 MAP_CURSOR_X = 0;
static i32 MAP_CURSOR_Y = 0;

static bool CURSOR_INSIDE_MAP = false;

static bool CAMERA_LOCKED = true;

static f64 FREQUENCY = 1.0;
static f64 AMPLITUDE = 0.5;
static f64 PHASE = 1.0;

static f64 SINE_VALUE = 0.0;

static std::mt19937 RNG{ std::random_device{ }() };

static std::uniform_int_distribution<i32> MAP_X_DIST{ MAP_ORIGIN_X, MAP_EXTENT_X };
static std::uniform_int_distribution<i32> MAP_Y_DIST{ MAP_ORIGIN_Y, MAP_EXTENT_Y };

static constexpr Octant OCTANTS[]
{
	Octant{  0,  1,  1,  0 },
	Octant{  1,  0,  0,  1 },
	Octant{  0, -1,  1,  0 },
	Octant{ -1,  0,  0,  1 },
	Octant{  0, -1, -1,  0 },
	Octant{ -1,  0,  0, -1 },
	Octant{  0,  1, -1,  0 },
	Octant{  1,  0,  0, -1 }
};

static usize GetIndex(cref<Point> p) { return p.x + (usize)p.y * MAP_WIDTH; }
static usize GetIndex(i32 x, i32 y) { return x + (usize)y * MAP_WIDTH; }

static bool IsSolid(cref<Point> p) { return SOLID[GetIndex(p)]; }
static bool IsSolid(i32 x, i32 y) { return SOLID[GetIndex(x, y)]; }

static Point FindOpen()
{
	Point result;

	usize iterations{ 0 };

	do
	{
		++iterations;
		result = { MAP_X_DIST(RNG), MAP_Y_DIST(RNG) };
	}
	while (IsSolid(result) && iterations < MAP_VOLUME);

	if (iterations >= MAP_VOLUME)
	{
		ERRORS.log("Failed to find an open space.", __TIME__, __FILE__, __LINE__);
		return { -1, -1 };
	};

	return result;
}

static bool IsBorder(cref<Point> p);

static void ShadowCast(cref<Point> origin, ref<std::vector<Point>> fov, f32 row, f32 start, f32 end, cref<Octant> octant, f32 radius)
{
	f32 newStart = 0;

	if (start < end)
		return;

	bool blocked = false;

	for (f32 distance = row; distance <= radius && distance < MAP_AREA && !blocked; distance++)
	{
		f32 deltaY = -distance;

		for (f32 deltaX = -distance; deltaX <= 0.0f; deltaX++)
		{
			Point position{ (i32)(origin.x + deltaX * octant.x + deltaY * octant.dx), (i32)(origin.y + deltaX * octant.y + deltaY * octant.dy) };

			f32 leftSlope = (deltaX - 0.5f) / (deltaY + 0.5f);
			f32 rightSlope = (deltaX + 0.5f) / (deltaY - 0.5f);

			if (IsBorder(position) || start < rightSlope)
				continue;
			if (end > leftSlope)
				break;
			
			f32 deltaRadius = std::hypotf(deltaX, deltaY);

			if (deltaRadius <= radius)
				fov.push_back(position);

			usize index{ GetIndex(position) };

			if (blocked)
			{
				if (OPAQUE[index])
					newStart = rightSlope;
				else
				{
					blocked = false;
					start = newStart;
				}
			}
			else if (OPAQUE[index] && distance < radius)
			{
				blocked = true;

				ShadowCast(origin, fov, (int)distance + 1.0f, start, leftSlope, octant, radius);

				newStart = rightSlope;
			}
		}
	}
}

static std::vector<Point> CalculateFOV(cref<Point> origin, f32 viewDistance)
{
	std::vector<Point> fov{};

	viewDistance = std::max(1.0f, viewDistance);

	for (i32 j{ -1 }; j <= 1; ++j)
		for (i32 i{ -1 }; i <= 1; ++i)
			fov.push_back({ origin.x + i, origin.y + j });

	for (int i{ 0 }; i < 8; ++i)
		ShadowCast(origin, fov, 1, 1.0, 0.0, OCTANTS[i], viewDistance);

	return fov;
}

static void Reveal()
{
	for (usize i{ 0 }; i < MAP_VOLUME; ++i)
		VISIBLE[i] = true;
}

static void Reveal(cref<std::vector<Point>> fov)
{
	for (auto& pos : fov)
		VISIBLE[GetIndex(pos)] = true;
}

static void Reveal(cref<Rect> area)
{
	for (i32 j{ area.y }; j < area.y + area.h; ++j)
		for (i32 i{ area.x }; i < area.x + area.w; ++i)
			VISIBLE[GetIndex(i, j)] = true;
}

static void Obscure()
{
	for (usize i{ 0 }; i < MAP_VOLUME; ++i)
		VISIBLE[i] = false;
}

static void Obscure(cref<std::vector<Point>> fov)
{
	for (auto& pos : fov)
		VISIBLE[GetIndex(pos)] = false;
}

static void Obscure(cref<Rect> area)
{
	for (i32 j{ area.y }; j < area.y + area.h; ++j)
		for (i32 i{ area.x }; i < area.x + area.w; ++i)
			VISIBLE[GetIndex(i, j)] = false;
}

static constexpr bool IsPointInsideWindow(i32 x, i32 y) { return x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT; }

static constexpr bool IsPointInsideUI(i32 x, i32 y) { return x >= UI_ORIGIN_X && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT; }
static constexpr bool IsPointInsideGame(i32 x, i32 y) { return x >= 0 && x < GAME_WIDTH && y >= 0 && y < GAME_HEIGHT; }

static constexpr i32 SelectCellWidth(bool ui) { return ui ? UI_CELL_WIDTH : CELL_WIDTH; }
static constexpr i32 SelectCellHeight(bool ui) { return ui ? UI_CELL_HEIGHT : CELL_HEIGHT; }

static Vector2 UnflattenIndex(u8 index)
{
	div_t div_result = div(index, 16);

	f32 x = (f32)div_result.rem;
	f32 y = (f32)div_result.quot;

	return { x, y };
}

static Rectangle CalculateGridRectangle(u8 index, bool ui = false)
{
	Vector2 position = UnflattenIndex(index);

	i32 w = SelectCellWidth(ui);
	i32 h = SelectCellHeight(ui);

	return Rectangle{ position.x * w, position.y * h, (f32)w, (f32)h };
}

static void DrawGlyph(u8 index, i32 x, i32 y, Color color, bool ui = false, i32 nx = 0, i32 ny = 0)
{
	DrawTextureRec
	(
		ui ? *UI_GLYPHS : *GAME_GLYPHS,
		CalculateGridRectangle(index, ui),
		{
			static_cast<f32>(x * SelectCellWidth(ui) + nx),
			static_cast<f32>(y * SelectCellHeight(ui) + ny)
		},
		color
	);
}

static Vector2 CalculateStringSize(cref<std::string> s)
{
	if (s == "")
		return { 0, 0 };

	i32 maxWidth = 0;
	
	i32 width = 0;
	i32 height = 0;

	for (auto& c : s)
	{
		i32 rem{};

		switch (c)
		{
		case '\0':
			break;
		case '\n':
			if (width > maxWidth)
				maxWidth = width;
			width = 0;
			++height;
			continue;

		case '\t':
			rem = width % HORIZONTAL_TAB_SIZE;
			width += rem != 0 ? rem : HORIZONTAL_TAB_SIZE;
			continue;

		case '\v':
			rem = height % VERTICAL_TAB_SIZE;
			width += rem != 0 ? rem : VERTICAL_TAB_SIZE;
			continue;

		default:
			++width;
			continue;
		}
	}

	if (width > maxWidth)
		maxWidth = width;

	return { (f32)maxWidth, (f32)height };
}

static void DrawGlyphs(cref<std::string> glyphs, i32 x, i32 y, Color color, bool ui = false, i32 nx = 0, i32 ny = 0, bool truncate = true, bool wrap = false)
{
	if (glyphs == "")
		return;

	Vector2 size = CalculateStringSize(glyphs);

	i32 carriage_x = 0;
	i32 carriage_y = 0;

	for (auto& glyph : glyphs)
	{
		i32 rem{};

		if (carriage_x >= (ui ? UI_GRID_WIDTH : GRID_WIDTH) - 5)
		{
			if (truncate)
			{
				for (int i{ 0 }; i < 3; ++i)
					DrawGlyph('.', x + carriage_x + i, y + carriage_y, color, ui);

				return;
			}

			if (wrap)
			{
				carriage_x = 2;
				++carriage_y;
			}
		}

		switch (glyph)
		{
			case '\0':
				return;
			case '\n':
				carriage_x = 0;
				++carriage_y;
				continue;

			case '\t':
				rem = carriage_x % HORIZONTAL_TAB_SIZE;
				carriage_x += rem != 0 ? rem : HORIZONTAL_TAB_SIZE;
				continue;

			case '\v':
				rem = carriage_y % VERTICAL_TAB_SIZE;
				carriage_y += rem != 0 ? rem : VERTICAL_TAB_SIZE;
				continue;

			default:
				DrawGlyph(glyph, x + carriage_x, y + carriage_y, color, ui, nx, ny);
				carriage_x++;
				continue;
		}
	}
}

static void DrawMessages(cref<Log> log, Color color = WHITE)
{
	int i{ 0 };

	std::string last{ "" };
	int duplicates{ 0 };

	for (auto& message : log)
	{
		if (message == last)
		{
			++duplicates;
			continue;
		}
		else if (duplicates > 0)
		{
			DrawGlyphs("\tx " + std::to_string(duplicates + 1), UI_GRID_ORIGIN_X + 1, i * 2, color, true);
			duplicates = 0;
		}

		last = message;

		DrawGlyphs(">: " + message, UI_GRID_ORIGIN_X + 1, i * 2 + 1, color, true);
		++i;
	}
	
	if (duplicates > 0)
	{
		DrawGlyphs("\tx " + std::to_string(duplicates + 1), UI_GRID_ORIGIN_X + 1, i * 2, color, true);
		duplicates = 0;
	}
}

static void RecordEpoch()
{
	++CURRENT_EPOCH;
	LAST_EPOCH_TIME = GetTime();

	PLAYER_ACTED = false;
}

static void RecordInput()
{
	++INPUT_COUNT;
	LAST_INPUT_TIME = GetTime();
}

static bool HasEpochTranspired()
{
	return GetTime() - LAST_EPOCH_TIME >= MIN_EPOCH_INTERVAL;
}

static bool HasInputTranspired()
{
	return GetTime() - LAST_INPUT_TIME >= MIN_INPUT_INTERVAL;
}

static void UpdateCursor()
{
	Vector2 rawMousePos = GetMousePosition();

	MOUSE_X = (i32)rawMousePos.x;
	MOUSE_Y = (i32)rawMousePos.y;

	CURSOR_X = MOUSE_X / CELL_WIDTH;
	CURSOR_Y = MOUSE_Y / CELL_HEIGHT;

	UI_CURSOR_X = MOUSE_X / UI_CELL_WIDTH;
	UI_CURSOR_Y = MOUSE_Y / UI_CELL_HEIGHT;

	CURSOR_INSIDE_WINDOW = IsPointInsideWindow(MOUSE_X, MOUSE_Y);

	CURSOR_INSIDE_GAME = IsPointInsideGame(MOUSE_X, MOUSE_Y);
	CURSOR_INSIDE_UI = IsPointInsideUI(MOUSE_X, MOUSE_Y);
}

static std::string GetCardinalDirection(i32 x, i32 y)
{
	if (x == 0 && y == -1) return "north";
	if (x == 1 && y == -1) return "northeast";
	if (x == 1 && y == 0) return "east";
	if (x == 1 && y == 1) return "southeast";
	if (x == 0 && y == 1) return "south";
	if (x == -1 && y == 1) return "southwest";
	if (x == -1 && y == 0) return "west";
	if (x == -1 && y == -1) return "northwest";

	return "nowhere";
}

static void UpdatePlayer()
{
	if (!HasEpochTranspired())
		return;

	PLAYER_ACTED = false;

	if (!HasInputTranspired())
		return;

	if (IsKeyDown(KEY_KP_5))
	{
		RecordInput();
		RecordEpoch();

		MESSAGES.log("You wait.");
		MESSAGES.log("");
		return;
	}

	i32 x{ 0 }, y{ 0 };

	if (AnyKeysDown(KEY_W, KEY_KP_7, KEY_KP_8, KEY_KP_9) && PLAYER_Y > MAP_ORIGIN_Y) --y;
	if (AnyKeysDown(KEY_A, KEY_KP_1, KEY_KP_4, KEY_KP_7) && PLAYER_X > MAP_ORIGIN_X) --x;
	if (AnyKeysDown(KEY_S, KEY_KP_1, KEY_KP_2, KEY_KP_3) && PLAYER_Y < MAP_EXTENT_Y) ++y;
	if (AnyKeysDown(KEY_D, KEY_KP_3, KEY_KP_6, KEY_KP_9) && PLAYER_X < MAP_EXTENT_X) ++x;

	if (x != 0 || y != 0)
	{
		RecordInput();

		if (SOLID[(usize)PLAYER_X + x + ((usize)PLAYER_Y + y) * MAP_WIDTH])
		{
			MESSAGES.push("You bump into a wall.");
			RecordEpoch();
			PLAYER_ACTED = true;
			return;
		}

		RecordEpoch();

		PLAYER_X += x;
		PLAYER_Y += y;

		Obscure();

		for (auto& pos : CalculateFOV({ PLAYER_X, PLAYER_Y }, 8.0f))
		{
			usize index = GetIndex(pos);

			EXPLORED[index] = true;
			VISIBLE[index] = true;
		}

		log("You move " + GetCardinalDirection(x, y));
	}
	else return;

	PLAYER_ACTED = true;
}

static void UpdateCamera()
{
	if (IsKeyPressed(KEY_SPACE)) CAMERA_LOCKED = !CAMERA_LOCKED;

	if (!CAMERA_LOCKED)
	{
		if (!HasInputTranspired())
			return;

		i32 x{ 0 }, y{ 0 };

		if (IsKeyDown(KEY_UP) && CAMERA_Y > MAP_ORIGIN_Y) --y;
		if (IsKeyDown(KEY_DOWN) && CAMERA_Y < MAP_EXTENT_Y - GRID_EXTENT_Y) ++y;
		if (IsKeyDown(KEY_RIGHT) && CAMERA_X < MAP_EXTENT_X - GRID_EXTENT_X) ++x;
		if (IsKeyDown(KEY_LEFT) && CAMERA_X > MAP_ORIGIN_X) --x;

		if (x != 0 || y != 0)
		{
			CAMERA_X += x;
			CAMERA_Y += y;

			RecordInput();
		}
	}
	else
	{
		CAMERA_X = std::clamp(PLAYER_X - GRID_CENTER_X, MAP_ORIGIN_X, MAP_EXTENT_X - GRID_EXTENT_X);
		CAMERA_Y = std::clamp(PLAYER_Y - GRID_CENTER_Y, MAP_ORIGIN_Y, MAP_EXTENT_Y - GRID_EXTENT_Y);
	}

	MAP_CURSOR_X = CAMERA_X + CURSOR_X;
	MAP_CURSOR_Y = CAMERA_Y + CURSOR_Y;

	CURSOR_INSIDE_MAP = MAP_CURSOR_X >= MAP_ORIGIN_X && MAP_CURSOR_X <= MAP_EXTENT_X && MAP_CURSOR_Y >= MAP_ORIGIN_Y && MAP_CURSOR_Y <= MAP_EXTENT_Y;
}

static void UpdateSineWave(f64 time)
{
	SINE_VALUE = AMPLITUDE * (PHASE + std::sin(2.0 * PI * FREQUENCY * time));
}

static bool IsBorder(cref<Point> p) { return p.x < MAP_ORIGIN_X + MAP_BORDER_SIZE || p.x > MAP_EXTENT_X - MAP_BORDER_SIZE || p.y < MAP_ORIGIN_Y + MAP_BORDER_SIZE || p.y > MAP_EXTENT_Y - MAP_BORDER_SIZE; }

static bool IsBorder(i32 x, i32 y) { return x < MAP_ORIGIN_X + MAP_BORDER_SIZE || x > MAP_EXTENT_X - MAP_BORDER_SIZE || y < MAP_ORIGIN_Y + MAP_BORDER_SIZE || y > MAP_EXTENT_Y - MAP_BORDER_SIZE; }

static void GenerateBorder()
{
	for (i32 j{ 0 }; j < MAP_HEIGHT; ++j)
	{
		for (i32 i{ 0 }; i < MAP_WIDTH; ++i)
		{
			if (IsBorder(i, j))
				SOLID[GetIndex(i, j)] = true;
		}
	}
}

static void GenerateNoise()
{
	std::bernoulli_distribution dist{ 0.4 };

	for (usize i{ 0 }; i < MAP_VOLUME; ++i)
		SOLID[i] = dist(RNG);

	GenerateBorder();
}

static void SmoothNoise()
{
	std::vector<bool> temp{ SOLID };

	for (i32 j{ 1 }; j < MAP_EXTENT_Y; ++j)
	{
		for (i32 i{ 1 }; i < MAP_EXTENT_X; ++i)
		{
			usize index = GetIndex(i, j);

			if (IsBorder(i, j))
			{
				SOLID[index] = true;
				continue;
			}

			i32 count{ 0 };

			for (i32 y{ -1 }; y <= 1; ++y)
			{
				for (i32 x{ -1 }; x <= 1; ++x)
				{
					if (x == 0 && y == 0)
						continue;

					if (temp[(usize)i + x + ((usize)j + y) * MAP_WIDTH])
						++count;
				}
			}

			if (count > 4)
				SOLID[index] = true;
			else if (count < 4)
				SOLID[index] = false;
		}
	}
}

static void SmoothNoise(usize iterations)
{
	std::vector<bool> temp{ SOLID };

	for (int iter{ 0 }; iter < iterations; ++iter)
	{
		for (i32 j{ 1 }; j < MAP_HEIGHT - 1; ++j)
		{
			for (i32 i{ 1 }; i < MAP_WIDTH - 1; ++i)
			{
				usize index = GetIndex(i, j);

				if (IsBorder(i, j))
				{
					SOLID[index] = true;
					continue;
				}

				i32 count{ 0 };

				for (i32 y{ -1 }; y <= 1; ++y)
				{
					for (i32 x{ -1 }; x <= 1; ++x)
					{
						if (temp[GetIndex(i + x, j + y)])
							++count;
					}
				}

				if (count > 4)
					SOLID[index] = true;
				else if (count < 4)
					SOLID[index] = false;
			}
		}

		temp = SOLID;
	}
}

static void GenerateOpacity()
{
	for (usize i{ 0 }; i < MAP_VOLUME; ++i)
		OPAQUE[i] = SOLID[i];
}

static void GenerateCaverns()
{
	GenerateNoise();

	SmoothNoise(10);
}

int main(void)
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + 12, fmt::format("{} v{}", GAME_TITLE, GAME_VERSION).c_str());
	//SetWindowState(FLAG_WINDOW_UNDECORATED);

	SetTargetFPS(60);
	HideCursor();

	GenerateCaverns();
	GenerateOpacity();

	Point startingPosition = FindOpen();

	if (startingPosition.x != -1 && startingPosition.y != -1)
	{
		PLAYER_X = startingPosition.x;
		PLAYER_Y = startingPosition.y;
	}
	else
	{
		log("Failed to find a starting position.", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	Obscure();

	for (auto& pos : CalculateFOV({ PLAYER_X, PLAYER_Y }, 8.0f))
	{
		usize index = GetIndex(pos);

		EXPLORED[index] = true;
		VISIBLE[index] = true;
	}

	UI_GLYPHS = new Texture2D(LoadTexture("Assets/glyphs_8x8.png"));
	GAME_GLYPHS = new Texture2D(LoadTexture("Assets/glyphs_12x12.png"));

	CURSOR = new Texture2D(LoadTexture("Assets/cursor.png"));

	LAST_INPUT_TIME = GetTime();
	LAST_EPOCH_TIME = GetTime();

	while (!WindowShouldClose())
	{
		Prune(MESSAGES, MAX_MESSAGES);
		Prune(ERRORS, MAX_ERRORS);

		UpdatePlayer();

		UpdateCursor();
		UpdateCamera();

		UpdateSineWave(GetTime());

		BeginDrawing();

		ClearBackground(BLACK);

#ifndef NDEBUG
		if (IsKeyPressed(KEY_F1)) SHOW_ERRORS = !SHOW_ERRORS;

		SHOW_ERRORS ? DrawMessages(ERRORS, RED) : DrawMessages(MESSAGES);

		DrawGlyphs(fmt::format("[F1]: view {}", SHOW_ERRORS ? "messages" : "errors"), UI_GRID_ORIGIN_X, UI_GRID_EXTENT_Y + 1, GRAY, true, 2, 1);
#else
		DrawMessages(MESSAGES);
#endif

		if (CURSOR_INSIDE_WINDOW)
		{
			if (CURSOR_INSIDE_GAME)
			{

			}
			else if (CURSOR_INSIDE_UI)
			{
				if (UI_CURSOR_Y % 2 != 0)
				{
					i32 messagePos = UI_CURSOR_Y / 2;
#ifndef NDEBUG
					if (messagePos < (SHOW_ERRORS ? ERRORS.size() : MESSAGES.size()))
#else
					if (messagePos < MESSAGES.size())
#endif
					{
#ifndef NDEBUG
						que<std::string> messages{ SHOW_ERRORS ? ERRORS : MESSAGES };
#else
						que<std::string> messages{ MESSAGES };
#endif
						while (messagePos != 0)
						{
							messages.pop();
							--messagePos;
						}
#ifndef NDEBUG
						DrawGlyphs(">: " + messages.front(), 0, UI_GRID_EXTENT_Y + 1, SHOW_ERRORS ? RED : WHITE, true, 2, 1, false);
#else
						DrawGlyphs(">: " + messages.front(), 0, UI_GRID_EXTENT_Y + 1, WHITE, true, 2, 1, false);
#endif
					}
				}
			}
		}

		for (i32 j{ CAMERA_Y }; j < CAMERA_Y + GRID_HEIGHT; ++j)
		{
			for (i32 i{ CAMERA_X }; i < CAMERA_X + GRID_WIDTH; ++i)
			{
				if (i >= MAP_ORIGIN_X && i <= MAP_EXTENT_X && j >= MAP_ORIGIN_Y && j <= MAP_EXTENT_Y)
				{
					usize index = GetIndex(i, j);

					if (!EXPLORED[index])
						continue;

					bool solid = SOLID[index];
					bool visible = VISIBLE[index];

					Color color = solid ? visible ? LIGHTGRAY : GRAY : DARKGRAY;

					DrawGlyph(solid ? 0xB2 : 0xB0, i - CAMERA_X, j - CAMERA_Y, color, false);
				}
			}
		}

		if (PLAYER_X >= CAMERA_X && PLAYER_X < CAMERA_X + GRID_WIDTH && PLAYER_Y >= CAMERA_Y && PLAYER_Y < CAMERA_Y + GRID_HEIGHT)
			DrawGlyph('@', PLAYER_X - CAMERA_X, PLAYER_Y - CAMERA_Y, GREEN, false);

		if (CURSOR_INSIDE_GAME && CURSOR_INSIDE_MAP)
			DrawRectangleLines(CURSOR_X * CELL_WIDTH - 1, CURSOR_Y * CELL_HEIGHT - 1, CELL_WIDTH + 2, CELL_HEIGHT + 2, { 255, 215, 0, (u8)(255 * SINE_VALUE) });
		else DrawTexture(*CURSOR, MOUSE_X, MOUSE_Y, WHITE);

		DrawRectangleLinesEx({ 0, 0, GAME_WIDTH, GAME_HEIGHT }, 1, WHITE);
		DrawRectangleLinesEx({ UI_ORIGIN_X, 0, UI_WIDTH, UI_HEIGHT }, 1, WHITE);

		DrawRectangleLinesEx({ 0, WINDOW_EXTENT_Y, GAME_WIDTH, 12 }, 1, WHITE);
		DrawRectangleLinesEx({ UI_ORIGIN_X, WINDOW_EXTENT_Y, UI_WIDTH, 12 }, 1, WHITE);

#ifndef NDEBUG
		std::std::string fps_text = fmt::format("FPS: {}", GetFPS());

		DrawGlyphs(fps_text.c_str(), UI_GRID_EXTENT_X - (i32)fps_text.length(), UI_GRID_EXTENT_Y + 1, GREEN, true, 2, 1);
#endif

		EndDrawing();
	}

	delete UI_GLYPHS;
	delete GAME_GLYPHS;

	delete CURSOR;

	CloseWindow();

	return EXIT_SUCCESS;
}
