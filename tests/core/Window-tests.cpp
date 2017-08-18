#include <string>
#include <GLFW/glfw3.h>
#include <catch/catch.hpp>
#include <core/Window.hpp>

using namespace eto;

TEST_CASE("Window is created", "[Window]") {
	Window w;
	w.setWinHint(GLFW_VISIBLE, GLFW_FALSE);
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
	w.setWinHint(GLFW_VISIBLE, GLFW_FALSE);
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
	w.setWinHint(GLFW_VISIBLE, GLFW_FALSE);
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
	w.setWinHint(GLFW_VISIBLE, GLFW_FALSE);
	w.create(800, 600, "title");
	w.setShouldClose(true);
	REQUIRE( w.shouldClose() == true );
}

TEST_CASE("Window key callback is called", "[Window]") {
	using namespace eto::Input;
	Window w;
	w.setWinHint(GLFW_VISIBLE, GLFW_FALSE);
	w.create(800, 600, "callback window");

	SECTION("Pressed 'A' key") {
		w.onKey(Key::A, 1, Action::Press, 0);
		GLFWevent event;
	       	REQUIRE( w.getEvent(event) == 1 );
		REQUIRE( event.type == GLFWevent::Type::Key );

		GLFWevent::KeyEvent key = event.key;
		REQUIRE( key.key == Key::A );
		REQUIRE( key.action == Action::Press );
		REQUIRE( key.scancode == 1 );
		REQUIRE( key.modifier == 0 );
	}
	SECTION("Released 'alt+F4' keys") {
		w.onKey(Key::F4, 1, Action::Release, Modifier::Alt);
		GLFWevent event;
	       	REQUIRE( w.getEvent(event) == 1 );
		REQUIRE( event.type == GLFWevent::Type::Key );

		GLFWevent::KeyEvent key = event.key;
		REQUIRE( key.key == Key::F4 );
		REQUIRE( key.action == Action::Release );
		REQUIRE( key.scancode == 1 );
		REQUIRE( key.modifier == Modifier::Alt );
	}
}


TEST_CASE("Window mouse button callback is called", "[Window]") {
	using namespace eto::Input;
	Window w;
	w.setWinHint(GLFW_VISIBLE, GLFW_FALSE);
	w.create(100, 100, "title");

	SECTION ("Pressed 'MOUSE_BUTTON_1") {
		w.onMouseButton(MouseButton::Button1, Action::Press, 0);
		GLFWevent event;
		REQUIRE( w.getEvent(event) == 1 );
		REQUIRE( event.type == GLFWevent::Type::MouseButton );

		GLFWevent::MouseButtonEvent mouse = event.mouseButton;
		REQUIRE( mouse.button == MouseButton::Button1 );
		REQUIRE( mouse.action == Action::Press );
		REQUIRE( mouse.modifier == 0 );
	}
	SECTION ("Held 'MOUSE_BUTTON_3 + Conrol + Super") {
		w.onMouseButton(MouseButton::Button3, Action::Repeat, Modifier::Super | Modifier::Control);
		GLFWevent event;
		REQUIRE( w.getEvent(event) == 1 );
		REQUIRE( event.type == GLFWevent::Type::MouseButton );

		GLFWevent::MouseButtonEvent mouse = event.mouseButton;
		REQUIRE( mouse.button == MouseButton::Button3);
		REQUIRE( mouse.action == Action::Repeat );
		REQUIRE( (mouse.modifier | Modifier::Super) );
		REQUIRE( (mouse.modifier | Modifier::Control) );
	}
}

TEST_CASE("Window mouse position callback is called", "[Window]") {
	Window w;
	w.setWinHint(GLFW_VISIBLE, GLFW_FALSE);
	w.create(100, 100, "title");

	w.onCursorPosition(100, 200);
	GLFWevent event;
	REQUIRE( w.getEvent(event) == 1 );
	REQUIRE( event.type == GLFWevent::Type::CursorPosition );
	REQUIRE( event.cursorPos.x == 100 );
	REQUIRE( event.cursorPos.y == 200 );

	w.onCursorPosition(-1, !1);
	REQUIRE( w.getEvent(event) == 1 );
	REQUIRE( event.type == GLFWevent::Type::CursorPosition );
	REQUIRE( event.cursorPos.x == -1 );
	REQUIRE( event.cursorPos.y == !1 );
}


