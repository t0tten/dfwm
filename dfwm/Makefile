CXX	= g++
CXXFLAGS=-std=c++11 -lX11
BNAME	= dfwm
BDIR	= bin
ODIR	= src/obj
IDIR	= include
SDIR	= src

_DEPS	= Dfwm.h KeyBindings.h StatusBar.h Color.h Constants.h StatusBarItem.h StatusBarItemDesktop.h StatusBarClock.h StatusBarItemInfo.h Menu.h Desktop.h Launcher.h DfwmWindow.h Configuration.h DfwmStatus.h dfwm_utils.h
DEPS	= $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ	= Main.o Dfwm.o KeyBindings.o StatusBar.o StatusBarItem.o StatusBarItemDesktop.o StatusBarClock.o StatusBarItemInfo.o Menu.o Desktop.o Launcher.o DfwmWindow.o Configuration.o DfwmStatus.o dfwm_utils.o
OBJ	= $(patsubst %,$(SDIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CXX) -c -o $@ $<

$(BDIR)/$(BNAME): $(OBJ)
	@if [ ! -d $(BDIR) ]; then \
		mkdir $(BDIR); \
	fi
	$(CXX) -o $@ $^ -lX11
	@echo " << Make: done >> "
	@echo "Run: make run"
	@echo "Clean: make clean"

.PHONY: clean

debug: CXXFLAGS += -DDEBUG
debug: $(BDIR)/$(BNAME)

clean:
	@rm -R $(BDIR) $(SDIR)/*.o 2>/dev/null
	@echo " << Clean: done >> "

run:
	@./$(BDIR)/$(BNAME)
	
install:
	sudo cp bin/dfwm /usr/local/bin/
	@printf "[Desktop Entry]\nEncoding=UTF-8\nName=Dfwm\nComment=Damn Fine Window Manager\nExec=dfwm\nIcon=dfwm\nType=XSession\n" > bin/dfwm.desktop
	sudo cp bin/dfwm.desktop /usr/share/xsessions/
