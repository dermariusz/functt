#include <iostream>
#include <functt.h>

int main() {
    functt::Template view("{{var}}{{!comment}}");
    std::string output = view.render({{"var", "Hello"}});
    std::string expected("Hello");
    if (output != expected) {
	std::cout << output << std::endl;
	return 1;
    }
    return 0;
}
