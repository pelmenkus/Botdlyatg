#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <regex>

std::string removeHTMLTags(const std::string& text) {
    // �������� HTML-����� � �������������� ���������� ���������
    std::regex tagsRegex("<[^>]+>");
    std::string result = std::regex_replace(text, tagsRegex, "");

    // ������ ����������� &lt; � &gt; �� < � >
    std::regex ltRegex("&lt;");
    std::regex gtRegex("&gt;");
    result = std::regex_replace(result, ltRegex, "<");
    result = std::regex_replace(result, gtRegex, ">");

    return result;
}

void processHTMLFiles(const std::string& directoryPath) {
    std::filesystem::path dir(directoryPath);

    // �������� ������������� ���������� ��������
    if (!std::filesystem::exists(dir)) {
        std::cout << "��������� ������� �� ����������." << std::endl;
        return;
    }

    // ��������, �������� �� ��������� ���� �����������
    if (!std::filesystem::is_directory(dir)) {
        std::cout << "��������� ���� �� �������� ���������." << std::endl;
        return;
    }

    // �������� �� ������ � ��������� ����������
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            std::filesystem::path filePath = entry.path();
            std::string fileExtension = filePath.extension().string();

            // �������� ���������� �����
            if (fileExtension == ".html") {
                // �������� � ������ HTML-�����
                std::ifstream inputFile(filePath);
                if (inputFile.is_open()) {
                    std::string fileContents((std::istreambuf_iterator<char>(inputFile)),
                        std::istreambuf_iterator<char>());

                    // ������� ������ �� HTML-�����
                    std::string cleanedText = removeHTMLTags(fileContents);

                    // ������������ ����� ��� ���������� ���������� ������
                    std::string outputFileName = filePath.stem().string() + ".txt";
                    std::filesystem::path outputPath = dir / outputFileName;

                    // ���������� ���������� ������ � ����� � ����������� .txt
                    std::ofstream outputFile(outputPath);
                    if (outputFile.is_open()) {
                        outputFile << cleanedText;
                        outputFile.close();
                        std::cout << "���� \"" << outputPath.filename() << "\" ������." << std::endl;
                    } else {
                        std::cout << "�� ������� ������� ���� \"" << outputPath.filename() << "\"." << std::endl;
                    }
                } else {
                    std::cout << "�� ������� ������� ���� \"" << filePath.filename() << "\"." << std::endl;
                }
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "������� ���� � �������� � ��������� ��������� ������." << std::endl;
        return 0;
    }

    std::string directory
