#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include "abcgOpenGL.hpp"

class Window : public abcg::OpenGLWindow {
protected:
  void onCreate() override;
  void onPaintUI() override;

private:
  static int const m_N{4};

  enum class CalcState {
    firstNumber,
    Signal,
    Sum,
    Sub,
    Mult,
    waitEqual,
    Result
  };
  CalcState m_calcState;
  int fstNumber, signal, sndNumber, result;

  std::array<char, m_N * m_N> m_board{};

  ImFont *m_font{};

  void clearMemory();
};

#endif