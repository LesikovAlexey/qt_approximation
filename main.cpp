
#include <QApplication>
#include <QMainWindow>
#include <QVBoxLayout>
#include <QAction>
#include <QMenuBar>
#include <QMessageBox>

#include "window.h"

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QMainWindow *window = new QMainWindow;
  QMenuBar *tool_bar = new QMenuBar(window);
  Window *graph_area = new Window(window);
  QAction *action;

  if (graph_area->parse_command_line(argc, argv))
  {
    QMessageBox::warning(0, "Wrong input arguments!",
                         "Wrong input arguments!");
    return -1;
  }

  action = tool_bar->addAction("&Change function", graph_area, SLOT(change_func()));
  action->setShortcut(QString("Ctrl+C"));

  action = tool_bar->addAction("&Function from file", graph_area, SLOT(change_func_file()));
  action->setShortcut(QString("Ctrl+F"));

  action = tool_bar->addAction("&Add appr points", graph_area, SLOT(add_n_appr()));
  action->setShortcut(QString("Ctrl++"));

  action = tool_bar->addAction("&Reduce appr points", graph_area, SLOT(reduce_n_appr()));
  action->setShortcut(QString("Ctrl+-"));

  action = tool_bar->addAction("&Show method 1", graph_area, SLOT(show_method_1()));
  action->setShortcut(QString("1"));

  action = tool_bar->addAction("&Show method 2", graph_area, SLOT(show_method_2()));
  action->setShortcut(QString("2"));

  action = tool_bar->addAction("&Show error graph", graph_area, SLOT(show_err()));
  action->setShortcut(QString("3"));

  action = tool_bar->addAction("&Scale +", graph_area, SLOT(scale_up()));
  action->setShortcut(QString("+"));

  action = tool_bar->addAction("&Scale -", graph_area, SLOT(scale_down()));
  action->setShortcut(QString("-"));

  action = tool_bar->addAction("E&xit", window, SLOT(close()));
  action->setShortcut(QString("Ctrl+X"));

  tool_bar->setMaximumHeight(30);

  window->setMenuBar(tool_bar);
  window->setCentralWidget(graph_area);
  window->setWindowTitle("Graph");

  window->show();
  return app.exec();
}
