#pragma once

#include "GameManager.h"

namespace Shoot {

    using namespace System;
    using namespace System::ComponentModel;
    using namespace System::Collections;
    using namespace System::Windows::Forms;
    using namespace System::Data;
    using namespace System::Drawing;

    public ref class Form1 : public System::Windows::Forms::Form
    {
    public:
        Form1(void)
        {
            InitializeComponent();
            this->DoubleBuffered = true;

            game = gcnew GameManager();

            gameTimer = gcnew Timer();
            gameTimer->Interval = 1000; // 1 секунда
            gameTimer->Tick += gcnew EventHandler(this, &Form1::OnTimerTick);
            gameTimer->Start();

            game->SpawnTarget(this->ClientSize.Width, this->ClientSize.Height);
        }

    protected:
        ~Form1()
        {
            if (components)
            {
                delete components;
            }
        }

    private:
        GameManager^ game;
        Timer^ gameTimer;
        System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
        void InitializeComponent(void)
        {
            this->SuspendLayout();
            // 
            // Form1
            // 
            this->ClientSize = System::Drawing::Size(800, 450);
            this->Name = L"Form1";
            this->Text = L"Shooting Game";
            this->Paint += gcnew PaintEventHandler(this, &Form1::Form1_Paint);
            this->MouseClick += gcnew MouseEventHandler(this, &Form1::Form1_MouseClick);
            this->ResumeLayout(false);
        }
#pragma endregion

    private:
        System::Void OnTimerTick(System::Object^ sender, System::EventArgs^ e) {
            game->Tick();
            this->Invalidate();
        }

        System::Void Form1_Paint(System::Object^ sender, System::Windows::Forms::PaintEventArgs^ e) {
            game->DrawAll(e->Graphics);
            e->Graphics->DrawString("Очки: " + game->Score, gcnew Font("Arial", 14), Brushes::Black, 10, 10);
            e->Graphics->DrawString("Час: " + game->TimeLeft, gcnew Font("Arial", 14), Brushes::Black, 10, 30);
        }

        System::Void Form1_MouseClick(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e) {
            game->CheckHit(e->X, e->Y);
            this->Invalidate();
        }
    };
}
