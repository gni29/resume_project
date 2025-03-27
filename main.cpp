#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <QStyleFactory>
#include <QTimer>
#include <QScreen>
#include <QStandardPaths>
#include <QSqlDatabase>
#include <QFileInfo>

#include <stdexcept>
#include <iostream>
#include <memory>

#include "first_window.h"
#include "database_utils.h"

// 디렉토리 생성 유틸리티 함수
bool createDirectoryIfNotExists(const QString& path) {
    QDir dir(path);
    if (dir.exists()) {
        return true;  // 이미 존재하면 성공으로 간주
    }
    
    if (dir.mkpath(".")) {
        qDebug() << "디렉토리 생성 성공:" << path;
        return true;
    }
    
    qWarning() << "디렉토리 생성 실패:" << path;
    return false;
}

int main(int argc, char *argv[]) {
    try {
        // QApplication 설정
        QApplication app(argc, argv);
        
        // 앱 정보 설정
        QApplication::setApplicationName("Job Application Manager");
        QApplication::setApplicationVersion("1.0");
        QApplication::setOrganizationName("Job Manager");
        
        qDebug() << "Job Application Manager 시작 중...";
        
        // Qt 스타일 설정
        QApplication::setStyle(QStyleFactory::create("Fusion"));
        
        // 홈 디렉토리 확인
        QString homeDir = QDir::homePath();
        qDebug() << "사용자 홈 디렉토리:" << homeDir;
        
        // 데이터 저장 디렉토리 (홈 디렉토리 내)
        QString dataDir = homeDir + "/.jobapp";
        createDirectoryIfNotExists(dataDir);
        
        // 기본 job_postings 디렉토리 (홈 디렉토리 내)
        QString jobPostingsDir = dataDir + "/job_postings";
        if (!createDirectoryIfNotExists(jobPostingsDir)) {
            QMessageBox::warning(nullptr, QObject::tr("디렉토리 오류"),
                QObject::tr("저장 디렉토리를 생성할 수 없습니다. 권한을 확인하세요."));
        }
        
        // 데이터베이스 초기화 - 홈 디렉토리 사용
        bool dbInitialized = DatabaseUtils::initializeDatabase();
        
        if (!dbInitialized) {
            qCritical() << "데이터베이스 초기화 실패";
            QMessageBox::warning(nullptr, QObject::tr("초기화 실패"),
                QObject::tr("데이터베이스 초기화 실패: 프로그램이 제대로 작동하지 않을 수 있습니다."));
            // 실패해도 계속 진행
        } else {
            qDebug() << "데이터베이스 초기화 성공";
        }
        
        // 메인 창 생성
        FirstWindow* mainWindow = get_main_window();
        if (!mainWindow) {
            throw std::runtime_error("메인 윈도우 생성 실패");
        }
        
        // 화면 중앙에 위치시키기
        QScreen* screen = QGuiApplication::primaryScreen();
        if (screen) {
            QRect screenGeometry = screen->geometry();
            int x = (screenGeometry.width() - mainWindow->width()) / 2;
            int y = (screenGeometry.height() - mainWindow->height()) / 2;
            mainWindow->move(x, y);
        }
        
        // 메인 창 표시
        mainWindow->show();
        
        // 애플리케이션 실행
        int result = app.exec();
        
        // 정리
        delete mainWindow;
        DatabaseUtils::closeDatabase();
        
        return result;
    }
    catch (const std::bad_alloc& e) {
        qCritical() << "치명적 메모리 할당 오류:" << e.what();
        QMessageBox::critical(nullptr, QObject::tr("오류"),
            QObject::tr("메모리 부족: %1").arg(e.what()));
        return 1;
    }
    catch (const std::exception& e) {
        qCritical() << "치명적 오류:" << e.what();
        QMessageBox::critical(nullptr, QObject::tr("오류"),
            QObject::tr("프로그램 시작 중 오류: %1").arg(e.what()));
        return 1;
    }
    catch (...) {
        qCritical() << "알 수 없는 치명적 오류 발생";
        QMessageBox::critical(nullptr, QObject::tr("오류"),
            QObject::tr("알 수 없는 오류가 발생했습니다."));
        return 1;
    }
}
