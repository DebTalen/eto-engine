#include <string>
#include <GLFW/glfw3.h>
#include <catch.hpp>
#include <core/Window.hpp>

using namespace eto;

TEST_CASE("Window is created", "[Window]") {
	Window w;
	w.set_win_hint(GLFW_VISIBLE, GLFW_FALSE);
	SECTION("valid parameters") {
		int is_sucsess_create = w.create(800, 600, "valid test window");
		REQUIRE( is_sucsess_create == 1 );
		Window::WinSize size = w.get_size();
		REQUIRE( size.x == 800 );
		REQUIRE( size.y == 600 );
	}
	SECTION("invalide parameters") {
		int is_sucsess_create = w.create(-100, 0, "invalide test window");
		REQUIRE( is_sucsess_create == 0 );
		Window::WinSize size = w.get_size();
		REQUIRE( size.x == 0);
		REQUIRE( size.y == 0);
	}
	SECTION("window is created twice") {
		w.create(100, 100, "title");
		int is_sucsess_create = w.create(1200, 800, "title2");
		REQUIRE( is_sucsess_create == 0 );
		Window::WinSize size = w.get_size();
		REQUIRE( size.x == 100 );
		REQUIRE( size.y == 100 );
	}
}

TEST_CASE("Window is resized", "[Window]") {
	Window w;
	w.set_win_hint(GLFW_VISIBLE, GLFW_FALSE);
	w.create(800, 600, "resized window");
	SECTION("valid parameters") {
		w.set_size(600, 800);
		Window::WinSize size = w.get_size();
		REQUIRE( size.x == 600 );
		REQUIRE( size.y == 800 );
	}
	SECTION("invalide parameters") {
		w.set_size(600, -100);
		Window::WinSize size = w.get_size();
		REQUIRE( size.x == 800 );
		REQUIRE( size.y == 600 );
	}
}

TEST_CASE("Window is moved", "[Window]") {
	Window w;
	w.set_win_hint(GLFW_VISIBLE, GLFW_FALSE);
	w.create(800, 600, "moved window");
	w.set_pos(0, 0);
	Window::WinPos pos = w.get_pos();
	REQUIRE( pos.x == 0 );
	REQUIRE( pos.y == 0 );

	w.set_pos(1000, 1000);
	pos = w.get_pos();
	REQUIRE( pos.x == 1000 );
	REQUIRE( pos.y == 1000 );

	w.set_pos(-1000, -1000);
	pos = w.get_pos();
	REQUIRE( pos.x == -1000 );
	REQUIRE( pos.y == -1000 );

	w.set_pos(999999, 999999);
	pos = w.get_pos();
	REQUIRE( pos.x == 999999 );
	REQUIRE( pos.y == 999999 );
}

TEST_CASE("Window is closed", "[Window]") {
	Window w;
	w.set_win_hint(GLFW_VISIBLE, GLFW_FALSE);
	w.create(800, 600, "title");
	w.set_should_close(true);
	REQUIRE( w.should_close() == true );
}

