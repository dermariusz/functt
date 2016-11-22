#include <iostream>
#include <functt.h>

int main() {
    functt::Template view("{{html}}");
    std::string output = view.render({{"html", "<strong>Hello World</strong>"}});
    std::string expected("&lt;strong&gt;Hello World&lt;&#x2F;strong&gt;");
    if (output != expected) return 1;
    return 0;
}
