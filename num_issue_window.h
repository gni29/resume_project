//
//  num_issue_window.h
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#ifndef num_issue_window_h
#define num_issue_window_h

#include <QDialog>
#include <QString>
#include <QDir>

class QComboBox;
class QPushButton;
class WriteResumeWindow;

class NumIssueWindow : public QDialog {
    Q_OBJECT
    
public:
    NumIssueWindow(QWidget* parent, const QString& company_dir, WriteResumeWindow* parent_win = nullptr);
    ~NumIssueWindow();
    
    // 특정 디렉토리에 이미 issue 파일이 있는지 확인하는 정적 메서드
    static bool isIssuesSet(const QString& companyDir);
    
public slots:
    void onCheckClicked();
    void onCancelClicked();
    
private:
    // UI Elements
    QComboBox* num_issue_input;
    QPushButton* check;
    QPushButton* cancel;
    
    // Data
    QString company_directory;
    WriteResumeWindow* parent_window;
    
    // Helper methods
    void setupUi();
    void setNumIssues();
};

#endif /* num_issue_window_h */
