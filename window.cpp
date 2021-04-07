
#include <QPainter>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "window.h"
#include "method_compute.h"
#include "method1_init.h"
#include "method2_init.h"

#define DEFAULT_A -10
#define DEFAULT_B 10
#define DEFAULT_N 100

static double f_0(double x)
{
  return 1;
}

static double f_1(double x)
{
  return x;
}

static double f_2(double x)
{
  return x * x;
}

static double f_3(double x)
{
  return x * x * x;
}

static double f_4(double x)
{
  return x * x * x * x;
}

static double f_5(double x)
{
  return exp(x);
}

static double f_6(double x)
{
  return 1 / (25 * x * x + 1);
}

double Window::f_file(double x)
{
  return this->file_values[(int)(((int)x - a) / ((b - a) / n))];
}

double Window::f(double x)
{
  switch (func_id)
  {
  case 0:
    f_name = "k = 0   f (x) = 1";
    return (f_0(x));
  case 1:
    f_name = "k = 1   f (x) = x";
    return (f_1(x));
  case 2:
    f_name = "k = 2   f (x) = x * x";
    return (f_2(x));
  case 3:
    f_name = "k = 3   f (x) = x * x * x";
    return (f_3(x));
  case 4:
    f_name = "k = 4   f (x) = x * x * x * x";
    return (f_4(x));
  case 5:
    f_name = "k = 5   f (x) = exp(x)";
    return (f_5(x));
  case 6:
    f_name = "k = 6   f (x) = 1 / (25 * x * x + 1)";
    return (f_6(x));
  case 7:
    return (this->f_file(x));
  default:
    return 0;
  }
}

int Window::Pf_init(int n_appr, double a, double b)
{
  double h = (b - a) / (n_appr - 1);
  for (int i = 0; i < n_appr; i++)
  {
    values[i] = f(a + i * h);
  }
  derivatives[0] = (f(a + h / 100) - f(a)) / (h / 100);
  derivatives[1] = (f(b) - f(b - h / 100)) / (h / 100);
  method1_init(n_appr, a, b, values, derivatives, state);
  return 0;
}

int Window::Pf2_init(int n_appr, double a, double b)
{
  double h = (b - a) / (n_appr - 1);
  for (int i = 0; i < n_appr; i++)
  {
    values[i] = f(a + i * h);
  }
  method2_init(n_appr, a, b, values, derivatives, state);
  return 0;
}

double Window::Pf(double x, int n_appr)
{
  return method_compute(n_appr, a, b, x, state);
}

double Window::Errf(double x, int n_appr)
{
  return fabs(Pf(x, n_appr) - f(x));
}

Window::Window(QWidget *parent)
    : QWidget(parent)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  n = DEFAULT_N;
  n_appr = 20;
  working_with_file = 0;
  show_graph_1 = 1;
  show_graph_2 = 0;
  show_graph_err = 0;
  scale_parameter = 1;

  func_id = 6;

  change_func();

  values = (double *)calloc(n, sizeof(double));
  file_values = (double *)calloc(n, sizeof(double));
  derivatives = (double *)calloc(2, sizeof(double));
  state = (double *)calloc(4 * n, sizeof(double));
}

Window::~Window()
{
  free(values);
  free(file_values);
  free(derivatives);
  free(state);
}
QSize Window::minimumSizeHint() const
{
  return QSize(100, 100);
}

QSize Window::sizeHint() const
{
  return QSize(1000, 1000);
}

int Window::parse_command_line(int argc, char *argv[])
{
  FILE *input_file;
  if (argc == 1)
    return 0;

  if (argc == 2)
    return -1;

  if (sscanf(argv[1], "%lf", &a) != 1 || sscanf(argv[2], "%lf", &b) != 1 || sscanf(argv[4], "%d", &func_id) != 1 || b - a < 1.e-6 || (argc > 3 && sscanf(argv[3], "%d", &n) != 1) || n <= 0 || func_id < 0 || func_id > 6)
    return -2;
  if (argc == 6)
  {
    input_file = fopen(argv[5], "r");
    if (input_file == NULL)
    {
      printf("Не удалось считать файл\n");
      fclose(input_file);
      return -3;
    }
    for (int i = 0; i < n; i++)
    {
      if (fscanf(input_file, "%lf", &file_values[i]) != 1)
      {
        fclose(input_file);
        return -1;
      }
    }
    fclose(input_file);
    working_with_file = 1;
  }
  return 0;
}

/// change current function for drawing
void Window::change_func()
{
  func_id = (func_id + 1) % 7;

  switch (func_id)
  {
  case 0:
    f_name = "k = 0   f (x) = 1";
    break;
  case 1:
    f_name = "k = 1   f (x) = x";
    break;
  case 2:
    f_name = "k = 2   f (x) = x * x";
    break;
  case 3:
    f_name = "k = 3   f (x) = x * x * x";
    break;
  case 4:
    f_name = "k = 4   f (x) = x * x * x * x";
    break;
  case 5:
    f_name = "k = 5   f (x) = exp(x)";
    break;
  case 6:
    f_name = "k = 6   f (x) = 1 / (25 * x * x + 1)";
    break;
  }
  update();
}

void Window::change_func_file()
{
  if (working_with_file == 0)
  {
    QMessageBox::warning(0, "No input file!",
                         "No input file!");
  }
  else
  {
    func_id = 7;
    f_name = "func from file";
    update();
  }
}
void Window::add_n_appr()
{
  n_appr += 10;
  update();
}

void Window::reduce_n_appr()
{
  if (n_appr - 10 > 0)
  {
    n_appr -= 10;
  }
  update();
}

void Window::show_method_1()
{
  show_graph_1 = (show_graph_1 + 1) % 2;
  update();
}
void Window::show_method_2()
{
  show_graph_2 = (show_graph_2 + 1) % 2;
  update();
}
void Window::show_err()
{
  show_graph_err = (show_graph_err + 1) % 2;
  show_graph_1 = 0;
  show_graph_2 = 0;
  update();
}

void Window::scale_up()
{
  scale_parameter *= 2;
  update();
}
void Window::scale_down()
{
  if (scale_parameter > 1)
  {
    scale_parameter /= 2;
    update();
  }
}
/// render graph
void Window::paintEvent(QPaintEvent * /* event */)
{
  QPainter painter(this);
  double x1, x2, y1, y2;
  double max_y, min_y;
  double delta_y, delta_x = (b - a) / n;

  painter.setPen("black");

  // calculate min and max for current function
  max_y = min_y = 0;
  for (x1 = a; x1 - b < 1.e-6; x1 += delta_x)
  {
    y1 = f(x1);
    if (y1 < min_y)
      min_y = y1;
    if (y1 > max_y)
      max_y = y1;
  }

  delta_y = 0.01 * (max_y - min_y);
  min_y -= delta_y;
  max_y += delta_y;

  // save current Coordinate System
  painter.save();

  // make Coordinate Transformations
  painter.translate(0.5 * width(), 0.5 * height());
  painter.scale(scale_parameter * width() / (b - a), -height() / (max_y - min_y));
  painter.translate(-0.5 * (a + b), -0.5 * (min_y + max_y));

  // draw approximated line for graph
  if (show_graph_err == 0)
  {
    x1 = a;
    y1 = f(x1);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = f(x2);
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = f(x2);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
  }

  if (show_graph_1 == 1)
  {
    painter.setPen("blue");
    Pf_init(n_appr, a, b);
    x1 = a;
    y1 = Pf(x1, n_appr);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = Pf(x2, n_appr);
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = Pf(x2, n_appr);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
  }

  if (show_graph_2 == 1)
  {
    painter.setPen("green");
    Pf2_init(n_appr, a, b);
    x1 = a;
    y1 = Pf(x1, n_appr);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = Pf(x2, n_appr);
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = Pf(x2, n_appr);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
  }

  if (show_graph_err == 1)
  {
    painter.setPen("blue");
    Pf_init(n_appr, a, b);
    x1 = a;
    y1 = Errf(x1, n_appr);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = Errf(x2, n_appr);
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = Errf(x2, n_appr);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
    painter.setPen("green");
    Pf2_init(n_appr, a, b);
    x1 = a;
    y1 = Errf(x1, n_appr);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = Errf(x2, n_appr);
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = Errf(x2, n_appr);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
  }

  // draw axis
  painter.setPen("red");
  painter.drawLine(a, 0, b, 0);
  painter.drawLine(0, max_y, 0, min_y);

  // restore previously saved Coordinate System
  painter.restore();

  // render function name
  painter.setPen("black");
  painter.drawText(0, 20, f_name);
  painter.drawText(0, 40, "appr points = " + QString::number(n_appr));

  if (show_graph_1 == 1 || show_graph_err == 1)
  {
    painter.setPen("blue");
    painter.drawText(0, 60, "- method 1");
  }
  if (show_graph_2 == 1 || show_graph_err == 1)
  {
    painter.setPen("green");
    painter.drawText(0, 80, "- method 2");
  }
}
