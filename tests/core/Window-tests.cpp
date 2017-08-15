#include <string>
#include <GLFW/glfw3.h>
#include <catch/catch.hpp>
#include <core/Window.hpp>

using namespace eto;

TEST_CASE("Window is created", "[Window]") {
	Window w;
	SECTION("valid parameters") {
		int isSucsessCreate = w.create(800, 600, "valid test window");
		REQUIRE( isSucsessCreate == 1 );
		WinSize size = w.getSize();
		REQUIRE( size.x == 800 );
		REQUIRE( size.y == 600 );
	}
	SECTION("invalide parameters") {
		int isSucsessCreate = w.create(-100, 0, "invalide test window");
		REQUIRE( isSucsessCreate == 0 );
		WinSize size = w.getSize();
		REQUIRE( size.x == 0);
		REQUIRE( size.y == 0);
	}
	SECTION("window is created twice") {
		w.create(100, 100, "title");
		int isSucsessCreate = w.create(1200, 800, "title2");
		REQUIRE( isSucsessCreate == 0 );
		WinSize size = w.getSize();
		REQUIRE( size.x == 100 );
		REQUIRE( size.y == 100 );
	}
}

TEST_CASE("Window is resized", "[Window]") {
	Window w;
	w.create(800, 600, "resized window");
	SECTION("valid parameters") {
		w.setSize(600, 800);
		WinSize size = w.getSize();
		REQUIRE( size.x == 600 );
		REQUIRE( size.y == 800 );
	}
	SECTION("invalide parameters") {
		w.setSize(600, -100);
		WinSize size = w.getSize();
		REQUIRE( size.x == 800 );
		REQUIRE( size.y == 600 );
	}
}

TEST_CASE("Window is moved", "[Window]") {
	Window w;
	w.create(800, 600, "moved window");
	w.setPos(0, 0);
	WinPos pos = w.getPos();
	REQUIRE( pos.x == 0 );
	REQUIRE( pos.y == 0 );

	w.setPos(1000, 1000);
	pos = w.getPos();
	REQUIRE( pos.x == 1000 );
	REQUIRE( pos.y == 1000 );

	w.setPos(-1000, -1000);
	pos = w.getPos();
	REQUIRE( pos.x == -1000 );
	REQUIRE( pos.y == -1000 );

	w.setPos(999999, 999999);
	pos = w.getPos();
	REQUIRE( pos.x == 999999 );
	REQUIRE( pos.y == 999999 );
}

TEST_CASE("Window is closed", "[Window]") {
	Window w;
	w.create(800, 600, "title");
	w.setShouldClose(true);
	REQUIRE( w.isShouldClose() == true );
}

