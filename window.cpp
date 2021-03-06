
#include <QPainter>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>

#include "window.h"

#define DEFAULT_A -10
#define DEFAULT_B 10

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
  return this->file_values[(int)(((int)x - a) / ((b - a) / n_appr))];
}

double Window::f(double x)
{

  switch (func_id)
  {
  case 0:
    f_name = "k = 0   f (x) = 1";
    if (fabs(x - a - ((b - a) / (n_appr - 1)) * (n_appr / 2)) < 1e-13)
      return f_0(x) + disturb * 0.1 * max_buf;
    return (f_0(x));
  case 1:
    f_name = "k = 1   f (x) = x";
    if (fabs(x - a - ((b - a) / (n_appr - 1)) * (n_appr / 2)) < 1e-13)
      return f_1(x) + disturb * 0.1 * max_buf;
    return (f_1(x));
  case 2:
    f_name = "k = 2   f (x) = x * x";
    if (fabs(x - a - ((b - a) / (n_appr - 1)) * (n_appr / 2)) < 1e-13)
      return f_2(x) + disturb * 0.1 * max_buf;
    return (f_2(x));
  case 3:
    f_name = "k = 3   f (x) = x * x * x";
    if (fabs(x - a - ((b - a) / (n_appr - 1)) * (n_appr / 2)) < 1e-13)
      return f_3(x) + disturb * 0.1 * max_buf;
    return (f_3(x));
  case 4:
    f_name = "k = 4   f (x) = x * x * x * x";
    if (fabs(x - a - ((b - a) / (n_appr - 1)) * (n_appr / 2)) < 1e-13)
      return f_4(x) + disturb * 0.1 * max_buf;
    return (f_4(x));
  case 5:
    f_name = "k = 5   f (x) = exp(x)";
    if (fabs(x - a - ((b - a) / (n_appr - 1)) * (n_appr / 2)) < 1e-13)
      return f_5(x) + disturb * 0.1 * max_buf;
    return (f_5(x));
  case 6:
    f_name = "k = 6   f (x) = 1 / (25 * x * x + 1)";
    if (fabs(x - a - ((b - a) / (n_appr - 1)) * (n_appr / 2)) < 1e-13)
      return f_6(x) + disturb * 0.1 * max_buf;
    return (f_6(x));
  case 7:
    if (fabs(x - a - ((b - a) / (n_appr - 1)) * (n_appr / 2)) < 1e-13)
      return this->f_file(x) + disturb * 0.1 * max_buf;
    return (this->f_file(x));
  default:
    return 0;
  }
}

int Window::Pf_init(int n_appr, double a, double b)
{
  double h = (b - a) / (double)(n_appr - 1);
  values = (double *)realloc(values, n_appr * sizeof(double));
  file_values = (double *)realloc(file_values, n_appr * sizeof(double));
  derivatives = (double *)realloc(derivatives, 2 * sizeof(double));
  state = (double *)realloc(state, 4 * n_appr * sizeof(double));
  for (int i = 0; i < n_appr; i++)
  {
    values[i] = 0;
    file_values[i] = 0;
  }
  for (int i = 0; i < 4 * n_appr; i++)
  {
    state[i] = 0;
  }
  for (int i = 0; i < 2; i++)
  {
    derivatives[i] = 0;
  }
  for (int i = 0; i < n_appr; i++)
  {
    values[i] = f(a + i * h);
  }
  derivatives[0] = (f(a + h / 1e10) - f(a)) / (h / 1e10);
  derivatives[1] = (f(b) - f(b - h / 1e10)) / (h / 1e10);
  method1_init(n_appr, a, b, values, derivatives, state);
  return 0;
}

int Window::Pf2_init(int n_appr, double a, double b)
{
  double h = (b - a) / (double)(n_appr - 1);
  values_2 = (double *)realloc(values_2, n_appr * sizeof(double));
  file_values_2 = (double *)realloc(file_values_2, n_appr * sizeof(double));
  derivatives_2 = (double *)realloc(derivatives_2, 2 * sizeof(double));
  state_2 = (double *)realloc(state_2, 4 * n_appr * sizeof(double));
  for (int i = 0; i < n_appr; i++)
  {
    values_2[i] = 0;
    file_values_2[i] = 0;
  }
  for (int i = 0; i < 4 * n_appr; i++)
  {
    state_2[i] = 0;
  }
  for (int i = 0; i < 2; i++)
  {
    derivatives_2[i] = 0;
  }
  for (int i = 0; i < n_appr; i++)
  {
    values_2[i] = f(a + i * h);
  }
  derivatives_2[0] = 0;
  derivatives_2[1] = 0;
  method2_init(n_appr, a, b, values_2, derivatives_2, state_2);
  return 0;
}

double Window::Pf(double x, int n_appr)
{
  return method_compute(n_appr, a, b, x, state);
}

double Window::Pf_2(double x, int n_appr)
{
  return method_compute(n_appr, a, b, x, state_2);
}

double Window::Errf(double x, int n_appr)
{
  return fabs(Pf(x, n_appr) - f(x));
}

double Window::Errf_2(double x, int n_appr)
{
  return fabs(Pf_2(x, n_appr) - f(x));
}

Window::Window(QWidget *parent)
    : QWidget(parent)
{
  a = DEFAULT_A;
  b = DEFAULT_B;
  n = width();
  n_appr = 20;
  working_with_file = 0;
  show_graph_1 = 1;
  show_graph_2 = 0;
  show_graph_err = 0;
  scale_parameter = 1;
  disturb = 0;
  max_buf = 0;
  max_buf_1 = 0;
  max_buf_2 = 0;
  initialized = 0;
  initialized_2 = 0;
  func_id = 6;

  change_func();

  values = (double *)malloc(n_appr * sizeof(double));
  file_values = (double *)malloc(n_appr * sizeof(double));
  derivatives = (double *)malloc(2 * sizeof(double));
  state = (double *)malloc(4 * n_appr * sizeof(double));
  values_2 = (double *)malloc(n_appr * sizeof(double));
  file_values_2 = (double *)malloc(n_appr * sizeof(double));
  derivatives_2 = (double *)malloc(2 * sizeof(double));
  state_2 = (double *)malloc(4 * n_appr * sizeof(double));
}

Window::~Window()
{
  free(values);
  free(file_values);
  free(derivatives);
  free(state);
  free(values_2);
  free(file_values_2);
  free(derivatives_2);
  free(state_2);
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

  if (sscanf(argv[1], "%lf", &a) != 1 || sscanf(argv[2], "%lf", &b) != 1 || sscanf(argv[4], "%d", &func_id) != 1 || b - a < 1.e-6 || (sscanf(argv[3], "%d", &n_appr) != 1) || n_appr <= 9 || func_id < 0 || func_id > 6)
    return -2;
  if (argc == 6)
  {
    input_file = fopen(argv[5], "r");
    if (input_file == NULL)
    {
      printf("???? ?????????????? ?????????????? ????????\n");
      fclose(input_file);
      return -3;
    }
    for (int i = 0; i < n_appr; i++)
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
  initialized = 0;
  initialized_2 = 0;
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
    initialized = 0;
    initialized_2 = 0;
    update();
  }
}
void Window::add_n_appr()
{
  n_appr += 10;
  initialized = 0;
  initialized_2 = 0;
  update();
}

void Window::reduce_n_appr()
{
  if (n_appr - 10 > 9)
  {
    n_appr -= 10;
  }
  initialized = 0;
  initialized_2 = 0;
  update();
}

void Window::add_disturb()
{
  disturb += 1;
  initialized = 0;
  initialized_2 = 0;
  update();
}

void Window::sub_disturb()
{
  disturb -= 1;
  initialized = 0;
  initialized_2 = 0;
  update();
}

void Window::show_method_1()
{
  show_graph_1 = (show_graph_1 + 1) % 2;
  initialized = 0;
  update();
}
void Window::show_method_2()
{
  show_graph_2 = (show_graph_2 + 1) % 2;
  initialized_2 = 0;
  update();
}
void Window::show_err()
{
  show_graph_err = (show_graph_err + 1) % 2;
  show_graph_1 = 0;
  show_graph_2 = 0;
  initialized = 0;
  initialized_2 = 0;
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

void Window::paintEvent(QPaintEvent * )
{
  QPainter painter(this);
  double x1, x2, y1, y2;
  double max_y, min_y;
  n = width();
  double delta_y, delta_x = (b - a) / (n * scale_parameter);

  
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
  painter.setPen(QPen(Qt::black, 10/height()));
  painter.save();

  painter.translate(0.5 * width(), 0.5 * height());
  painter.scale(scale_parameter * width() / (b - a), -height() / (max_y - min_y));
  painter.translate(-0.5 * (a + b), -0.5 * (min_y + max_y));


  if (show_graph_err == 0)
  {
    max_buf = 0;
    x1 = a;
    y1 = f(x1);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = f(x2);
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
      if (max_buf < fabs(y2))
      {
        max_buf = fabs(y2);
      }
      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = f(x2);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
  }

  if (show_graph_1 == 1)
  {
    painter.setPen(QPen(Qt::blue, 10/height()));
    if (initialized == 0)
    {
      Pf_init(n_appr, a, b);
      initialized = 1;
    }
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
    painter.setPen(QPen(Qt::darkGreen, 10/height()));
    if (initialized_2 == 0)
    {
      Pf2_init(n_appr, a, b);
      initialized_2 = 1;
    }
    x1 = a;
    y1 = Pf_2(x1, n_appr);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = Pf_2(x2, n_appr);
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = Pf_2(x2, n_appr);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
  }

  if (show_graph_err == 1)
  {
    max_buf_1 = 0;
    painter.setPen(QPen(Qt::blue, 10/height()));
    if (initialized == 0)
    {
      Pf_init(n_appr, a, b);
      initialized = 1;
    }

    x1 = a;
    y1 = Errf(x1, n_appr);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = Errf(x2, n_appr);
      if (max_buf_1 < y2)
      {
        max_buf_1 = y2;
      }
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = Errf(x2, n_appr);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

    max_buf_2 = 0;
    painter.setPen(QPen(Qt::darkGreen, 10/height()));
    if (initialized_2 == 0)
    {
      Pf2_init(n_appr, a, b);
      initialized_2 = 1;
    }
    x1 = a;
    y1 = Errf_2(x1, n_appr);
    for (x2 = x1 + delta_x; x2 - b < 1.e-6; x2 += delta_x)
    {
      y2 = Errf_2(x2, n_appr);
      if (max_buf_2 < y2)
      {
        max_buf_2 = y2;
      }
      painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));

      x1 = x2, y1 = y2;
    }
    x2 = b;
    y2 = Errf_2(x2, n_appr);
    painter.drawLine(QPointF(x1, y1), QPointF(x2, y2));
  }

  painter.setPen(QPen(Qt::red, 10/height()));
  painter.drawLine(a, 0, b, 0);
  painter.drawLine(0, max_y, 0, min_y);

  painter.restore();

  painter.setPen(QPen(Qt::black, 10/height()));
  painter.drawText(0, 20, f_name);
  painter.drawText(0, 40, "appr points = " + QString::number(n_appr));

  if (show_graph_1 == 1 || show_graph_err == 1)
  {
    painter.setPen(QPen(Qt::blue, 10/height()));
    painter.drawText(0, 80, "- method 1");
    if (show_graph_err == 1)
    {
      painter.drawText(0, 100, "Err1 = " + QString::number(max_buf_1));
    }
  }
  if (show_graph_2 == 1 || show_graph_err == 1)
  {
    painter.setPen(QPen(Qt::darkGreen, 10/height()));
    painter.drawText(0, 140, "- method 2");
    if (show_graph_err == 1)
    {
      painter.drawText(0, 160, "Err2 = " + QString::number(max_buf_2));
    }
  }

  painter.drawText(0, 200, "disturbance = " + QString::number(disturb));
}
