add_executable(Minesweeper WIN32
			   win32_platform.cpp
			   resource.rc
			   MainWindow.cpp
			   SettingsDialog.cpp)

target_link_libraries(Minesweeper PRIVATE logger game comctl32.lib)