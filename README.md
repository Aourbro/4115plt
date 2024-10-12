# Advance Calculator (COMS 4115 PLT)

## Installation

### Environment Preparation (Developped on `Ubuntu 24.04`)
install g++ and make: `sudo apt install g++ make`

### 'Install' from source
run make: `make` or `make main`  
you can also run `make clean` to 'uninstall' the calculator

### Usage
```
Usage: ./main [options]\n" <<
Options:\n" <<
  --help                 Display this information.
  -s <string>            Take the <string> as input LaTeX expression.
  -f <file>              Take content in the <file> as input.
  -o <file>              Place the output into <file>.
Option -f has higher priority than -s\n
```

test cases are listed in dir `./testcases`, named `test{n}.tex`.  
run `./main -f ./testcases/test{n}.tex` to do test

## Teammates
Yunhao Xing (yx2812)  
Ziang Xu (zx2462)  

## Lexical Grammar
Symbol := [a-z]|alpha|beta|...|zeta  
Number := [0-9]+  
Keyword := frac  
Operator := \\+|\\-|\\*|/|%  
EscapeChar := \\\  
LeftParenthesis := \\(  
RightParenthesis := \\)  
LeftBrace := \\{  
RightBrace := \\}  

### How the lexer works
