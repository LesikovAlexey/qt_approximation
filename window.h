
#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QMessageBox>

#include "method_compute.h"
#include "method1_init.h"
#include "method2_init.h"

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
  double *values_2;
  double *file_values_2;
  double *derivatives_2;
  double *state_2;
  bool working_with_file;
  bool show_graph_1;
  bool show_graph_2;
  bool show_graph_err;
  bool initialized;
  bool initialized_2;
  double scale_parameter;
  double max_buf;
  double max_buf_1;
  double max_buf_2;
  int disturb;

public:
  Window(QWidget *parent);
  ~Window();
  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  int parse_command_line(int argc, char *argv[]);
  double Pf(double x, int n_appr);
  double Pf_2(double x, int n_appr);
  double Errf(double x, int n_appr);
  double Errf_2(double x, int n_appr);
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
  void add_disturb();
  void sub_disturb();

protected:
  void paintEvent(QPaintEvent *event);
};

#endif
