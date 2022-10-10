#include "window.hpp"
#include <gsl/gsl_util>

void Window::onCreate() {
  // Load font with bigger size for the X's and O's
  auto const filename{abcg::Application::getAssetsPath() +
                      "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 72.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError{"Cannot load font file"};
  }

  clearMemory();
}

void Window::onPaintUI() {
  // Get size of application's window
  auto const appWindowWidth{gsl::narrow<float>(getWindowSettings().width)};
  auto const appWindowHeight{gsl::narrow<float>(getWindowSettings().height)};

  // "Calculadora" window
  {
    ImGui::SetNextWindowSize(ImVec2(appWindowWidth, appWindowHeight));
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    auto const flags{ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoCollapse};
    ImGui::Begin("Calculadora", nullptr, flags);

    // Menu
    {
      bool restartSelected{};
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Calculadora")) {
          ImGui::MenuItem("Clear", nullptr, &restartSelected);
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }
      if (restartSelected) {
        clearMemory();
      }
    }

    // Static text showing current turn or win/draw messages
    {
      std::string text;
      switch (m_calcState) {
      case CalcState::firstNumber:
        text = "0";
        break;
      case CalcState::Signal:
        text = std::to_string(fstNumber);
        break;
      case CalcState::Sum:
        text = fmt::format("{} + ", std::to_string(fstNumber));
        break;
      case CalcState::Sub:
        text = fmt::format("{} - ", std::to_string(fstNumber));
        break;
      case CalcState::Mult:
        text = fmt::format("{} x ", std::to_string(fstNumber));
        break;
      case CalcState::waitEqual:
        text = fmt::format("{} {} {} ", std::to_string(fstNumber), char(signal),
                           std::to_string(sndNumber));
        ;
        break;
      case CalcState::Result:
        text = std::to_string(result);
        break;
      }
      ImGui::SetCursorPosX(
          (appWindowWidth - ImGui::CalcTextSize(text.c_str()).x) - 30);
      ImGui::Text("%s", text.c_str());
      ImGui::Spacing();
    }

    ImGui::Spacing();

    {
      auto const gridHeight{appWindowHeight - 22 - 60 - (m_N * 10) - 60};
      auto const buttonHeight{gridHeight / m_N};

      // Use custom font
      ImGui::PushFont(m_font);
      if (ImGui::BeginTable("Calculator", m_N)) {
        for (auto i : iter::range(m_N)) {
          ImGui::TableNextRow();
          for (auto j : iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);
            auto const offset{i * m_N + j};

            // Get current character
            auto ch{m_board.at(offset)};

            switch (i) {
            case 3:
              if (j == 1) {
                ch = '0';
              } else if (j == 3) {
                ch = '=';
              }
              break;
            case 2:
              if (j == 0) {
                ch = '1';
              } else if (j == 1) {
                ch = '2';
              } else if (j == 2) {
                ch = '3';
              } else {
                ch = 'x';
              }
              break;
            case 1:
              if (j == 0) {
                ch = '4';
              } else if (j == 1) {
                ch = '5';
              } else if (j == 2) {
                ch = '6';
              } else {
                ch = '-';
              }
              break;
            case 0:
              if (j == 0) {
                ch = '7';
              } else if (j == 1) {
                ch = '8';
              } else if (j == 2) {
                ch = '9';
              } else {
                ch = '+';
              }
              break;
            }

            auto buttonText{fmt::format("{}", ch)};
            if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              if (m_calcState == CalcState::firstNumber) {
                fstNumber = gsl::narrow_cast<int>(ch) - 48;
                m_calcState = CalcState::Signal;
              } else if (m_calcState == CalcState::Signal) {
                signal = gsl::narrow_cast<int>(ch);
                switch (signal) {
                case 43:
                  m_calcState = CalcState::Sum;
                  break;
                case 45:
                  m_calcState = CalcState::Sub;
                  break;
                case 120:
                  m_calcState = CalcState::Mult;
                  break;
                }
              } else if (m_calcState == CalcState::waitEqual) {
                int equal = gsl::narrow_cast<int>(ch);
                if (equal == 61) {
                  m_calcState = CalcState::Result;
                }
              } else {
                sndNumber = gsl::narrow_cast<int>(ch) - 48;
                if (m_calcState == CalcState::Sum) {
                  result = fstNumber + sndNumber;
                } else if (m_calcState == CalcState::Sub) {
                  result = fstNumber - sndNumber;
                } else if (m_calcState == CalcState::Mult) {
                  result = fstNumber * sndNumber;
                }
                m_calcState = CalcState::waitEqual;
              }
            }
          }
          ImGui::Spacing();
        }
        ImGui::EndTable();
      }
      ImGui::PopFont();
    }

    ImGui::Spacing();

    // "Clear memory" button
    {
      if (ImGui::Button("Clear", ImVec2(-1, 50))) {
        clearMemory();
      }
    }

    ImGui::End();
  }
}

void Window::clearMemory() {
  fstNumber = 0;
  m_calcState = CalcState::firstNumber;
}