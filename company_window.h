//
//  company_window.h
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#ifndef company_window_h
#define company_window_h

#include <QDialog>
#include <QString>
#include <QVector>
#include "company_model.h"

class QLineEdit;
class QLabel;
class QPushButton;
class QListWidget;
class WriteResumeWindow;
class TypeQualificationWindow;
class QMenu;

class CompanyWindow : public QDialog {
    Q_OBJECT
    
public:
    // 생성자 및 소멸자
    CompanyWindow(QWidget* parent, int id, const QString& company, const QString& jd);
    virtual ~CompanyWindow(); // 가상 소멸자로 변경
    
    void updateQualifications();
    void addQualification(const QString& qualification);
    void loadCompanyInfo();
    void saveCompanyInfo();
    
public slots:
    // 슬롯 메서드
    void onCheckClicked();
    void onAdaptClicked();
    void onCancelClicked();
    void onWriteResumeClicked();
    void onAddQualificationClicked();
    void onOpenJdClicked();
    
    // 새로 추가된 메서드들
    void onDeleteQualificationClicked();
    void setupQualificationsContextMenu();
    
private:
    // 데이터 멤버
    int company_id;
    QString company_name;
    QString job_description;
    QVector<Qualification> qualifications;
    QVector<ResumeIssue> resume_issues;
    
    // UI 요소
    QLineEdit* print_company_name;
    QLineEdit* print_jd;
    QPushButton* check_button;
    QPushButton* adapt_button;
    QPushButton* cancel_button;
    QPushButton* write_resume_button;
    QPushButton* open_add_qual_button;
    QPushButton* open_jd_button;
    QListWidget* qualifications_browser;
    QMenu* qualifications_context_menu;
    
    // 하위 창
    WriteResumeWindow* write_resume_win;
    TypeQualificationWindow* type_qualification_win;
    
    // 상태 플래그
    bool has_resume;
    
    // 헬퍼 메서드
    void setupUi();
    void openResumeWindow();
    void openQualificationWindow();
    bool checkForResume();
};

// 전역 함수 선언
CompanyWindow* create_company_window(int id, const std::string& company, const std::string& jd);

#endif /* company_window_h */
