/*
 * Program to verify that a student has installed the virtual machine.
 * This program will be configured with a secret and provided as a precompiled executable
 * The implementation of std::hash is platform dependent and cannot be used for this application 
 */
#include <string>
#include <experimental/filesystem>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <string_view>

std::string GetDeviceID() {
    // Get the alphanumerically lowest UUID of the disk devices on an ubuntu system
    constexpr std::string_view device_uuids_path = "/dev/disk/by-uuid";
    std::string lowest_uuid;
    for(const auto& entry : std::experimental::filesystem::directory_iterator(device_uuids_path)) {
        std::string entry_uuid = entry.path().filename().string();
        lowest_uuid = lowest_uuid == "" ? entry_uuid : std::min(lowest_uuid, entry_uuid);
    }
    return lowest_uuid;
}

std::array<unsigned char, 36> ComputeVerificationCode(std::string student_id, std::string device_id) {
    /*
     * Compute a users verification code
     * student_id: The students ID. Used as a shared secret between the student and the verification system.
     * device_id: A 36 char UUID used to identify this users device
     */
    // TODO: Use a salted hash of the students ID as opposed to the raw ID
    std::array<unsigned char, 36> result;
    for(unsigned int index = 0; index < 36; index++) {
        char sid_char = student_id.at(index % student_id.length());
        char did_char = device_id.at(index);
        result[index] = sid_char ^ did_char;
    }
    return result;
}

std::string VerificationCodeToString(std::array<unsigned char, 36> code) {
    std::stringstream result;
    result << std::hex;
    for(unsigned char c : code) {
       result << std::setw(2) << std::setfill('0') << (int)c;
    }
    return result.str();
}

int main(int argc, char** argv) {
    std::cout << "Enter your student id: ";
    std::string student_id;
    std::cin >> student_id;
    auto uuid = GetDeviceID();
    auto code = ComputeVerificationCode(student_id, GetDeviceID());
    std::cout << "Your verification code is: " << std::endl << VerificationCodeToString(code) << std::endl;
    return 0;
}
