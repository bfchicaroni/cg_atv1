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
        text = "0"; // mt::format("{}'s turn", m_XsTurn ?
                    // 'X' : 'O');
        break;
      case CalcState::secondNumber:
        text = std::to_string(number);
        break;
      case CalcState::Result:
        text = std::to_string(number);
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
      if (ImGui::BeginTable("Claculator", m_N)) {
        for (auto i : iter::range(m_N)) {
          ImGui::TableNextRow();
          for (auto j : iter::range(m_N)) {
            ImGui::TableSetColumnIndex(j);
            auto const offset{i * m_N + j};

            // Get current character
            auto ch{m_board.at(offset)};

            switch (i) {
            case 2:
              if (j == 0) {
                ch = '1';
              } else if (j == 1) {
                ch = '2';
              } else {
                ch = '3';
              }
              break;
            case 1:
              if (j == 0) {
                ch = '4';
              } else if (j == 1) {
                ch = '5';
              } else {
                ch = '6';
              }
              break;
            case 0:
              if (j == 0) {
                ch = '7';
              } else if (j == 1) {
                ch = '8';
              } else {
                ch = '9';
              }
              break;
            }

            // Button text is ch followed by an ID in the format ##ij
            auto buttonText{fmt::format("{}", ch)};
            if (ImGui::Button(buttonText.c_str(), ImVec2(-1, buttonHeight))) {
              if (m_calcState == CalcState::firstNumber) {
                number = gsl::narrow_cast<int>(ch) - 48;
                m_calcState = CalcState::secondNumber;
              } else if (m_calcState == CalcState::secondNumber) {
                number = number + gsl::narrow_cast<int>(ch) - 48;
                m_calcState = CalcState::Result;
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
  number = 0;
  m_calcState = CalcState::firstNumber;
}