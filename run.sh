g++ input_generator.cpp -o ig
./ig > input.txt
g++ main.cpp -o main
./main < input.txt 
g++ output_validator.cpp -o ov
./ov < input.txt
