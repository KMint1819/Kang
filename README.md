# Kang
Making my own language using LLVM. This project is intended to be a pre-work before learning MLIR.

- features of the language:
    - all numbers will be a double
    - can define functions
    - can declare the signature of the function before defining its definition

## Usage
```bash
git clone https://github.com/KMint1819/Kang
cd Kang
mkdir build && cd build
cmake .. && cmake --build .
bin/kang
```
## Examples
1. Expressions
```
Enter:> 10-6;
define double @0() {
k_entry:
  ret double 4.000000e+00
}
```
2. Functions 
```
Enter:> def squaredSum(a b) (a+b)*(a+b);  
define double @squaredSum(double %a, double %b) {
k_entry:
  %k_add = fadd double %a, %b
  %k_add1 = fadd double %a, %b
  %k_mul = fmul double %k_add, %k_add1
  ret double %k_mul
}
```
3. Calling functions in functions
```
Enter:> def func(a b) squaredSum(a, b) + squaredSum(a, b);
define double @func(double %a, double %b) {
k_entry:
  %k_call = call double @squaredSum(double %a, double %b)
  %k_call1 = call double @squaredSum(double %a, double %b)
  %k_add = fadd double %k_call, %k_call1
  ret double %k_add
}
```
## To be solved
1. Fix output buffer to console