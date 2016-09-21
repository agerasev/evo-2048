#pragma once

#include <cstdlib>

#include <la/vec.hpp>

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

class Game {
public:
	enum Dir {
		NONE=0, UP, DOWN, LEFT, RIGHT
	};
	
	ivec2 dim;
	std::vector<int> cells;
	
	Game(const ivec2 &s = ivec2(4,4)) :
		dim(s), cells(s.x()*s.y()) 
	{
		for(int &x : cells) {
			x = 0;
		}
	}
	
	int &cell(int ix, int iy) {
		return cells[iy*dim.x() + ix];
	}
	
	void spawn() {
		int count = 0;
		for(int c : cells) {
			count += (c == 0);
		}
		if(count > 0) {
			count = rand() % count;
			for(int &c : cells) {
				if(c == 0) {
					if(count <= 0) {
						if((rand() % 10) == 0) {
							c = 2;
						} else {
							c = 1;
						}
						break;
					}
				}
				count -= 1;
			}
		}
	}
	
	bool slide(Dir dir) {
		bool moved = false;
		switch(dir) {
		case Dir::UP:
			for(int ix = 0; ix < dim.x(); ++ix) {
				int dst = 0;
				for(int iy = 0; iy < dim.y(); ++iy) {
					int &c = cell(ix, iy);
					if(c > 0 && dst != iy) {
						int &dc = cell(ix, dst);
						if(dc == 0) {
							dc = c;
							c = 0;
							moved = true;
						} else if(dc == c) {
							dc = c + 1;
							c = 0;
							dst += 1;
							moved = true;
						} else {
							dst += 1;
							iy -= 1;
						}
					}
				}
			}
			break;
		case Dir::DOWN:
			for(int ix = 0; ix < dim.x(); ++ix) {
				int dst = dim.y() - 1;
				for(int iy = dim.y() - 1; iy >= 0; --iy) {
					int &c = cell(ix, iy);
					if(c > 0 && dst != iy) {
						int &dc = cell(ix, dst);
						if(dc == 0) {
							dc = c;
							c = 0;
							moved = true;
						} else if(dc == c) {
							dc = c + 1;
							c = 0;
							dst -= 1;
							moved = true;
						} else {
							dst -= 1;
							iy += 1;
						}
					}
				}
			}
			break;
		case Dir::LEFT:
			for(int iy = 0; iy < dim.y(); ++iy) {
				int dst = 0;
				for(int ix = 0; ix < dim.x(); ++ix) {
					int &c = cell(ix, iy);
					if(c > 0 && dst != ix) {
						int &dc = cell(dst, iy);
						if(dc == 0) {
							dc = c;
							c = 0;
							moved = true;
						} else if(dc == c) {
							dc = c + 1;
							c = 0;
							dst += 1;
							moved = true;
						} else {
							dst += 1;
							ix -= 1;
						}
					}
				}
			}
			break;
		case Dir::RIGHT:
			for(int iy = 0; iy < dim.y(); ++iy) {
				int dst = dim.x() - 1;
				for(int ix = dim.x() - 1; ix >= 0; --ix) {
					int &c = cell(ix, iy);
					if(c > 0 && dst != ix) {
						int &dc = cell(dst, iy);
						if(dc == 0) {
							dc = c;
							c = 0;
							moved = true;
						} else if(dc == c) {
							dc = c + 1;
							c = 0;
							dst -= 1;
							moved = true;
						} else {
							dst -= 1;
							ix += 1;
						}
					}
				}
			}
			break;
		case Dir::NONE:
			break;
		}
		return moved;
	}
	
	bool step() {
		spawn();
		int count = 0;
		for(int c : cells) {
			count += (c == 0);
		}
		return count >= 16;
	}
	
	void reset() {
		for(int &c : cells) {
			c = 0;
		}
		spawn();
		spawn();
	}
	
	virtual ~Game() = default;
};

static const QColor BG = QColor("#bbada0");
static const int P2LEN = 12;
static const QColor P2[P2LEN] = {
	QColor("#cdc0b4"), // empty
	QColor("#eee4da"), // 2
	QColor("#ede0c8"), // 4
	QColor("#f2b179"), // 8
	QColor("#f59563"), // 16
	QColor("#f59563"), // 32
	QColor("#f65e3b"), // 64
	QColor("#edcf72"), // 128
	QColor("#edcc61"), // 256
	QColor("#edc850"), // 512
	QColor("#edc53f"), // 1024
	QColor("#edc22e")  // 2048 ...
};

class GameQt : public Game, public QWidget {
public:
	GameQt(const ivec2 &s = ivec2(4,4)) : Game(s), QWidget() {
		
	}
	
	void paintEvent(QPaintEvent *event) override {
		QWidget::paintEvent(event);
		QPainter painter(this);
		painter.fillRect(rect(), BG);
		double w = double(rect().width())/dim.x(), h = double(rect().height())/dim.y();
		double b = 8;
		for(int iy = 0; iy < dim.y(); ++iy) {
			for(int ix = 0; ix < dim.x(); ++ix) {
				int c = cell(ix, iy);
				QRectF r(ix*w + b, iy*h + b, w - 2*b, h - 2*b);
				vec2 rv(r.width(), r.height());
				painter.fillRect(r, P2[c >= P2LEN ? P2LEN : c]);
				if(c > 0) {
					QString name(std::to_string(1 << c).c_str());
					vec2 fm(painter.fontMetrics().width(name), painter.fontMetrics().height());
					float tf = min(0.8*rv/fm);
					if ((tf < 1) || (tf > 1.25)) {
						QFont f = painter.font();
						f.setPointSizeF(f.pointSizeF()*tf);
						painter.setFont(f);
					}
					painter.drawText(r, Qt::AlignCenter, name);
				}
			}
		}
	}
	
	void keyPressEvent(QKeyEvent *event) override {
		QWidget::keyPressEvent(event);
		Dir dir = NONE;
		switch (event->key()) {
		case Qt::Key_Up:
		case Qt::Key_W:
			dir = Dir::UP;
			break;
		case Qt::Key_Left:
		case Qt::Key_A:
			dir = Dir::LEFT;
			break;
		case Qt::Key_Down:
		case Qt::Key_S:
			dir = Dir::DOWN;
			break;
		case Qt::Key_Right:
		case Qt::Key_D:
			dir = Dir::RIGHT;
			break;
		}
		if(dir != NONE && slide(dir)) {
			step();
			update();
		}
	}
};
