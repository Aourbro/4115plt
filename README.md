# Advance Calculator (COMS 4115 PLT)

## **Project Overview**

The first deliverable implements a **lexical analyzer (Tokenizer)** using a **Finite State Machine (FSM)** to process input strings or files. The tokenizer identifies **tokens** such as numbers, symbols, and keywords, handling errors for illegal characters and unrecognized symbols. The primary goal is to parse input systematically by breaking it down into meaningful tokens and reporting any parsing issues.

## Teammates
Yunhao Xing (yx2812)  
Ziang Xu (zx2462) 

## Installation

### Environment Preparation (Developed on `Ubuntu 24.04`)
install g++ and make: `sudo apt install g++ make`

### 'Install' from source
run make: `make` or `make main`  
You can run `make clean` to clean up the build files.

### Usage
```
Usage: ./main [options]
Options:
  --help                 Display this information.
  -s <string>            Take the <string> as input LaTeX expression.
  -f <file>              Take content in the <file> as input.
  -o <file>              Place the output into <file>.
Option -f has higher priority than -s
```

## Running Test Cases
Test cases are available in the `./testcases` directory, named `test{n}.tex`.  
To run a test: `./main -f ./testcases/test0.tex`

### test cases details:
  -  `./testcases/test0.tex`: a valid one
  -  `./testcases/test1.tex`: empty string error
  -  `./testcases/test2.tex`: bad symbols error
  -  `./testcases/test3.tex`: digit after letter error
  -  `./testcases/test4.tex`: another bad symbol error
  -  `./testcases/test5.tex`: a nested valid one

#### Sample output from test0
```
total 14 tokens:
  <Escape Character, \>
  <Keyword, frac>
  <Left Brace, {>
  <Number, 1>
  <Right Brace, }>
  <Left Brace, {>
  <Number, 10>
  <Right Brace, }>
  <Symbol, a>
  <Escape Character, \>
  <Symbol, alpha>
  <Operator, +>
  <Escape Character, \>
  <Symbol, beta>
``` 


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

## How the Lexer Works
### **Features**
1. **Tokenization of Strings and Files:** 
   - Supports both string inputs and file-based tokenization.
2. **Finite State Machine (FSM):** 
   - Uses a state-transition table to manage parsing logic.
3. **Error Handling:** 
   - Detects illegal characters and logs unrecognized symbols.
4. **Token Classification:** 
   - Distinguishes between numbers, keywords, and symbols.
5. **Detailed Token Output:** 
   - Outputs token streams and error reports for easier debugging.


### **Core Concepts**
#### **1. Tokenization Process**
- The parser reads input **character by character**.
- Each character triggers **state transitions** based on its type (e.g., number, symbol, keyword).
- **Actions** are executed based on the new state, including:
  - Adding characters to tokens
  - Pushing tokens to the token stream
  - Handling errors for illegal characters

#### **2. State Transitions and Actions**
The core logic for state transitions is implemented in `readChar()`. 
##### Summary of the actions:
- **`DoNothing`**: Skip the character.
- **`AddToken`**: Add the character to the current token.
- **`PushToken`**: Push the current token to the stream and reset it.
- **`PushThis`**: Push the current character as a separate token.
- **`ErrorHandle`**: Handle and report parsing errors.