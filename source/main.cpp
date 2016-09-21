#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>

//#include <genn/netview.hpp>

#include <game.hpp>

class Window : public QWidget {
public:
	QHBoxLayout layout;
	GameQt field;
	
	//Network net;
	//NetView netview;
	
	Window() : QWidget()//,
		//net(), netview(net)
	{
		resize(800, 800);
		setWindowTitle("Evo2048");
		
		layout.addWidget(&field);
		//layout.addWidget(&netview);
		
		setLayout(&layout);
	}
	
	void keyPressEvent(QKeyEvent *event) override {
		QWidget::keyPressEvent(event);
		field.keyPressEvent(event);
	}
};

int main(int argc, char *argv[]) {
	srand(87654321);
	
	QApplication app(argc, argv);
	
	Window window;
	window.show();
	
	window.field.reset();
	// window.field.update();

	return app.exec();
}
