#include <iostream>
#include <functt.h>

int main() {
    functt::Template view("{{{html}}}");
    std::string output = view.render({{"html", "<strong>Hello World</strong>"}});

    std::string expected("<strong>Hello World</strong>");
    if (output != expected) {
    	std::cout << output << std::endl;
	return 1;
    }
    return 0;
}
