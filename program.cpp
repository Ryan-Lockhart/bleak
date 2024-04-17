#include "typedef.hpp"

#include <raylib.h>

constexpr cstr GAME_TITLE = "Bleakdepth";
constexpr cstr GAME_VERSION = "0.0.1";

constexpr u32 WINDOW_WIDTH = 1280;
constexpr u32 WINDOW_HEIGHT = 480;

constexpr f32 WINDOW_ASPECT_RATIO = static_cast<f32>(WINDOW_WIDTH) / static_cast<f32>(WINDOW_HEIGHT);

constexpr u32 WINDOW_ORIGIN_X = 0;
constexpr u32 WINDOW_ORIGIN_Y = 0;

constexpr u32 WINDOW_EXTENT_X = WINDOW_WIDTH - 1;
constexpr u32 WINDOW_EXTENT_Y = WINDOW_HEIGHT - 1;

constexpr u32 WINDOW_CENTER_X = WINDOW_WIDTH / 2;
constexpr u32 WINDOW_CENTER_Y = WINDOW_HEIGHT / 2;

constexpr u32 UI_WIDTH = 320;
constexpr u32 UI_HEIGHT = WINDOW_HEIGHT;

constexpr u32 UI_ORIGIN_X = WINDOW_WIDTH - UI_WIDTH;
constexpr u32 UI_ORIGIN_Y = 0;

constexpr u32 UI_EXTENT_X = WINDOW_WIDTH - 1;
constexpr u32 UI_EXTENT_Y = WINDOW_HEIGHT - 1;

constexpr u32 UI_CENTER_X = UI_ORIGIN_X + UI_WIDTH / 2;
constexpr u32 UI_CENTER_Y = WINDOW_CENTER_Y;

constexpr u32 GAME_WIDTH = WINDOW_WIDTH - UI_WIDTH;
constexpr u32 GAME_HEIGHT = WINDOW_HEIGHT;

constexpr u32 GAME_CENTER_X = GAME_WIDTH / 2;
constexpr u32 GAME_CENTER_Y = WINDOW_CENTER_Y;

constexpr u32 CELL_WIDTH = 12;
constexpr u32 CELL_HEIGHT = 12;

constexpr u32 UI_CELL_WIDTH = 8;
constexpr u32 UI_CELL_HEIGHT = 8;

constexpr u32 UI_GRID_WIDTH = UI_WIDTH / UI_CELL_WIDTH;
constexpr u32 UI_GRID_HEIGHT = UI_HEIGHT / UI_CELL_HEIGHT;

constexpr u32 UI_GRID_ORIGIN_X = UI_ORIGIN_X / UI_CELL_WIDTH;
constexpr u32 UI_GRID_ORIGIN_Y = 0;

constexpr u32 UI_GRID_EXTENT_X = UI_GRID_ORIGIN_X + UI_GRID_WIDTH - 1;
constexpr u32 UI_GRID_EXTENT_Y = UI_GRID_HEIGHT - 1;

constexpr u32 UI_GRID_CENTER_X = UI_GRID_ORIGIN_X + UI_GRID_WIDTH / 2;
constexpr u32 UI_GRID_CENTER_Y = UI_GRID_HEIGHT / 2;

constexpr u32 GRID_WIDTH = GAME_WIDTH / CELL_WIDTH;
constexpr u32 GRID_HEIGHT = GAME_HEIGHT / CELL_HEIGHT;

constexpr u32 GRID_ORIGIN_X = 0;
constexpr u32 GRID_ORIGIN_Y = 0;

constexpr u32 GRID_EXTENT_X = GRID_WIDTH - 1;
constexpr u32 GRID_EXTENT_Y = GRID_HEIGHT - 1;

constexpr u32 GRID_CENTER_X = GRID_WIDTH / 2;
constexpr u32 GRID_CENTER_Y = GRID_HEIGHT / 2;

constexpr usize GRID_SIZE = GRID_WIDTH * GRID_HEIGHT;

constexpr u32 MAP_WIDTH = 256;
constexpr u32 MAP_HEIGHT = 256;
constexpr u32 MAP_DEPTH = 1;

constexpr usize MAP_AREA = MAP_WIDTH * MAP_HEIGHT;
constexpr usize MAP_VOLUME = MAP_WIDTH * MAP_HEIGHT * MAP_DEPTH;

constexpr u32 MAP_ORIGIN_X = 0;
constexpr u32 MAP_ORIGIN_Y = 0;
constexpr u32 MAP_ORIGIN_Z = 0;

constexpr u32 MAP_EXTENT_X = MAP_WIDTH - 1;
constexpr u32 MAP_EXTENT_Y = MAP_HEIGHT - 1;
constexpr u32 MAP_EXTENT_Z = MAP_DEPTH - 1;

constexpr u32 MAP_CENTER_X = MAP_WIDTH / 2;
constexpr u32 MAP_CENTER_Y = MAP_HEIGHT / 2;
constexpr u32 MAP_CENTER_Z = MAP_DEPTH / 2;

constexpr u32 HORIZONTAL_TAB_SIZE = 4;
constexpr u32 VERTICAL_TAB_SIZE = 4;

static constexpr bool IsPointInsideWindow(u32 x, u32 y) { return x >= 0 && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT; }

static constexpr bool IsPointInsideUI(u32 x, u32 y) { return x >= UI_ORIGIN_X && x < WINDOW_WIDTH && y >= 0 && y < WINDOW_HEIGHT; }
static constexpr bool IsPointInsideGame(u32 x, u32 y) { return x >= 0 && x < GAME_WIDTH && y >= 0 && y < GAME_HEIGHT; }

static constexpr u32 SelectCellWidth(bool ui) { return ui ? UI_CELL_WIDTH : CELL_WIDTH; }
static constexpr u32 SelectCellHeight(bool ui) { return ui ? UI_CELL_HEIGHT : CELL_HEIGHT; }

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

	u32 w = SelectCellWidth(ui);
	u32 h = SelectCellHeight(ui);

	return Rectangle{ position.x * w, position.y * h, (f32)w, (f32)h };
}

static Texture2D* UI_GLYPHS = nullptr;
static Texture2D* GAME_GLYPHS = nullptr;

static Texture2D* CURSOR = nullptr;

static void DrawGlyph(u8 index, u32 x, u32 y, Color color, bool ui = false, u32 nx = 0, u32 ny = 0)
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

static Vector2 CalculateStringSize(cref<string> s)
{
	if (s == "")
		return { 0, 0 };

	u32 maxWidth = 0;
	
	u32 width = 0;
	u32 height = 0;

	for (auto& c : s)
	{
		u32 rem{};

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

static void DrawGlyphs(cref<string> glyphs, u32 x, u32 y, Color color, bool ui = false, u32 nx = 0, u32 ny = 0, bool truncate = true, bool wrap = false)
{
	if (glyphs == "")
		return;

	Vector2 size = CalculateStringSize(glyphs);

	u32 carriage_x = 0;
	u32 carriage_y = 0;

	for (auto& glyph : glyphs)
	{
		u32 rem{};

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

static void DrawMessages(cref<que<string>> queue, Color color = WHITE)
{
	que<string> messages{ queue };

	int i{ 0 };

	string last{ "" };
	int duplicates{ 0 };

	while (messages.size() > 0)
	{
		if (messages.front() == last)
		{
			messages.pop();
			++duplicates;
			continue;
		}
		else if (duplicates > 0)
		{
			DrawGlyphs("\tx " + std::to_string(duplicates + 1), UI_GRID_ORIGIN_X + 1, i * 2, color, true);
			duplicates = 0;
		}

		last = messages.front();

		DrawGlyphs(">: " + messages.front(), UI_GRID_ORIGIN_X + 1, i * 2 + 1, color, true);
		messages.pop();

		++i;
	}
	
	if (duplicates > 0)
	{
		DrawGlyphs("\tx " + std::to_string(duplicates + 1), UI_GRID_ORIGIN_X + 1, i * 2, color, true);
		duplicates = 0;
	}
}

#ifndef NDEBUG
static bool showErrors = false;
#endif

static u32 MOUSE_X = 0;
static u32 MOUSE_Y = 0;

static u32 CURSOR_X = 0;
static u32 CURSOR_Y = 0;

static u32 UI_CURSOR_X = 0;
static u32 UI_CURSOR_Y = 0;

static bool CURSOR_INSIDE_WINDOW = false;

static bool CURSOR_INSIDE_GAME = false;
static bool CURSOR_INSIDE_UI = false;

static void UpdateCursor()
{
	Vector2 rawMousePos = GetMousePosition();

	MOUSE_X = (u32)rawMousePos.x;
	MOUSE_Y = (u32)rawMousePos.y;

	CURSOR_X = MOUSE_X / CELL_WIDTH;
	CURSOR_Y = MOUSE_Y / CELL_HEIGHT;

	UI_CURSOR_X = MOUSE_X / UI_CELL_WIDTH;
	UI_CURSOR_Y = MOUSE_Y / UI_CELL_HEIGHT;

	CURSOR_INSIDE_WINDOW = IsPointInsideWindow(MOUSE_X, MOUSE_Y);

	CURSOR_INSIDE_GAME = IsPointInsideGame(MOUSE_X, MOUSE_Y);
	CURSOR_INSIDE_UI = IsPointInsideUI(MOUSE_X, MOUSE_Y);
}

static u32 CAMERA_X = 0;
static u32 CAMERA_Y = 0;

static u32 MAP_CURSOR_X = 0;
static u32 MAP_CURSOR_Y = 0;

static bool CURSOR_INSIDE_MAP = false;

static void UpdateCamera()
{
	if (IsKeyDown(KEY_UP) && CAMERA_Y > MAP_ORIGIN_Y) CAMERA_Y -= 1;
	if (IsKeyDown(KEY_DOWN) && CAMERA_Y < MAP_EXTENT_Y - GRID_EXTENT_Y) CAMERA_Y += 1;
	if (IsKeyDown(KEY_RIGHT) && CAMERA_X < MAP_EXTENT_X - GRID_EXTENT_X) CAMERA_X += 1;
	if (IsKeyDown(KEY_LEFT) && CAMERA_X > MAP_ORIGIN_X) CAMERA_X -= 1;

	MAP_CURSOR_X = CAMERA_X + CURSOR_X;
	MAP_CURSOR_Y = CAMERA_Y + CURSOR_Y;

	CURSOR_INSIDE_MAP = MAP_CURSOR_X >= MAP_ORIGIN_X && MAP_CURSOR_X <= MAP_EXTENT_X && MAP_CURSOR_Y >= MAP_ORIGIN_Y && MAP_CURSOR_Y <= MAP_EXTENT_Y;
}

static vec<bool> SOLIDS{ MAP_VOLUME, false, std::allocator<bool>{ } };

int main(void)
{
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT + 12, fmt::format("{} v{}", GAME_TITLE, GAME_VERSION).c_str());
	SetWindowState(FLAG_WINDOW_UNDECORATED);

	SetTargetFPS(30);
	HideCursor();

	UI_GLYPHS = new Texture2D(LoadTexture("Assets/glyphs_8x8.png"));
	GAME_GLYPHS = new Texture2D(LoadTexture("Assets/glyphs_12x12.png"));

	CURSOR = new Texture2D(LoadTexture("Assets/cursor.png"));

	for (usize j{ 0 }; j < MAP_HEIGHT; ++j)
	{
		for (usize i{ 0 }; i < MAP_WIDTH; ++i)
		{
			SOLIDS[j * MAP_WIDTH + i] = i == MAP_ORIGIN_X || i == MAP_EXTENT_X || j == MAP_ORIGIN_Y || j == MAP_EXTENT_Y;
		}
	}

	while (!WindowShouldClose())
	{
		prune_messages();
		prune_errors();

		UpdateCursor();
		UpdateCamera();

		BeginDrawing();

		ClearBackground(BLACK);

#ifndef NDEBUG
		if (IsKeyPressed(KEY_F1)) showErrors = !showErrors;

		showErrors ? DrawMessages(ERRORS, RED) : DrawMessages(MESSAGES);

		DrawGlyphs(fmt::format("[F1]: view {}", showErrors ? "messages" : "errors"), UI_GRID_ORIGIN_X, UI_GRID_EXTENT_Y + 1, GRAY, true, 2, 1);
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
					u32 messagePos = UI_CURSOR_Y / 2;
#ifndef NDEBUG
					if (messagePos < (showErrors ? ERRORS.size() : MESSAGES.size()))
#else
					if (messagePos < MESSAGES.size())
#endif
					{
#ifndef NDEBUG
						que<string> messages{ showErrors ? ERRORS : MESSAGES };
#else
						que<string> messages{ MESSAGES };
#endif
						while (messagePos != 0)
						{
							messages.pop();
							--messagePos;
						}
#ifndef NDEBUG
						DrawGlyphs(">: " + messages.front(), 0, UI_GRID_EXTENT_Y + 1, showErrors ? RED : WHITE, true, 2, 1, false);
#else
						DrawGlyphs(">: " + messages.front(), 0, UI_GRID_EXTENT_Y + 1, WHITE, true, 2, 1, false);
#endif
					}
				}
			}
		}

		for (u32 j{ CAMERA_Y }; j < CAMERA_Y + GRID_HEIGHT; ++j)
		{
			for (u32 i{ CAMERA_X }; i < CAMERA_X + GRID_WIDTH; ++i)
			{
				if (i >= MAP_ORIGIN_X && i <= MAP_EXTENT_X && j >= MAP_ORIGIN_Y && j <= MAP_EXTENT_Y)
				{
					bool solid = SOLIDS[i + j * (usize)MAP_WIDTH];

					DrawGlyph(solid ? 0xB2 : 0xB0, i - CAMERA_X, j - CAMERA_Y, solid ? LIGHTGRAY : GRAY, false);
				}
			}
		}

		if (CURSOR_INSIDE_GAME && CURSOR_INSIDE_MAP)
			DrawRectangleLines(CURSOR_X * CELL_WIDTH, CURSOR_Y * CELL_HEIGHT, CELL_WIDTH, CELL_HEIGHT, { 255, 215, 0, 128 });
		else DrawTexture(*CURSOR, MOUSE_X, MOUSE_Y, WHITE);

		DrawRectangleLinesEx({ 0, 0, GAME_WIDTH, GAME_HEIGHT }, 1, WHITE);
		DrawRectangleLinesEx({ UI_ORIGIN_X, 0, UI_WIDTH, UI_HEIGHT }, 1, WHITE);

		DrawRectangleLinesEx({ 0, WINDOW_EXTENT_Y, GAME_WIDTH, 12 }, 1, WHITE);
		DrawRectangleLinesEx({ UI_ORIGIN_X, WINDOW_EXTENT_Y, UI_WIDTH, 12 }, 1, WHITE);

#ifndef NDEBUG
		string fps_text = fmt::format("FPS: {}", GetFPS());

		DrawGlyphs(fps_text.c_str(), UI_GRID_EXTENT_X - (u32)fps_text.length(), UI_GRID_EXTENT_Y + 1, GREEN, true, 2, 1);
#endif

		EndDrawing();
	}

	delete UI_GLYPHS;
	delete GAME_GLYPHS;

	delete CURSOR;

	CloseWindow();

	return 0;
}
