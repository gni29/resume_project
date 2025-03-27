//
//  type_qualification_winodw.h
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#ifndef type_qualification_window_h
#define type_qualification_window_h

#include <QDialog>
#include <QString>

class QLineEdit;
class QPushButton;
class CompanyWindow;

class TypeQualificationWindow : public QDialog {
    Q_OBJECT
    
public:
    TypeQualificationWindow(QWidget* parent, CompanyWindow* companyParent);
    ~TypeQualificationWindow();
    
public slots:
    void onCheckClicked();
    void onCancelClicked();
    
private:
    // UI Elements
    QLineEdit* qualification_input;
    QPushButton* check;
    QPushButton* cancel;
    
    // Parent window reference
    CompanyWindow* parent_window;
    
    // Helper methods
    void setupUi();
    void addQualification();
};

#endif /* TYPE_QUALIFICATION_WINDOW_H */

