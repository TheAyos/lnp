# Logic and Proofs

Project made by Youssef Seghrouchni, Siyuan Lu, Ayoub Agouzoul and Rachid Tazi.

# Next steps : 
- MAKEFILE to do
- USE the library 'fstream' to parse the input file and generate an output file

How to open a file for reading : 
std::ifstream inputFile("input.txt");

How to open a file for writing : 
std::ofstream outputFile("output.txt");

Read a line from the file :
std::getline(inputFile, line)
--> while (std::getline(inputFile, line))
it reads each line from the file and stores it in the line string, it continues until the end of the file is reached

