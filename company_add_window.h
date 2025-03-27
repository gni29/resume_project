//
//  company_add_window.h
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#ifndef company_add_window_h
#define company_add_window_h
#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;

// Forward declaration
class FirstWindow;

// 회사 추가 창
class CompanyAddWindow : public QDialog {
    Q_OBJECT

public:
    CompanyAddWindow(QWidget* parent = nullptr);
    ~CompanyAddWindow();
    
    // 부모 창 참조
    FirstWindow* parent_window;
    
private slots:
    // Qt의 signal/slot으로 대체된 콜백 메서드
    void onCheckClicked();
    void onCancelClicked();
    
private:
    // UI 요소
    QLineEdit* print_company;
    QLineEdit* JD;
    QPushButton* check_button;
    QPushButton* cancel;
    
    // 헬퍼 메서드
    void saveCompany();
    void setupUi();
};

// 전역 함수 - 싱글톤 패턴
CompanyAddWindow* company_add();


#endif /* company_add_window_h */
