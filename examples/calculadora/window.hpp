#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static int const m_N{3}; // Board size is m_N x m_N

  enum class CalcState { firstNumber, secondNumber, Result };
  CalcState m_calcState;
  int number;

  std::array<char, m_N * m_N> m_board{}; // '\0', 'X' or 'O'

  ImFont *m_font{};

  void clearMemory();
};

#endif