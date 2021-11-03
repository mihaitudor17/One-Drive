#include <iostream>
#include <string>

int main()
{
    for (int i = 0; i < 10; i++) {
        std::cout<<i << ". Simple commit test for commit counter script\n";
    }
    std::string input;
    do {
        std::getline(std::cin, input);
        std::cout << input;
    } while (input!="Close");
}