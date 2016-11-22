#include <iostream>
#include <functt.h>

int main() {
    functt::Template view("Hello {{name}}!\n{{#question}}How do you ask a {{question}}?{{/question}}");
    std::string output = view.render({{"name", "World"}}, {{"question", [](std::string question) {
							question += " Ask as simple as possible.";
                                                        return question;
                                                        }
    }});

    std::string expected("Hello World!\nHow do you ask a {{question}}? Ask as simple as possible.");

    if (output != expected) {
	std::cout << output << std::endl;
	return 1;
    }
    return 0;
}
