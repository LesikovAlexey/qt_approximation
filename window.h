
#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMessageBox>

class Window : public QWidget
{
  Q_OBJECT

private:
  int func_id;
  const char *f_name;
  double a;
  double b;
  int n;
  int n_appr;
  double f(double);
  double *values;
  double *file_values;
  double *derivatives;
  double *state;
  bool working_with_file;
  bool show_graph_1;
  bool show_graph_2;
  bool show_graph_err;
  double scale_parameter;

public:
  Window(QWidget *parent);
  ~Window();
  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  int parse_command_line(int argc, char *argv[]);
  double Pf(double x, int n_appr);
  double Errf(double x, int n_appr);
  int Pf_init(int n_appr, double a, double b);
  int Pf2_init(int n_appr, double a, double b);
  double f_file(double x);
public slots:
  void change_func();
  void change_func_file();
  void add_n_appr();
  void reduce_n_appr();
  void show_method_1();
  void show_method_2();
  void show_err();
  void scale_up();
  void scale_down();

protected:
  void paintEvent(QPaintEvent *event);
};

#endif
