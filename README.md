# functt

functt is heavily reduced template language which is inspired by the Mustache template language.

## Features

 * functional interface
 * use of c++14 features
 * simple to integrate into own codebase because it only consists of one header

## Example Usage

``` cpp
functt::Template view("{{variable}}, {{{variable}}, {{#lambda}}Hello{{/lambda}}");

std::map<std::string, std::string> vars;
std::map<std::string, std::function<std::string(std::string)>> funs;

vars["variable"] = "Some Variable";
funs["lambda"] = [](std::string str) {
	return str;
};

std::cout << view.render() << std::endl;
```

## Template Language Reference

```
Variables are by default html escaped:                                  {{variable}}
This variable is unescaped:                                             {{{variable}}
The function called lambda will be called with the string param "Hello" {{#lambda}}Hello{{/lambda}}
```

## License

functt ist licensed under the MIT license.
