#include "defines.hpp"
#include "profiler.hpp"
#include "CPasswordTester.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wnull-dereference"
#pragma GCC diagnostic ignored "-Wshadow"
#include "taskflow/taskflow.hpp"
#pragma GCC diagnostic pop


//#define PROFILER_ENABLED

[[nodiscard]] tf::Executor& executor_instance()
{
    static tf::Executor exec{};
    return exec;
}

// Turn binary file into ascii
namespace
{
[[nodiscard]] bool ascii_byte(int32_t byte)
{
    return (byte < 127 && byte > 31) || (byte == 10);
}

[[nodiscard]] char as_ascii(int32_t data)
{
    ASSERT(ascii_byte(static_cast<uint8_t>(data)), "Parameter is not an no 'normal' ascii value!");
    
    return static_cast<char>(data);
}

[[nodiscard]] std::fstream open_file(const std::filesystem::path& filepath, std::ios_base::openmode mode)
{
    std::fstream file{ filepath, mode };
    if(!file.is_open())
    {
        LOG_FATAL_FMT("Failed to open file: {}", filepath.c_str());
    }
    
    return file;
}

[[nodiscard]] std::string extract_ascii(const std::filesystem::path& filepath)
{
    LOG_INFO("Extracting file data...");
    
    std::fstream file = open_file(filepath, std::ios::in | std::ios::binary);
    
    std::string output{};
    while (true)
    {
        int32_t data = file.get();
        if(file.eof())
            break;
        
        if(file.fail() || file.bad())
        {
            LOG_FATAL_FMT("I/O Failure when reading file: {}", filepath.c_str());
        }
        
        if(ascii_byte(data))
            output += as_ascii(data);
    }
    
    return output;
}
}   // namespace

// Find all potential passwords using regex
namespace
{
std::mutex storageLock;
void store_password(std::unordered_set<std::string>& set, std::string_view password)
{
    std::lock_guard<std::mutex> lock{ storageLock };
    [[maybe_unused]] auto[iter, emplaced] = set.emplace(password);
}

void regex_search(std::unordered_set<std::string>& potentialPasswords, const std::string& source, const std::regex& pattern)
{
    std::sregex_iterator begin{ source.begin(), source.end(), pattern };
    std::sregex_iterator end{};
    
    for (std::sregex_iterator iter = begin; iter != end; ++iter)
    {
        store_password(potentialPasswords, iter->str());
    }
}

[[nodiscard]] std::vector<std::regex> build_patterns()
{
    std::vector<std::regex> output{};
    for(size_t x = 1u; x < 3; ++x)
        for(size_t y = 1u; y < 3; ++y)
            for(size_t z = 1u; z < 3; ++z)
                output.emplace_back(std::format("\\b\\d{{1,{}}}[a-z]{{3,8}}\\d{{1,{}}}[a-z]{{3,8}}\\d{{1,{}}}", x, y, z));
    
    return output;
}

[[nodiscard]] std::unordered_set<std::string> find_potential_passwords(const std::string& source)
{
    LOG_INFO("Looking for passwords with regex..");
    
    std::unordered_set<std::string> potentialPasswords{};
    
    // build regex patterns
    std::vector<std::regex> patterns = build_patterns();
    
    // Search all patterns
    tf::Taskflow taskflow{};
    for(auto& pattern : patterns)
    {
        taskflow.emplace([&potentialPasswords, &source, &pattern]()
        {
            regex_search(potentialPasswords, source, pattern);
        });
    }
    executor_instance().run(taskflow).wait();
    
    return potentialPasswords;
}
}   // namespace

// Try each password against the eight encrypted files
namespace
{
[[nodiscard]] std::vector<std::string> find_working_passwords(
        const std::filesystem::path& dataDirPath,
        const std::unordered_set<std::string>& potentialPasswords)
{
    LOG_INFO("Testing passwords...");
    
    
    std::array<std::pair<std::atomic<bool>, std::string>, 8u> workingPasswords{};
    for(size_t i = 0u; i < 8u; ++i)
        workingPasswords[i] = std::make_pair(false, "");
    
    
    tf::Taskflow taskflow{};
    for(const auto& password : potentialPasswords)
    {
        static constexpr size_t NUM_ENCRYPTED_FILES = 8u;
        taskflow.emplace([&password, &workingPasswords, &dataDirPath]()
        {
            for(size_t i = 0u; i < NUM_ENCRYPTED_FILES; ++i)
            {
                if(!workingPasswords[i].first.load(std::memory_order_relaxed))
                {
                    size_t keyNumber = i + 1u;
                    CPasswordTester tester{ dataDirPath, keyNumber, password };
                    if(tester.success())
                    {
                        workingPasswords[i].first.store(true, std::memory_order_relaxed);
                        workingPasswords[i].second = password;
                        break;
                    }
                }
            }
        });
    }
    executor_instance().run(taskflow).wait();
    
    
    std::vector<std::string> passwords{};
    passwords.reserve(workingPasswords.size());
    
    std::transform(
            std::begin(workingPasswords),
            std::end(workingPasswords),
            std::back_inserter(passwords),
            [](std::pair<std::atomic<bool>, std::string>& pair){ return std::move(pair.second); });

    return passwords;
}
}   // namespace


#ifndef PROFILER_ENABLED
int main(int argc, char** argv)
{
    try
    {
        // command line arguments
        if(argc < 2)
        {
            LOG_FATAL("Filepath to data folder was not provided...");
        }
        std::filesystem::path dataDirPath{ argv[1] };
        
        // Turn binary file into ascii
        std::string fileContent = extract_ascii(dataDirPath / "act3-cpy.img");
        
        // Find all potential passwords using regex
        std::unordered_set<std::string> potentialPasswords = find_potential_passwords(fileContent);
        
        // Try each password against the eight encrypted files
        std::vector<std::string> passwords = find_working_passwords(dataDirPath, potentialPasswords);
        
        // Print keys..
        for(size_t i = 0u; i < passwords.size(); ++i)
        {
            LOG_INFO_FMT("Key {}: {}", i + 1u, passwords[i]);
        }
    }
    catch (const std::runtime_error& err)
    {
        LOG_ERROR_FMT("Exception message: {}", err.what());
    }
    
    return 0;
}
#else
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    try
    {
        // Profile data
        std::vector<std::pair<std::string, double>> profileData{};

        double fullTimeElapsed = profiler::benchmark([&]()
        {
            if(argc < 2)
            {
                LOG_FATAL("Filepath to data folder was not provided...");
            }
            std::filesystem::path dataDirPath{ argv[1] };
            
            // Turn binary file into ascii
            std::string fileContent{};
            double extractTime = profiler::benchmark([&]()
            {
                fileContent = extract_ascii(dataDirPath / "act3-cpy.img");
            });
            profileData.emplace_back("Extract Ascii Time", extractTime);
            
            // Find all potential passwords using regex
            std::unordered_set<std::string> potentialPasswords{};
            double findPotentialTime = profiler::benchmark([&]()
            {
                potentialPasswords = find_potential_passwords(fileContent);
            });
            profileData.emplace_back("Find Potential Passwords Time", findPotentialTime);
            
            // Try each password against the eight encrypted files
            std::vector<std::string> passwords{};
            double tryPasswordsTime = profiler::benchmark([&]()
            {
                passwords = find_working_passwords(dataDirPath, potentialPasswords);
            });
            profileData.emplace_back("Find Passwords Time", tryPasswordsTime);
            
            for(size_t i = 0u; i < passwords.size(); ++i)
            {
                LOG_INFO_FMT("Key {}: {}", i + 1u, passwords[i]);
            }
        });
        
        profileData.emplace_back("Complete Program", fullTimeElapsed);
        
        for(auto& timeData : profileData)
        {
            LOG_INFO_FMT("{}: {} ms", timeData.first, timeData.second);
        };
    }
    catch (const std::runtime_error& err)
    {
        LOG_ERROR_FMT("Exception message: {}", err.what());
    }

    return 0;
}
#endif
