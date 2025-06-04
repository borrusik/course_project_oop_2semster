#pragma once

#include "Game.h"

namespace ShootingTargetsApp {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;
    using namespace System::Drawing::Drawing2D;

    public ref class MyForm : public System::Windows::Forms::Form
    {
    public:
        MyForm(void)
        {
            InitializeComponent();

            game = new Game();
            game->InitTargets(5, this->ClientSize.Width, this->ClientSize.Height);

            timer = gcnew System::Windows::Forms::Timer();
            timer->Interval = 1000;
            timer->Tick += gcnew System::EventHandler(this, &MyForm::OnTimerTick);
            timer->Start();

            // Кнопка рестарта
            restartButton = gcnew System::Windows::Forms::Button();
            restartButton->Text = L"Restart";
            restartButton->Font = gcnew Drawing::Font("Segoe UI", 13, FontStyle::Bold);
            restartButton->BackColor = Color::FromArgb(255, 60, 200, 130);
            restartButton->ForeColor = Color::White;
            restartButton->Size = Drawing::Size(110, 45);
            restartButton->Location = Drawing::Point(this->ClientSize.Width - 120, 10);
            restartButton->FlatStyle = FlatStyle::Flat;
            restartButton->FlatAppearance->BorderSize = 0;
            restartButton->Click += gcnew System::EventHandler(this, &MyForm::OnRestartClick);
            this->Controls->Add(restartButton);

            this->DoubleBuffered = true;
            this->KeyPreview = true;
        }

    protected:
        ~MyForm()
        {
            if (components) delete components;
            delete game;
        }

    private:
        Game* game;
        System::Windows::Forms::Timer^ timer;
        System::Windows::Forms::Button^ restartButton;
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->SuspendLayout();
            // 
            // MyForm
            // 
            this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
            this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
            this->ClientSize = System::Drawing::Size(700, 500);
            this->Name = L"MyForm";
            this->Text = L"Стрільба по мішеням";
            this->BackColor = System::Drawing::Color::White;
            this->DoubleBuffered = true;
            this->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::MyForm_Paint);
            this->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::MyForm_MouseDown);
            this->Resize += gcnew System::EventHandler(this, &MyForm::MyForm_Resize);
            this->KeyDown += gcnew System::Windows::Forms::KeyEventHandler(this, &MyForm::MyForm_KeyDown);
            this->ResumeLayout(false);
        }
#pragma endregion

    private:
        // Современный визуал
        System::Void MyForm_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e)
        {
            // Градиентный фон
            LinearGradientBrush^ bg = gcnew LinearGradientBrush(
                Rectangle(0, 0, this->ClientSize.Width, this->ClientSize.Height),
                Color::FromArgb(210, 240, 255), Color::FromArgb(100, 170, 255), LinearGradientMode::Vertical);
            e->Graphics->FillRectangle(bg, 0, 0, this->ClientSize.Width, this->ClientSize.Height);

            // Полупрозрачная панель
            Rectangle panelRect(18, 15, 205, 70);
            SolidBrush^ panelBg = gcnew SolidBrush(Color::FromArgb(130, 40, 60, 80));
            e->Graphics->FillRectangle(panelBg, panelRect);
            e->Graphics->DrawRectangle(gcnew Pen(Color::White, 2), panelRect);

            String^ scoreStr = "Рахунок: " + game->score.ToString();
            String^ timeStr = "Час: " + game->timeLeft.ToString();

            e->Graphics->DrawString(scoreStr, gcnew Drawing::Font("Segoe UI", 22, FontStyle::Bold), Brushes::Gold, 31, 22);
            e->Graphics->DrawString(timeStr, gcnew Drawing::Font("Segoe UI", 18, FontStyle::Bold), Brushes::Cyan, 31, 53);

            // Мишени (красивый bullseye уже реализован в Target)
            game->Draw(e->Graphics);

            // Game Over — большая надпись
            if (game->timeLeft <= 0) {
                String^ over = "Гру завершено!";
                auto sz = e->Graphics->MeasureString(over, gcnew Drawing::Font("Impact", 38, FontStyle::Bold));
                auto cx = (this->ClientSize.Width - sz.Width) / 2;
                auto cy = (this->ClientSize.Height - sz.Height) / 2;
                e->Graphics->DrawString(over, gcnew Drawing::Font("Impact", 38, FontStyle::Bold), Brushes::Red, cx, cy);
            }
        }

        // Выстрел мышкой
        System::Void MyForm_MouseDown(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
        {
            if (game->timeLeft > 0) {
                game->OnClick(e->X, e->Y);
                this->Invalidate();
            }
        }

        // Таймер
        System::Void OnTimerTick(System::Object^ sender, System::EventArgs^ e)
        {
            if (game->timeLeft > 0) {
                game->Tick();
                this->Invalidate();
            }
            else {
                timer->Stop();
                this->Invalidate();
            }
        }

        // Кнопка рестарта
        System::Void OnRestartClick(System::Object^ sender, System::EventArgs^ e)
        {
            delete game;
            game = new Game();
            game->InitTargets(5, this->ClientSize.Width, this->ClientSize.Height);
            timer->Start();
            this->Invalidate();
        }

        // Горячая клавиша "R" — рестарт
        System::Void MyForm_KeyDown(System::Object^ sender, System::Windows::Forms::KeyEventArgs^ e)
        {
            if (e->KeyCode == Keys::R) {
                OnRestartClick(nullptr, nullptr);
            }
        }

        // Переместить кнопку при ресайзе окна + обновить размеры поля
        System::Void MyForm_Resize(System::Object^ sender, System::EventArgs^ e)
        {
            if (restartButton != nullptr) {
                restartButton->Location = Drawing::Point(this->ClientSize.Width - 120, 10);
            }
            if (game != nullptr) {
                game->Resize(this->ClientSize.Width, this->ClientSize.Height);
            }
            this->Invalidate();
        }
    };
}
