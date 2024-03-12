//
// Created by hackerman on 1/9/24.
//

#pragma once


class CPasswordTester
{
public:
    CPasswordTester(const std::filesystem::path& dataDirPath, size_t keyNumber, std::string_view password);
    ~CPasswordTester();
    CPasswordTester(const CPasswordTester& other) = default;
    CPasswordTester(CPasswordTester&& other) = default;
    CPasswordTester& operator=(const CPasswordTester& other) = default;
    CPasswordTester& operator=(CPasswordTester&& other) = default;
    
    [[nodiscard]] inline bool success() const { return m_CorrectPasswd; };
private:
    std::filesystem::path m_ResultFilepath;
    bool m_CorrectPasswd;
};