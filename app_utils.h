//
//  app_utils.h
//  qt_resume
//
//  Created by Pyoung Jin Ji on 3/26/25.
//

// 이 함수는 header 파일에 추가하세요 (예: app_utils.h)

#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <QString>
#include <QDir>
#include <QDebug>

// 앱 데이터 디렉토리 경로 가져오기
inline QString getAppDataDir() {
    // 사용자 홈 디렉토리 사용
    QString homeDir = QDir::homePath();
    QString appDataDir = homeDir + "/.jobapp";
    
    // 디렉토리 확인 및 생성
    QDir dir(appDataDir);
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            qWarning() << "앱 데이터 디렉토리 생성 실패:" << appDataDir;
            // 대체 경로 - 임시 폴더
            appDataDir = QDir::tempPath() + "/jobapp";
            dir = QDir(appDataDir);
            if (!dir.exists() && !dir.mkpath(".")) {
                qWarning() << "임시 디렉토리 생성 실패:" << appDataDir;
                // 최후의 수단 - 현재 디렉토리
                appDataDir = QDir::currentPath();
            }
        }
    }
    
    return appDataDir;
}

// 특정 회사의 디렉토리 경로 가져오기
inline QString getCompanyDir(int companyId) {
    QString baseDir = getAppDataDir() + "/job_postings";
    
    // 기본 디렉토리 확인 및 생성
    QDir baseDirObj(baseDir);
    if (!baseDirObj.exists()) {
        if (!baseDirObj.mkpath(".")) {
            qWarning() << "기본 디렉토리 생성 실패:" << baseDir;
            return "";
        }
    }
    
    // 회사별 디렉토리 경로 생성
    QString companyDir = baseDir + "/" + QString::number(companyId);
    QDir companyDirObj(companyDir);
    if (!companyDirObj.exists()) {
        if (!companyDirObj.mkpath(".")) {
            qWarning() << "회사 디렉토리 생성 실패:" << companyDir;
            return "";
        }
    }
    
    return companyDir;
}

#endif // APP_UTILS_H
