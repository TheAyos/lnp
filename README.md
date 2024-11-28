# Logic and Proofs

Project made by Youssef Seghrouchni, Siyuan Lu, Ayoub Agouzoul and Rachid Tazi.

# Next steps : 
1. MAKEFILE to do
2. USE the library 'fstream' to parse the input file and generate an output file

- How to open a file for reading : 
std::ifstream inputFile("input.txt");

- How to open a file for writing : 
std::ofstream outputFile("output.txt");

- Read a line from the file :
std::getline(inputFile, line)
--> while (std::getline(inputFile, line))
it reads each line from the file and stores it in the line string, it continues until the end of the file is reached

3. Modelize the board and pieces
- from a1 to h8
- pawn, knight, rook, bishop, queen, king

4. Modelize basic moves
each move is described as follow : <srcLetter><srcNumber><destLetter><destNumber>{<promotion code>}

5. CHECK detection
- we can do it by propagation : we start at the king position and increase the size of the square each time to see if the line or the diagonal is protected or not reached by the ennemy. 