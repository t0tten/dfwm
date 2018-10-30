#include "../include/Launcher.h"

Launcher::Launcher (Display* disp, Window* parent) {
	this->disp 	= disp;
	
	XWindowAttributes wndAttr;
        XGetWindowAttributes (disp, *parent, &wndAttr);

	this->width 	= 300;
	this->height 	= 50;

        this->x    	= (wndAttr.width / 2) - (width / 2);
        this->y		= (wndAttr.height / 4) - (height / 2);	

	this->launcher	= XCreateSimpleWindow(disp, *parent, x, y, width, height, 0, 0, COL_LAUNCHER_BG);
	this->gc 	= XCreateGC(disp, launcher, 0, NULL);
	this->autoCorrectIndex = -1;

	this->setState(HIDING);
}

Launcher::~Launcher () {
	XDestroyWindow(disp, launcher);
}

void Launcher::show() {
	this->setState(SHOWING);
	XMapWindow(disp, launcher);
	XRaiseWindow(disp, launcher);
	this->draw();
	this->getPaths();
}

void Launcher::hide() {
	this->searchPhrase = "";
	this->files.clear();
	this->results.clear();
	this->autoCorrectIndex = -1;
	this->setState(HIDING);
	XUnmapWindow(disp, launcher);
}

void Launcher::draw() {
	int size = (results.size() <= 10) ? (results.size()) : 10;

	XSetForeground(disp, gc, COL_LAUNCHER_BG);
	XFillRectangle(disp, launcher, gc, 0, 0, width, height);

	XMoveResizeWindow(disp, launcher, x, y, width, height + ((size) * 25));

	XSetForeground(disp, gc, COL_LAUNCHER_FG);
	if(searchPhrase.length() == 0) {
		XDrawString(disp, launcher, gc, 20, (height / 2) + 5, "Command...", 10);
	} else {
		XDrawString(disp, launcher, gc, 20, (height / 2) + 5, searchPhrase.c_str(), searchPhrase.length());
	}

	//if(autoCorrectIndex != -1) {
		for(int i = 0; i < size; i++) {
			if(autoCorrectIndex >= 0 && i == autoCorrectIndex) 	XSetForeground(disp, gc, COL_LAUNCHER_RESULT_SEL_BG);
			else				XSetForeground(disp, gc, COL_LAUNCHER_RESULT_BG);
        		XFillRectangle(disp, launcher, gc, 5, height + (25 * i), width - 10, 20);
		
			XSetForeground(disp, gc, COL_LAUNCHER_RESULT_FG);
			if(results.size() > i) {
				XDrawString(disp, launcher, gc, 20, height + 13 + (25 * i), results.at(i).c_str(), results.at(i).length());
			}
		}
	//}
}

void Launcher::redraw() {
	XEvent exp;
        exp.type                = Expose;
        exp.xexpose.window      = launcher;
        XSendEvent(disp, launcher, False, ExposureMask, &exp);
}

void Launcher::addChar(char toAdd) {
        this->searchPhrase += toAdd;
        this->search();
	this->redraw();
}

void Launcher::removeLastChar() {
	if(searchPhrase.length() > 0) this->searchPhrase.pop_back();
	this->search();
}

Window Launcher::getLauncherWindow() { return this->launcher; }

std::string Launcher::getText() { return this->searchPhrase; }

bool Launcher::fetchFiles(std::string path) {
	DIR *dp;
	struct dirent *dirp;
	if((dp  = opendir(path.c_str())) != NULL) {
		while ((dirp = readdir(dp)) != NULL) {
			std::string bin = dirp->d_name;
			if(bin != "." && bin != ".." && dirp->d_type != DT_CHR) files.push_back(bin);
		}
		closedir(dp);
		return true;
	}
	return false;
}

void Launcher::autoComplete() {
	if(this->results.size() > 0) {
		autoCorrectIndex++;
		if(autoCorrectIndex >= results.size() || autoCorrectIndex >= 10) autoCorrectIndex = 0;
		if(autoCorrectIndex >= 0) this->searchPhrase = this->results.at(autoCorrectIndex);
	}
}

void Launcher::getPaths() {
	this->path =  std::getenv("PATH");
	std::stringstream ss_path(path);
	std::string path;

	while(std::getline(ss_path, path, ':')) {
		fetchFiles(path);
	}
}

void Launcher::search() {
	/* Used to search in path after suggestions */
	this->autoCorrectIndex = -1;
	this->results.clear();
	if(searchPhrase.length() != 0) {
		for(int i = 0; i < files.size(); i++) {
			if (files.at(i).find(searchPhrase) != std::string::npos) {
			this->results.push_back(files.at(i));
			}
		}
	}
}

LauncherState Launcher::getState() { return this->state; }
void Launcher::setState(LauncherState state) { this->state = state; }
int Launcher::getWindowID() { return this->launcher; }

