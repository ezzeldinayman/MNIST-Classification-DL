#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

const int BUFFER_SIZE = 4096;
const char* SERVER_IP = "127.0.0.0";
const int SERVER_PORT = 8080;

int main() {
    std::string imagePath;
    std::cout << "Enter image path: ";
    std::cin >> imagePath;

    // Read image data
    std::ifstream imageFile(imagePath, std::ios::binary);
    if (!imageFile) {
        std::cerr << "Failed to open image file." << std::endl;
        return 1;
    }
    
    std::stringstream imageBuffer;
    imageBuffer << imageFile.rdbuf();
    std::string imageContent = imageBuffer.str();

    // Construct HTTP request
    std::string requestBody = "POST /upload_image HTTP/1.1\r\n"
                              "Host: " + std::string(SERVER_IP) + "\r\n"
                              "Content-Type: application/json\r\n"
                              "Content-Length: " + std::to_string(imageContent.size()) + "\r\n"
                              "\r\n" + imageContent;

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation error");
        return 1;
    }

    // Connect to the server
    struct sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection error");
        return 1;
    }

    // Send the HTTP request
    if (send(sockfd, requestBody.c_str(), requestBody.size(), 0) == -1) {
        perror("Send error");
        return 1;
    }

    // Receive and print the server response
    char buffer[BUFFER_SIZE];
    std::string response;
    int bytesRead;

    while ((bytesRead = recv(sockfd, buffer, BUFFER_SIZE, 0)) > 0) {
        response.append(buffer, bytesRead);
    }

    std::cout << "Server response:\n" << response << std::endl;

    // Close the socket
    close(sockfd);

    return 0;
}
