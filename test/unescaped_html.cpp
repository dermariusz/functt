#include <iostream>
#include <functt.h>

int main() {
    functt::Template view("{{{html}}}");
    std::string output = view.render({{"html", "<strong>Hello World</strong>"}});
    std::cout << output << std::endl;

    std::string expected("<strong>Hello World</strong>");
    if (output != expected) {
	return 1;
    }
    return 0;
}
