#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <vector>
#include <QLineEdit>
using namespace std;

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

private slots:
    void slotOnButtonClicked();

private:
    vector<vector<string>> buttons;
    QVBoxLayout *outerLayout;
    QGridLayout *grid;
    QLineEdit *display;
    vector<string> tokenize(const string s);
    float expr(vector<string>& word, int& idx);
    float expr_(vector<string>& word, int& idx, float value);
    float term(vector<string>& word, int& idx);
    float term_(vector<string>& word, int& idx, float value);
    float factor(vector<string>& word, int& idx);
    void clear();
    void del();
};

#endif // WINDOW_H
