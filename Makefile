
all:
	@cd build && make -j4 && cd .. && mv ./build/ftxui-starter.exe ./grep.exe
