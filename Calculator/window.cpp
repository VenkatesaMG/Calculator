#include "window.h"
#include <QString>

Window::Window(QWidget *parent)
    : QWidget{parent}
{
    this->setStyleSheet(
        "QWidget {"
        "   background-color: #1e1e1e;"
        "   color: white;"
        "   font-size: 20px;"
        "}"

        "QPushButton {"
        "   background-color: #3a3a3a;"
        "   border: 1px solid #555;"
        "   padding: 10px;"
        "   width: 20px;"
        "   height: 20px;"
        "   border-radius: 10px;"
        "}"

        "QPushButton:hover {"
        "   background-color: #505050;"
        "}"

        "QPushButton:pressed {"
        "   background-color: #222;"
        "}"

        "QLineEdit {"
        "   background-color: black;"
        "   border: 1px solid #888;"
        "   padding: 6px;"
        "   font-size: 24px;"
        "   border-radius: 10px 10px 0px 0px;"
        "}"
        );

    buttons = {
        {"C","D","(",")"},
        {"1", "2", "3", "+"},
        {"4", "5", "6", "-"},
        {"7", "8", "9", "*"},
        {"0", ".", "=", "/"},
    };

    outerLayout = new QVBoxLayout;
    grid        = new QGridLayout;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {

            QPushButton *btn = new QPushButton(
                QString::fromStdString(buttons[i][j]),
                this
                );
            connect(btn, &QPushButton::clicked, this, &Window::slotOnButtonClicked);
            grid->addWidget(btn, i, j);
        }
    }
    display = new QLineEdit("", this);
    display->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    outerLayout->addWidget(display);
    outerLayout->addLayout(grid);
    setLayout(outerLayout);
}

void Window::slotOnButtonClicked(){
    QPushButton *btn = qobject_cast<QPushButton*>(sender());
    if(!btn) return;

    QString text = btn->text();
    if(text == "CLR"){
        display->clear();
        return;
    } else if(text == "DEL"){
        QString tmp = display->text();
        if(!tmp.isEmpty()){
            display->backspace();
        }
        return;
    } else if (text == "="){
        string value = display->text().toStdString();
        auto tokens = this->tokenize(value);
        int idx = 0;
        float result = this->expr(tokens, idx);
        display->setText(QString::number(result));
        return;
    }
    display->setText(display->text() + text);
}

vector<string> Window::tokenize(const string s){
    vector<string> words;
    int i = 0;
    while(i < s.size()){
        if(s[i] == ' '){
            i++;
            continue;
        }

        string tmp = string(1, s[i]);
        if(s[i] >= '0' && s[i] <= '9'){
            int j = i + 1;
            while(j < s.size() && s[j] >= '0' && s[j] <= '9'){
                tmp += s[j];
                j++;
            }
            i = j - 1;
        }
        words.push_back(tmp);
        i++;
    }
    return words;
}

float Window::expr(vector<string>& word, int& idx) {
    float value = this->term(word, idx);
    return this->expr_(word, idx, value);
}

float Window::expr_(vector<string>& word, int& idx, float value) {
    if (idx >= word.size()) return value;

    if (word[idx] == "+") {
        idx++;
        float t = this->term(word, idx);
        return this->expr_(word, idx, value + t);
    }
    else if (word[idx] == "-") {
        idx++;
        float t = this->term(word, idx);
        return this->expr_(word, idx, value - t);
    }

    return value;
}

float Window::term(vector<string>& word, int& idx) {
    float value = factor(word, idx);
    return this->term_(word, idx, value);
}

float Window::term_(vector<string>& word, int& idx, float value) {
    if (idx >= word.size()) return value;

    if (word[idx] == "*") {
        idx++;
        float f = this->factor(word, idx);
        return this->term_(word, idx, value * f);
    }
    else if (word[idx] == "/") {
        idx++;
        float f = this->factor(word, idx);
        return term_(word, idx, value / f);
    }

    return value;
}

float Window::factor(vector<string>& word, int& idx) {
    if (idx >= word.size()) return NAN;

    if (word[idx] == "(") {
        idx++;
        float value = this->expr(word, idx);

        if (idx < word.size() && word[idx] == ")") {
            idx++;
            return value;
        }
        return NAN;
    }

    float x = stof(word[idx]);
    idx++;
    return x;
}
