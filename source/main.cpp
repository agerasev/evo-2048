#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>

#include <genn/netview.hpp>

#include <game.hpp>

class Window : public QWidget {
public:
	QHBoxLayout layout;
	GameQt field;
	
	NetworkGene gene;
	NetView netview;
	
	Window() : QWidget(),
		gene(), netview()
	{
		resize(1200, 600);
		setWindowTitle("Evo2048");
		
		layout.addWidget(&field, 1);
		layout.addWidget(&netview, 1);
		
		setLayout(&layout);
		
		gene.nodes[NodeID(1)] = NodeGene(1);
		gene.nodes[NodeID(2)] = NodeGene(-1);
		gene.nodes[NodeID(3)] = NodeGene(0);
		
		gene.links[LinkID(1,3)] = LinkGene(1);
		gene.links[LinkID(2,3)] = LinkGene(-1);
		
		netview.sync(gene);
		netview.move(0.0);
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

	return app.exec();
}
