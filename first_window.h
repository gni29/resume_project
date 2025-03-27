//
//  first_window.h
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#ifndef first_window_h
#define first_window_h

#include <QMainWindow>
#include <QVector>
#include "company_model.h"

class QPushButton;
class QListWidget;
class QListWidgetItem;
class QMenu;

class FirstWindow : public QMainWindow {
    Q_OBJECT
    
public:
    // 싱글톤 인스턴스
    static FirstWindow* instance_;
    
    // 생성자와 소멸자
    FirstWindow(QWidget* parent = nullptr);
    virtual ~FirstWindow();
    
    // 회사 목록 업데이트
    void updateCompanyList();
    
public slots:
    // 슬롯 함수들
    void onAddCompanyClicked();
    void onCloseClicked();
    void onCompanyItemDoubleClicked(QListWidgetItem* item);
    void onCompanyItemRightClicked(const QPoint& pos);
    void onDeleteCompanyAction();
    
private:
    // UI 요소
    QPushButton* add_company_button;
    QPushButton* close_button;
    QListWidget* company_browser;
    QMenu* company_context_menu;
    
    // 헬퍼 메서드
    void openCompanyAddWindow();
    void openCompanyWindow(int id, const QString& company, const QString& jd);
    bool initializeDatabase();
    void setupUi();
    void setupContextMenu();
    
    // 회사 목록 데이터
    QVector<Company> companies;
};

// 전역 함수 - 싱글톤 인스턴스 접근
FirstWindow* get_main_window();


#endif /* first_window_h */
