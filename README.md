# functt

functt is a template toolkit that uses a template language which is inspired by the Mustache template language.

## Features

 * functional interface
 * use of c++14 features
 * simple to integrate into another codebase because it only consists of a single header

## Example Usage

``` cpp
functt::Template view("{{variable}}, {{{variable}}, {{#lambda}}Hello{{/lambda}}");

functt::varmap vars;
functt::funmap funs;

vars["variable"] = "Some Variable";
funs["lambda"] = [](std::string str) {
	return str;
};

std::cout << view.render() << std::endl;
```

## Template Language Reference

```
Variables are by default html escaped:                                  {{variable}}
This variable is unescaped:                                             {{{variable}}}
The function called lambda will be called with the string param "Hello" {{#lambda}}Hello{{/lambda}}
```

## License

functt ist licensed under the MIT license.
