//
// Created by hackerman on 1/9/24.
//
#include "CPasswordTester.hpp"
#include "defines.hpp"


namespace
{
constexpr std::string_view gpg = "gpg";
constexpr std::string_view argIgnoreMdc = " --ignore-mdc-error";
constexpr std::string_view argBatch = " --batch";
constexpr std::string_view argYes = " --yes";
constexpr std::string_view argPassphrase = " --passphrase ";
constexpr std::string_view argDecrypt = " --decrypt ";
constexpr std::string_view writeToFile = " > ";
constexpr std::string_view noTerminalOutput = " 2>/dev/null";

constexpr std::string_view swisskey = "/swiss_keys/swisskey{}.gpg";
constexpr std::string_view resultFile = "/swiss_keys/result{}.txt";

[[nodiscard]] std::string build_gpg_call(
        const std::filesystem::path& dataDirPath,
        const std::filesystem::path& resultFilePath,
        size_t keyNumber,
        std::string_view password)
{
    std::string output{ gpg };
    output.reserve(256u);
    
    output += argIgnoreMdc;
    output += argBatch;
    output += argYes;
    
    output += argPassphrase;
    output += password;
    
    output += argDecrypt;
    output += dataDirPath;
    output += std::format(swisskey, keyNumber);
    
    output += writeToFile;
    output += resultFilePath;
    
    output += noTerminalOutput;
    
    return output;
}

[[nodiscard]] size_t generate_file_number()
{
    static std::atomic<size_t> fileNumber = 0u;
    return fileNumber.fetch_add(1u, std::memory_order_relaxed);
}

[[nodiscard]] std::filesystem::path build_result_filepath(const std::filesystem::path& dataDirPath)
{
    return std::filesystem::path{ dataDirPath.c_str() + std::format(resultFile, generate_file_number()) };
}
}   // namespace

CPasswordTester::CPasswordTester(const std::filesystem::path& dataDirPath, size_t keyNumber, std::string_view password)
    : m_ResultFilepath{ build_result_filepath(dataDirPath) }
    , m_CorrectPasswd{ false }
{
    std::string gpgCall = build_gpg_call(dataDirPath, m_ResultFilepath, keyNumber, password);
    [[maybe_unused]] int32_t result = std::system(gpgCall.c_str());
    
    if (std::filesystem::exists(m_ResultFilepath) && std::filesystem::is_regular_file(m_ResultFilepath))
    {
        m_CorrectPasswd = std::filesystem::file_size(m_ResultFilepath) > 0u;
    }
    else
    {
        LOG_ERROR_FMT("Result file: {} does not exist. When testing password: {}. On keynumber: {}", m_ResultFilepath.c_str(), password, keyNumber);
    }
}

CPasswordTester::~CPasswordTester()
{
    if (std::filesystem::exists(m_ResultFilepath))
    {
        std::filesystem::remove(m_ResultFilepath);
    }
}