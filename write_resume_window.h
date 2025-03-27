#ifndef write_resume_window_h
#define write_resume_window_h

#include <QDialog>
#include <QString>
#include <QVector>
#include <QTimer>
#include <QFile>
#include <QStackedWidget>

class QLineEdit;
class QTextEdit;
class QLabel;
class QPushButton;
class QVBoxLayout;
class QScrollArea;
class NumIssueWindow;

class WriteResumeWindow : public QDialog {
    Q_OBJECT
    
public:
    WriteResumeWindow(QWidget* parent, const QString& company_dir);
    virtual ~WriteResumeWindow(); // 소멸자를 virtual로 선언
    
    void loadResume();
    void saveResume(bool silent = false);
    void setNumIssues(int num);
    
public slots:
    void onCheckClicked();
    void onAdaptClicked();
    void onRetouchClicked();
    void onCancelClicked();
    void onContentChanged();
    
private:
    QString company_directory;
    int num_issues;
    
    QVector<QLineEdit*> issue_inputs;
    QVector<QTextEdit*> write_issue_inputs;
    QVector<QLabel*> character_counters;
    QPushButton* check_button;
    QPushButton* adapt_button;
    QPushButton* retouch_button;
    QPushButton* cancel_button;
    QStackedWidget* page_stack;
    QPushButton* prev_button;
    QPushButton* next_button;
    QLabel* page_indicator;
    QTimer* auto_save_timer;
    
    void setupUi();
    void openNumIssueWindow();
    void autoSaveResume();
    void createResumeDirectory();
    void saveIssueToFile(int issueNumber, const QString& title, const QString& content);
    void loadIssueFromFile(int issueNumber);
    void showNextPage();
    void showPreviousPage();
    void updateNavigationButtons();

    QTimer* autoSaveTimer() {
        if (!auto_save_timer) {
            auto_save_timer = new QTimer(this);
            connect(auto_save_timer, &QTimer::timeout, this, &WriteResumeWindow::autoSaveResume);
        }
        return auto_save_timer;
    }
};

#endif // WRITE_RESUME_WINDOW_HPP
