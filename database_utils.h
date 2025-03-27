//
//  database_utils.h
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

#ifndef database_utils_h
#define database_utils_h

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QVariant>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QDateTime>
#include <stdexcept>
#include <functional>

// SQLite 결과 행을 처리하기 위한 콜백 타입
using SQLiteCallback = std::function<bool(const QSqlQuery&)>;

class DatabaseException : public std::runtime_error {
public:
    DatabaseException(const std::string& message) : std::runtime_error(message) {}
};

namespace DatabaseUtils {
    // 필요한 함수들 전방 선언
    bool createTables();
    bool executeQuery(const QString& query);
    
    // 데이터베이스 연결 종료
    inline void closeDatabase() {
        if (QSqlDatabase::contains("default")) {
            QSqlDatabase::database("default").close();
            QSqlDatabase::removeDatabase("default");
        }
    }
    
    // 데이터베이스 경로 가져오기 - 사용자 홈 디렉토리에 저장
    inline QString getDatabasePath() {
        // 사용자 홈 디렉토리 확인
        QString homeDir = QDir::homePath();
        qDebug() << "사용자 홈 디렉토리:" << homeDir;
        
        // 애플리케이션 데이터 디렉토리 생성
        QString appDataDir = homeDir + "/.jobapp";
        QDir dir(appDataDir);
        if (!dir.exists()) {
            if (!dir.mkpath(".")) {
                qWarning() << "애플리케이션 데이터 디렉토리 생성 실패:" << appDataDir;
                
                // 홈 디렉토리에 직접 저장 시도
                return homeDir + "/jobapp.db";
            }
        }
        
        return appDataDir + "/jobapp.db";
    }
    
    // SQL 쿼리 실행 (반환값 없는 경우)
    inline bool executeQuery(const QString& query) {
        try {
            QSqlDatabase db = QSqlDatabase::database("default");
            if (!db.isOpen()) {
                qCritical() << "데이터베이스가 열려있지 않습니다.";
                return false;
            }
            
            QSqlQuery sqlQuery(db);
            
            if (!sqlQuery.exec(query)) {
                qCritical() << "SQL 오류:" << sqlQuery.lastError().text() << "쿼리:" << query;
                return false;
            }
            
            return true;
        } catch (const std::exception& e) {
            qCritical() << "쿼리 실행 중 오류 발생:" << e.what();
            return false;
        }
    }
    
    // 데이터베이스 연결
    inline bool initializeDatabase(const QString& customPath = QString()) {
        try {
            // 이미 열려있는 경우 닫기
            closeDatabase();
            
            // 데이터베이스 경로 설정
            QString dbPath = customPath.isEmpty() ? getDatabasePath() : customPath;
            qDebug() << "데이터베이스 경로:" << dbPath;
            
            // 파일 디렉토리 확인
            QFileInfo fileInfo(dbPath);
            QDir dir = fileInfo.dir();
            if (!dir.exists()) {
                if (!dir.mkpath(".")) {
                    qWarning() << "데이터베이스 디렉토리 생성 실패:" << dir.path();
                    // 계속 진행, 파일 생성 시도
                }
            }
            
            // SQLite 드라이버 확인
            if (!QSqlDatabase::isDriverAvailable("QSQLITE")) {
                qCritical() << "SQLite 드라이버를 사용할 수 없습니다.";
                return false;
            }
            
            // 데이터베이스 연결 설정
            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "default");
            db.setDatabaseName(dbPath);
            
            // 추가 연결 옵션 설정
            db.setConnectOptions("QSQLITE_BUSY_TIMEOUT=5000"); // 5초 타임아웃
            
            // 데이터베이스 열기
            if (!db.open()) {
                qCritical() << "데이터베이스 열기 실패:" << db.lastError().text();
                
                // 임시 폴더에 시도
                QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/jobapp.db";
                qDebug() << "임시 경로에 시도:" << tempPath;
                
                db.setDatabaseName(tempPath);
                if (!db.open()) {
                    qCritical() << "임시 경로에도 데이터베이스 열기 실패:" << db.lastError().text();
                    return false;
                }
            }
            
            // 외래키 제약조건 활성화
            QSqlQuery query(db);
            if (!query.exec("PRAGMA foreign_keys = ON")) {
                qWarning() << "외래키 활성화 실패:" << query.lastError().text();
            }
            
            // 동시 접근 설정
            query.exec("PRAGMA journal_mode = WAL");
            query.exec("PRAGMA busy_timeout = 5000");
            
            // 테이블 생성
            if (!createTables()) {
                qCritical() << "테이블 생성 실패";
                closeDatabase();
                return false;
            }
            
            qDebug() << "데이터베이스 초기화 성공";
            return true;
        } catch (const std::exception& e) {
            qCritical() << "데이터베이스 초기화 중 예외 발생:" << e.what();
            return false;
        }
    }
    
    // SQL 쿼리 실행 (결과를 콜백으로 처리)
    inline bool executeQueryWithCallback(const QString& query, SQLiteCallback callback) {
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery sqlQuery(db);
        
        if (!sqlQuery.exec(query)) {
            qCritical() << "SQL 오류:" << sqlQuery.lastError().text() << "쿼리:" << query;
            return false;
        }
        
        // 결과 처리
        while (sqlQuery.next()) {
            if (!callback(sqlQuery)) {
                break;
            }
        }
        
        return true;
    }
    
    // 최근 INSERT 작업으로 생성된 ID 반환
    inline int getLastInsertRowId() {
        QSqlDatabase db = QSqlDatabase::database("default");
        QSqlQuery query(db);
        query.exec("SELECT last_insert_rowid()");
        
        if (query.next()) {
            return query.value(0).toInt();
        }
        
        return -1;
    }
    
    // 필요한 테이블 생성
    inline bool createTables() {
        try {
            // 회사 테이블
            QString createCompanyTable =
                "CREATE TABLE IF NOT EXISTS companies ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "name TEXT NOT NULL, "
                "job_description TEXT NOT NULL, "
                "created_at INTEGER NOT NULL, "
                "UNIQUE(name, job_description)"
                ");";
            
            // 자격요건 테이블
            QString createQualificationTable =
                "CREATE TABLE IF NOT EXISTS qualifications ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "company_id INTEGER NOT NULL, "
                "description TEXT NOT NULL, "
                "FOREIGN KEY (company_id) REFERENCES companies(id) ON DELETE CASCADE"
                ");";
            
            // 이력서 테이블
            QString createResumeTable =
                "CREATE TABLE IF NOT EXISTS resumes ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                "company_id INTEGER NOT NULL, "
                "issue_number INTEGER NOT NULL, "
                "issue_title TEXT, "
                "issue_content TEXT, "
                "FOREIGN KEY (company_id) REFERENCES companies(id) ON DELETE CASCADE, "
                "UNIQUE(company_id, issue_number)"
                ");";
            
            // 각 테이블 생성 시도
            bool success = true;
            
            if (!executeQuery(createCompanyTable)) {
                qCritical() << "회사 테이블 생성 실패";
                success = false;
            }
            
            if (!executeQuery(createQualificationTable)) {
                qCritical() << "자격요건 테이블 생성 실패";
                success = false;
            }
            
            if (!executeQuery(createResumeTable)) {
                qCritical() << "이력서 테이블 생성 실패";
                success = false;
            }
            
            return success;
        } catch (const std::exception& e) {
            qCritical() << "테이블 생성 중 오류 발생:" << e.what();
            return false;
        }
    }
} // namespace DatabaseUtils

#endif /* database_utils_h */
